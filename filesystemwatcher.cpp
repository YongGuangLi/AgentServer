#include "filesystemwatcher.h"

FileSystemWatcher* FileSystemWatcher::m_pInstance = NULL;

FileSystemWatcher::FileSystemWatcher(QObject *parent)
    : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerWatchFile()));
    timer->start(1000);
}


FileSystemWatcher *FileSystemWatcher::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new FileSystemWatcher();
        m_pInstance->m_pSystemWatcher = new QFileSystemWatcher();

        // 连接QFileSystemWatcher的directoryChanged和fileChanged信号到相应的槽
        connect(m_pInstance->m_pSystemWatcher, SIGNAL(directoryChanged(QString)), m_pInstance, SLOT(directoryUpdated(QString)));
    }

    return m_pInstance;
}


void FileSystemWatcher::addWatchPath(QString path)
{
    qDebug() << QString("Add to watch: %1").arg(path);
    // 添加监控路径
    m_pSystemWatcher->addPath(path);

    // 如果添加路径是一个目录，保存当前内容列表
    QFileInfo file(path);
    if (file.isDir())
    {
        const QDir dirw(path);
        m_currentDirContentsMap[path] = dirw.entryInfoList();
    }
    else
    {
        m_currentFileContentsMap[path] = file;
    }
}

void FileSystemWatcher::clearWatchPath()
{


}


void FileSystemWatcher::changeWatchStatus(int status)
{
    if(status == 1)
        timer->start(1000);
    else
        timer->stop();
}

void FileSystemWatcher::postFileWatchData(QString file, QString status, QString time)
{
    MainMessage mainMessage;
    mainMessage.set_msgtype(MessageType::MT_CollectData);

    CollectData *collectData = mainMessage.mutable_collectdata();
    collectData->set_devid(SingletonConfig->getDeviceID().toStdString());
    collectData->set_filepath(file.toStdString());
    collectData->set_state(status.toLocal8Bit());
    collectData->set_time(time.toStdString());

    string strMainMessage;
    mainMessage.SerializePartialToString(&strMainMessage);

    emit signal_MainMessage(strMainMessage);
}



// 只要任何监控的目录更新（添加、删除、重命名），就会调用。
void FileSystemWatcher::directoryUpdated(const QString &path)
{
    qDebug() << QString("Directory updated: %1").arg(path);

    // 比较最新的内容和保存的内容找出区别(变化)
    QFileInfoList currEntryInfoList = m_currentDirContentsMap[path];
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
            postFileWatchData(file, QString::fromLocal8Bit("新建"), QDateTime::currentDateTime().toString("yyyy-MM-dd mm:hh:ss"));
        }
    }
    else if(!deleteFileList.isEmpty())   // 从Dir中删除文件/目录
    {
        foreach(QString file, deleteFileList)
        {
            qDebug()<<QString("delete file:%1").arg(file);
            postFileWatchData(file, QString::fromLocal8Bit("删除"), QDateTime::currentDateTime().toString("yyyy-MM-dd mm:hh:ss"));
        }
    }

    m_currentDirContentsMap[path] = newEntryInfoList;
}


void FileSystemWatcher::timerWatchFile()
{
    QMapIterator<QString, QFileInfo> it(m_currentFileContentsMap);
    while(it.hasNext())
    {
        it.next();
        QString file = it.key();
        QFileInfo currentFileInfo = it.value();
        QFileInfo newFileInfo(file);

        //文件访问时间改变
        if(currentFileInfo.lastRead() != newFileInfo.lastRead())
        {
            qDebug()<<QString("file:%1 has read").arg(file);
            postFileWatchData(file, QString::fromLocal8Bit("读取"), newFileInfo.lastRead().toString("yyyy-MM-dd mm:hh:ss"));
        }

        //文件修改时间改变
        if(currentFileInfo.lastModified() != newFileInfo.lastModified())
        {
            qDebug()<<QString("file:%1 has modify").arg(file);
            postFileWatchData(file, QString::fromLocal8Bit("修改"), newFileInfo.lastModified().toString("yyyy-MM-dd mm:hh:ss"));
        }

        m_currentFileContentsMap[file] = newFileInfo;
    }
}
