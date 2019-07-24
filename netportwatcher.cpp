#include "netportwatcher.h"

NetPortWatcher::NetPortWatcher(QObject *parent) : QThread(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerWatchNetPort()));
    timer->start(1000);
}

void NetPortWatcher::run()
{
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
            qDebug()<<protocol<<port;
        }
    }
}
