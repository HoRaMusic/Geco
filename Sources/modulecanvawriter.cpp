#include "modulecanvawriter.h"
#include <QDir>
ModuleCanvaWriter::ModuleCanvaWriter(QString module_name, QString plugin_file_name, QString _plugDir)
{
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
    if (screenUsed == false)
    {
        TStream << "    " + moduleName + "();" << endl;
    }
    else
    {
        TStream << "    " + moduleName + "() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}" << endl;
    }
    TStream << "    void step()override;" << endl;
    TStream << "};\n" << endl;

    if (screenUsed == false)
    {
        TStream <<moduleName + "::" + moduleName + "() {" << endl;
        if (param == true)
        {
            TStream << "    params.resize(NUM_PARAMS);" << endl;
        }
        if (light == true)
        {
            TStream << "    lights.resize(NUM_LIGHTS);" << endl;
        }
        if (input == true)
        {
            TStream << "    inputs.resize(NUM_INPUTS);" << endl;
        }
        if (output == true)
        {
            TStream << "    outputs.resize(NUM_OUTPUTS);" << endl;
        }
        TStream << "}" << endl;
    }
}

void ModuleCanvaWriter::writeStepFunctionCanva()
{
    TStream << "void " + moduleName + "::step() {\n\n}" << endl;
}

void ModuleCanvaWriter::writeWidgetDeclar(QVector<Controllerstore*> &controllerVec, QString panelWidth, QString panelName, QString pluginName)
{
     QVector<Controllerstore*> *Qv = &controllerVec;
     TStream << "struct " + moduleName + "Widget : ModuleWidget {" << endl;
     TStream << "   " + moduleName + "Widget(" + moduleName + " *module);"  << endl;
     TStream << "};\n" << endl;
     TStream << moduleName + "Widget::" + moduleName + "Widget(" + moduleName + " *module) : ModuleWidget(module) {" << endl;
     TStream << "   box.size = Vec(" + panelWidth + ", 380);" << endl;
     TStream << "   {" << endl;
     TStream << "       SVGPanel *panel = new SVGPanel();" << endl;
     TStream << "       panel->box.size = box.size;" << endl;
     TStream << "       panel->setBackground(SVG::load(assetPlugin(plugin, \"res/" + panelName + ".svg\")));" << endl;
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
            //TStream << "    addChild(Widget::create<" + cName + ">(Vec(" + control_->x_pos() + "," + control_->y_pos() + ")));" << endl;
        }
        if (control_->type() == "screw")
        {
            TStream << "    addChild(Widget::create<" + cName + ">(Vec(" + control_->x_pos() + "," + control_->y_pos() + ")));" << endl;
        }
        else if (control_->type() == "rotary_switch")
        {
            TStream << "    addParam(ParamWidget::create<" + cName + "_snap>(Vec(" + xPos + "," + yPos + "), module, " + moduleName + "::" + pName + ", " + minVal + ", " + maxVal  + ", " + defaultVal + "));" << endl;
        }
        else if (control_->type() == "knob" || control_->type() == "button" || control_->type() == "switch")
        {
            TStream << "    addParam(ParamWidget::create<" + cName + ">(Vec(" + xPos + "," + yPos + "), module, " + moduleName + "::" + pName + ", " + minVal + ", " + maxVal  + ", " + defaultVal + "));" << endl;
        }
        else if (control_->type() == "input")
        {
            TStream << "    addInput(Port::create<" + cName + ">(Vec(" + xPos + "," + yPos + "), Port::INPUT, module, " + moduleName + "::" + pName + "));" << endl;
        }
        else if (control_->type() == "output")
        {
            TStream << "    addOutput(Port::create<" + cName + ">(Vec(" + xPos + "," + yPos + "), Port::OUTPUT, module, " + moduleName + "::" + pName + "));" << endl;
        }
        else if (control_->type() == "led")
        {
            TStream << "    addChild(" + cName + ">(Vec(" + xPos + "," + yPos + "), module, " +  moduleName + "::" + pName + "));" << endl;
        }
     }
     TStream << "}" << endl;
}

void ModuleCanvaWriter::writeModuleModel(QString IDName, QString module_name, QString manufacturer, QString tags)
{
    TStream << "Model *model"  + moduleName + " = Model::create<" + moduleName + "," +  moduleName + "Widget>(\"" + manufacturer + "\", \"" + IDName + "\", \"" + module_name + "\", " + tags + ");" << endl;
    moduleFile.close();
}
//Model *modelFrequencyDivider = Model::create<FrequencyDivider, FrequencyDividerWidget>("Hora", "Frequency divider", "Frequency Divider", CLOCK_MODULATOR_TAG);
