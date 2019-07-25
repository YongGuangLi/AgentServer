#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include <QObject>
#include <QMap>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QDateTime>
#include <QFileSystemWatcher>
#include <QTimer>
#include <string>
#include "ServerMonitor.pb.h"
#include "confighelper.h"


using namespace std;

class FileSystemWatcher : public QObject
{
    Q_OBJECT

public:
    static FileSystemWatcher * getInstance();

    void addWatchPath(QString path);

    void clearWatchPath();

    void changeWatchStatus(int status);

    void postFileWatchData(QString file, QString status, QString time);
signals:
    void signal_MainMessage(string);

public slots:
    void directoryUpdated(const QString &path);  // 目录更新时调用，path是监控的路径

    void timerWatchFile();

private:
    explicit FileSystemWatcher(QObject *parent = 0);

private:
    static FileSystemWatcher *m_pInstance; // 单例

    QFileSystemWatcher *m_pSystemWatcher;  // QFileSystemWatcher变量

    QMap<QString, QFileInfoList> m_currentDirContentsMap; // 当前每个监控的内容目录列表

    QMap<QString, QFileInfo> m_currentFileContentsMap;

    QTimer *timer;
};

#endif // FILESYSTEMWATCHER_H
