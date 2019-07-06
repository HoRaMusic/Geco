#include "Headers/modulecanvawriter.h"
#include <QDir>
ModuleCanvaWriter::ModuleCanvaWriter(QString module_name, QString plugin_file_name, QString _plugDir, bool bigDPI)
{
    dpi96 = bigDPI;
    moduleName = module_name;
    moduleFilename = module_name;
    plugDir = _plugDir;
    QDir fileDir(QDir::currentPath());
    fileDir.cd(plugDir);
    QString fileDirPath = fileDir.absoluteFilePath(plugDir);
    QFile file(fileDirPath);
    if (!QDir(plugDir + "/" + plugin_file_name).exists())
    {
        fileDir.mkdir( plugin_file_name);
    }
    if (!QDir(plugDir + "/"  + plugin_file_name + "/src").exists())
    {
        QDir srcDir;
        srcDir.cd(plugDir + "/"  + plugin_file_name);
        srcDir.mkdir("src");
    }
    QString cppfilename = plugDir + "/"  + plugin_file_name + "/src/" + moduleName + "Can.cpp";

    moduleFile.setFileName(cppfilename);

    if ( moduleFile.open(QIODevice::ReadWrite))
    {
        TStream.setDevice(&moduleFile);
    }
}

void ModuleCanvaWriter::writeModuleStruct(QVector<Controllerstore*> &controllerVec, QString pluginName)
{
    QVector<Controllerstore*> *Qv = &controllerVec;

    TStream << "#include \"" + pluginName + ".hpp\"\n" << endl;
    TStream << "struct " + moduleName + " : Module {" << endl;
    bool param = false;
    bool input = false;
    bool output = false;
    bool light = false;
    bool screenUsed = false;
    for( int i = 0; i< Qv->size(); i++)
    {
        Controllerstore *scontrol_ = Qv->at(i);
        if (scontrol_->type() == "screen")
        {
            screenUsed = true;
            TStream << "    char " + scontrol_->controller_name() + "_" + moduleName + "_lineMessage1[100] = \"\";"<< endl;
            TStream << "    char " + scontrol_->controller_name() + "_" + moduleName + "_lineMessage2[100] = \"\";"<< endl;
        }
    }

    for( int i = 0; i< Qv->size(); i++)
    {
         Controllerstore *control_ = Qv->at(i);
         QString cName = control_->controller_name();
         QString pName = control_->param_name();
         QString xPos = control_->x_pos();
         QString yPos = control_->y_pos();
         QString minVal = control_->low_value();
         QString maxVal = control_->high_value();
         QString defaultVal = control_->default_value();
         if (control_->type() == "knob" || control_->type() == "button" || control_->type() == "switch" || control_->type() == "rotary_switch")
         {
             if (param == false)
             {
                 TStream << "    enum ParamIds {" << endl;
             }
             param = true;
             TStream << "           " + control_->param_name() + "," << endl;
         }
    }
    if (param == true)
    {
        TStream << "        NUM_PARAMS\n    };" << endl;
    }
    else
    {
        TStream << "    enum ParamIds {" << endl;
        TStream << "        NUM_PARAMS " << endl;
        TStream << "    };" << endl;
    }
    for( int j = 0; j< Qv->size(); j++)
    {
         Controllerstore *control_ = Qv->at(j);
         if (control_->type() == "input")
         {
             if (input == false)
             {
                 TStream << "    enum InputIds {" << endl;
             }
             input = true;
             TStream << "          " + control_->param_name() + "," << endl;
         }
    }
    if (input == true)
    {
        TStream << "        NUM_INPUTS\n    };" << endl;
    }
    else
    {
        TStream << "    enum InputIds {" << endl;
        TStream << "        NUM_INPUTS" << endl;
        TStream << "    };" << endl;
    }
    for( int k = 0; k< Qv->size(); k++)
    {
         Controllerstore *control_ = Qv->at(k);
         if (control_->type() == "output")
         {
             if (output == false)
             {
                 TStream << "    enum OutputIds {" << endl;
             }
             output = true;
             TStream << "          " + control_->param_name() + "," << endl;
         }
    }
    if (output == true)
    {
        TStream << "        NUM_OUTPUTS\n    };" << endl;
    }
    else
    {
        TStream << "    enum OutputIds {" << endl;
        TStream << "        NUM_OUTPUTS " << endl;
        TStream << "    };" << endl;
    }
    for( int k = 0; k< Qv->size(); k++)
    {
         Controllerstore *control_ = Qv->at(k);
         if (control_->type() == "led")
         {
             if (light == false)
             {
                 TStream << "    enum LightIds {" << endl;
             }
             light = true;
             TStream << "          " + control_->param_name() + "," << endl;
         }
    }
    if (light == true)
    {
        TStream << "        NUM_LIGHTS\n    };" << endl;
    }
    else
    {
        TStream << "    enum LightIds {" << endl;
        TStream << "        NUM_LIGHTS " << endl;
        TStream << "    };" << endl;
    }
  /*  if (screenUsed == false)
    {
        TStream << "    " + moduleName + "();" << endl;
    }
    else
    {
        TStream << "    " + moduleName + "() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}" << endl;
    }*/
    TStream << "    void process(const ProcessArgs &args)override;" << endl;

    /*LDrum()
       {
           config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
           configParam(kno_158_PARAM, 0.0f, 0.7f, 0.0f);
           configParam(rot_129_PARAM, 0.0f, 4.0f, 0.0f);
           configParam(but_101_PARAM, 0.0f, 10.0f, 0.0f);
           configParam(kno_100_PARAM, 0.0f, 6.0f, 3.0f);
           configParam(rot_99_PARAM, 0.0f, 4.0f, 0.0f);
           configParam(LOFI_PARAM, 0.0f, 1.0f, 0.0f);
       }*/

        TStream <<moduleName + "() {" << endl;
        TStream << "    config(";
        if (param == true)
        {
            TStream << "NUM_PARAMS";
        }
        if (input == true)
        {
            if (param == true)
            {
                TStream << ", NUM_INPUTS";
            }
            else
            {
                TStream << "NUM_INPUTS";
            }

        }
        if (output == true)
        {
            if (param == true || input == true)
            {
                TStream << ", NUM_OUTPUTS";
            }
            else
            {
                TStream << "NUM_OUTPUTS";
            }
        }
        if (light == false)
        {
            if (param == true || output == true || input == true)
            {
                TStream << ", NUM_LIGHTS";
            }
            else
            {
                TStream << "NUM_LIGHTS";
            }
        }
        TStream << ");" << endl;
        for( int i = 0; i< Qv->size(); i++)
        {
            Controllerstore *control_ = Qv->at(i);
            QString cName = control_->controller_name();
            QString pName = control_->param_name();
            QString xPos = control_->x_pos();
            QString yPos = control_->y_pos();
            QString minVal = control_->low_value();
            QString maxVal = control_->high_value();
            QString defaultVal = control_->default_value();
            if (control_->type() == "rotary_switch" || control_->type() == "knob" || control_->type() == "button" || control_->type() == "switch")
            {
                TStream << "    configParam(" + pName + ", " + minVal + ", " + maxVal  + ", " + defaultVal + ");"<< endl;
            }
        }

        TStream << "}" << endl;
        TStream << "};\n" << endl;
}

void ModuleCanvaWriter::writeStepFunctionCanva()
{
    TStream << "void " + moduleName + "::process(const ProcessArgs &args) {\n\n}" << endl;
}

void ModuleCanvaWriter::writeWidgetDeclar(QVector<Controllerstore*> &controllerVec, QString panelWidth, QString panelName, QString pluginName)
{
     QVector<Controllerstore*> *Qv = &controllerVec;
     TStream << "struct " + moduleName + "Widget : ModuleWidget {" << endl;
     TStream << "   " + moduleName + "Widget(" + moduleName + " *module);"  << endl;
     TStream << "};\n" << endl;
     TStream << moduleName + "Widget::" + moduleName + "Widget(" + moduleName + " *module) {" << endl;
     TStream << "   setModule(module);" << endl;
     TStream << "   box.size = Vec(" + panelWidth + ", 380);" << endl;
     TStream << "   {" << endl;
     TStream << "       SvgPanel *panel = new SvgPanel();" << endl;
     TStream << "       panel->box.size = box.size;" << endl;
     TStream << "       panel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, \"res/" + panelName + ".svg\")));" << endl;
     TStream << "       addChild(panel);" << endl;
     TStream << "    }" << endl;

     for( int i = 0; i< Qv->size(); i++)
     {
        Controllerstore *control_ = Qv->at(i);
        QString cName = control_->controller_name();
        QString pName = control_->param_name();
        QString xPos = control_->x_pos();
        QString yPos = control_->y_pos();
        QString minVal = control_->low_value();
        QString maxVal = control_->high_value();
        QString defaultVal = control_->default_value();
        QString displayName = "display_" + cName;
        if (control_->type() == "screen")
        {
            TStream << "    {" << endl;
            TStream << "        " + cName + "_" + moduleName + " *" + displayName  + "= new " + cName + "_" + moduleName + "();"<< endl;
            TStream << "        "+ displayName + "->module = module;" << endl;
            TStream << "        "+ displayName + "->box.pos = Vec(" + control_->x_pos() + ", " + control_->y_pos() + ");" << endl;
            TStream << "        "+ displayName + "->box.size = Vec(234, 234);" << endl;
            TStream << "        addChild(" + displayName + ");" << endl;
            TStream << "    }" << endl;
            //TStream << "    addChild(createWidget<" + cName + ">(Vec(" + control_->x_pos() + "," + control_->y_pos() + ")));" << endl;
        }
        if (dpi96 == true)
        {
            double xpos = xPos.toDouble();
            double ypos = yPos.toDouble();
            xpos = xpos/1.2;
            ypos = ypos/1.2;
            xPos = QString::number(xpos);
            yPos = QString::number(ypos);
        }
        if (control_->type() == "screw")
        {
            TStream << "    addChild(createWidget<" + cName + ">(Vec(" + control_->x_pos() + "," + control_->y_pos() + ")));" << endl;
        }
        else if (control_->type() == "rotary_switch")
        {
            TStream << "    addParam(createParam<" + cName + "_snap>(Vec(" + xPos + "," + yPos + "), module, " + moduleName + "::" + pName + "));" << endl;
        }
        else if (control_->type() == "knob" || control_->type() == "button" || control_->type() == "switch")
        {
            TStream << "    addParam(createParam<" + cName + ">(Vec(" + xPos + "," + yPos + "), module, " + moduleName + "::" + pName + "));" << endl;
        }
        else if (control_->type() == "input")
        {
            TStream << "    addInput(createInput<" + cName + ">(Vec(" + xPos + "," + yPos + "), module, " + moduleName + "::" + pName + "));" << endl;
        }
        else if (control_->type() == "output")
        {
            TStream << "    addOutput(createOutput<" + cName + ">(Vec(" + xPos + "," + yPos + "), module, " + moduleName + "::" + pName + "));" << endl;
        }
        else if (control_->type() == "led")
        {

            TStream << "    addChild(createLight<" + cName + ">(Vec(" + xPos + "," + yPos + "), module, " +  moduleName + "::" + pName + "));" << endl;
        }
     }
     TStream << "}" << endl;
}

void ModuleCanvaWriter::writeModuleModel(QString module_name)
{
    TStream << "Model *model"  + moduleName + " = createModel<" + moduleName + "," +  moduleName + "Widget>(\"" + module_name + "\");" << endl;
    moduleFile.close();
}
//Model *modelFrequencyDivider = Model::create<FrequencyDivider, FrequencyDividerWidget>("Hora", "Frequency divider", "Frequency Divider", CLOCK_MODULATOR_TAG);
