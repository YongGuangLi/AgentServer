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
        process.start("ps -A");
    #endif
    process.waitForFinished();
    QByteArray output = process.readAllStandardOutput();

    QList<QByteArray> listProcessData = output.split('\n');
    for(int i = 2; i < listProcessData.size(); ++i)
    {
        QString processData(listProcessData.at(i));
        QStringList listProcessDataItem = processData.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if(listProcessDataItem.size() > 3)
            qDebug()<<listProcessDataItem.at(0)<<listProcessDataItem.at(3);
    }
}
