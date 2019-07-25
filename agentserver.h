#ifndef AGENTSERVER_H
#define AGENTSERVER_H

#include <QObject>
#include <QTimer>
#include <QtConcurrent>

#include "filesystemwatcher.h"
#include "netportwatcher.h"
#include "processwatcher.h"
#include "RedisHelper.h"
#include "confighelper.h"
#include "ServerMonitor.pb.h"
#include "rsacryptography.h"

class AgentServer : public QObject
{
    Q_OBJECT
public:
    explicit AgentServer(QObject *parent = 0);

    void redisSubscribe();
signals:

public slots:
    void slot_heartBeat();

    void slot_RsaEncrypt(string);
private:
    RedisHelper *redisHelper;

    ProcessWatcher *processWatcher;

    NetPortWatcher *netPortWatcher;

    RSACryptography rsaCryptography;

    bool isRunning;
};


#endif // AGENTSERVER_H
