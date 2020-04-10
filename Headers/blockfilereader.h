#ifndef BLOCKFILEREADER_H
#define BLOCKFILEREADER_H

#endif // BLOCKFILEREADER_H
#include <iostream>
#include <vector>
#include <string>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
class FileReader: public QFile
{
    public: FileReader();
    QStringList readBlockLine(const QString &fileName, const QString &dirName);
    QFileInfoList detectFiles(const QString &dirname);
    QStringList readPropertiesLine(const QString &fileName, const QString &dirName);
private :
    // unused field
    // QTextStream *stream;
};

