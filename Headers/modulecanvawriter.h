#ifndef MODULECANVAWRITER_H
#define MODULECANVAWRITER_H
#include "controllerstore.h"
#include<QFile>
#include<QTextStream>
#include <QVector>
#include <QStringList>
class ModuleCanvaWriter
{
public:
    ModuleCanvaWriter(QString module_name, QString plugin_file_name, QString _plugDir);
    void writeModuleStruct(QVector<Controllerstore*> &controllerVec, QString pluginName);
    void writeStepFunctionCanva();
    void writeWidgetDeclar( QVector<Controllerstore*> &controllerVec, QString panelWidth, QString panelName, QString pluginName);
    void writeModuleModel(QString IDName, QString module_name, QString manufacturer, QString tags);
    QFile* canvaFile();
private:
    QString moduleFilename;
    QString moduleName;
    QFile moduleFile;
    QTextStream TStream;
    QVector<Controllerstore*> paramVec;
    float PanelWidth;
    QString plugDir;
};

#endif // MODULECANVAWRITER_H
