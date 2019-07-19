#include "netportwatcher.h"

NetPortWatcher::NetPortWatcher(QObject *parent) : QThread(parent)
{

}

void NetPortWatcher::run()
{
    qDebug()<<"in1";
}
