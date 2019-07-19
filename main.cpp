#include <QCoreApplication>
#include <QDebug>
#include <QByteArray>

#include "filesystemwatcher.h"
#include "netportwatcher.h"
#include "processwatcher.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    FileSystemWatcher::addWatchPath("/home");

    ProcessWatcher processWatcher;
    processWatcher.start();

    NetPortWatcher netPortWatcher;
    netPortWatcher.start();


    return a.exec();
}
