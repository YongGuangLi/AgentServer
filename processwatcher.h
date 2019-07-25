#ifndef PROCESSWATCHER_H
#define PROCESSWATCHER_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QProcess>
#include <QString>
#include <QTimer>
#include "ServerMonitor.pb.h"
#include "confighelper.h"

#include <string>


using namespace std;

class ProcessWatcher : public QObject
{
    Q_OBJECT
public:
    explicit ProcessWatcher(QObject *parent = 0);

    void changeWatchStatus(int status);
protected:

signals:
    void signal_MainMessage(string);

public slots:
    void timerWatchProcess();

private:
     QTimer *timer;
};

#endif // PROCESSWATCHER_H
