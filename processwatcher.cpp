#include "processwatcher.h"

ProcessWatcher::ProcessWatcher(QObject *parent) : QThread(parent)
{

}

void ProcessWatcher::run()
{
    qDebug()<<"in2";
}
