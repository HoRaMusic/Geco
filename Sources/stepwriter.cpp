#include "Headers/stepwriter.h"
#include <QDebug>

StepWriter::StepWriter(QObject *parent):
     QObject(parent)
{
     numberOfBlocks = 0;
     numberOfPrevSampleBlocks = 0;
     module_name = "";
     vultUsed = false;
     extClassUsed = false;
     followerUsed = false;
     getSampleRateWrited = false;
     includeVultWrited = false;
     includeExtClassUsedWrited = false;
     samplerateUsed = false;
     includeFollowerWrited = false;
     digitalDSPUsed = false;
     includeDigitalDSPWrited = false;
     numberOfOperations = 0;
     numberOfVultEngines = 0;
     jsonUsed = false;
     jsonWrited = false;
     screenUsed = false;
     screenStructWrited = false;
     stepWrited = false;
};

// this function fill a vector with the blocks of the nodes editor classified in order that allow to write the instructions in the good order.
void StepWriter::fillBlockVector(QGraphicsScene *scene, QString _plugDir)
{
    plugDir = _plugDir;
    m_scene = scene;
    QVector<int> previousSampleBlocksID;
    QVector<QString> variablesNames;
    bool variableDoublon = false;
    QVector<QString> doublonsNames;
    // fill the vector with the blocks that don't have any input.
    foreach(QGraphicsItem *item, m_scene->items())
    {
        QString param_name;
        QNEBlock *BL = (QNEBlock*) item;
        QVector<QNEPort*> ports = BL->ports();


        if (item->type() == QNEBlock::Type)
        {
            QString object_type = ports.at(1)->portName();
            if (BL->blockName() != "plugin_setting" && BL->blockName() != "smart_panel" && BL->blockName() != "add_in_step" && BL->blockName() != "screw" && BL->blockName() != "add_in_widget" && BL->blockName() != "add_in_struct" && BL->blockName() != "add_in_module_CPP" && BL->blockName() != "add_include")
            {
                QString object_name = ports.at(2)->portName();
                variablesNames.append(object_name);
                numberOfBlocks ++;
            }

            if (BL->blockName() == "prev_sample")
            {
                previousSampleBlocksID.append(BL->uniqueID());
                numberOfPrevSampleBlocks ++;
            }
            /*if (BL->numberOfInputs() == 0 && BL->blockName() != "plugin_setting" && BL->blockName() != "smart_panel")
            {
               smart_block.append(BL);
            }*/
        }
    }
    QVector<QString> variablesNamesB;
    QVector<bool> doublons;
    for (int i = 0; i < variablesNames.size() ; i++)
    {
        bool doublon = false;
        for ( int j = 0; j < variablesNamesB.size() ; j++)
        {
            if ( variablesNames.at(i) == variablesNamesB.at(j))
            {
                 doublon = true;
                 doublonsNames.append(variablesNames.at(i));
            }

        }
        variablesNamesB.append(variablesNames.at(i));
        doublons.append(doublon);
    }
    for (int i = 0; i < doublons.size() ; i++)
    {
        if (doublons.at(i) == true)
        {
            variableDoublon = true;
        }
    }

   // fill the vector with blocks that has at least one input and one output connected
   // and verify that all block connected to their inputs are already in the smart_block_vector
    int numberOfRegularBlocks = numberOfBlocks - numberOfPrevSampleBlocks;
   bool vectorFilled = false;
   int loopsNum = 0;
   while (vectorFilled == false && loopsNum < numberOfBlocks * 2)
   {
       loopsNum ++;
       foreach(QGraphicsItem *item, m_scene->items())
       {
           QString param_name;
           QNEBlock *BL = (QNEBlock*) item;
           if (item->type() == QNEBlock::Type && BL->blockName() != "plugin_setting" && BL->blockName() != "screw" && BL->blockName() != "smart_panel" && BL->blockName() != "add_in_step" && BL->blockName() != "add_in_widget" && BL->blockName() != "add_in_struct" && BL->blockName() != "add_in_module_CPP" && BL->blockName() != "add_include")
           {
               // get the IDnumber of all blocks connected to the input ports
               QVector<int> inComingBlocksID;
               foreach(QGraphicsItem *port_t, BL->childItems())
               {
                   QNEPort *port = (QNEPort*) port_t;
                   QVector<QNEConnection*> connections = port->connections();
                   if (port->isOutput() == false)
                   {
                       for (int i = 0; i < connections.size(); i++)
                       {
                           QNEConnection *currentConnection = connections.at(i);
                           QNEPort *port1 = currentConnection->port1();
                           QNEPort *port2 = currentConnection->port2();
                           if (port1->isOutput())
                           {
                               inComingBlocksID.append(port1->block()->uniqueID());
                           }
                           else if (port2->isOutput())
                           {
                               inComingBlocksID.append(port2->block()->uniqueID());
                           }
                       }
                   }
               }
               // verify if all incoming blocks are elements of smart_block yet
               QVector<bool> isSmart_BlockElement;
               for (int j = 0; j < inComingBlocksID.size(); j++ )
               {
                   bool isInSmart_block = false;
                   for (int k = 0; k < smart_block.size(); k++)
                   {
                       if (smart_block.at(k)->uniqueID() == inComingBlocksID.at(j))
                       {
                           isInSmart_block = true;
                       }
                   }
                   for (int k = 0; k < previousSampleBlocksID.size(); k++)
                   {
                       if (previousSampleBlocksID.at(k) == inComingBlocksID.at(j))
                       {
                           isInSmart_block = true;
                       }
                   }
                   if (isInSmart_block == true)
                   {
                       isSmart_BlockElement.append(true);
                   }
                   else
                   {
                       isSmart_BlockElement.append(false);
                   }
               }
               // add the block into smart_block vector if all incoming blocks are already part of it
               bool allIncomingBlocksWrited = true;
               for (int in = 0; in < isSmart_BlockElement.size(); in++ )
               {
                   if (isSmart_BlockElement.at(in) == false)
                   {

                       allIncomingBlocksWrited = false;
                   }
               }
               bool isInVector = false;
               if (allIncomingBlocksWrited == true)
               {
                   for (int k = 0; k < smart_block.size(); k++)
                   {
                       if (smart_block.at(k)->uniqueID() == BL->uniqueID())
                       {
                           isInVector = true;
                       }
                   }
                   if (isInVector == false)
                   {
                       //smart_block.append(BL);
                       if (BL->blockName() != "prev_sample")
                       {
                           smart_block.append(BL);
                       }
                       else if (smart_block.size() >= numberOfRegularBlocks)
                       {
                           smart_block.append(BL);
                       }
                   }
               }
           }
       }
       // verify if all blocks are in the vector
       if (smart_block.size() >= numberOfBlocks )
       {
           vectorFilled = true;
       }
   }
   if (loopsNum >= numberOfBlocks * 2)
   {
       CustomDialog d("Properties");
       d.addLabel("WARNING:");
       d.addLabel("UNABLE TO WRITE");
       d.exec();
   }
   else
   {
       if (variableDoublon == false)
       {
           writeInstuctions();
       }
       else
       {
           CustomDialog d("Properties");
           d.addLabel("WARNING:");
           d.addLabel("many variables share the same name:");
           for (int i = 0; i < doublonsNames.size(); i++)
           {
               d.addLabel(doublonsNames.at(i));
           }
           d.exec();
       }
   }
}

void StepWriter::writeInstuctions()
{
    // simply use each elements of the vector (they are in the good order) to write the instructions
    for (int i = 0; i < smart_block.size(); i++)
    {
        QString instruction = "0";
        QNEBlock *currentBlock = smart_block.at(i);
        QVector<QNEPort*> ports = currentBlock->ports();
        QString object_type = ports.at(1)->portName();

        if (currentBlock->blockName() == "module_input")
        {
            instruction = inputInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "module_output")
        {
            instruction = outputInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "double" || currentBlock->blockName() == "integer")
        {
            instruction = variableInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "string")
        {
            instruction = stringInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "counter")
        {
            digitalDSPUsed = true;
            instruction = counterInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "gate->trig")
        {
            digitalDSPUsed = true;
            instruction = gateToTrigInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "knob" || currentBlock->blockName() == "switch" || currentBlock->blockName() == "rotary_switch" || currentBlock->blockName() == "button")
        {
            instruction = controllerInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "screen")
        {
            screenUsed = true;
            QVector<QNEPort*> ports = currentBlock->ports();
            QString screen_name = ports.at(3)->portName();
            if (screen_name.contains("/"))
            {
                QString vv;
                vv = screen_name.left(screen_name.size()-1);
                screen_name = vv;
            }
            displayNames.append("display_" + screen_name);
            displayStructNames.append(screen_name);
            instruction = screenInstruction(currentBlock, screen_name);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "led")
        {
            instruction = ledInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "clock")
        {
            instruction = clockInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "(x+y)" || currentBlock->blockName() == "(x-y)" || currentBlock->blockName() == "(x/y)" || currentBlock->blockName() == "(x%y)" || currentBlock->blockName() == "(x*y)" || currentBlock->blockName() == "sqrt" || currentBlock->blockName() == "x_to_the_y(power)" || currentBlock->blockName() == "x and y" ||  currentBlock->blockName() == "x||y" ||  currentBlock->blockName() == "x<y" ||  currentBlock->blockName() == "x>y" || currentBlock->blockName() == "x<=y" || currentBlock->blockName() == "x>=y" || currentBlock->blockName() == "XOR" || currentBlock->blockName() == "x=y")
        {
            instruction = operationInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "array_4" || currentBlock->blockName() == "array_64" || currentBlock->blockName() == "array_16" || currentBlock->blockName() == "array_32" )
        {
            instruction = arrayInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "prev_sample")
        {
            instruction = nMinus1Instruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "env_follower")
        {
            followerUsed = true;
            instruction = envInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "sample_rate")
        {
            samplerateUsed = true;
            instruction = sampleRateInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "to_Json_bool")
        {
            jsonUsed = true;
            instruction = toJsonBoolInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "to_Json_int")
        {
            jsonUsed = true;
            instruction = toJsonIntInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "to_Json_real")
        {
            jsonUsed = true;
            instruction = toJsonRealInstruction(currentBlock);
            instructions.append(instruction);
        }
        else if (currentBlock->blockName() == "toInt"  || currentBlock->blockName() == "toFLoat")
        {
            if (currentBlock->blockName() == "toInt")
            {
                instruction = numConverterInstruction(currentBlock, 0);
                instructions.append(instruction);
            }
            else if (currentBlock->blockName() == "tofloat")
            {
                instruction = numConverterInstruction(currentBlock, 1);
                instructions.append(instruction);
            }
        }
        else if (currentBlock->blockName().contains("++"))
        {
            extClassUsed = true;
            instruction = extClassInstruction(currentBlock);
            instructions.append(instruction);
            QString complete_block_name = currentBlock->blockName();
            QStringList wordList;
            wordList = complete_block_name.split(':');
            QString wordList1 = wordList.at(1);
            QString wordListOk = wordList1.left(wordList1.size()-2);
            bool isInVector = false;
            for(int i = 0; i < extClasses.size(); i++)
            {
                if (extClasses.at(i) == wordListOk)
                {
                    isInVector = true;
                }
            }
            if (isInVector == false)
            {
                extClasses.append(wordListOk);
            }
        }
        else
        {
            vultUsed = true;
            instruction = genericInstruction(currentBlock);
            instructions.append(instruction);
            QString complete_block_name = currentBlock->blockName();
            QStringList wordList;
            wordList = complete_block_name.split(':');
            bool isInVector = false;
            for(int i = 0; i < vultEngines.size(); i++)
            {
                if (vultEngines.at(i) == wordList.at(1))
                {
                    isInVector = true;
                }
            }
            if (isInVector == false)
            {
                vultEngines.append(wordList.at(1));
            }
        }
    }
    fillStepFunction();
}

void StepWriter::fillStepFunction()
{
    // get the plugin and module names
    foreach(QGraphicsItem *item, m_scene->items())
    {
        QString param_name;
        QNEBlock *BL = (QNEBlock*) item;
        if (item->type() == QNEBlock::Type)
        {

            bool blockIsPlugin = false;
            int j = 0;
            foreach(QGraphicsItem *port_t, BL->childItems())
            {
                QNEPort *port = (QNEPort*) port_t;
                if (port->portName() == "plugin_setting")
                {
                    blockIsPlugin = true;
                }
                if (blockIsPlugin == true)
                {
                    if ( j == 6)
                    {
                       plugin_File_name = port->portName();
                    }
                    else if (j == 7)
                    {
                        module_name = port->portName();
                    }
                }
                j++;
            }
        }

    }

    // ouverture du fichier cpp
    QString fileToWriteInName = plugDir + "/" + plugin_File_name + "/src/" + module_name + ".cpp";
    QDir fileToWriteInDir(QDir::currentPath());
    fileToWriteInDir.cd(fileToWriteInName);
    QString fileToWriteInDirPath = fileToWriteInDir.absoluteFilePath(fileToWriteInName);
    QFile FileToWriteIn(fileToWriteInDirPath);
    QString contentL1;
    // si il s'ouvre on rempli un vecteur (FileToWriteInList) avec son contenu (ligne par ligne)
    // puis on supprime le fichier cpp
    if (FileToWriteIn.open(QIODevice::ReadOnly)) {
        QTextStream streamFTW(&FileToWriteIn);
        while (!streamFTW.atEnd())
        {
            FileToWriteInList.append(streamFTW.readLine());
        }
        FileToWriteIn.close();
    }
    FileToWriteIn.remove();

    // on cherche la première ligne de la fonction step dans le vecteur contenant le texte du fichier cpp
    // puis on charge le contenu de step au bon endroit dans une stringlist (newFileContent).
    QStringList newFileContent;
    QString includeLine;
    QString searchStepString("::process(const ProcessArgs &args) {");
    QString searchStructString("struct ");


    bool structWrited = false;
    for(int i = 0; i < FileToWriteInList.size(); i++)
    {
        QString line_ = FileToWriteInList.at(i);
        newFileContent.append(line_);
        if (vultUsed == true && includeVultWrited == false)
        {
            for(int i = 0; i < vultEngines.size();i++)
            {
                newFileContent.append("#include \"" + vultEngines.at(i) + ".h\" ");
            }

            includeVultWrited = true;
        }
        if (extClassUsed == true && includeExtClassUsedWrited == false)
        {
            for(int i = 0; i < extClasses.size();i++)
            {
                newFileContent.append("#include \"" + extClasses.at(i) + ".h\" ");
            }

            includeExtClassUsedWrited = true;
        }
        if (followerUsed == true && includeFollowerWrited == false)
        {
            //newFileContent.append("#include \"dsp/filter.hpp\"");
            includeFollowerWrited = true;
        }
        if ((vultUsed == true || followerUsed == true || samplerateUsed == true) && getSampleRateWrited == false)
        {
            //newFileContent.append("    float getSampleRate()\n    {\n        return engineGetSampleRate();\n    }\n");
            getSampleRateWrited = true;
        }
        if (digitalDSPUsed == true && includeDigitalDSPWrited == false)
        {
            //newFileContent.append("#include \"dsp/digital.hpp\" ");
            includeDigitalDSPWrited = true;
        }

        if (line_.contains(searchStructString, Qt::CaseSensitive) && structWrited == false) {
            for(int j = 0; j < structInstructions.size(); j++)
            {
                newFileContent.append(structInstructions.at(j));
            }

            if (jsonUsed == true)
            {

                newFileContent.append("\n    json_t *dataToJson() override {\n        json_t *rootJ = json_object();\n");
                for(int b = 0; b < json_bool.size(); b++)
                {
                    QString varName = json_bool.at(b);
                    newFileContent.append("\n        json_object_set_new(rootJ, \"" + varName + "\", json_boolean(" + varName + "));\n");
                }
                for(int r = 0; r < json_real.size(); r++)
                {
                    QString varName = json_real.at(r);
                    newFileContent.append("\n        json_object_set_new(rootJ, \"" + varName + "\", json_real(" + varName + "));\n");
                }
                for(int i = 0; i < json_int.size(); i++)
                {
                    QString varName = json_int.at(i);
                    newFileContent.append("\n        json_object_set_new(rootJ, \"" + varName + "\", json_integer(" + varName + "));\n");
                }
                newFileContent.append("\n        return rootJ;\n    }");


                newFileContent.append("    void dataFromJson(json_t *rootJ) override {\n");
                for(int b = 0; b < json_bool.size(); b++)
                {
                    QString varName = json_bool.at(b);
                    newFileContent.append("\n        json_t *" + varName + "J = json_object_get(rootJ, \"" + varName + "\");\n");
                    newFileContent.append("\n        if (" + varName + "J)\n        {\n            " + varName + "= json_is_true(" + varName + "J);\n        }");
                }
                for(int i = 0; i < json_int.size(); i++)
                {
                    QString varName = json_int.at(i);
                    newFileContent.append("\n        json_t *" + varName + "J = json_object_get(rootJ, \"" + varName + "\");\n");
                    newFileContent.append("\n        if (" + varName + "J)\n        {\n                    " + varName + "= json_integer_value(" + varName + "J);\n        }");
                }
                for(int r = 0; r < json_real.size(); r++)
                {
                    QString varName = json_real.at(r);
                    newFileContent.append("\n        json_t *" + varName + "J = json_object_get(rootJ, \"" + varName + "\");\n");
                    newFileContent.append("\n        if (" + varName + "J)\n        {\n                    " + varName + "= json_number_value(" + varName + "J);\n        }");
                }
                newFileContent.append("    }\n");
            }
            structWrited = true;
        }

        if (line_.contains(searchStepString, Qt::CaseSensitive)) {
            for(int k = 0; k < instructions.size(); k++)
            {
                newFileContent.append(instructions.at(k));
            }
            stepWrited = true;
        }
        if (screenUsed == true && screenStructWrited == false && stepWrited == true && line_.contains("}"))
        {
            screenStructWrited = true;
            for( int j = 0; j < displayStructNames.size(); j++)
            {
                newFileContent.append("struct " + displayStructNames.at(j) + "_" + module_name + " : TransparentWidget {");
                newFileContent.append("    " + module_name + " *module;");
                newFileContent.append("    std::shared_ptr<Font> font;");
                newFileContent.append("    " + displayStructNames.at(j) + "_" + module_name + " () {");
                newFileContent.append("    font = APP->window->loadFont(asset::plugin(pluginInstance, \"res/" + fontName + "\"));");
                newFileContent.append("    }");
                newFileContent.append("    void updateLine1(NVGcontext *vg, Vec pos, NVGcolor DMDtextColor, char* lineMessage1) {");
                newFileContent.append("    nvgFontSize(vg, " + textSize + ");");
                newFileContent.append("    nvgFontFaceId(vg, font->handle);");
                newFileContent.append("    nvgTextLetterSpacing(vg, -2);");
                newFileContent.append("    nvgFillColor(vg, nvgTransRGBA(nvgRGB( "+ textColorR + ", "  + textColorG + ", " + textColorB + "), 0xff));");
                newFileContent.append("    nvgText(vg, pos.x, pos.y, lineMessage1, NULL);");
                newFileContent.append("    }");
                newFileContent.append("    void updateLine2(NVGcontext *vg, Vec pos, NVGcolor DMDtextColor, int xOffsetValue, char* lineMessage2) {");
                newFileContent.append("    nvgFontSize(vg, " + textSize + ");");
                newFileContent.append("    nvgFontFaceId(vg, font->handle);");
                newFileContent.append("    nvgTextLetterSpacing(vg, -2);");
                newFileContent.append("    nvgFillColor(vg, nvgTransRGBA(nvgRGB( "+ textColorR + ", "  + textColorG + ", " + textColorB + "), 0xff));");
                newFileContent.append("    nvgText(vg, pos.x, pos.y, lineMessage2, NULL);");
                newFileContent.append("    }");
                newFileContent.append("    void draw(NVGcontext *vg) override {");
                newFileContent.append("    if(module){");
                newFileContent.append("      updateLine1(vg, Vec( 5, 5), nvgRGB(0x08, 0x08, 0x08), module->" + displayStructNames.at(j) + "_" + module_name + "_lineMessage1);");
                newFileContent.append("      updateLine2(vg, Vec(  5, (" + textSize + "+" + textSize +"/2"+")), nvgRGB(0x08, 0x08, 0x08), 20, module->" + displayStructNames.at(j) + "_" + module_name + "_lineMessage2);");
                newFileContent.append("     }");
                newFileContent.append("    }");
                newFileContent.append("};");
            }
        }
    }

    // on créé un nouveau fichier cpp et on le rempli avec newFileContent
    QDir fileDir(QDir::currentPath());
    fileDir.cd(plugDir);
    QString fileDirPath = fileDir.absoluteFilePath(plugDir);
    QFile file(fileDirPath);
    if (!QDir(plugDir + "/" + plugin_File_name).exists())
    {
        fileDir.mkdir( plugin_File_name);
    }
    QFile *canFile=new QFile(plugDir + "/" + plugin_File_name + "/src/" + module_name + "Can.cpp");
    canFile->remove();
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
    // move needed vult files, classes and wavetables
    for(int i = 0 ; i < vultEngines.size(); i ++)
    {
        if (!QFile(plugDir + "/" + plugin_File_name + "/" + "src/" + vultEngines.at(i) + ".cpp" ).exists())
        {
            QString newVultCPPFilePath = plugDir + "/"+ plugin_File_name + "/" + "src/" + vultEngines.at(i) + ".cpp";
            QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
            QString fontLoc = docLocation + "Geco/VultEngines/"+ vultEngines.at(i) + ".cpp";
            QDir rootDir(QDir::currentPath());
            QString fileDirPath = rootDir.relativeFilePath(fontLoc);
            QFile::copy(fileDirPath, newVultCPPFilePath);
        }
        if (!QFile(plugDir + "/" + plugin_File_name + "/" + "src/" + vultEngines.at(i) + ".h" ).exists())
        {
            QString newVultHeaderFilePath = plugDir + "/" + plugin_File_name + "/" + "src/" + vultEngines.at(i) + ".h";
            QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
            QString fontLoc = docLocation + "Geco/VultEngines/"+ vultEngines.at(i) + ".h";
            QDir rootDir(QDir::currentPath());
            QString fileDirPath = rootDir.relativeFilePath(fontLoc);
            QFile::copy(fileDirPath, newVultHeaderFilePath);
        }
        if (!QFile(plugDir + "/" + plugin_File_name + "/" + "src/" + "vultin.c" ).exists())
        {
            QString newVultinCPPFilePath = plugDir + "/" + plugin_File_name + "/" + "src/" + "vultin.c";
            QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
            QString fontLoc = docLocation + "Geco/VultEngines/vultin.c";
            QDir rootDir(QDir::currentPath());
            QString fileDirPath = rootDir.relativeFilePath(fontLoc);
            QFile::copy(fileDirPath, newVultinCPPFilePath);
        }
        if (!QFile(plugDir + "/" + plugin_File_name + "/" + "src/" + "vultin.h" ).exists())
        {
            QString newVultinHeaderFilePath = plugDir + "/" + plugin_File_name + "/" + "src/" + "vultin.h";
            QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
            QString fontLoc = docLocation + "Geco/VultEngines/vultin.h";
            QDir rootDir(QDir::currentPath());
            QString fileDirPath = rootDir.relativeFilePath(fontLoc);
            QFile::copy(fileDirPath, newVultinHeaderFilePath);
        }
    }    
    for(int i = 0 ; i < extClasses.size(); i ++)
    {
        if (!QFile(plugDir + "/" + plugin_File_name + "/" + "src/" + extClasses.at(i) + ".cpp" ).exists())
        {
            QString newExtClassCPPFilePath = plugDir + "/"+ plugin_File_name + "/" + "src/" + extClasses.at(i) + ".cpp";
            QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
            QString fontLoc = docLocation + "Geco/Classes/" + extClasses.at(i) + ".cpp";
            QDir rootDir(QDir::currentPath());
            QString fileDirPath = rootDir.relativeFilePath(fontLoc);
            QFile::copy(fileDirPath, newExtClassCPPFilePath);
        }
        if (!QFile(plugDir + "/" + plugin_File_name + "/" + "src/" + extClasses.at(i) + ".h" ).exists())
        {
            QString newVultHeaderFilePath = plugDir + "/" + plugin_File_name + "/" + "src/" + extClasses.at(i) + ".h";
            QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
            QString fontLoc = docLocation + "Geco/Classes/" + extClasses.at(i) + ".h";
            QDir rootDir(QDir::currentPath());
            QString fileDirPath = rootDir.relativeFilePath(fontLoc);
            QFile::copy(fileDirPath, newVultHeaderFilePath);
        }
    }
    for(int i = 0 ; i < waveTables.size(); i ++)
    {
        if (!QFile(plugDir + "/" + plugin_File_name + "/" + "res/" +  waveTables.at(i)).exists())
        {
            QString tableName = "";
            tableName =  waveTables.at(i);
            QString newTableFilePath = plugDir + "/"+ plugin_File_name + "/" + "res/" + tableName;
            QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
            QString fontLoc = docLocation + "Geco/WaveTables/" + tableName;
            QDir rootDir(QDir::currentPath());
            QString fileDirPath = rootDir.relativeFilePath(fontLoc);
            QFile::copy(fileDirPath, newTableFilePath);
            QString gg = newTableFilePath;
            QByteArray s = "rrrrrr" + gg.toUtf8();
            qDebug() << s;
        }
    }
}

QString StepWriter::sampleRateInstruction(QNEBlock *currentBlock)
{
    QString instruction = "0";
    QVector<QNEPort*> ports = currentBlock->ports();
    QString input_name = ports.at(2)->portName();
    QString outputPortName = ports.at(3)->portName();
    instruction = "    float " + input_name + "_" + outputPortName + " = args.sampleRate;";
    return instruction;
}
QString StepWriter::toJsonBoolInstruction(QNEBlock *currentBlock)
{
    QString instructions = "";
    QVector<QString> instruction;
    QString output_name = "noName";
    QVector<QString> inputsSum;
    QVector<QString> incomingSignals;
    QVector<QNEPort*> ports = currentBlock->ports();
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            for (int i = 0; i < connections.size(); i++)
            {
                QNEConnection *currentConnection = connections.at(i);
                QNEPort *port1 = currentConnection->port1();
                QNEPort *port2 = currentConnection->port2();
                QString signal_name;

                if (port1->isOutput())
                {
                    QNEBlock *incomingBlock = port1->block();
                    QString incomingBlockVarName;
                    QString incomingPortName = port1->portName();
                    QVector<QNEPort*> Iports = incomingBlock->ports();
                    incomingBlockVarName = Iports.at(2)->portName();
                    incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                }
                else if (port2->isOutput())
                {
                    QNEBlock *incomingBlock = port2->block();
                    QString incomingBlockVarName;
                    QString incomingPortName = port2->portName();
                    QVector<QNEPort*> Iports = incomingBlock->ports();
                    incomingBlockVarName = Iports.at(2)->portName();
                    incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                }
            }
            for (int j = 0; j < incomingSignals.size(); j++)
            {
                    inputsSum.append(incomingSignals.at(j));
            }
        }
        if (portIndex == 2)
        {
            output_name = port->portName();
        }
    }
    //if step < 1 : step++; && inputsumati = inputsum+forJson

    instruction.append("    if (step < 1) \n    {\n        step++;\n    ");
    for (int i = 0; i < inputsSum.size(); i++)
    {
        instruction.append("        " + inputsSum.at(i) +  "="  + inputsSum.at(i) + "_forJson;\n");
        structInstructions.append("    bool " + inputsSum.at(i) + "_forJson = 0.0");
        json_bool.append(inputsSum.at(i) + "_forJson");
    }
    instruction.append("    }\n");
    for(int i = 0; i < instruction.size(); i++)
    {
        instructions = instructions + instruction.at(i);

    }
    return instructions;
}

QString StepWriter::toJsonIntInstruction(QNEBlock *currentBlock)
{
    QString instructions = "";
    QVector<QString> instruction;
    QString output_name = "noName";
    QVector<QString> inputsSum;
    QVector<QString> incomingSignals;
    QVector<QNEPort*> ports = currentBlock->ports();
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            for (int i = 0; i < connections.size(); i++)
            {
                QNEConnection *currentConnection = connections.at(i);
                QNEPort *port1 = currentConnection->port1();
                QNEPort *port2 = currentConnection->port2();
                QString signal_name;

                if (port1->isOutput())
                {
                    QNEBlock *incomingBlock = port1->block();
                    QString incomingBlockVarName;
                    QString incomingPortName = port1->portName();
                    QVector<QNEPort*> Iports = incomingBlock->ports();
                    incomingBlockVarName = Iports.at(2)->portName();
                    incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                }
                else if (port2->isOutput())
                {
                    QNEBlock *incomingBlock = port2->block();
                    QString incomingBlockVarName;
                    QString incomingPortName = port2->portName();
                    QVector<QNEPort*> Iports = incomingBlock->ports();
                    incomingBlockVarName = Iports.at(2)->portName();
                    incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                }
            }
            for (int j = 0; j < incomingSignals.size(); j++)
            {
                    inputsSum.append(incomingSignals.at(j));
            }
        }
        if (portIndex == 2)
        {
            output_name = port->portName();
        }
    }
    //if step < 1 : step++; && inputsumati = inputsum+forJson

    instruction.append("    if (step < 1) \n    {\n        step++;\n    ");
    for (int i = 0; i < inputsSum.size(); i++)
    {
        instruction.append("        " + inputsSum.at(i) +  "="  + inputsSum.at(i) + "_forJson;\n");
        structInstructions.append("    int " + inputsSum.at(i) + "_forJson = 0.0");
        json_int.append(inputsSum.at(i) + "_forJson");
    }
    instruction.append("    }\n");
    for(int i = 0; i < instruction.size(); i++)
    {
        instructions = instructions + instruction.at(i);

    }
    return instructions;
}

QString StepWriter::toJsonRealInstruction(QNEBlock *currentBlock)
{
    QString instructions = "";
    QVector<QString> instruction;
    QString output_name = "noName";
    QVector<QString> inputsSum;
    QVector<QString> incomingSignals;
    QVector<QNEPort*> ports = currentBlock->ports();
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            for (int i = 0; i < connections.size(); i++)
            {
                QNEConnection *currentConnection = connections.at(i);
                QNEPort *port1 = currentConnection->port1();
                QNEPort *port2 = currentConnection->port2();
                QString signal_name;

                if (port1->isOutput())
                {
                    QNEBlock *incomingBlock = port1->block();
                    QString incomingBlockVarName;
                    QString incomingPortName = port1->portName();
                    QVector<QNEPort*> Iports = incomingBlock->ports();
                    incomingBlockVarName = Iports.at(2)->portName();
                    incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                }
                else if (port2->isOutput())
                {
                    QNEBlock *incomingBlock = port2->block();
                    QString incomingBlockVarName;
                    QString incomingPortName = port2->portName();
                    QVector<QNEPort*> Iports = incomingBlock->ports();
                    incomingBlockVarName = Iports.at(2)->portName();
                    incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                }
            }
            for (int j = 0; j < incomingSignals.size(); j++)
            {
                    inputsSum.append(incomingSignals.at(j));
            }
        }
        if (portIndex == 2)
        {
            output_name = port->portName();
        }
    }
    //if step < 1 : step++; && inputsumati = inputsum+forJson

    instruction.append("    if (step < 1) \n    {\n        step++;\n    ");
    for (int i = 0; i < inputsSum.size(); i++)
    {
        instruction.append("        " + inputsSum.at(i) +  "="  + inputsSum.at(i) + "_forJson;\n");
        structInstructions.append("    real " + inputsSum.at(i) + "_forJson = 0.0");
        json_real.append(inputsSum.at(i) + "_forJson");
    }
    instruction.append("    }\n");
    for(int i = 0; i < instruction.size(); i++)
    {
        instructions = instructions + instruction.at(i);

    }
    return instructions;
}

QString StepWriter::envInstruction(QNEBlock *currentBlock)
{
    QVector<QString> instructions;
    QVector<QString> params;
    QVector<QNEPort*> ports = currentBlock->ports();
    QVector<QString> inputsSums;
    QString object_name = ports.at(2)->portName();
    QString rate = ports.at(4)->portName();
    QString object_type = currentBlock->blockName();
    QString outputPortName;
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QString inputsSum = "";
        QVector<QString> incomingSignals;
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            if (connections.size() > 0)
            {
                for (int i = 0; i < connections.size(); i++)
                {
                    QNEConnection *currentConnection = connections.at(i);
                    QNEPort *port1 = currentConnection->port1();
                    QNEPort *port2 = currentConnection->port2();
                    QString signal_name;

                    if (port1->isOutput())
                    {
                        QNEBlock *incomingBlock = port1->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port1->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                    else if (port2->isOutput())
                    {
                        QNEBlock *incomingBlock = port2->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port2->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                }
                for (int j = 0; j < incomingSignals.size(); j++)
                {
                    if (j < incomingSignals.size() - 1)
                    {
                        inputsSum.append(incomingSignals.at(j) + " + ");
                    }
                    else
                    {
                        inputsSum.append(incomingSignals.at(j));
                    }
                }
                if (inputsSum != "")
                {
                    inputsSums.append(inputsSum);
                }
            }
            else if (port->thirdPtr() == 6)
            {
                inputsSums.append("0");
            }
        }
        else
        {
           outputPortName = port->portName();
        }
    }
    QString varName = object_name;
    QString rateName = object_name +"_" + outputPortName + "_rate";
    QString outputName = object_name +"_" + outputPortName;
    instructions.append("    float " + rateName + " = " + rate + " / engineGetSampleRate();\n");
    instructions.append("    " + varName + ".setRate(" + rateName + ");\n");
    instructions.append("    " + varName + ".process(fabs(" + inputsSums.at(0) + "));\n");
    instructions.append("    float " + outputName + " = " + varName + ".peak();\n");
    QString instruction = instructions.at(0) + instructions.at(1) + instructions.at(2) + instructions.at(3);
    structInstructions.append("    dsp::PeakFilter " + varName + ";\n");
    return instruction;
}

QString StepWriter::nMinus1Instruction(QNEBlock *currentBlock)
{
    QVector<QString> instructions;
    QVector<QString> params;
    QVector<QNEPort*> ports = currentBlock->ports();
    QVector<QString> inputsSums;
    QString object_name = ports.at(2)->portName();
    QString object_type = currentBlock->blockName();
    QString outputPortName;
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QString inputsSum = "";
        QVector<QString> incomingSignals;
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            if (connections.size() > 0)
            {
                for (int i = 0; i < connections.size(); i++)
                {
                    QNEConnection *currentConnection = connections.at(i);
                    QNEPort *port1 = currentConnection->port1();
                    QNEPort *port2 = currentConnection->port2();
                    QString signal_name;

                    if (port1->isOutput())
                    {
                        QNEBlock *incomingBlock = port1->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port1->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                    else if (port2->isOutput())
                    {
                        QNEBlock *incomingBlock = port2->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port2->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                }
                for (int j = 0; j < incomingSignals.size(); j++)
                {
                    if (j < incomingSignals.size() - 1)
                    {
                        inputsSum.append(incomingSignals.at(j) + " + ");
                    }
                    else
                    {
                        inputsSum.append(incomingSignals.at(j));
                    }
                }
                if (inputsSum != "")
                {
                    inputsSums.append(inputsSum);
                }
            }
            else if (port->thirdPtr() == 6)
            {
                inputsSums.append("0");
            }
        }
        else
        {
           outputPortName = port->portName();
        }
    }

    QString varName = object_name +"_" + outputPortName;
    instructions.append("    " + varName + " = " + inputsSums.at(0) + ";\n");
    QString instruction = instructions.at(0);
    structInstructions.append("    float " + varName + " = 0;\n");
    return instruction;
}


QString StepWriter::inputInstruction(QNEBlock *currentBlock)
{
    QString instruction = "0";
    QVector<QNEPort*> ports = currentBlock->ports();
    QString input_name = ports.at(2)->portName();
    QString outputPortName = ports.at(5)->portName();
    instruction = "    float " + input_name + "_" + outputPortName + " = inputs[" + input_name + "].getVoltage();";
    return instruction;
}
QString StepWriter::numConverterInstruction(QNEBlock *currentBlock, int type)
{

    QString instruction = "0";
    QString var_name = "noName";
    QString inputsSum;
    QVector<QString> incomingSignals;
    QVector<QNEPort*> ports = currentBlock->ports();
    QString outputPortName = ports.at(3)->portName();
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            for (int i = 0; i < connections.size(); i++)
            {
                QNEConnection *currentConnection = connections.at(i);
                QNEPort *port1 = currentConnection->port1();
                QNEPort *port2 = currentConnection->port2();
                QString signal_name;

                if (port1->isOutput())
                {
                    QNEBlock *incomingBlock = port1->block();
                    QString incomingBlockVarName;
                    QString incomingPortName = port1->portName();
                    QVector<QNEPort*> Iports = incomingBlock->ports();
                    incomingBlockVarName = Iports.at(2)->portName();
                    incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                }
                else if (port2->isOutput())
                {
                    QNEBlock *incomingBlock = port2->block();
                    QString incomingBlockVarName;
                    QString incomingPortName = port2->portName();
                    QVector<QNEPort*> Iports = incomingBlock->ports();
                    incomingBlockVarName = Iports.at(2)->portName();
                    incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                }
            }
            for (int j = 0; j < incomingSignals.size(); j++)
            {
                if (j < incomingSignals.size() - 1)
                {
                    inputsSum.append(incomingSignals.at(j) + " + ");
                }
                else
                {
                    inputsSum.append(incomingSignals.at(j));
                }
            }
        }
        if (portIndex == 2)
        {
            var_name = port->portName();
        }
    }
    if (type == 1)
    {
        instruction = "    float " + var_name + "_" + outputPortName + " = float(" + inputsSum + ");" ;
    }
    else if (type == 0)
    {
        instruction = "    int " + var_name + "_" + outputPortName + " = int(" + inputsSum + ");" ;
    }
    return instruction;
}
QString StepWriter::variableInstruction(QNEBlock *currentBlock)
{
    QString instruction = "0";
    QVector<QNEPort*> ports = currentBlock->ports();
    QString var_name = ports.at(2)->portName();
    QString var_value = ports.at(3)->portName();
    QString outputPortName = ports.at(4)->portName();
    if (currentBlock->blockName() == "double")
    {
        instruction = "    float " + var_name + "_" + outputPortName + " = " + var_value + ";";
    }
    else if (currentBlock->blockName() == "integer")
    {
        instruction = "    int " + var_name + "_" + outputPortName + " = " + var_value + ";";
    }
    return instruction;
}

QString StepWriter::stringInstruction(QNEBlock *currentBlock)
{
    QString instruction = "0";
    QVector<QNEPort*> ports = currentBlock->ports();
    QString var_name = ports.at(2)->portName();
    QString var_value = ports.at(3)->portName();
    QString outputPortName = ports.at(4)->portName();
    instruction = "    char " + var_name + "_" + outputPortName + "[100] = \"" + var_value + "\";";
    if (var_value.contains(".gwt"))
    {
        waveTables.append(var_value);
    }
    return instruction;
}

QString StepWriter::controllerInstruction(QNEBlock *currentBlock)
{
    QString instruction = "";
    QVector<QNEPort*> ports = currentBlock->ports();
    QString param_name = ports.at(2)->portName();
    QString object_type = currentBlock->blockName();
    QString outputPortName = "";
    if ( object_type != "button" && object_type != "switch" )
    {
        outputPortName = ports.at(9)->portName();
    }
    else
    {
        outputPortName = ports.at(8)->portName();
    }
    instruction = "    float " +  param_name + "_" + outputPortName + " = params[" + param_name + "].getValue();";
    return instruction;
}
QString StepWriter::ledInstruction(QNEBlock *currentBlock)
{
    QString instruction = "0";
    QString output_name = "noName";
    QString inputsSum;
    QVector<QString> incomingSignals;
    QVector<QNEPort*> ports = currentBlock->ports();
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            for (int i = 0; i < connections.size(); i++)
            {
                QNEConnection *currentConnection = connections.at(i);
                QNEPort *port1 = currentConnection->port1();
                QNEPort *port2 = currentConnection->port2();
                QString signal_name;

                if (port1->isOutput())
                {
                    QNEBlock *incomingBlock = port1->block();
                    QString incomingBlockVarName;
                    QString incomingPortName = port1->portName();
                    QVector<QNEPort*> Iports = incomingBlock->ports();
                    incomingBlockVarName = Iports.at(2)->portName();
                    incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                }
                else if (port2->isOutput())
                {
                    QNEBlock *incomingBlock = port2->block();
                    QString incomingBlockVarName;
                    QString incomingPortName = port2->portName();
                    QVector<QNEPort*> Iports = incomingBlock->ports();
                    incomingBlockVarName = Iports.at(2)->portName();
                    incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                }
            }
            for (int j = 0; j < incomingSignals.size(); j++)
            {
                if (j < incomingSignals.size() - 1)
                {
                    inputsSum.append(incomingSignals.at(j) + " + ");
                }
                else
                {
                    inputsSum.append(incomingSignals.at(j));
                }
            }
        }
        if (portIndex == 2)
        {
            output_name = port->portName();
        }
    }
    instruction = "lights[" + output_name + "].value = " + inputsSum + ";" ;
    return instruction;
}
QString StepWriter::screenInstruction(QNEBlock *currentBlock, QString screenName)
{
    QVector<QString> instruction;
    QString params = "";
    QVector<QNEPort*> ports = currentBlock->ports();
    //int numberOfOutputs = currentBlock->numberOfOutputs();
    //int numberOfOutputs = 1;
    QVector<QString> inputsSums;
    QString object_name = ports.at(2)->portName();
    QString font = ports.at(5)->portName();
    QString colorR = ports.at(6)->portName();
    QString colorG = ports.at(7)->portName();
    QString colorB = ports.at(8)->portName();
    QString fontSize = ports.at(9)->portName();
    QString object_type = currentBlock->blockName();

    QVector<QString> outputPortName;
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QString inputsSum = "";
        QVector<QString> incomingSignals;
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            if (connections.size() > 0)
            {
                for (int i = 0; i < connections.size(); i++)
                {
                    QNEConnection *currentConnection = connections.at(i);
                    QNEPort *port1 = currentConnection->port1();
                    QNEPort *port2 = currentConnection->port2();
                    QString signal_name;

                    if (port1->isOutput())
                    {
                        QNEBlock *incomingBlock = port1->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port1->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                    else if (port2->isOutput())
                    {
                        QNEBlock *incomingBlock = port2->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port2->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                }
                for (int j = 0; j < incomingSignals.size(); j++)
                {
                    if (j < incomingSignals.size() - 1)
                    {
                        inputsSum.append(incomingSignals.at(j) + " + ");
                    }
                    else
                    {
                        inputsSum.append(incomingSignals.at(j));
                    }
                }
                if (inputsSum != "")
                {
                    inputsSums.append(inputsSum);
                }
            }
            else if (port->thirdPtr() == 6)
            {
                inputsSums.append("0");
            }

        }
        else
        {
           outputPortName.append(port->portName());
        }
    }
    foreach(QGraphicsItem *item, m_scene->items())
    {
        QString param_name;
        QNEBlock *BL = (QNEBlock*) item;
        if (item->type() == QNEBlock::Type)
        {

            bool blockIsPlugin = false;
            int j = 0;
            foreach(QGraphicsItem *port_t, BL->childItems())
            {
                QNEPort *port = (QNEPort*) port_t;
                if (port->portName() == "plugin_setting")
                {
                    blockIsPlugin = true;
                }
                if (blockIsPlugin == true)
                {
                    if ( j == 6)
                    {
                       plugin_File_name = port->portName();
                    }
                    else if (j == 7)
                    {
                        module_name = port->portName();
                    }
                }
                j++;
            }
        }

    }
        instruction.append("    strcpy(" + screenName + "_" + module_name + "_lineMessage1" + ", " + inputsSums.at(0) + ");\n");
        instruction.append("    strcpy(" + screenName + "_" + module_name + "_lineMessage2" + ", " + inputsSums.at(1) + ");\n");
     textColorR = colorR;
     textColorG = colorG;
     textColorB = colorB;
     textSize = fontSize;
     fontName = font;
    QString instructions;
    for (int k = 0; k < instruction.size(); k++)
    {
        instructions = instructions + instruction.at(k);
    }
    return instructions;
}
QString StepWriter::outputInstruction(QNEBlock *currentBlock)
{
    QString instruction = "0";
    QString output_name = "noName";
    QString inputsSum;
    QVector<QString> incomingSignals;
    QVector<QNEPort*> ports = currentBlock->ports();
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            for (int i = 0; i < connections.size(); i++)
            {
                QNEConnection *currentConnection = connections.at(i);
                QNEPort *port1 = currentConnection->port1();
                QNEPort *port2 = currentConnection->port2();
                QString signal_name;

                if (port1->isOutput())
                {
                    QNEBlock *incomingBlock = port1->block();
                    QString incomingBlockVarName;
                    QString incomingPortName = port1->portName();
                    QVector<QNEPort*> Iports = incomingBlock->ports();
                    incomingBlockVarName = Iports.at(2)->portName();
                    incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                }
                else if (port2->isOutput())
                {
                    QNEBlock *incomingBlock = port2->block();
                    QString incomingBlockVarName;
                    QString incomingPortName = port2->portName();
                    QVector<QNEPort*> Iports = incomingBlock->ports();
                    incomingBlockVarName = Iports.at(2)->portName();
                    incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                }
            }
            for (int j = 0; j < incomingSignals.size(); j++)
            {
                if (j < incomingSignals.size() - 1)
                {
                    inputsSum.append(incomingSignals.at(j) + " + ");
                }
                else
                {
                    inputsSum.append(incomingSignals.at(j));
                }
            }
        }
        if (portIndex == 2)
        {
            output_name = port->portName();
        }
    }
    instruction = "    outputs[" + output_name + "].setVoltage ( " + inputsSum + ");" ;
    return instruction;
}
QString StepWriter::extClassInstruction(QNEBlock *currentBlock)
{
    QVector<QString> instruction;
    QString params = "";
    QVector<QNEPort*> ports = currentBlock->ports();
    // unused
    // int numberOfOutputs = currentBlock->numberOfOutputs();
    int nO = 0;
    //int numberOfOutputs = 1;
    QVector<QString> inputsSums;
    QVector<QString> inputsNames;
    QString object_name = ports.at(2)->portName();
    QString complete_block_name = currentBlock->blockName();
    QStringList wordList;
    wordList = complete_block_name.split(':');
    QString object_type = wordList.at(0);
    QString class_nameRAW = wordList.at(1);
    QString class_name = class_nameRAW.left(class_nameRAW.size()-2);
    int cs = 0;
    QVector<QString> outputPortName;
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QString inputsSum = "";
        QString inputName = "";
        QVector<QString> incomingSignals;
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            inputName = port->portName();
            cs = connections.size();
            if (connections.size() > 0)
            {
                for (int i = 0; i < connections.size(); i++)
                {
                    QNEConnection *currentConnection = connections.at(i);
                    QNEPort *port1 = currentConnection->port1();
                    QNEPort *port2 = currentConnection->port2();
                    QString signal_name;

                    if (port1->isOutput())
                    {
                        QNEBlock *incomingBlock = port1->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port1->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                    else if (port2->isOutput())
                    {
                        QNEBlock *incomingBlock = port2->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port2->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                }
                for (int j = 0; j < incomingSignals.size(); j++)
                {
                    if (j < incomingSignals.size() - 1)
                    {
                        inputsSum.append(incomingSignals.at(j) + " + ");
                    }
                    else
                    {
                        inputsSum.append(incomingSignals.at(j));
                    }
                }
                if (inputsSum != "")
                {
                    inputsSums.append(inputsSum);
                    inputsNames.append(inputName);
                }
            }
            else if (port->thirdPtr() == 6)
            {
                inputsSums.append("0");
                inputsNames.append(inputName);
            }

        }
        else
        {
           outputPortName.append(port->portName());
           nO++;
        }
    }
    for (int i = 0 ; i < inputsSums.size(); i++)
    {
        if (class_name == "intToString" || class_name == "floatToString" || class_name == "stringPack" || class_name == "waveTable" || class_name == "stringDemux" || class_name == "stringMux" || class_name == "stringLeft")
        {
            if( inputsSums.at(i) != "0" )
            {
                instruction.append("    " + class_name + "_" + object_name + ".set" + inputsNames.at(i) + "(" + inputsSums.at(i) + ");\n");
            }

        }
        else
        {
                instruction.append("    " + class_name + "_" + object_name + ".set" + inputsNames.at(i) + "(" + inputsSums.at(i) + ");\n");
        }
    }

    for (int j = 0; j < nO; j++)
    {
        if (class_name == "intToString" || class_name == "floatToString" || class_name == "stringPack" || class_name == "stringDemux" || class_name == "stringMux" || class_name == "stringLeft")
        {
            instruction.append("    char *" + object_name + "_" + outputPortName.at(j) + " = " + class_name + "_" + object_name + ".get" + outputPortName.at(j) + "();\n");
        }
        else
        {
            instruction.append("    float " + object_name + "_" + outputPortName.at(j) + " = " + class_name + "_" + object_name + ".get" + outputPortName.at(j) + "();\n");
        }

    }

    QString instructions;
    for (int k = 0; k < instruction.size(); k++)
    {
        instructions = instructions + instruction.at(k);
    }
    structInstructions.append("    "  + class_name + " " +  class_name + "_" + object_name + ";\n");
    return instructions;
}
QString StepWriter::genericInstruction(QNEBlock *currentBlock)
{
    QVector<QString> instruction;
    QString params = "";
    QVector<QNEPort*> ports = currentBlock->ports();
    int numberOfOutputs = currentBlock->numberOfOutputs();
    //int numberOfOutputs = 1;
    QVector<QString> inputsSums;
    QString object_name = ports.at(2)->portName();
    QString complete_block_name = currentBlock->blockName();
    QStringList wordList;
    wordList = complete_block_name.split(':');
    QString object_type = wordList.at(0);
    QString vult_engine_name = wordList.at(1);

    QVector<QString> outputPortName;
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QString inputsSum = "";
        QVector<QString> incomingSignals;
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            if (connections.size() > 0)
            {
                for (int i = 0; i < connections.size(); i++)
                {
                    QNEConnection *currentConnection = connections.at(i);
                    QNEPort *port1 = currentConnection->port1();
                    QNEPort *port2 = currentConnection->port2();
                    QString signal_name;

                    if (port1->isOutput())
                    {
                        QNEBlock *incomingBlock = port1->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port1->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                    else if (port2->isOutput())
                    {
                        QNEBlock *incomingBlock = port2->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port2->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                }
                for (int j = 0; j < incomingSignals.size(); j++)
                {
                    if (j < incomingSignals.size() - 1)
                    {
                        inputsSum.append(incomingSignals.at(j) + " + ");
                    }
                    else
                    {
                        inputsSum.append(incomingSignals.at(j));
                    }
                }
                if (inputsSum != "")
                {
                    inputsSums.append(inputsSum);
                }
            }
            else if (port->thirdPtr() == 6)
            {
                inputsSums.append("0");
            }

        }
        else
        {
           outputPortName.append(port->portName());
        }
    }
    for (int i = 0 ; i < inputsSums.size(); i++)
    {
        if ( i < inputsSums.size() - 1)
        {
            params.append(inputsSums.at(i) + ", ");
        }
        else
        {
            params.append(inputsSums.at(i));
        }
    }
    QString reals = "";
    QString real = "real_";
    if ( numberOfOutputs > 1)
    {
        for (int o = 0; o < numberOfOutputs - 1 ; o++)
        {
            reals.append(real);
        }
        instruction.append("    _tuple___" + reals + "_ out;\n");
        instruction.append("    " + vult_engine_name + "_" + object_type + "(" + object_name+ ", " + params + ", out);\n");
        for (int j = 0; j < numberOfOutputs - 1 ; j++)
        {
            instruction.append("    float " + object_name + "_" + outputPortName.at(j) + " = "  + "out.field_" + QString::number(j) + ";\n");
        }
    }
    else
    {
        instruction.append("    float " + object_name + "__" + outputPortName.at(0) + "= " + vult_engine_name + "_" + object_type + "(" + object_name+ ", " + params + ");\n");
        instruction.append("    float " + object_name + "_" + outputPortName.at(0) + " = " + object_name + "__" + outputPortName.at(0) + ";\n");
    }
    QString instructions;
    for (int k = 0; k < instruction.size(); k++)
    {
        instructions = instructions + instruction.at(k);
    }
    structInstructions.append("    "  + vult_engine_name + "_"+ object_type + "_type " + object_name + ";\n");
    return instructions;
}

QString StepWriter::counterInstruction(QNEBlock *currentBlock)
{
    QVector<QString> instruction;
    QString params = "";
    QVector<QNEPort*> ports = currentBlock->ports();
    QVector<QString> inputsSums;
    QString object_name = ports.at(2)->portName();
    QString counterLength = ports.at(3)->portName();
    QString object_type = currentBlock->blockName();
    QString outputPortName;
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QString inputsSum = "";
        QVector<QString> incomingSignals;
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            if (connections.size() > 0)
            {
                for (int i = 0; i < connections.size(); i++)
                {
                    QNEConnection *currentConnection = connections.at(i);
                    QNEPort *port1 = currentConnection->port1();
                    QNEPort *port2 = currentConnection->port2();
                    QString signal_name;

                    if (port1->isOutput())
                    {
                        QNEBlock *incomingBlock = port1->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port1->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                    else if (port2->isOutput())
                    {
                        QNEBlock *incomingBlock = port2->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port2->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                }
                for (int j = 0; j < incomingSignals.size(); j++)
                {
                    if (j < incomingSignals.size() - 1)
                    {
                        inputsSum.append(incomingSignals.at(j) + " + ");
                    }
                    else
                    {
                        inputsSum.append(incomingSignals.at(j));
                    }
                }
                if (inputsSum != "")
                {
                    inputsSums.append(inputsSum);
                }
            }
            else if (port->thirdPtr() == 6)
            {
                inputsSums.append("0");
            }

        }
        else
        {
           outputPortName = port->portName();
        }
    }
    for (int i = 0 ; i < inputsSums.size(); i++)
    {
        if ( i < inputsSums.size() - 1)
        {
            params.append(inputsSums.at(i) + ", ");
        }
        else
        {
            params.append(inputsSums.at(i));
        }
    }
    QString clock = inputsSums.at(0);
    QString lengthInput  = inputsSums.at(1);
    QString reset  = inputsSums.at(2);
    QString count  = object_name + "_currentCount";
    QString reals = "";
    QString real = "real_";
    instruction.append("    if(clockTrigger"+ object_name +".process(" + clock + "))\n    {\n");
    instruction.append("    " + count + "++;\n    }\n");
    if (lengthInput == "0")
    {
        instruction.append("    if(" + count + ">" + counterLength + ")\n    {\n");
    }
    else
    {
        instruction.append("    if(" +count + ">=" + lengthInput + ")\n    {\n");
    }
    instruction.append("    " + count +  "= 0;\n    }\n");
    instruction.append("    if (" + reset + " > 0.2 ) \n    {\n     " + count + " = 0;\n   }\n");
    instruction.append("    int " + object_name + "_" + outputPortName + " = "  + count + ";\n");


    QString instructions = instruction.at(0) + instruction.at(1) + instruction.at(2) + instruction.at(3) + instruction.at(4) + instruction.at(5);
    structInstructions.append("    int " + object_name + "_currentCount" + " = 0;\n");
    structInstructions.append("    dsp::SchmittTrigger clockTrigger" + object_name  + ";\n");
    return instructions;
}

QString StepWriter::arrayInstruction(QNEBlock *currentBlock)
{
    QVector<QString> instruction;
    QString params = "";
    QVector<QNEPort*> ports = currentBlock->ports();
    QVector<QString> inputsSums;
    QVector<bool> outputConnected;
    QString object_Type = ports.at(0)->portName();
    QString object_name = ports.at(2)->portName();
    QString _realSize = ports.at(3)->portName();
    QString realSize;
    QString _values = ports.at(4)->portName();
    QString values;
    QString object_type = currentBlock->blockName();
    QString outputPortName;
    if (_realSize.contains("/"))
    {
        realSize = _realSize.left(_realSize.size() - 1);
    }
    else
    {
        realSize = _realSize;
    }
    if (_values.contains("/"))
    {
        values = _values.left(_values.size() - 1);
    }
    else
    {
        values = _values;
    }
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QString inputsSum = "";
        QVector<QString> incomingSignals;
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            if (connections.size() > 0)
            {
                for (int i = 0; i < connections.size(); i++)
                {
                    QNEConnection *currentConnection = connections.at(i);
                    QNEPort *port1 = currentConnection->port1();
                    QNEPort *port2 = currentConnection->port2();
                    QString signal_name;

                    if (port1->isOutput())
                    {
                        QNEBlock *incomingBlock = port1->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port1->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                    else if (port2->isOutput())
                    {
                        QNEBlock *incomingBlock = port2->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port2->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                }
                for (int j = 0; j < incomingSignals.size(); j++)
                {
                    if (j < incomingSignals.size() - 1)
                    {
                        inputsSum.append(incomingSignals.at(j) + " + ");
                    }
                    else
                    {
                        inputsSum.append(incomingSignals.at(j));
                    }
                }
                if (inputsSum != "")
                {
                    inputsSums.append(inputsSum);
                }
            }
            else if (port->thirdPtr() == 6)
            {
                inputsSums.append("0");
            }

        }
        else
        {
            if (connections.size() > 0)
            {
                   outputConnected.append(true);
            }
            else
            {
                  outputConnected.append(false);
            }
            outputPortName = port->portName();
        }
    }
    structInstructions.append("    float " + object_name  + "_inputs[" + realSize + "] = {" + values + "};\n");
    structInstructions.append("    float " + object_name  + "[" + realSize + "] = {" + values + "};\n");
    for (int i = 0 ; i < inputsSums.size() - 2; i++)
    {
        instruction.append("    " + object_name  + "_inputs[" +  QString::number(i) + "] = " + inputsSums.at(i) + ";\n ");
    }

    QString reals = "";
    QString real = "real_";
    int arraylength = 0;
    if (object_Type == "array_4")
    {
        arraylength = 4;
        QString index = ports.at(13)->portName();
        instruction.append("    for(int " + object_name  + "_realsize = 0; " + object_name  + "_realsize < " + realSize + ";" + object_name  + "_realsize ++)\n    {");
        instruction.append("\n        if(" +object_name  + "_realsize" + "!=" + inputsSums.at(arraylength) + ")\n        {\n");
        instruction.append("            " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + object_name  + "_realsize];\n        }\n");
        if (ports.at(15)->connections().size() <= 0)
        {
            instruction.append("        else\n        {\n             " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + inputsSums.at(arraylength)  + "];\n        }\n    }\n");
        }
        else
        {
            if (inputsSums.at(arraylength + 1) == "0")
            {
               instruction.append("        else\n        {\n             " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + QString::number(arraylength)  + "];\n        }\n    }\n");
            }
            else
            {
                instruction.append("        else\n        {\n             " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + QString::number(arraylength + 1)  + "];\n        }\n    }\n");
            }
        }
    }
    else if (object_Type == "array_8")
    {
        arraylength = 8;
        QString index = ports.at(17)->portName();
        instruction.append("    for(int " + object_name  + "_realsize = 0; " + object_name  + "_realsize < " + realSize + ";" + object_name  + "_realsize ++)\n    {");
        instruction.append("\n        if(" +object_name  + "_realsize" + "!=" + inputsSums.at(arraylength) + ")\n        {\n");
        instruction.append("            " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + object_name  + "_realsize];\n        }\n");
        if (ports.at(26)->connections().size() <= 0)
        {
            instruction.append("        else\n        {\n     " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + inputsSums.at(arraylength)  + "];\n        }\n    }\n");
        }
        else
        {
            if (inputsSums.at(arraylength + 1) == "0")
            {
               instruction.append("        else\n        {\n     " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + QString::number(arraylength)  + "];\n        }\n    }\n");
            }
            else
            {
                instruction.append("        else\n        {\n     " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + QString::number(arraylength + 1)  + "];\n        }\n    }\n");
            }
        }
    }
    else if (object_Type == "array_16")
    {
        arraylength = 16;
        QString index = ports.at(24)->portName();
        instruction.append("    for(int " + object_name  + "_realsize = 0; " + object_name  + "_realsize < " + realSize + ";" + object_name  + "_realsize ++)\n    {");
        instruction.append("\n        if(" +object_name  + "_realsize" + "!=" + inputsSums.at(arraylength) + ")\n        {\n");
        instruction.append("            " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + object_name  + "_realsize];\n        }\n");
        if (ports.at(27)->connections().size() <= 0)
        {
            instruction.append("        else\n        {\n     " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + inputsSums.at(arraylength)  + "];\n        }\n    }\n");
        }
        else
        {
            if (inputsSums.at(arraylength + 1) == "0")
            {
               instruction.append("        else\n        {\n     " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + QString::number(arraylength)  + "];\n        }\n    }\n");
            }
            else
            {
                instruction.append("        else\n        {\n     " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + QString::number(arraylength + 1)  + "];\n        }\n    }\n");
            }
        }
    }
    else if (object_Type == "array_32")
    {
        arraylength = 32;
        QString index = ports.at(41)->portName();
        instruction.append("    for(int " + object_name  + "_realsize = 0; " + object_name  + "_realsize < " + realSize + ";" + object_name  + "_realsize ++)\n    {");
        instruction.append("\n        if(" +object_name  + "_realsize" + "!=" + inputsSums.at(arraylength) + ")\n        {\n");
        instruction.append("            " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + object_name  + "_realsize];\n        }\n");
        if (ports.at(33)->connections().size() <= 0)
        {
            instruction.append("        else\n        {\n     " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + inputsSums.at(arraylength)  + "];\n        }\n    }\n");
        }
        else
        {
            if (inputsSums.at(arraylength + 1) == "0")
            {
               instruction.append("        else\n        {\n     " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + QString::number(arraylength)  + "];\n        }\n    }\n");
            }
            else
            {
                instruction.append("        else\n        {\n     " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + QString::number(arraylength + 1)  + "];\n        }\n    }\n");
            }
        }
    }
    else if (object_Type == "array_64")
    {
        arraylength = 64;
        QString index = ports.at(73)->portName();
        instruction.append("    for(int " + object_name  + "_realsize = 0; " + object_name  + "_realsize < " + realSize + ";" + object_name  + "_realsize ++)\n    {");
        instruction.append("\n        if(" +object_name  + "_realsize" + "!=" + inputsSums.at(arraylength) + ")\n        {\n");
        instruction.append("            " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + object_name  + "_realsize];\n        }\n");
        if (ports.at(75)->connections().size() <= 0)
        {
            instruction.append("        else\n        {\n     " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + inputsSums.at(arraylength)  + "];\n        }\n    }\n");
        }
        else
        {
            if (inputsSums.at(arraylength + 1) == "0")
            {
               instruction.append("        else\n        {\n     " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + QString::number(arraylength)  + "];\n        }\n    }\n");
            }
            else
            {
                instruction.append("        else\n        {\n     " + object_name  + "[" +  object_name  + "_realsize " + "] = " + object_name  + "_inputs[" + QString::number(arraylength + 1)  + "];\n        }\n    }\n");
            }
        }
    }
    for (int o = 0; o < outputConnected.size(); o++)
    {
        if (outputConnected.at(o) == true)
        {
            int ind = o + 1;
            if (o < outputConnected.size() - 1)
            {
                instruction.append("    float " + object_name  + "_" + QString::number(ind) + " = " + object_name  + "[" + QString::number(o) + "];\n");
            }
            else
            {
                instruction.append("    float " + object_name  + "_out" + " = " + object_name  + "[" + inputsSums.at(arraylength) + "];\n");
            }
        }
    }

    QString instructions;
    for (int k = 0; k < instruction.size(); k++)
    {
        instructions = instructions + instruction.at(k);
    }
    return instructions;
}

QString StepWriter::operationInstruction(QNEBlock *currentBlock)
{
    QString instruction = "0";
    QVector<QString> params;
    QVector<QNEPort*> ports = currentBlock->ports();
    QVector<QString> inputsSums;
    QString object_name = ports.at(2)->portName();
    QString object_type = currentBlock->blockName();
    QString outputPortName;
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QString inputsSum = "";
        QVector<QString> incomingSignals;
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            if (connections.size() > 0)
            {
                for (int i = 0; i < connections.size(); i++)
                {
                    QNEConnection *currentConnection = connections.at(i);
                    QNEPort *port1 = currentConnection->port1();
                    QNEPort *port2 = currentConnection->port2();
                    QString signal_name;

                    if (port1->isOutput())
                    {
                        QNEBlock *incomingBlock = port1->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port1->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                    else if (port2->isOutput())
                    {
                        QNEBlock *incomingBlock = port2->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port2->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                }
                for (int j = 0; j < incomingSignals.size(); j++)
                {
                    if (j < incomingSignals.size() - 1)
                    {
                        inputsSum.append(incomingSignals.at(j) + " + ");
                    }
                    else
                    {
                        inputsSum.append(incomingSignals.at(j));
                    }
                }
                if (inputsSum != "")
                {
                    inputsSums.append(inputsSum);
                }
            }
            else if (port->thirdPtr() == 6)
            {
                inputsSums.append("0");
            }
        }
        else
        {
           outputPortName = port->portName();
        }
    }
    for (int i = 0 ; i < inputsSums.size(); i++)
    {
            params.append(inputsSums.at(i));
    }

    if (object_type == "(x*y)")
    {
       instruction = "    float "  + object_name + "_" + outputPortName + " = " + params.at(0) + "*" +  params.at(1) + ";";
    }
    else if (object_type == "(x/y)")
    {
       instruction = "    float " + object_name + "_" + outputPortName + " = " + params.at(0) + "/" +  params.at(1) + ";";
    }
    else if (object_type == "(x+y)")
    {
       instruction = "    float " + object_name + "_" + outputPortName + " = " + params.at(0) + "+" +  params.at(1) + ";";
    }
    else if (object_type == "(x-y)")
    {
       instruction = "  float " + object_name + "_" + outputPortName + " = " + params.at(0) + "-" +  params.at(1) + ";";
    }
    else if (object_type == "(x%y)")
    {
       instruction = "    float " + object_name + "_" + outputPortName + " = " + params.at(0) + "%" +  params.at(1) + ";";
    }
    else if (object_type == "x<y")
    {
       instruction = "    float " + object_name + "_" + outputPortName + " = 0;\n" + "  if (" +  params.at(0) + " < " + params.at(1) + ") \n" + "    { \n" + "   " + object_name + "_" + outputPortName + " = 1;\n   }";
    }
    else if (object_type == "x>y")
    {
       instruction = "    float " + object_name + "_" + outputPortName + " = 0;\n" + "  if (" +  params.at(0) + " > " + params.at(1) + ") \n" + "   { \n" + "   " + object_name + "_" + outputPortName + " = 1;\n   }";
    }
    else if (object_type == "x<=y")
    {
       instruction = "    float " + object_name + "_" + outputPortName + " = 0;\n" + "  if (" +  params.at(0) + " <= " + params.at(1) + ") \n" + "  { \n" + "      " + object_name + "_" + outputPortName + " = 1;\n    }";
    }
    else if (object_type == "x=y")
    {
       instruction = "    float " + object_name + "_" + outputPortName + " = 0;\n" + "  if (" +  params.at(0) + " == " + params.at(1) + ") \n" + "  { \n" + "      " + object_name + "_" + outputPortName + " = 1;\n    }";
    }
    else if (object_type == "x>=y")
    {
       instruction = "    float " + object_name + "_" + outputPortName + " = 0;\n" + "  if (" +  params.at(0) + " >= " + params.at(1) + ") \n" + "  { \n" + "      " + object_name + "_" + outputPortName + " = 1;\n    }";
    }
    else if (object_type == "x and y")
    {
        instruction = "    float " + object_name + "_" + outputPortName + " = 0;\n" + "  if (" +  params.at(0) + " != 0" + " && " + params.at(1) + " != 0" + ") \n" + "{ \n" + "   " + object_name + "_" + outputPortName + " = 1;\n    }";
    }
    else if (object_type == "x||y")
    {
        instruction = "    float " + object_name + "_" + outputPortName + " = 0;\n" + "  if (" +  params.at(0) + " != 0" + " || " + params.at(1) + " != 0" + ") \n" + "{ \n" + "     " + object_name + "_" + outputPortName + " = 1;\n  }";
    }
    else if (object_type == "XOR")
    {
        instruction = "    float " + object_name + "_" + outputPortName + " = 0;\n" + "  if ((" +  params.at(0) + " != 0" + " && " + params.at(1) + " == 0)" + " || " + "(" +  params.at(1) + " != 0" + " && " + params.at(0) + " == 0)" + ") \n" + "   { \n" + "     " + object_name + "_" + outputPortName + " = 5;\n   }";
    }
    else if (object_type == "x_to_the_y(power)")
    {
        instruction = "    float " + object_name + "_" + outputPortName + " = powf(" + params.at(0) + "," +  params.at(1) + ");";
    }
    else if (object_type == "sqrt")
    {
        instruction = "    sqrt " + object_name + "_" + outputPortName + " = sqrt(" + params.at(0) + ");";
    }
    return instruction;
}

QString StepWriter::gateToTrigInstruction(QNEBlock *currentBlock)
{
    QVector<QString> instructions;
    QVector<QString> params;
    QVector<QNEPort*> ports = currentBlock->ports();
    QVector<QString> inputsSums;
    QString object_name = ports.at(2)->portName();
    QString object_type = currentBlock->blockName();
    QString outputPortName;
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QString inputsSum = "";
        QVector<QString> incomingSignals;
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            if (connections.size() > 0)
            {
                for (int i = 0; i < connections.size(); i++)
                {
                    QNEConnection *currentConnection = connections.at(i);
                    QNEPort *port1 = currentConnection->port1();
                    QNEPort *port2 = currentConnection->port2();
                    QString signal_name;

                    if (port1->isOutput())
                    {
                        QNEBlock *incomingBlock = port1->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port1->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                    else if (port2->isOutput())
                    {
                        QNEBlock *incomingBlock = port2->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port2->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                }
                for (int j = 0; j < incomingSignals.size(); j++)
                {
                    if (j < incomingSignals.size() - 1)
                    {
                        inputsSum.append(incomingSignals.at(j) + " + ");
                    }
                    else
                    {
                        inputsSum.append(incomingSignals.at(j));
                    }
                }
                if (inputsSum != "")
                {
                    inputsSums.append(inputsSum);
                }
            }
            else if (port->thirdPtr() == 6)
            {
                inputsSums.append("0");
            }
        }
        else
        {
           outputPortName = port->portName();
        }
    }

    QString Wtrigged = object_name + "_wasTrigged";
    QString fPulse = object_name + "_pulse";
    instructions.append("    bool " + fPulse + " = false;\n");
    instructions.append("    if(" + inputsSums.at(0) + " != 0 && " + Wtrigged + " == false)\n    {\n");
    instructions.append("        " + Wtrigged + " = true;\n");
    instructions.append("        " + fPulse + " = true;\n   }");
    instructions.append("    else if( "+ inputsSums.at(0) + " == 0)"+ "\n    {\n    " + Wtrigged + " = false;\n      }\n");
    instructions.append("    float " + object_name + "_" + outputPortName + " = " + fPulse + " ? 10.0 : 0.0;\n");
    QString instruction = instructions.at(0) + instructions.at(1) + instructions.at(2) + instructions.at(3) + instructions.at(4) + instructions.at(5);
    structInstructions.append("    bool " + Wtrigged  + " = false;\n");
    return instruction;
}

QString StepWriter::clockInstruction(QNEBlock *currentBlock)
{
    QVector<QString> instructions;
    QVector<QString> params;
    QVector<QNEPort*> ports = currentBlock->ports();
    QVector<QString> inputsSums;
    QString object_name = ports.at(2)->portName();
    QString object_type = currentBlock->blockName();
    QString outputPortName;
    for(int portIndex = 0; portIndex < ports.size();  portIndex ++)
    {
        QString inputsSum = "";
        QVector<QString> incomingSignals;
        QNEPort *port = ports.at(portIndex);
        QVector<QNEConnection*> connections = port->connections();
        if (port->isOutput() == false)
        {
            if (connections.size() > 0)
            {
                for (int i = 0; i < connections.size(); i++)
                {
                    QNEConnection *currentConnection = connections.at(i);
                    QNEPort *port1 = currentConnection->port1();
                    QNEPort *port2 = currentConnection->port2();
                    QString signal_name;

                    if (port1->isOutput())
                    {
                        QNEBlock *incomingBlock = port1->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port1->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                    else if (port2->isOutput())
                    {
                        QNEBlock *incomingBlock = port2->block();
                        QString incomingBlockVarName;
                        QString incomingPortName = port2->portName();
                        QVector<QNEPort*> Iports = incomingBlock->ports();
                        incomingBlockVarName = Iports.at(2)->portName();
                        incomingSignals.append(incomingBlockVarName + "_" + incomingPortName);
                    }
                }
                for (int j = 0; j < incomingSignals.size(); j++)
                {
                    if (j < incomingSignals.size() - 1)
                    {
                        inputsSum.append(incomingSignals.at(j) + " + ");
                    }
                    else
                    {
                        inputsSum.append(incomingSignals.at(j));
                    }
                }
                if (inputsSum != "")
                {
                    inputsSums.append(inputsSum);
                }
            }
            else if (port->thirdPtr() == 6)
            {
                inputsSums.append("0");
            }
        }
        else
        {
           outputPortName = port->portName();
        }
    }
    QString clockTime = object_name + "_clockTime";
    QString phase = object_name + "_phase";
    QString nextStep = object_name + "_nextStep";
    QString reset = inputsSums.at(1);
    QString wasReset = object_name + "_wasReset";
    instructions.append("    bool " + nextStep + " = false;\n");
    instructions.append("    float " + clockTime + " = powf(2.0, " + inputsSums.at(0) + ");\n");
    instructions.append("    " + phase + " += " + clockTime + "/args.sampleRate;\n");
    if (reset == "0")
    {
        instructions.append("    if ("  + phase + "  != 0)\n    {\n");
    }
    else
    {
        instructions.append("    if (" + reset + " == 0 )\n    {\n");
    }

    instructions.append("        if (" + phase + " >= 1.0)\n        {\n       " + phase + "  -= 1.0;\n");
    instructions.append("        " + nextStep + " = true;\n      " + wasReset + " = false;\n        }\n    }\n");
    if (reset != "0")
    {
        instructions.append("        else if(" + wasReset + " == false) \n    {\n      " + nextStep + " = true;\n        " + wasReset + " = true;\n        " + phase + " = 0;\n    }\n");
    }
    instructions.append("    float " + object_name + "_" + outputPortName + " = 0;\n");
    instructions.append("    if(" + nextStep + " == true)\n    {\n        " + object_name + "_" + outputPortName + "= 5.0;\n    }\n");
    instructions.append("    else\n    {\n        " + object_name + "_" + outputPortName + " = 0.0;\n    }\n" );
    QString instruction = "";
    if (reset == "0")
    {
        instruction = instructions.at(0) + instructions.at(1) + instructions.at(2) + instructions.at(3) + instructions.at(4) + instructions.at(5) + instructions.at(6) + instructions.at(7) + instructions.at(8);
    }
    else
    {
        instruction = instructions.at(0) + instructions.at(1) + instructions.at(2) + instructions.at(3) + instructions.at(4) + instructions.at(5) + instructions.at(6) + instructions.at(7) + instructions.at(8) + instructions.at(9);
    }
    structInstructions.append("    bool " + wasReset  + " = false;\n");
    structInstructions.append("    float " + phase  + " = 0;\n");
    return instruction;
}






















