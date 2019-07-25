#include "agentserver.h"

AgentServer::AgentServer(QObject *parent) : QObject(parent)
{
    connect(FileSystemWatcher::getInstance(), SIGNAL(signal_MainMessage(string)), this, SLOT(slot_RsaEncrypt(string)));

    processWatcher = new ProcessWatcher(this);
    connect(processWatcher, SIGNAL(signal_MainMessage(string)), this, SLOT(slot_RsaEncrypt(string)));
    processWatcher->changeWatchStatus(1);

    netPortWatcher = new NetPortWatcher(this);
    connect(netPortWatcher, SIGNAL(signal_MainMessage(string)), this, SLOT(slot_RsaEncrypt(string)));
    netPortWatcher->changeWatchStatus(1);

    isRunning = true;
    QtConcurrent::run(this, &AgentServer::redisSubscribe);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_heartBeat()));
    timer->start(1000);
}

void AgentServer::redisSubscribe()
{
    redisHelper = new RedisHelper(QString("%1:%2").arg(SingletonConfig->getIpRedis()).arg(SingletonConfig->getPortRedis()).toStdString(), SingletonConfig->getPasswdRedis().toStdString());

    while(isRunning)
    {
        if(!redisHelper->check_connect())
        {
            if(redisHelper->open())
            {
                qDebug()<<QString("Redis 连接成功:%1:%2").arg(SingletonConfig->getIpRedis()).arg(SingletonConfig->getPortRedis());
                if(redisHelper->subscribe(redisHelper->getLocalIp(), NULL) >= 1)
                    qDebug()<<QString("Redis 订阅成功:%1").arg(redisHelper->getLocalIp());
            }else
            {
                qDebug()<<QString("Redis 连接失败:%1:%2").arg(SingletonConfig->getIpRedis()).arg(SingletonConfig->getPortRedis());
                QThread::sleep(1);
                continue;
            }
        }

        char* message = NULL;
        if(redisHelper->getMessage(message) > 0)
        {
            char *plainText = NULL;
            int rsa_len = rsaCryptography.RSA_Decrypt(message, plainText, QString(qApp->applicationDirPath() + QDir::separator() + OPENSSLKEY).toStdString().c_str());

            MainMessage mainMessage;
            mainMessage.ParseFromArray(plainText, rsa_len);

            switch (mainMessage.msgtype())
            {
            case MessageType::MT_ParamModify:
            {
                ParamModify paramModify = mainMessage.parammodify();

                SingletonConfig->setDeviceID(QString::fromStdString(paramModify.devid()));

                if(paramModify.type() == ParamModifyType::PMT_PATH)        //修改监控路径
                {
                    for(int i = 0; i < paramModify.monitorpath_size(); ++i)
                        FileSystemWatcher::getInstance()->addWatchPath(QString::fromStdString(paramModify.monitorpath(i)));
                }
                else if(paramModify.type() == ParamModifyType::PMT_FUNC)   //修改功能启停
                {
                    int func_file = paramModify.func_file();              //1-启用 0-关闭
                    FileSystemWatcher::getInstance()->changeWatchStatus(func_file);

                    int func_process = paramModify.func_process();
                    processWatcher->changeWatchStatus(func_process);

                    int func_port = paramModify.func_port();
                    netPortWatcher->changeWatchStatus(func_port);
                }
                break;
            }
            default:
                break;
            }

            if (message != NULL) {
                free(message);
            }

            if (plainText != NULL) {
                free(plainText);
            }
        }
        QThread::usleep(1000);
    }
}

void AgentServer::slot_heartBeat()
{
    MainMessage mainMessage;

    mainMessage.set_msgtype(MessageType::MT_HeartBeatMessage);

    HeartBeatMessage *heartBeatMessage = mainMessage.mutable_heartbeatmessage();
    heartBeatMessage->set_devid(SingletonConfig->getDeviceID().toStdString());

    heartBeatMessage->set_time(QDateTime::currentDateTime().toTime_t());

    heartBeatMessage->set_ipaddr(redisHelper->getLocalIp());

    string strMainMessage;
    mainMessage.SerializePartialToString(&strMainMessage);

    slot_RsaEncrypt(strMainMessage);
}

void AgentServer::slot_RsaEncrypt(string strMainMessage)
{
    char *cipherText = NULL;
    int rsa_len = rsaCryptography.RSA_Encrypt(strMainMessage.c_str(), cipherText, QString(qApp->applicationDirPath() + QDir::separator() + PUBLICKEY).toStdString().c_str());

    redisHelper->publish(REDIS_CHANNEL, cipherText, rsa_len);

    if (cipherText != NULL) {
        free(cipherText);
    }
}
