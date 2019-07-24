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

class FileSystemWatcher : public QObject
{
    Q_OBJECT

public:
    static void addWatchPath(QString path);

public slots:
    void directoryUpdated(const QString &path);  // 目录更新时调用，path是监控的路径
    void fileUpdated(const QString &path);   // 文件被修改时调用，path是监控的路径

    void timerWatchFile();
private:
    explicit FileSystemWatcher(QObject *parent = 0);

private:
    static FileSystemWatcher *m_pInstance; // 单例

    QFileSystemWatcher *m_pSystemWatcher;  // QFileSystemWatcher变量

    QMap<QString, QFileInfoList> m_currentDirContentsMap; // 当前每个监控的内容目录列表

    QMap<QString, QFileInfo> m_currentFileContentsMap;
};

#endif // FILESYSTEMWATCHER_H
