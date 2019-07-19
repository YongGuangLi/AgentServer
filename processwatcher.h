#ifndef PROCESSWATCHER_H
#define PROCESSWATCHER_H

#include <QObject>
#include <QDebug>
#include <QThread>

class ProcessWatcher : public QThread
{
    Q_OBJECT
public:
    explicit ProcessWatcher(QObject *parent = 0);

protected:
    void run();
signals:

public slots:
};

#endif // PROCESSWATCHER_H
