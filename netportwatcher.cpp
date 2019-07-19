#include "netportwatcher.h"

NetPortWatcher::NetPortWatcher(QObject *parent) : QThread(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerWatchNetPort()));
    timer->start(1000);
}

void NetPortWatcher::run()
{
    qDebug()<<"in1";
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

    QList<QByteArray> listOutPut = output.split('\n');
    for(int i = 2; i < listOutPut.size(); ++i)
    {
        qDebug()<<listOutPut.at(i);
    }
}
