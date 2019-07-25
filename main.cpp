#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QByteArray>

#include "agentserver.h"
#include "confighelper.h"
#include "message_handler.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MessageHandler::setLogFile(qApp->applicationDirPath() + QDir::separator() + "log.txt");
    //qInstallMessageHandler(MessageHandler::FormatMessage);

    qDebug()<<"---------------start-------------------";

    SingletonConfig->initConfigHelper(qApp->applicationDirPath() + QDir::separator() + "SysConfig.ini");

    AgentServer agentServer;
    return a.exec();
}
