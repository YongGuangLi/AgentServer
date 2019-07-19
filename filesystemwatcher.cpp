#include "filesystemwatcher.h"

FileSystemWatcher* FileSystemWatcher::m_pInstance = NULL;

FileSystemWatcher::FileSystemWatcher(QObject *parent)
    : QObject(parent)
{

}

// 监控文件或目录
void FileSystemWatcher::addWatchPath(QString path)
{
    qDebug() << QString("Add to watch: %1").arg(path);

    if (m_pInstance == NULL)
    {
        m_pInstance = new FileSystemWatcher();
        m_pInstance->m_pSystemWatcher = new QFileSystemWatcher();

        // 连接QFileSystemWatcher的directoryChanged和fileChanged信号到相应的槽
        connect(m_pInstance->m_pSystemWatcher, SIGNAL(directoryChanged(QString)), m_pInstance, SLOT(directoryUpdated(QString)));
        connect(m_pInstance->m_pSystemWatcher, SIGNAL(fileChanged(QString)), m_pInstance, SLOT(fileUpdated(QString)));
    }

    // 添加监控路径
    m_pInstance->m_pSystemWatcher->addPath(path);

    // 如果添加路径是一个目录，保存当前内容列表
    QFileInfo file(path);
    if (file.isDir())
    {
        const QDir dirw(path);
        m_pInstance->m_currentContentsMap[path] = dirw.entryInfoList();
    }
}

// 只要任何监控的目录更新（添加、删除、重命名），就会调用。
void FileSystemWatcher::directoryUpdated(const QString &path)
{
    qDebug() << QString("Directory updated: %1").arg(path);

    // 比较最新的内容和保存的内容找出区别(变化)
    QFileInfoList currEntryInfoList = m_currentContentsMap[path];
    QFileInfoList newEntryInfoList = QDir(path).entryInfoList();

    QSet<QString> currEntrySet;
    for(int i = 0; i < currEntryInfoList.size(); ++i)
        currEntrySet.insert(currEntryInfoList.at(i).absoluteFilePath());

    QSet<QString> newEntrySet;
    for(int i = 0; i < newEntryInfoList.size(); ++i)
        newEntrySet.insert(newEntryInfoList.at(i).absoluteFilePath());

    // 添加了文件
    QSet<QString> newFileSet = newEntrySet - currEntrySet;
    QStringList newFileList = newFileSet.toList();

    // 文件已被移除
    QSet<QString> deletedFileSet = currEntrySet - newEntrySet;
    QStringList deleteFileList = deletedFileSet.toList();

    if (!newFileList.isEmpty() && !deleteFileList.isEmpty())
    {
        // 文件/目录重命名
        if ((newFileList.count() == 1) && (deleteFileList.count() == 1))
        {
            qDebug() << QString("rename file from %1 to %2").arg(deleteFileList.first()).arg(newFileList.first());
        }
    }
    else if (!newFileList.isEmpty())   // 添加新文件/目录至Dir
    {
        foreach (QString file, newFileList)
        {
            qDebug()<<QString("new file:%1").arg(file);
        }
    }
    else if(!deleteFileList.isEmpty())   // 从Dir中删除文件/目录
    {
        foreach(QString file, deleteFileList)
        {
            qDebug()<<QString("delete file:%1").arg(file);
        }
    }

    m_currentContentsMap[path] = newEntryInfoList;
}

// 文件修改时调用
void FileSystemWatcher::fileUpdated(const QString &path)
{
    QFileInfo file(path);
    QString strPath = file.absolutePath();
    QString strName = file.fileName();

    qDebug() << QString("The file %1 at path %2 is updated").arg(strName).arg(strPath);
}
