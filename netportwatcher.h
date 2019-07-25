#ifndef NETPORTWATCHER_H
#define NETPORTWATCHER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QString>
#include <QProcess>
#include "ServerMonitor.pb.h"
#include "confighelper.h"
#include <string>


using namespace std;

class NetPortWatcher : public QObject
{
    Q_OBJECT
public:
    explicit NetPortWatcher(QObject *parent = 0);

    void changeWatchStatus(int status);
protected:

signals:
    void signal_MainMessage(string);

public slots:
    void timerWatchNetPort();

private:
     QTimer *timer;
};

#endif // NETPORTWATCHER_H
