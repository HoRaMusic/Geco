#include "externalwriter.h"

ExternalWriter::ExternalWriter(QString _plugDir)
{
    wasRead = false;
    plugDir = _plugDir;
}
void ExternalWriter::writeNewFile()
{
    QStringList newFileContent;
    QString includeLine;
    QString searchStructString("struct " + module_name + " : Module {");
    QString searchStepString("void " + module_name + "::step() {");
    QString searchWidgetString("ModuleWidget(module) {");
    QString searchWritedIncludesString("#include");
    for(int i = 0; i < FileToWriteInList.size(); i++)
    {
        QString line_ = FileToWriteInList.at(i);
        if (line_.contains(searchWritedIncludesString, Qt::CaseSensitive)) {
        newFileContent.append(line_);
        }

    }
    if( includeListToPaste.isEmpty() == false)
    {
        for (int j = 0; j < includeListToPaste.size(); j++)
        {
            includeLine = includeListToPaste.at(j);
            newFileContent.append(includeLine);
        }
    }
    if( moduleCPPListToPaste.isEmpty() == false)
    {
        for (int z = 0; z < moduleCPPListToPaste.size(); z++)
        {
            newFileContent.append(moduleCPPListToPaste.at(z));
        }
    }
    for(int i = 0; i < FileToWriteInList.size(); i++)
    {

        QString line = FileToWriteInList.at(i);
        if (!line.contains(searchWritedIncludesString, Qt::CaseSensitive))
        {
            newFileContent.append(line);
            if (line.contains(searchStructString, Qt::CaseSensitive)) {
                if( structListToPaste.isEmpty() == false)
                {
                    for (int k = 0; k < structListToPaste.size(); k++)
                    {
                        newFileContent.append(structListToPaste.at(k));
                    }
                }
            }
            if (line.contains(searchStepString, Qt::CaseSensitive)) {
                if( stepListToPaste.isEmpty() == false)
                {
                    for (int x = 0; x < stepListToPaste.size(); x++)
                    {
                        newFileContent.append(stepListToPaste.at(x));
                    }
                }
            }
            if (line.contains(searchWidgetString, Qt::CaseSensitive)) {
                if( widgetListToPaste.isEmpty() == false)
                {
                    for (int y = 0; y < widgetListToPaste.size(); y++)
                    {
                        newFileContent.append(widgetListToPaste.at(y));
                    }
                }
            }
        }
    }
    QDir fileDir(QDir::currentPath());
    fileDir.cd(plugDir);
    QString fileDirPath = fileDir.absoluteFilePath(plugDir);
    QFile file(fileDirPath);
    if (!QDir(plugDir + "/" + plugin_File_name).exists())
    {
        fileDir.mkdir( plugin_File_name);
    }
    QString cppfilename = plugDir + "/" + plugin_File_name + "/src/" + module_name + ".cpp";
   // cppfilename.truncate(cppfilename.lastIndexOf(QChar(',')));
    QFile newOutFile(cppfilename);
    if ( newOutFile.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &newOutFile );
        for (int l = 0; l < newFileContent.size(); l++)
        {
            stream << newFileContent.at(l) << endl;
        }
        newOutFile.close();
    }
}
void ExternalWriter::setFiles(QVector<QString> portVec)
{
    bool isStruct = false;
    bool isWidget = false;
    bool isStep = false;
    bool isInclude = false;
    bool isModuleCPP = false;
    bool isPlugin_setting = false;

    for (int i = 0; i < portVec.size(); i++)
    {
        if (portVec.at(i) == "add_in_struct")
        {
               isStruct = true;
        }
        if (portVec.at(i) == "add_in_step")
        {
               isStep = true;
        }
        if (portVec.at(i) == "add_in_widget")
        {
               isWidget = true;
        }
        if (portVec.at(i) == "add_include")
        {
               isInclude = true;
        }
        if (portVec.at(i) == "add_in_module_CPP")
        {
               isModuleCPP = true;
        }
        if (portVec.at(i) == "plugin_setting")
        {
            isPlugin_setting = true;
        }
    }

    if (isPlugin_setting == true)
    {
        plugin_name = portVec.at(6);
        plugin_File_name = portVec.at(6);
        QString module_nameT = portVec.at(7);
        module_nameT.truncate(module_nameT.lastIndexOf(QChar(',')));
        if (module_nameT != 0)
        {
            module_name = module_nameT;
        }
        else
        {
            module_name = portVec.at(7);
        }
        QString fileToWriteInName = plugDir + "/" + plugin_File_name + "/src/" + module_name + "Can.cpp";
        QDir fileToWriteInDir(QDir::currentPath());
        fileToWriteInDir.cd(fileToWriteInName);
        QString fileToWriteInDirPath = fileToWriteInDir.absoluteFilePath(fileToWriteInName);
        QFile FileToWriteIn(fileToWriteInDirPath);
        QString contentL1;
        if (FileToWriteIn.open(QIODevice::ReadOnly)) {
            QTextStream streamFTW(&FileToWriteIn);
            while (!streamFTW.atEnd())
            {
                FileToWriteInList.append(streamFTW.readLine());
            }
            FileToWriteIn.close();
        }
    }
    if (isStruct == true)
    {
        QString fileName = portVec.at(2);
        QDir fileDir(QDir::currentPath());
        fileDir.cd("CodeParts");
        fileDir.cd(fileName);
        QString fileDirPath = fileDir.absoluteFilePath(fileName);
        QFile structToPaste(fileDirPath);
        QString contentL1;
        if (structToPaste.open(QIODevice::ReadOnly)) {
            QTextStream stream(&structToPaste);
            while (!stream.atEnd())
               {
                  structListToPaste.append(stream.readLine());
               }
        }
        structToPaste.close();
    }
    if (isWidget == true)
    {
        QString fileNameW = portVec.at(2);
        QDir fileDirW(QDir::currentPath());
        fileDirW.cd("CodeParts");
        fileDirW.cd(fileNameW);
        QString fileDirPathW = fileDirW.absoluteFilePath(fileNameW);
        QFile widgetToPaste(fileDirPathW);
        QString contentL1;
        if (widgetToPaste.open(QIODevice::ReadOnly)) {
            QTextStream streamW(&widgetToPaste);
            while (!streamW.atEnd())
               {
                  widgetListToPaste.append(streamW.readLine());
               }
        }
        widgetToPaste.close();
    }
    if (isStep == true)
    {
        QString fileNameSt = portVec.at(2);
        QDir fileDirSt(QDir::currentPath());
        fileDirSt.cd("CodeParts");
        fileDirSt.cd(fileNameSt);
        QString fileDirPathSt = fileDirSt.absoluteFilePath(fileNameSt);
        QFile stepToPaste(fileDirPathSt);
        QString contentL1;
        if (stepToPaste.open(QIODevice::ReadOnly)) {
            QTextStream streamSt(&stepToPaste);
            while (!streamSt.atEnd())
               {
                  stepListToPaste.append(streamSt.readLine());
               }
        }
        stepToPaste.close();
    }
    if (isInclude == true)
    {
        QString fileNameIn = portVec.at(2);
        QDir fileDirIn(QDir::currentPath());
        fileDirIn.cd("CodeParts");
        fileDirIn.cd(fileNameIn);
        QString fileDirPathIn = fileDirIn.absoluteFilePath(fileNameIn);
        QFile structToPasteIn(fileDirPathIn);
        QString contentL1;
        if (structToPasteIn.open(QIODevice::ReadOnly)) {
            QTextStream streamIn(&structToPasteIn);
            while (!streamIn.atEnd())
               {
                  includeListToPaste.append(streamIn.readLine());
               }
        }
        structToPasteIn.close();
    }
    if (isModuleCPP == true)
    {
        QString fileNameM = portVec.at(2);
        QDir fileDirM(QDir::currentPath());
        fileDirM.cd("CodeParts");
        fileDirM.cd(fileNameM);
        QString fileDirPathM = fileDirM.absoluteFilePath(fileNameM);
        QFile structToPasteM(fileDirPathM);
        QString contentL1;
        if (structToPasteM.open(QIODevice::ReadOnly)) {
            QTextStream streamM(&structToPasteM);
            while (!streamM.atEnd())
               {
                  moduleCPPListToPaste.append(streamM.readLine());
               }
        }
        structToPasteM.close();
    }
}
