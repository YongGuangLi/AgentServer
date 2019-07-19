#ifndef NETPORTWATCHER_H
#define NETPORTWATCHER_H

#include <QObject>
#include <QThread>
#include <QDebug>

class NetPortWatcher : public QThread
{
    Q_OBJECT
public:
    explicit NetPortWatcher(QObject *parent = 0);

protected:
    void run();

signals:

public slots:
};

#endif // NETPORTWATCHER_H
