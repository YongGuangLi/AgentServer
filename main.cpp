#include <QCoreApplication>
#include <QDebug>
#include <QByteArray>
#include <QtConcurrent>

#include "filesystemwatcher.h"
#include "netportwatcher.h"
#include "processwatcher.h"
#include "RedisHelper.h"
#include "confighelper.h"
#include "message_handler.h"

RedisHelper *redisHelper = NULL;

bool isRunning = true;

void redisSubscribe()
{
    redisHelper = new RedisHelper(QString("%1:%2").arg(SingletonConfig->getIpRedis()).arg(SingletonConfig->getPortRedis()).toStdString(), SingletonConfig->getPasswdRedis().toStdString());

    while(isRunning)
    {
        if(!redisHelper->check_connect())
        {
            if(redisHelper->open())
            {
                qDebug()<<QString("Redis 连接成功:%1:%2").arg(SingletonConfig->getIpRedis()).arg(SingletonConfig->getPortRedis());
                if(redisHelper->subscribe(REDIS_CHANNEL, NULL) >= 1)
                    qDebug()<<QString("Redis 订阅成功:%1").arg(REDIS_CHANNEL);
            }else
            {
                qDebug()<<QString("Redis 连接失败:%1:%2").arg(SingletonConfig->getIpRedis()).arg(SingletonConfig->getPortRedis());
                QThread::sleep(1);
                continue;
            }
        }

        qDebug()<<redisHelper->getLocalIp();
        string message;
        if(redisHelper->getMessage(message))
        {

        }
        QThread::usleep(1000);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MessageHandler::setLogFile(qApp->applicationDirPath() + QDir::separator() + "log.txt");
    qInstallMessageHandler(MessageHandler::FormatMessage);

    qDebug()<<"---------------start-------------------";

    SingletonConfig->initConfigHelper(qApp->applicationDirPath() + QDir::separator() + "SysConfig.ini");

    FileSystemWatcher::addWatchPath("/home");
    FileSystemWatcher::addWatchPath("/home/1");

    ProcessWatcher processWatcher;
    processWatcher.start();

    NetPortWatcher netPortWatcher;
    netPortWatcher.start();

    QtConcurrent::run(redisSubscribe);

    return a.exec();
}
