#ifndef PROCESSWATCHER_H
#define PROCESSWATCHER_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QProcess>
#include <QTimer>

class ProcessWatcher : public QThread
{
    Q_OBJECT
public:
    explicit ProcessWatcher(QObject *parent = 0);

protected:
    void run();
signals:

public slots:
    void timerWatchProcess();
};

#endif // PROCESSWATCHER_H
