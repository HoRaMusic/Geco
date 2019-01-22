
#include "Headers/blockfilereader.h"

FileReader::FileReader()
    : QFile()
{

}

QFileInfoList FileReader::detectFiles(const QString &dirName)
{
    QDir dir(QDir::currentPath());
    dir.cdUp();
    dir.cd(dirName);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    return list;
}

QStringList FileReader::readBlockLine(const QString &fileName, const QString &dirName)
{
    QDir fileDir(QDir::currentPath());
    fileDir.cdUp();
    fileDir.cd(dirName);
    QString fileDirPath = fileDir.absoluteFilePath(fileName);
    QFile file(fileDirPath);
    QString contentL1;
    QStringList wordList;
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        contentL1 = stream.readLine();
        wordList = contentL1.split(',');
    }
    file.close();
    return wordList;
}
QStringList FileReader::readPropertiesLine(const QString &fileName, const QString &dirName)
{
    QDir fileDir(QDir::currentPath());
    fileDir.cdUp();
    fileDir.cd(dirName);
    QString fileDirPath = fileDir.absoluteFilePath(fileName);
    QFile file(fileDirPath);
    QString contentL2;
    QStringList wordList;
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        stream.readLine();
        contentL2 = stream.readLine();
        wordList = contentL2.split(',');
    }
    file.close();
    return wordList;
}
