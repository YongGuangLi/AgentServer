#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QByteArray>

#include "filesystemwatcher.h"
#include "netportwatcher.h"
#include "processwatcher.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    QProcess process;
    process.start("ps -ef");
    process.waitForFinished();
    QByteArray output = process.readAllStandardOutput();

    QList<QByteArray> listOutPut = output.split('\n');
    for(int i = 2; i < listOutPut.size(); ++i)
    {
        qDebug()<<listOutPut.at(i);
    }

    FileSystemWatcher::addWatchPath("/home");

    ProcessWatcher processWatcher;
    processWatcher.start();

    NetPortWatcher netPortWatcher;
    netPortWatcher.start();


    return a.exec();
}
