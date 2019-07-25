#include "netportwatcher.h"

NetPortWatcher::NetPortWatcher(QObject *parent) : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerWatchNetPort()));
}

void NetPortWatcher::changeWatchStatus(int status)
{
    if(status == 1)
        timer->start(1000);
    else
        timer->stop();
}


void NetPortWatcher::timerWatchNetPort()
{
    QProcess process;
    #if _MSC_VER
        process.start("netstat -an");
    #else
        process.start("netstat -ntpl");
    #endif
    process.waitForFinished();
    QByteArray output = process.readAllStandardOutput();

    QList<QByteArray> listNetPortData = output.split('\n');
    for(int i = 2; i < listNetPortData.size(); ++i)
    {
        QString netPortData(listNetPortData.at(i));
        QStringList listNetPortDataItem = netPortData.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if(listNetPortDataItem.size() > 4)
        {
            QString protocol = listNetPortDataItem.at(0);
            QString localAddress = listNetPortDataItem.at(3);
            QString port = localAddress.mid(localAddress.lastIndexOf(':') + 1);

            MainMessage mainMessage;

            string strMainMessage;
            mainMessage.SerializePartialToString(&strMainMessage);

            CollectData *collectData = mainMessage.mutable_collectdata();
            collectData->set_datatype(CollectDataType::CDT_PORT);
            collectData->set_devid(SingletonConfig->getDeviceID().toStdString());
            collectData->set_porttype(protocol.toStdString());
            collectData->set_portname(port.toStdString());

            emit signal_MainMessage(strMainMessage);
        }
    }
}
