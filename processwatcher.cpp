#include "processwatcher.h"

ProcessWatcher::ProcessWatcher(QObject *parent) : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerWatchProcess()));
}

void ProcessWatcher::changeWatchStatus(int status)
{
    if(status == 1)
        timer->start(1000);
    else
        timer->stop();
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
        {
            QString pid = listProcessDataItem.at(0);
            QString procName = listProcessDataItem.at(3);
            MainMessage mainMessage;

            string strMainMessage;
            mainMessage.SerializePartialToString(&strMainMessage);

            CollectData *collectData = mainMessage.mutable_collectdata();
            collectData->set_datatype(CollectDataType::CDT_PROC);
            collectData->set_devid(SingletonConfig->getDeviceID().toStdString());
            collectData->set_pid(pid.toStdString());
            collectData->set_procname(procName.toStdString());

            emit signal_MainMessage(strMainMessage);
        }
    }
}
