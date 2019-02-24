#include "Headers/canvawrite.h"
#include <stdio.h>
#include "Headers/modulecanvawriter.h"
Canvawrite::Canvawrite(QString _plugDir)
{
    plugDir = _plugDir;
}
void Canvawrite::setControllersAttributes(QVector<QString> portVec)
{
    bool isInput_setting = false;
    bool isOuput_setting = false;
    bool isKnob_setting = false;
    bool isSwitch_setting = false;
    bool isRotarySwitch_setting = false;
    bool isLed_setting = false;
    bool isButton_setting = false;
    bool isScrew_setting = false;
    bool isScreen_setting = false;
    screenUsed = false;
    fontFileTransfered = false;
    for (int i = 0; i < portVec.size(); i++)
    {
        if (portVec.at(i) == "led")
        {
            isLed_setting = true;
        }
        else if (portVec.at(i) == "knob")
        {
            isKnob_setting = true;
        }
        else if (portVec.at(i) == "button")
        {
            isButton_setting = true;
        }
        else if (portVec.at(i) == "switch")
        {
            isSwitch_setting = true;
        }
        else if (portVec.at(i) == "rotary_switch")
        {
            isRotarySwitch_setting = true;
        }
        else if (portVec.at(i) == "module_input")
        {
            isInput_setting = true;
        }
        else if (portVec.at(i) == "module_output")
        {
            isOuput_setting = true;
        }
        else if (portVec.at(i) == "screen")
        {
            isScreen_setting = true;
            screenUsed = true;
        }
        else if (portVec.at(i) == "screw")
        {
            isScrew_setting = true;
        }
    }
    if (isScreen_setting == true)
    {
        QString autoName = portVec.at(4);
        QString controller_name = portVec.at(5);
        QString XPos = portVec.at(2);
        QString YPos = portVec.at(3);
        QString image_name = portVec.at(6);
        QString font_name = portVec.at(7);
        Controllerstore *controlSetting = new Controllerstore("screen", autoName, controller_name, image_name, "0", "0", "0", XPos, YPos, font_name);
        controlVec.append(controlSetting);
    }
    if (isScrew_setting == true)
    {
        QString autoName = portVec.at(4);
        QString controller_name = portVec.at(5);
        QString XPos = portVec.at(2);
        QString YPos = portVec.at(3);
        QString image_name = portVec.at(6);
        Controllerstore *controlSetting = new Controllerstore("screw", autoName, controller_name, image_name, "0", "0", "0", XPos, YPos, "0");
        controlVec.append(controlSetting);
    }
    else if (isLed_setting == true)
    {
        QString param_name = portVec.at(4);
        QString controller_name = "0";
        QString ledXPos = portVec.at(2);
        QString ledYPos = portVec.at(3);
        QString image_name = portVec.at(6);

        if (portVec.at(8) == "1")
        {
            if (portVec.at(7) == "Red")
            {
                controller_name = "ModuleLightWidget::create<TinyLight<RedLight>";
            }
            if (portVec.at(7) == "Yellow")
            {
                controller_name = "ModuleLightWidget::create<TinyLight<YellowLight>";
            }
            if (portVec.at(7) == "Blue")
            {
                controller_name = "ModuleLightWidget::create<TinyLight<BlueLight>";
            }
            if (portVec.at(7) == "Green")
            {
                controller_name = "ModuleLightWidget::create<TinyLight<GreenLight>";
            }
        }
        if (portVec.at(8) == "2")
        {
            if (portVec.at(7) == "Red")
            {
                controller_name = "ModuleLightWidget::create<SmallLight<RedLight>";
            }
            if (portVec.at(7) == "Yellow")
            {
                controller_name = "ModuleLightWidget::create<SmallLight<YellowLight>";
            }
            if (portVec.at(7) == "Blue")
            {
                controller_name = "ModuleLightWidget::create<SmallLight<BlueLight>";
            }
            if (portVec.at(7) == "Green")
            {
                controller_name = "ModuleLightWidget::create<SmallLight<GreenLight>";
            }
        }
        if (portVec.at(8) == "3")
        {
            if (portVec.at(7) == "Red")
            {
                controller_name = "ModuleLightWidget::create<MediumLight<RedLight>";
            }
            if (portVec.at(7) == "Yellow")
            {
                controller_name = "ModuleLightWidget::create<MediumLight<YellowLight>";
            }
            if (portVec.at(7) == "Blue")
            {
                controller_name = "ModuleLightWidget::create<MediumLight<BlueLight>";
            }
            if (portVec.at(7) == "Green")
            {
                controller_name = "ModuleLightWidget::create<MediumLight<GreenLight>";
            }
        }
        if (portVec.at(8) == "4")
        {
            if (portVec.at(7) == "Red")
            {
                controller_name = "ModuleLightWidget::create<LargeLight<RedLight>";
            }
            if (portVec.at(7) == "Yellow")
            {
                controller_name = "ModuleLightWidget::create<LargeMediumLight<YellowLight>";
            }
            if (portVec.at(7) == "Blue")
            {
                controller_name = "ModuleLightWidget::create<LargeMediumLight<BlueLight>";
            }
            if (portVec.at(7) == "Green")
            {
                controller_name = "ModuleLightWidget::create<LargeMediumLight<GreenLight>";
            }
        }
        Controllerstore *controlSetting = new Controllerstore("led", param_name, controller_name, image_name, "0", "0", "0", ledXPos, ledYPos, "0");
        controlVec.append(controlSetting);
    }

    else if (isKnob_setting == true)
    {
            QString param_name = portVec.at(4);
            QString controller_name = portVec.at(5);
            QString image_name = portVec.at(6);
            QString low_value = portVec.at(7);
            QString high_value = portVec.at(8);
            QString default_value = portVec.at(9);
            QString ledXPos = portVec.at(2);
            QString ledYPos = portVec.at(3);
            QString shadowImageName = portVec.at(10);

        Controllerstore *controlSetting = new Controllerstore("knob", param_name, controller_name, image_name, low_value, high_value, default_value, ledXPos, ledYPos, shadowImageName);
        controlVec.append(controlSetting);
    }
    else if (isButton_setting == true)
    {
            QString param_name = portVec.at(4);
            QString controller_name = portVec.at(5);
            QString image_name = portVec.at(6);
            QString low_value = portVec.at(7);
            QString high_value = portVec.at(8);
            QString default_value = portVec.at(9);
            QString ledXPos = portVec.at(2);
            QString ledYPos = portVec.at(3);

        Controllerstore *controlSetting = new Controllerstore("button", param_name, controller_name, image_name, low_value, high_value, default_value, ledXPos, ledYPos, "0");
        controlVec.append(controlSetting);
    }
    else if (isSwitch_setting == true)
    {
            QString param_name = portVec.at(4);
            QString controller_name = portVec.at(5);
            QString image_name = portVec.at(6);
            QString low_value = portVec.at(7);
            QString high_value = portVec.at(8);
            QString default_value = portVec.at(9);
           // QString switch_positions_value = portVec.at(10);
            QString ledXPos = portVec.at(2);
            QString ledYPos = portVec.at(3);

        Controllerstore *controlSetting = new Controllerstore("switch", param_name, controller_name, image_name, low_value, high_value, default_value, ledXPos, ledYPos, "0");
        controlVec.append(controlSetting);
    }
    else if (isRotarySwitch_setting == true)
    {
            QString param_name = portVec.at(4);
            QString controller_name = portVec.at(5);
            QString image_name = portVec.at(6);
            QString low_value = portVec.at(7);
            QString high_value = portVec.at(8);
            QString default_value = portVec.at(9);
            QString shadow_image_name = portVec.at(10);
            QString ledXPos = portVec.at(2);
            QString ledYPos = portVec.at(3);

        Controllerstore *controlSetting = new Controllerstore("rotary_switch", param_name, controller_name, image_name, low_value, high_value, default_value, ledXPos, ledYPos, shadow_image_name);
        controlVec.append(controlSetting);
    }
    else if (isInput_setting == true)
    {
            QString param_name = portVec.at(4);
            QString controller_name = portVec.at(5);
            QString image_name = portVec.at(6);
            QString ledXPos = portVec.at(2);
            QString ledYPos = portVec.at(3);

        Controllerstore *controlSetting = new Controllerstore("input", param_name, controller_name, image_name, "0", "0", "0", ledXPos, ledYPos, "0");
        controlVec.append(controlSetting);
    }
    else if (isOuput_setting == true)
    {
            QString param_name = portVec.at(4);
            QString controller_name = portVec.at(5);
            QString image_name = portVec.at(6);
            QString ledXPos = portVec.at(2);
            QString ledYPos = portVec.at(3);

        Controllerstore *controlSetting = new Controllerstore("output", param_name, controller_name, image_name, "0", "0", "0", ledXPos, ledYPos, "0");
        controlVec.append(controlSetting);
    }
    CustomDialog d("menu");
    for (int i = 0 ; i<controlVec.size(); i++)
    {
        Controllerstore *control_setting;
        control_setting = controlVec.at(i);
        QString paramName = control_setting->param_name();
        QString typeName = control_setting->type();
        d.addLabel(typeName);
        d.addLabel(paramName);
    }
    //d.exec();
}


void Canvawrite::setMainModuleAttributes(QVector<QString> portVec)
{
    bool isPlugin_setting = false;
    bool isPanel_setting = false;

    for (int i = 0; i < portVec.size(); i++)
    {
        if (portVec.at(i) == "plugin_setting")
        {
            isPlugin_setting = true;
        }
        else if (portVec.at(i) == "smart_panel")
        {
            isPanel_setting = true;
        }
    }
   if (isPlugin_setting == true)
   {
       manufacturer = portVec.at(2);
       slug = portVec.at(3);
       version= portVec.at(4);
       tags = portVec.at(5);
       plugin_file_name = portVec.at(6);
       modules_files_names = portVec.at(7);
       QString current_module_file_nameT = modules_files_names;
       current_module_file_nameT.truncate(current_module_file_nameT.lastIndexOf(QChar(',')));
       if ( current_module_file_nameT != 0)
       {
          current_module_file_name = current_module_file_nameT;
       }
       else
       {
          current_module_file_name = modules_files_names;
       }
       module_name = portVec.at(8);
       IDName = portVec.at(9);
       website = portVec.at(10);
   }
   else if (isPanel_setting == true)
   {
       QString pngFileName = portVec.at(2);
       panelWidth = portVec.at(3);
       pngFileName.truncate(pngFileName .lastIndexOf(QChar('.')));
       panel_image_name = pngFileName ;
   }
}

void Canvawrite::writePluginCPP()
{

    QDir fileDir(QDir::currentPath());
    fileDir.cd(plugDir);
    QString fileDirPath = fileDir.absoluteFilePath(plugDir);
    QFile file(fileDirPath);
    if (!QFile(plugDir + "/"+ plugin_file_name + "/Makefile").exists())
    {
        QString Makefilename = plugDir + "/"+ plugin_file_name + "/Makefile";
        QFile makeFile(Makefilename);
        if ( makeFile.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &makeFile);

            stream << "RACK_DIR ?= ../..\n\n";
            stream << "SLUG = " + slug + "\n";
            stream << "VERSION = " + version + "\n";
            stream << "SOURCES = $(wildcard src/*.cpp) $(wildcard src/*.c)\n";
            stream << "\n\n";
            stream << "DISTRIBUTABLES += $(wildcard LICENSE*) res\n" ;
            stream << "include $(RACK_DIR)/plugin.mk\n" ;
            stream << "include $(RACK_DIR)/arch.mk\n" ;
            int FileDescriptor = makeFile.handle();
            FILE* fh = fdopen(FileDescriptor, "Makefile");
            fclose(fh);
            makeFile.close();
        }
    }
    if (!QDir(plugDir + "/"+ plugin_file_name).exists())
    {
        fileDir.mkdir( plugin_file_name);
    }
    if (!QDir(plugDir + "/"+ plugin_file_name + "/src").exists())
    {
        QDir srcDir;
        srcDir.cd(plugDir + "/"+ plugin_file_name);
        srcDir.mkdir("src");
    }
    if (!QFile(plugDir + "/"+ plugin_file_name + "/src/" + plugin_file_name + ".cpp").exists())
    {
        QString cppfilename = plugDir+ "/" + plugin_file_name + "/src/" + plugin_file_name + ".cpp";
        QFile fHeader(cppfilename);
        if ( fHeader.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &fHeader );
            QString pn = plugin_file_name;
            stream << "#include \"" + pn + ".hpp\"" << endl;
            stream << "#include <math.h>" << endl;
            stream << "    float getSampleRate()\n    {\n        return engineGetSampleRate();\n    }\n"<< endl;
            stream << "Plugin *plugin;" << endl;
            stream << "void init(rack::Plugin *p) {" << endl;
            stream << "    plugin = p;" << endl;
            stream << "    p->slug = \"" + slug + "\";" << endl;
            stream << "#ifdef VERSION" << endl;
            stream << "    p->version = TOSTRING(VERSION);" << endl;
            stream << "#endif\n" << endl;
            QStringList moduleList;
            moduleList = modules_files_names.split(',');
            for (int i=0; i<moduleList.size(); i++)
            {
                stream << "    p->addModel(model" + moduleList.at(i) +");" << endl;
            }
            stream << "}" << endl;
            fHeader.close();
        }
    }
    else
    {
        QString fileToWriteInName = plugDir + "/"+ plugin_file_name + "/src/" + plugin_file_name + ".cpp";
        QDir fileToWriteInDir(QDir::currentPath());
        fileToWriteInDir.cd(fileToWriteInName);
        QString fileToWriteInDirPath = fileToWriteInDir.absoluteFilePath(fileToWriteInName);
        QFile FileToWriteIn(fileToWriteInDirPath);
        bool wasModel = false;
        if (FileToWriteIn.open(QIODevice::ReadOnly)) {
            QTextStream streamFTW(&FileToWriteIn);
            while (!streamFTW.atEnd())
            {
                CPPFileToWriteInList.append(streamFTW.readLine());
            }
            FileToWriteIn.close();
        }
        QFile FileToWriteInReopen(fileToWriteInDirPath);
        if (FileToWriteIn.open(QIODevice::ReadWrite))
        {
            QTextStream stream( &FileToWriteIn );
            for (int l = 0; l < CPPFileToWriteInList.size(); l++)
            {
                QString line = CPPFileToWriteInList.at(l);

                if (line.contains("p->addModel(", Qt::CaseSensitive))
                {
                    wasModel = true;
                }
                if (!line.contains("p->addModel(", Qt::CaseSensitive) && wasModel == true && !line.contains("p->addModel(" + current_module_file_name, Qt::CaseSensitive))
                {
                    stream << "    p->addModel(model" + current_module_file_name +");" << endl;
                    wasModel = false;
                }
                stream << line << endl;
            }
            FileToWriteIn.close();
        }
    }
}

void Canvawrite::writeModuleCanva()
{
   ModuleCanvaWriter *MCW = new  ModuleCanvaWriter(current_module_file_name, plugin_file_name, plugDir);
   MCW->writeModuleStruct(controlVec, plugin_file_name);
   MCW->writeStepFunctionCanva();
   MCW->writeWidgetDeclar(controlVec, panelWidth, panel_image_name, plugin_file_name);
   MCW->writeModuleModel(IDName, module_name, manufacturer, tags);
}

void Canvawrite::writeHeader()
{
    QDir fileDir(QDir::currentPath());
    fileDir.cd(plugDir);
    QString fileDirPath = fileDir.absoluteFilePath(plugDir);
    QFile file(fileDirPath);
    QDir pDIr(plugDir + "/"+ plugin_file_name);
    if (!pDIr.exists())
    {
        fileDir.mkdir( plugin_file_name);
    }
    else
    {
        bool *exportInPlug = new bool(false);
        CustomDialog dial("Export Options:");
        dial.addCheckBox("export this module to the existing plugin? (if unchecked the existing plugin will be deleted)", exportInPlug, "checkbox");
        dial.exec();
        if (*exportInPlug == false)
        {
            pDIr.removeRecursively();
            fileDir.mkdir( plugin_file_name);

        }
    }
    if (!QDir(plugDir + "/"+ plugin_file_name + "/src").exists())
    {
        QDir srcDir;
        srcDir.cd(plugDir + "/"+ plugin_file_name);
        srcDir.mkdir("src");
    }
    if (!QFile(plugDir + "/"+ plugin_file_name + "/src/" + plugin_file_name + ".hpp").exists())
    {
        QString headername = plugDir + "/"+ plugin_file_name + "/src/" + plugin_file_name + ".hpp";
        //headername = plugin_file_name;
        //headername.append(".hpp");
        QFile fHeader(headername);

        if (fHeader.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&fHeader);
            stream << "#include \"rack.hpp\" \n" << endl;
            stream << "using namespace rack;" << endl;
            stream << "extern Plugin *plugin;" << endl;
            QStringList moduleList;
            moduleList = modules_files_names.split(',');
            for (int i=0; i<moduleList.size(); i++)
            {
                stream << "extern Model *model" + moduleList.at(i) +";" << endl;
            }
            stream << "" << endl;

            for (int i = 0; i <controlVec.size(); i++)
            {
                Controllerstore *control_ = controlVec.at(i);
                 bool writed = false;
                for(int j = 0; j < writedWidgetVec.size(); j++)
                {
                    if (control_->controller_name() == writedWidgetVec.at(j))
                    {
                        writed = true;
                    }
                }
                if (writed == false)
                {
                    writedWidgetVec.append(control_->controller_name());
                    controlVecWoDoublons.append(control_);
                }
            }
            writeControllerDef(stream);
            transferImageFiles();
            fHeader.close();
        }
    }
    else
    {
        for (int i = 0; i <controlVec.size(); i++)
        {
            Controllerstore *control_ = controlVec.at(i);
             bool writed = false;
            for(int j = 0; j < writedWidgetVec.size(); j++)
            {
                if (control_->controller_name() == writedWidgetVec.at(j))
                {
                    writed = true;
                }
            }
            if (writed == false)
            {
                writedWidgetVec.append(control_->controller_name());
                controlVecWoDoublons.append(control_);
            }
        }
        QString fileToWriteInName = plugDir + "/"+ plugin_file_name + "/src/" + plugin_file_name + ".hpp";
        QDir fileToWriteInDir(QDir::currentPath());
        fileToWriteInDir.cd(fileToWriteInName);
        QString fileToWriteInDirPath = fileToWriteInDir.absoluteFilePath(fileToWriteInName);
        QFile FileToWriteIn(fileToWriteInDirPath);
        bool wasModel = false;
        if (FileToWriteIn.open(QIODevice::ReadOnly)) {
            QTextStream streamFTW(&FileToWriteIn);
            while (!streamFTW.atEnd())
            {
                FileToWriteInList.append(streamFTW.readLine());
            }
            FileToWriteIn.close();
        }
        QFile FileToWriteInReopen(fileToWriteInDirPath);
        if (FileToWriteIn.open(QIODevice::ReadWrite))
        {
            QTextStream stream( &FileToWriteIn );
            for (int l = 0; l < FileToWriteInList.size(); l++)
            {
                QString line = FileToWriteInList.at(l);

                if (line.contains("extern Model *", Qt::CaseSensitive))
                {
                    wasModel = true;
                }
                if (!line.contains("extern Model *", Qt::CaseSensitive) && wasModel == true && !line.contains("extern Model *model" + current_module_file_name, Qt::CaseSensitive))
                {
                    stream << "extern Model *model" + current_module_file_name +";" << endl;
                    wasModel = false;
                }
                stream << line << endl;
                for (int j = 0; j <controlVecWoDoublons.size(); j++)
                {
                    Controllerstore *control = controlVecWoDoublons.at(j);
                    if (line.contains(control->controller_name(),Qt::CaseSensitive))
                    {
                        controlVecWoDoublons.remove(j);
                    }
                }
            }
            writeControllerDef(stream);
            transferImageFiles();
            FileToWriteIn.close();
        }
    }
}
void Canvawrite::transferImageFiles()
{
    if (!QFile(plugDir + "/" + plugin_file_name + "/" + "res/" + panel_image_name).exists())
    {
        QString newImagePath = plugDir + "/" + plugin_file_name + "/" + "res/" + panel_image_name + ".svg";
        QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
        QString fontLoc = docLocation + "Geco/Images/" + panel_image_name + ".svg";
        QDir rootDir(QDir::currentPath());
        QString fileDirPath = rootDir.relativeFilePath(fontLoc);
        QFile::copy(fileDirPath, newImagePath);
    }
    for (int i = 0; i <controlVecWoDoublons.size(); i++)
    {
        Controllerstore *control = controlVecWoDoublons.at(i);
        QString imageTotransfered_name = control->imageName();
        QString shadow_imageTotransfered_name = control->shadowImageName();
        QString controllerType = control->type();
        QDir fileDir(QDir::currentPath());
        fileDir.cd(plugDir + "/"+ plugin_file_name);
        QString fileDirPath = fileDir.absoluteFilePath(plugDir + "/"+ plugin_file_name);
        QFile file(fileDirPath);

        QString imageNameSt = control->imageName();
        QString positionsS = control->switch_positions();
        QString cutSwitchName;
        imageNameSt.truncate(imageNameSt.lastIndexOf(QChar('_')));
        if (imageNameSt != 0)
        {
            cutSwitchName = imageNameSt;
        }
        else
        {
            cutSwitchName = control->imageName();
        }
        int posN = positionsS.toInt();

        QString imageNameB;
        QString imageNameBt = control->imageName();
        imageNameBt.truncate(imageNameBt.lastIndexOf(QChar('.')));
        if (imageNameBt != 0)
        {
            imageNameB = imageNameBt;
        }
        else
        {
            imageNameB = control->imageName();
        }

        if (!QDir(plugDir + "/" + plugin_file_name + "/res").exists())
        {
            fileDir.mkdir("res");
        }

        if (controllerType == "screen" && fontFileTransfered == false)
        {
            /*QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
            QString destinationPath= desktopPath+QDir::separator()+fileName;
            QFile::copy(filePath, destinationPath)*/


            fontFileTransfered = true;
            QString newFontPath = plugDir + "/" + plugin_file_name + "/" + "res/" + shadow_imageTotransfered_name;
            QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
            QString fontLoc = docLocation + "Geco/Fonts/"+ shadow_imageTotransfered_name;
            QDir rootDir(QDir::currentPath());
            QString fileDirPath = rootDir.relativeFilePath(fontLoc);
            QFile::copy(fileDirPath, newFontPath);

        }
        if (!QFile(plugDir + "/" + "/" + plugin_file_name + "/" + "res/" + imageTotransfered_name).exists())
        {

            if (controllerType == "switch")
            {
                for (int k = 0; k < posN; k++)
                {
                    QString newImagePath = plugDir + "/" + plugin_file_name + "/" + "res/" + cutSwitchName + "_" + QString::number(k) + ".svg";
                    QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
                    QString fontLoc = docLocation + "Geco/Images/"+ cutSwitchName + "_" + QString::number(k) + ".svg";
                    QDir rootDir(QDir::currentPath());
                    QString fileDirPath = rootDir.relativeFilePath(fontLoc);
                    QFile::copy(fileDirPath, newImagePath);
                }
            }
            else if (controllerType == "button")
            {
                QString newImagePath = plugDir + "/" + plugin_file_name + "/" + "res/" + imageTotransfered_name;
                QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
                QString fontLoc = docLocation + "Geco/Images/"+ imageTotransfered_name;
                QDir rootDir(QDir::currentPath());
                QString fileDirPath = rootDir.relativeFilePath(fontLoc);
                QFile::copy(fileDirPath, newImagePath);
                QString newPushedImagePath = plugDir + "/" + plugin_file_name + "/" + "res/" + imageNameB + "Pushed.svg";
                QString docLocation2 = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
                QString fontLoc2 = docLocation + "Geco/Images/"+ imageNameB + "Pushed.svg";
                QDir rootDir2(QDir::currentPath());
                QString fileDirPath2 = rootDir.relativeFilePath(fontLoc);
                QFile::copy(fileDirPath2, newPushedImagePath);
            }
            else
            {
                QString newImagePath = plugDir + "/" + plugin_file_name + "/" + "res/" + imageTotransfered_name;
                QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
                QString fontLoc = docLocation + "Geco/Images/"+ imageTotransfered_name;
                QDir rootDir(QDir::currentPath());
                QString fileDirPath = rootDir.relativeFilePath(fontLoc);
                QFile::copy(fileDirPath, newImagePath);
            }
        }
        if (!QFile(plugDir + "/" + plugin_file_name + "/" + "res/" + panel_image_name + ".svg").exists())
        {
            QString newPanelImagePath = plugDir + "/" + plugin_file_name + "/" + "res/" + panel_image_name + ".svg";
            QString newImagePath = plugDir + "/" + plugin_file_name + "/" + "res/" + imageTotransfered_name;
            QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
            QString fontLoc = docLocation + "Geco/Images/"+ panel_image_name + ".svg";
            QDir rootDir(QDir::currentPath());
            QString fileDirPath = rootDir.relativeFilePath(fontLoc);
            QFile::copy(fileDirPath, newPanelImagePath);
        }
        if (!QFile(plugDir + "/" + plugin_file_name + "/" + "res/" + shadow_imageTotransfered_name).exists() && shadow_imageTotransfered_name != "0")
        {
            QString newShadowImagePath = plugDir + "/" + plugin_file_name + "/" + "res/" + shadow_imageTotransfered_name;
            QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
            QString fontLoc = docLocation + "Geco/Images/"+ shadow_imageTotransfered_name;
            QDir rootDir(QDir::currentPath());
            QString fileDirPath = rootDir.relativeFilePath(fontLoc);
            QFile::copy(fileDirPath, newShadowImagePath);
        }
    }
}
void Canvawrite::writeControllerDef(QTextStream &stream)
{
    for (int i = 0; i <controlVecWoDoublons.size(); i++)
    {
        Controllerstore *control = controlVecWoDoublons.at(i);

        if (control->type() == "knob")
        {
            QString controlNameK = control->controller_name();
            QString controlposXK = control->x_pos();
            QString controlposYK = control->y_pos();
            QString imageNameK;
            QString imageNameKt = control->imageName();
            QString shadow_image_name = control->shadowImageName();
            imageNameKt.truncate(imageNameKt.lastIndexOf(QChar('.')));
            if (imageNameKt != 0)
            {
                imageNameK = imageNameKt;
            }
            else
            {
                imageNameK = control->imageName();
            }
            stream << "struct " + controlNameK + " : SVGKnob{"  << endl;
            stream << controlNameK + "() {" << endl;
            stream << "    box.size = Vec(25, 25);" << endl; // the size won't be necessary on v1.0 PAPI
            stream << "    minAngle = -0.75*M_PI;" << endl;
            stream << "    maxAngle = 0.75*M_PI;" << endl;
            stream << "    setSVG(SVG::load(assetPlugin(plugin,\"res/" + imageNameK + ".svg\")));"<< endl;
            stream << "    SVGWidget *shadow = new SVGWidget();" << endl;
            stream << "    shadow->setSVG(SVG::load(assetPlugin(plugin, \"res/" + shadow_image_name + "\")));" << endl;
            stream << "    addChild(shadow);" << endl;
            stream << "    }" << endl;
            stream << "};" << endl;
        }
        else if (control->type() == "input" || control->type() == "output")
        {

            QString controlNameJ = control->controller_name();
            QString controlposXJ = control->x_pos();
            QString controlposYJ = control->y_pos();
            QString imageNameJ;
            QString imageNameJt = control->imageName();
            imageNameJt.truncate(imageNameJt.lastIndexOf(QChar('.')));
            if (imageNameJt != 0)
            {
                imageNameJ = imageNameJt;
            }
            else
            {
                imageNameJ = control->imageName();
            }
            stream << "struct " + controlNameJ + " : SVGPort{"  << endl;
            stream << controlNameJ + "() {" << endl;
            stream << "    background->svg = SVG::load(assetPlugin(plugin,\"res/" + imageNameJ + ".svg\"));"<< endl;
            stream << "    background->wrap();" << endl;
            stream << "    box.size = background->box.size;" << endl;
            stream << "    }" << endl;
            stream << "};" << endl;
        }

        else if (control->type() == "screen")
        {

        }

        else if (control->type() == "button")
        {
            QString controlNameB = control->controller_name();
            QString controlposXB = control->x_pos();
            QString controlposYB = control->y_pos();
            QString imageNameB;
            QString imageNameBt = control->imageName();
            imageNameBt.truncate(imageNameBt.lastIndexOf(QChar('.')));
            if (imageNameBt != 0)
            {
                imageNameB = imageNameBt;
            }
            else
            {
                imageNameB = control->imageName();
            }
            stream << "struct " + controlNameB + " : SVGSwitch, MomentarySwitch {"  << endl;
            stream << "    " + controlNameB + "(){"<< endl;
            stream << "    addFrame(SVG::load(assetPlugin(plugin,\"res/" + imageNameB + ".svg\")));"<< endl;
            stream << "    addFrame(SVG::load(assetPlugin(plugin,\"res/" + imageNameB + "Pushed.svg\")));"<< endl;
            stream << "    sw->wrap();"  << endl;
            stream << "    box.size = sw->box.size;"  << endl;
            stream << "    }" << endl;
            stream << "};" << endl;
        }
        else if (control->type() == "switch")
        {
            QString controlNameS = control->controller_name();
            QString controlposXS = control->x_pos();
            QString controlposYS = control->y_pos();
            QString imageNameS;
            QString imageNameSt = control->imageName();
            QString positionsS = control->switch_positions();
            imageNameSt.truncate(imageNameSt.lastIndexOf(QChar('_')));
            if (imageNameSt != 0)
            {
                imageNameS = imageNameSt;
            }
            else
            {
                imageNameS = control->imageName();
            }
            stream << "struct " + controlNameS + " : SVGSwitch, ToggleSwitch {"  << endl;
            stream << controlNameS + "() {" << endl;
            int posN = positionsS.toInt();
                for (int p = 0; p < posN ; p++)
                {
                    stream << "    addFrame(SVG::load(assetPlugin(plugin,\"res/" + imageNameS + "_" + QString::number(p) + ".svg\")));"<< endl;
                }
            stream << "    sw->wrap();"  << endl;
            stream << "    box.size = sw->box.size;"  << endl;
            stream << "    }" << endl;
            stream << "};" << endl;
        }
        else if (control->type() == "rotary_switch")
        {
            QString controlNameRS = control->controller_name();
            QString controlposXRS = control->x_pos();
            QString controlposYRS = control->y_pos();
            QString imageNameRS;
            QString imageNameRSt = control->imageName();
            QString r_shadow_image_name = control->shadowImageName();
            imageNameRSt.truncate(imageNameRSt.lastIndexOf(QChar('.')));
            if (imageNameRSt != 0)
            {
                imageNameRS = imageNameRSt;
            }
            else
            {
                imageNameRS = control->imageName();
            }
            stream << "struct " + controlNameRS + " : SVGKnob{"  << endl;
            stream << controlNameRS + "() {" << endl;
            stream << "    box.size = Vec(25, 25);" << endl; // the size won't be necessary on v1.0 PAPI
            stream << "    minAngle = -0.75*M_PI;" << endl;
            stream << "    maxAngle = 0.75*M_PI;" << endl;
            stream << "    setSVG(SVG::load(assetPlugin(plugin,\"res/" + imageNameRS + ".svg\")));"<< endl;
            stream << "    SVGWidget *shadow = new SVGWidget();" << endl;
            stream << "    shadow->setSVG(SVG::load(assetPlugin(plugin, \"res/" + r_shadow_image_name + "\")));" << endl;
            stream << "    addChild(shadow);" << endl;
            stream << "    }" << endl;
            stream << "};" << endl;
            stream << "struct " + controlNameRS + "_snap : "+ controlNameRS + "{"  << endl;
            stream << "    " + controlNameRS + "_snap() {" << endl;
            stream << "    snap = true;"<< endl;
            stream << "    }" << endl;
            stream << "};" << endl;
        }
        else if (control->type() == "screw")
        {
            QString controlNameSc = control->controller_name();
            QString controlposXSc = control->x_pos();
            QString controlposYSc = control->y_pos();
            QString imageNameSc;
            QString imageNameSct = control->imageName();
            imageNameSct.truncate(imageNameSct.lastIndexOf(QChar('.')));
            if (imageNameSct != 0)
            {
                imageNameSc = imageNameSct;
            }
            else
            {
                imageNameSc = control->imageName();
            }
            stream << "struct " + controlNameSc + " : SVGScrew {"  << endl;
            stream << controlNameSc + "() {" << endl;
            stream << "    sw->svg = SVG::load(assetPlugin(plugin,\"res/" + imageNameSc + ".svg\"));"<< endl;
            stream << "    sw->wrap();" << endl;
            stream << "    box.size = sw->box.size;" << endl;
            stream << "    }" << endl;
            stream << "};" << endl;
        }
    }
}
QString Canvawrite::getManufacturer()
{
    return manufacturer;
}
QString Canvawrite::getPlugin_name()
{
    return plugin_file_name;
}
QString Canvawrite::getModule_name()
{
    return module_name;
}
QString Canvawrite::getSlug()
{
    return slug;
}
QString Canvawrite::getVersion()
{
    return version;
}
QString Canvawrite::getTags()
{
    return tags;
}
QString Canvawrite::getPanel_image_name()
{
    return panel_image_name;
}
QVector<Controllerstore*> Canvawrite::getControlVec()
{
    return controlVec;
}
