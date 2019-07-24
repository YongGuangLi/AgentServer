#include "confighelper.h"


ConfigHelper* ConfigHelper::configHelper = NULL;

ConfigHelper *ConfigHelper::GetInstance()
{
    if(configHelper == NULL)
    {
        configHelper = new ConfigHelper();
    }
    return configHelper;
}


ConfigHelper::ConfigHelper(QObject *parent) : QObject(parent)
{
}

ConfigHelper::~ConfigHelper()
{
}

QString ConfigHelper::getIpRedis() const
{
    return ipRedis_;
}

QString ConfigHelper::getPasswdRedis() const
{
    return passwdRedis_;
}

int ConfigHelper::getPortRedis() const
{
    return portRedis;
}

bool ConfigHelper::initConfigHelper(QString filepath)
{
    if(!QFile::exists(filepath))
        qDebug()<<filepath + " not exist";

    QSettings *settings_ = new QSettings(filepath,QSettings::IniFormat, this);

    settings_->beginGroup("REDIS");
    ipRedis_ = settings_->value("ip").toString();
    portRedis = settings_->value("port").toInt();
    passwdRedis_ = settings_->value("passwd").toString();
    settings_->endGroup();


    return true;
}
