#include "processwatcher.h"

ProcessWatcher::ProcessWatcher(QObject *parent) : QThread(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerWatchProcess()));
    timer->start(1000);
}

void ProcessWatcher::run()
{
}

void ProcessWatcher::timerWatchProcess()
{
    QProcess process;
    #if _MSC_VER
        process.start("tasklist");
    #else
        process.start("ps -ef");
    #endif
    process.waitForFinished();
    QByteArray output = process.readAllStandardOutput();

    QList<QByteArray> listOutPut = output.split('\n');
    for(int i = 2; i < listOutPut.size(); ++i)
    {
        qDebug()<<listOutPut.at(i);
    }
}
