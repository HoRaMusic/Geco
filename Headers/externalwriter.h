#ifndef EXTERNALWRITER_H
#define EXTERNALWRITER_H
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QVector>

class ExternalWriter
{
public:
    ExternalWriter(QString _plugDir);
    void setFiles(QVector<QString> portVec);
    void writeNewFile();
private:
    QString plugin_name;
    QString module_name;
    QStringList structListToPaste;
    QStringList widgetListToPaste;
    QStringList stepListToPaste;
    QStringList moduleCPPListToPaste;
    QStringList includeListToPaste;
    QStringList FileToWriteInList;
    QString plugin_File_name;
    QString plugDir;
    bool wasRead;
};

#endif // EXTERNALWRITER_H
