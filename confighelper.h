#ifndef ConfigHelper_H
#define ConfigHelper_H

#include <QObject>
#include <QSettings>
#include <QFile>
#include <QDebug>


#define SingletonConfig ConfigHelper::GetInstance()

class ConfigHelper : public QObject
{
    Q_OBJECT
public:
    static ConfigHelper *GetInstance();

    //读取Config.ini
    bool initConfigHelper(QString);



    QString getIpRedis() const;

    int getPortRedis() const;

    QString getPasswdRedis() const;


private:
    explicit ConfigHelper(QObject *parent = 0);
    ~ConfigHelper();

    static ConfigHelper* configHelper;

    QString ipRedis_;
    int portRedis;
    QString passwdRedis_;
};

#endif // ConfigHelper_H
