/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include "Headers/qnemainwindow.h"
#include "Headers/ui_qnemainwindow.h"
#include "Headers/qneblock.h"
#include "Headers/qnodeseditor.h"
#include "Headers/paneleditor.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QFileDialog>
#include "Headers/qneport.h"
#include "Headers/panel.h"
#include "Headers/controller.h"
#include "Headers/property.h"
#include "Headers/paneleditor.h"
#include <QGraphicsSvgItem>

QNEMainWindow::QNEMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    textFont = "LEDCounter7.ttf";
    nodes_editor_scene = new QGraphicsScene();
    panel_editor_scene = new QGraphicsScene();
    loadBlocksFiles();
    const QIcon loadIcon = QIcon("icons/load.png");
    QToolBar *mainToolBar = addToolBar(tr("Edit"));
   // mainToolBar->setOrientation(Qt::Vertical);
    mainToolBar->setAllowedAreas(Qt::BottomToolBarArea);
    mainToolBar->setMovable(false);
    const QIcon saveIcon = QIcon("icons/save.png");
    //////////////////////////////////////////////////////////////////////////////// Menu pricipal (y compris ajout des blocks) /////////////////////////////////////////////////////////////
    QAction *quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    // no cennect signal et slot:
    // le signal de quitAct envoyé grâce a triggred() arrive jusqu'au slot quit() de qApp l'appilaction se ferme
    connect(quitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

    QAction *loadAct = new QAction(loadIcon, tr("&Load"), this);
    loadAct->setShortcuts(QKeySequence::Open);
    loadAct->setStatusTip(tr("Open a file"));
    // le signal loadact déclanche le solt loadFile (définit dans qneMainWiundow.h)
    connect(loadAct, SIGNAL(triggered()), this, SLOT(loadFile()));

    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save a file"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    QAction *AddBlocks = new QAction(tr("&Add Blocks"), this);
    AddBlocks->setStatusTip(tr("Add a block"));

    mainToolBar->addAction(loadAct);
    mainToolBar->addAction(saveAct);

    const QIcon exportIcon = QIcon("icons/export.png");
    QAction *exportAct = new QAction(exportIcon, tr("&Export project(ctrl+g))"), this);
    exportAct->setStatusTip(tr("Export module code"));
    connect(exportAct, SIGNAL(triggered()), this, SLOT(exportCode()));
    mainToolBar->addAction(exportAct);

    const QIcon duplicateIcon = QIcon("icons/dupplicate.png");
    QAction *dupplicateAct  = new QAction(duplicateIcon, tr("&Dupplicate selected blocks (ctrl+d)"), this);
    dupplicateAct->setStatusTip(tr("Dupplicate selected blocks"));
    connect(dupplicateAct, SIGNAL(triggered()), this, SLOT(duplicate()));
    mainToolBar->addAction(dupplicateAct);

    const QIcon deleteBlockIcon = QIcon("icons/deleteBlock.png");
    QAction *deleteBlockAct = new QAction(deleteBlockIcon, tr("&Delete block (delete key)"), this);
    deleteBlockAct->setStatusTip(tr("Delete selected blocks"));
    connect(deleteBlockAct, SIGNAL(triggered()), this, SLOT(deleteBlock()));
    mainToolBar->addAction(deleteBlockAct);

    const QIcon propOutIcon = QIcon("icons/propVisible.png");
    propOutAct = new QAction(propOutIcon, tr("&properties ('p' key)"), this);
    //propOutAct->isCheckable();
    propOutAct->setStatusTip(tr("properties ('p' key)"));
    connect(propOutAct, SIGNAL(triggered()), this, SLOT(setPropVisible()));
    mainToolBar->addAction(propOutAct);




/*
    const QIcon propIcon = QIcon("icons/propertyVisible.png");
    QToolButton* propToolButton = new QToolButton();
    //propToolButton->setCheckable(true);
    QAction* propToolButtonAction = new QWidgetAction(this);
    propToolButton->setIcon(propIcon);
    propToolButtonAction->setDefaultWidget(propToolButton);
    propToolButton->setToolTip("show properties('p' key)");
    mainToolBar->addAction(propToolButtonAction);
    /*propOutAct->setCheckable(true);
    propOutAct->setStatusTip(tr("set property visibility"));*/
    //connect(propToolButtonAction, SIGNAL(triggered()), this, SLOT(setPropVisible()));
    //mainToolBar->addAction(propToolButtonAction);

    QAction *displayHelpAct = new QAction(tr("&Help"), this);
    displayHelpAct->setShortcuts(QKeySequence::HelpContents);
    displayHelpAct->setStatusTip(tr("display help window"));
    // le signal loadact déclanche le solt loadFile (définit dans qneMainWiundow.h)
    connect(displayHelpAct, SIGNAL(triggered()), this, SLOT(displayHelp()));
    //QSpinBox *spinBox = new QSpinBox(this);

    //toolBar->addWidget(spinBox);
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->setStyleSheet("QMenuBar { background-color: black }");
    fileMenu->addAction(loadAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);
    // on ajoute un menu add blocks
    addBlockMenu = menuBar()->addMenu(tr("&Add Blocks"));
    QMenu* submenuA = addBlockMenu->addMenu( "&Generators" );
    QToolButton* geneToolButton = new QToolButton();
    geneToolButton->setMenu(submenuA);
    geneToolButton->setPopupMode(QToolButton::InstantPopup);
    const QIcon generators = QIcon("icons/Generators.png");
    QWidgetAction* geneToolButtonAction = new QWidgetAction(this);
    geneToolButton->setIcon(generators);
    geneToolButtonAction->setDefaultWidget(geneToolButton);
    geneToolButton->setToolTip("generators");
    mainToolBar->addAction(geneToolButtonAction);
    QmenuVec.append(submenuA);

    QMenu* submenuB = addBlockMenu->addMenu( "&Processors" );
    QToolButton* procToolButton = new QToolButton();
    procToolButton->setMenu(submenuB);
    procToolButton->setPopupMode(QToolButton::InstantPopup);
    const QIcon processors = QIcon("icons/Processors.png");
    QWidgetAction* procToolButtonAction = new QWidgetAction(this);
    procToolButton->setIcon(processors);
    procToolButtonAction->setDefaultWidget(procToolButton);
    procToolButton->setToolTip("processors");
    mainToolBar->addAction(procToolButtonAction);
    QmenuVec.append(submenuB);

    QMenu* submenuC = addBlockMenu->addMenu( "&Numbers" );
    QToolButton* numToolButton = new QToolButton();
    numToolButton->setMenu(submenuC);
    numToolButton->setPopupMode(QToolButton::InstantPopup);
    const QIcon numbers = QIcon("icons/Numbers.png");
    QWidgetAction* numToolButtonAction = new QWidgetAction(this);
    numToolButton->setIcon(numbers);
    numToolButtonAction->setDefaultWidget(numToolButton);
    numToolButton->setToolTip("numbers");
    mainToolBar->addAction(numToolButtonAction);
    QmenuVec.append(submenuC);

    QMenu* submenuD = addBlockMenu->addMenu( "&Arrays" );
    QToolButton* arrToolButton = new QToolButton();
    arrToolButton->setMenu(submenuD);
    arrToolButton->setPopupMode(QToolButton::InstantPopup);
    const QIcon arrays = QIcon("icons/Arrays.png");
    QWidgetAction* arrToolButtonAction = new QWidgetAction(this);
    arrToolButton->setIcon(arrays);
    arrToolButtonAction->setDefaultWidget(arrToolButton);
    arrToolButton->setToolTip("Arrays");
    mainToolBar->addAction(arrToolButtonAction);
    QmenuVec.append(submenuD);

    QMenu* submenuE = addBlockMenu->addMenu( "&Maths");
    QToolButton* mathToolButton = new QToolButton();
    mathToolButton->setMenu(submenuE);
    mathToolButton->setPopupMode(QToolButton::InstantPopup);
    const QIcon maths = QIcon("icons/Maths.png");
    QWidgetAction* mathToolButtonAction = new QWidgetAction(this);
    mathToolButton->setIcon(maths);
    mathToolButtonAction->setDefaultWidget(mathToolButton);
    mathToolButton->setToolTip("Maths");
    mainToolBar->addAction(mathToolButtonAction);
    QmenuVec.append(submenuE);

    QMenu* submenuF = addBlockMenu->addMenu( "&Counters/Seq");
    QToolButton* countToolButton = new QToolButton();
    countToolButton->setMenu(submenuF);
    countToolButton->setPopupMode(QToolButton::InstantPopup);
    const QIcon counter = QIcon("icons/Counters.png");
    QWidgetAction* countToolButtonAction = new QWidgetAction(this);
    countToolButton->setIcon(counter);
    countToolButtonAction->setDefaultWidget(countToolButton);
    countToolButton->setToolTip("Counters");
    mainToolBar->addAction(countToolButtonAction);
    QmenuVec.append(submenuF);

    QMenu* submenuG = addBlockMenu->addMenu( "&Comparators");
    QToolButton* compToolButton = new QToolButton();
    compToolButton->setMenu(submenuG);
    compToolButton->setPopupMode(QToolButton::InstantPopup);
    const QIcon comparators = QIcon("icons/Comparators.png");
    QWidgetAction* compToolButtonAction = new QWidgetAction(this);
    compToolButton->setIcon(comparators);
    compToolButtonAction->setDefaultWidget(compToolButton);
    compToolButton->setToolTip("Comparators");
    mainToolBar->addAction(compToolButtonAction);
    QmenuVec.append(submenuG);

    QMenu* submenuH = addBlockMenu->addMenu( "&I/O");
    QToolButton* jackToolButton = new QToolButton();
    jackToolButton->setMenu(submenuH);
    jackToolButton->setPopupMode(QToolButton::InstantPopup);
    const QIcon jacks = QIcon("icons/Jacks.png");
    QWidgetAction* jackToolButtonAction = new QWidgetAction(this);
    jackToolButton->setIcon(jacks);
    jackToolButtonAction->setDefaultWidget(jackToolButton);
    jackToolButton->setToolTip("Inputs and outpus");
    mainToolBar->addAction(jackToolButtonAction);
    QmenuVec.append(submenuH);

    QMenu* submenuI = addBlockMenu->addMenu( "&Controllers");
    QToolButton* ctlToolButton = new QToolButton();
    ctlToolButton->setMenu(submenuI);
    ctlToolButton->setPopupMode(QToolButton::InstantPopup);
    const QIcon controllers = QIcon("icons/Controllers.png");
    QWidgetAction* ctlToolButtonAction = new QWidgetAction(this);
    ctlToolButton->setIcon(controllers);
    ctlToolButton->setToolTip("Controllers");
    ctlToolButtonAction->setDefaultWidget(ctlToolButton);
    mainToolBar->addAction(ctlToolButtonAction);
    QmenuVec.append(submenuI);

    QMenu* submenuJ = addBlockMenu->addMenu( "&Others");
    QToolButton* othersToolButton = new QToolButton();
    othersToolButton->setMenu(submenuJ);
    othersToolButton->setPopupMode(QToolButton::InstantPopup);
    const QIcon others = QIcon("icons/Others.png");
    QWidgetAction* othersToolButtonAction = new QWidgetAction(this);
    othersToolButton->setIcon(others);
    othersToolButtonAction->setDefaultWidget(othersToolButton);
    othersToolButton->setToolTip("Others");
    mainToolBar->addAction(othersToolButtonAction);
    QmenuVec.append(submenuJ);

    editMenu = menuBar()->addMenu(tr("&Help"));
    editMenu->addAction(displayHelpAct);

    const QIcon displayControllerIcon = QIcon("icons/displayController.png");
    QAction *displayControllerAct = new QAction(displayControllerIcon, tr("&Display controller (ctrl+t))"), this);
    displayControllerAct->setStatusTip(tr("Display selected blocks on the panel editor"));
    connect(displayControllerAct, SIGNAL(triggered()), this, SLOT(displayOnPanel()));
    mainToolBar->addAction(displayControllerAct);

    const QIcon deleteControllerIcon = QIcon("icons/deleteController.png");
    QAction *deleteControllerAct = new QAction(deleteControllerIcon, tr("&Delete controller (right click)"), this);
    deleteControllerAct->setStatusTip(tr("Delete selected controller"));
    connect(deleteControllerAct, SIGNAL(triggered()), this, SLOT(deleteController()));
    mainToolBar->addAction(deleteControllerAct);

    const QIcon ZoomInIcon = QIcon("icons/zoomIn.png");
    QAction *ZoomInAct = new QAction(ZoomInIcon, tr("&Zoom in - panel editor ('h' key)"), this);
    ZoomInAct->setStatusTip(tr("Zoom in - panel editor"));
    connect(ZoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));
    mainToolBar->addAction(ZoomInAct);

    const QIcon ZoomOutIcon = QIcon("icons/zoomOut.png");
    QAction *ZoomOutAct = new QAction(ZoomOutIcon, tr("&Zoom out - panel editor ('g' key)"), this);
    ZoomOutAct->setStatusTip(tr("Zoom out - panel editor"));
    connect(ZoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));
    mainToolBar->addAction(ZoomOutAct);


    CustomDialog dial("Object setting", this);
    for (int i = 0; i <blockModelVec.size(); i++)
    {
        blockmodel *currentBlockmodel =  blockModelVec.at(i);
        QVector<QString> namesVec = currentBlockmodel->getNameVector();
        QVector<int> typesVec = currentBlockmodel->getTypeVector();
        QString itemName = "&";
        itemName.append(namesVec.at(1));
        dial.addLabel(itemName);

        for(int j = 0 ; j< QmenuVec.size(); j++)
        {
            //dial.addLabel(QmenuVec.at(j)->title());
            QMenu *subMenu = QmenuVec.at(j);
            if (itemName == subMenu->title())
            {
                QString mainType = "&";
                mainType.append(namesVec.at(0));
                QAction *currentAction = new QAction(mainType, this);
                QString statut = "";
                statut.append(mainType);
                currentAction->setStatusTip(statut);
                if (namesVec.at(0) != "plugin_setting" && namesVec.at(0) != "smart_panel")
                {
                    subMenu->addAction(currentAction);
                    dial.addLabel(statut);
                    connect(currentAction, SIGNAL(triggered()), this, SLOT(setNewItemCalling()));
                    connect(subMenu, SIGNAL(triggered(QAction*)), this, SLOT(addBlock(QAction*)));
                }
                QString *s = new QString(currentAction->statusTip());
                loadedBlockNameVec.append(s);
            }
        }
    }

    //dial.exec();
    setWindowTitle(tr("Geco"));

    QDockWidget *nodes_editor_dock = new QDockWidget(tr("Nodes"), this);
    nodes_editor_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    nodes_editor_view = new QGraphicsView(nodes_editor_dock);
    nodes_editor_view->setBackgroundBrush(QBrush(QColor(61,61,61), Qt::SolidPattern));
    nodes_editor_view->setScene(nodes_editor_scene);
    nodes_editor_dock->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    nodes_editor_dock->setFeatures(0);
    // This property holds the default render hints for the view.
    nodes_editor_view->setRenderHint(QPainter::Antialiasing, true);
    //Sets the widget for the dock widget to widget.
    nodes_editor_dock->setWidget(nodes_editor_view);
    nodes_editor_dock->setStyleSheet("QDockWidget {color:  #ffe1ae; background-color: lightGray}");
    addDockWidget(Qt::LeftDockWidgetArea, nodes_editor_dock);
    nodesEditor = new QNodesEditor(this);
    nodesEditor->install(nodes_editor_scene);

    QDockWidget *panel_editor_dock = new QDockWidget(tr("Panel"), this);
    panel_editor_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    panel_editor_dock->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    panel_editor_dock->setFeatures(0);
    panel_editor_view = new QGraphicsView(panel_editor_dock);
    panel_editor_view->setBackgroundBrush(QBrush(QColor(61,61,61), Qt::SolidPattern));
    panel_editor_view->setScene(panel_editor_scene);
    // This property holds the default render hints for the view.
    panel_editor_view->setRenderHint(QPainter::Antialiasing, true);
    //Sets the widget for the dock widget to widget.
    panel_editor_dock->setWidget(panel_editor_view);
    panel_editor_dock->setStyleSheet("QDockWidget {color:  #d48300; background-color: lightGray}");
    addDockWidget(Qt::RightDockWidgetArea, panel_editor_dock);
    paneleditor = new Paneditor(this);
    paneleditor->install(panel_editor_scene, nodes_editor_scene);
    blockID = 0;
    setNewItemCalling();
    QAction *addpluginBlock = new QAction (tr("smart_panel"), this);
    addpluginBlock->setStatusTip("&smart_panel");
    addBlock(addpluginBlock);
    setNewItemCalling();
    QAction *s_m_Block = new QAction (tr("plugin_setting"), this);
    s_m_Block->setStatusTip("&plugin_setting");
    addBlock(s_m_Block);
    propVisible = true;
}

QNEMainWindow::~QNEMainWindow()
{

}
void QNEMainWindow::deleteController()
{
    paneleditor->eraseSelectedItem();
}
void QNEMainWindow::zoomOut()
{
    panel_editor_view->scale(0.9, 0.9);
}

void QNEMainWindow::zoomIn()
{
    panel_editor_view->scale(1.1, 1.1);
}
void QNEMainWindow::setPropVisible()
{
    if (propVisible == false)
    {
        propVisible = true;
        const QIcon propOutIcon = QIcon("icons/propVisible.png");
        propOutAct->setIcon(propOutIcon);
    }
    else if (propVisible == true)
    {
        propVisible = false;
        const QIcon propOutIcon = QIcon("icons/propUnvisible.png");
        propOutAct->setIcon(propOutIcon);
    }
    nodesEditor->propVisible(propVisible);
}
void QNEMainWindow::deleteBlock()
{
    foreach(QGraphicsItem *item, nodes_editor_scene->items()  )
    {
        if (item->type() == QNEBlock::Type &&  nodes_editor_scene->isActive())
        {
            QNEBlock *block = (QNEBlock*) item;
            if (item->isSelected() && block->blockName() != "plugin_setting" && block->blockName() != "smart_panel")
            {
                CustomDialog d("!! no undo !!", this);
                d.addLabel("DELETE:");
                d.addLabel("Are you sure than you want to delete this block?");
                d.exec();
                if (d.wasCancelled() == false)
                {
                    QVector<QNEPort*> ports = block->ports();
                    QString object_name = ports.at(2)->portName();
                        foreach(QGraphicsItem *itemP, panel_editor_scene->items()  )
                        {
                            QString itemId = itemP->toolTip();
                            itemId.truncate(object_name.size());
                            if( itemId == object_name)
                            {
                                delete itemP;
                            }
                        }
                    delete item;
                }
                break;
            }
        }
    }
    loadControllerParamVec();
}
void QNEMainWindow::duplicate()
{
    ctrl = false;
    foreach(QGraphicsItem *item, nodes_editor_scene->items())
    {
        if (item->type() == QNEBlock::Type)
        {
            if (item->isSelected())
            {
                 QNEBlock *b =(QNEBlock*) item;
                 //blockID++;
                 if (b->blockName() != "plugin_setting" && b->blockName() != "smart_panel")
                 {
                     b->clone(blockID);
                     blockID++;
                 }
            }
        }
    }
}
void QNEMainWindow::exportCode()
{
    QString plugDir = QFileDialog::getExistingDirectory();
    if (plugDir.isEmpty())
        return;
    loadControllerParamVec();

    bool exaustiveProject =false;
    int conditions = 0;
    int controllerBlockN = 0;
    foreach(QGraphicsItem *item, nodes_editor_scene->items())
    {
        if (item->type() == QNEBlock::Type)
        {
            foreach(QGraphicsItem *port_, item->childItems())
            {
                 QNEPort *port = (QNEPort*) port_;
                 QString pName = port->portName();

                 if (pName == "smart_panel" || pName == "plugin_setting")
                 {
                     conditions++;
                 }

                 if (pName == "Controllers" || pName == "I/O")
                 {
                     controllerBlockN++;
                 }
            }
        }
    }

    if (panelControllerParams.size() >= controllerBlockN + 4)
    {
        conditions++;
    }
    if (conditions >= 3)
    {
        exaustiveProject =true;
    }
    if (exaustiveProject == true)
    {
        canWriter = new Canvawrite(plugDir);
        extWriter = new ExternalWriter(plugDir);
        stepWriter = new StepWriter();

        foreach(QGraphicsItem *item, nodes_editor_scene->items())
        {
            if (item->type() == QNEBlock::Type)
            {
                foreach(QGraphicsItem *port_, item->childItems())
                {
                     QNEPort *port = (QNEPort*) port_;
                     QString pName = port->portName();
                     if (pName.contains("/") && pName != "I/O" && pName != "(x/y)")
                     {
                         pName = pName.left(pName.size() - 1);
                     }
                     //int x = portVec.size();
                     //QString gg = QString::number(x);
                     QString gg = pName;
                     QByteArray s = gg.toUtf8();
                     qDebug(s);
                     foreach(QGraphicsItem *pItem, panel_editor_scene->items())
                     {
                         QString param_name = pItem->toolTip();
                         param_name.truncate(param_name.lastIndexOf(QChar(',')));
                         if (param_name == pName && param_name != "panel")
                         {
                                QString px = QString::number(pItem->scenePos().x());
                                QString py = QString::number(pItem->scenePos().y());
                                portVec.append(px);
                                portVec.append(py);
                         }
                     }
                     portVec.append(pName);
                }

                canWriter->setMainModuleAttributes(portVec);
                canWriter->setControllersAttributes(portVec);

            }

            for (int i = 0; i <portVec.size();  i++)
            {
               //portVec.erase(portVec.begin()+i);
               portVec.remove(i);
            }
        }

        canWriter->writeHeader();
        canWriter->writePluginCPP();
        canWriter->writeModuleCanva();
        foreach(QGraphicsItem *item_, nodes_editor_scene->items())
        {
            if (item_->type() == QNEBlock::Type)
            {
                foreach(QGraphicsItem *port_t, item_->childItems())
                {
                     QNEPort *portT = (QNEPort*) port_t;
                     QString pNameT = portT->portName();
                     portVec.append(pNameT);
                }
            }
            extWriter->setFiles(portVec);
            for (int i = 0; i <portVec.size();  i++)
            {
               //portVec.erase(portVec.begin()+i);
               portVec.remove(i);
            }
        }
        extWriter->writeNewFile();
        stepWriter->fillBlockVector(nodes_editor_scene, plugDir);


        //QVector<QNEBlock*> block_series = stepWriter->writeOutputInstructions();
        //stepWriter->writeGenericInstructions(block_series);
        //stepWriter->writeStepContent();
    }
    else
    {
        CustomDialog d("non exaustive project", this);
        d.addLabel("HELP:");
        d.addLabel("It seems that your project contain 'Controllers block(s)' that aren't transfered to the 'panel editor' ");
        d.exec();
    }
}
void QNEMainWindow::displayOnPanel()
{
    ctrl = false;
    foreach(QGraphicsItem *item, nodes_editor_scene->items())
    {
        QNEBlock *BL = (QNEBlock*) item;
        bool isPanel = false;
        bool isController = false;
        bool isScreen = false;
        bool isNamed = false;
        int portIndex = 0;
        QString param_name = "";
        if (item->isSelected())
        {
            foreach(QGraphicsItem *port_, item->childItems())
            {
                if (port_->type() == QNEPort::Type)
                {
                    QNEPort *port = (QNEPort*) port_;
                    QString portName = port->portName();
                    if (portName == "smart_panel")
                    {
                         isPanel = true;
                    }
                    if (portName == "screen")
                    {
                         isScreen = true;
                    }
                    if (portName == "knob" || portName == "screen" ||portName == "led" || portName == "button" || portName == "switch" || portName == "rotary_switch" || portName == "module_input" || portName == "module_output"|| portName == "screw")
                    {
                         isController = true;
                    }
                }
                if (isScreen == true)
                {
                    QNEPort *port = (QNEPort*) port_;

                        QVector<QNEPort*> ports = BL->ports();
                        QString object_name = ports.at(5)->portName();
                        QString size = ports.at(9)->portName();
                        QString red = ports.at(6)->portName();
                        QString green = ports.at(7)->portName();
                        QString blue = ports.at(8)->portName();
                        QByteArray s = "current font OK = " + object_name.toUtf8();
                        qDebug(s);
                        textFont = object_name;
                        textSize = size;
                        textColorR = red;
                        textColorG = green;
                        textColorB = blue;
                    //textFont = "LEDCounter7.ttf";
                }
                if (isPanel == true && portIndex == 2)
                {
                    QNEPort *port = (QNEPort*) port_;
                    selectedBlock_name = port->portName();
                    addPanelImage(selectedBlock_name);
                }
                if (isController == true && portIndex == 2)
                {
                    QNEPort *port = (QNEPort*) port_;
                    if (port->portName() != "enter_a_param_name" && port->portName() != "0")
                    {
                        param_name = port->portName();
                        isNamed = true;
                    }
                }
                if (isController == true && portIndex == 4)
                {
                    if (isNamed == true)
                    {
                        QNEPort *port = (QNEPort*) port_;
                        selectedBlock_name = port->portName();
                        addControllerImage(selectedBlock_name, param_name, 0, 0);
                        bool controlWrited = false;
                        for( int i = 0; i < panelControllerParams.size(); i++)
                        {
                            QVector<QString> deletedItemBuffer = paneleditor->deletedParamList();

                            for (int k = 0; k <deletedItemBuffer.size(); k++)
                            {
                                if (panelControllerParams.at(i) == deletedItemBuffer.at(k))
                                {
                                    panelControllerParams.remove(i);
                                    paneleditor->eraseDeletedParamList(k);
                                    k = deletedItemBuffer.size();
                                }
                            }
                        }
                        for( int i = 0; i < panelControllerParams.size(); i++)
                        {
                            if (panelControllerParams.at(i) == param_name)
                            {
                                controlWrited = true ;
                            }

                        }
                        if (controlWrited == false)
                        {
                            panelControllerParams.append(param_name);
                            BL->IsOnPanel(true);
                        }
                    }
                    else
                    {
                        CustomDialog d("Help-", this);
                        d.addLabel("Please enter a parameter name:");
                        d.exec();
                    }
                }
                 portIndex++;
            }
        }
    }
}

void QNEMainWindow::displayHelp()
{
    CustomDialog d("Help", this);
    d.addLabel("SHORTCUTS:");
    d.addLabel("save the propject-----Ctrl+S");
    d.addLabel("load a propject----Ctrl+O");
    d.addLabel("dupplicate selected block(s)----Ctrl+D");
    d.addLabel("NODES EDITOR:");
    d.addLabel("select a block----left mouse clic on a block");
    d.addLabel("select many blocks----Ctrl + left mouse clic on blocks");
    d.addLabel("dupplicate selected block(s)----Ctrl+D");
    d.addLabel("Horizontal scrolling----Right/Left arrows");
    d.addLabel("block setting/properties----Return/Enter");
    d.addLabel("PANEL EDITOR:");
    d.addLabel("Add the panel----select the panl block and press ctrl+t");
    d.addLabel("Add a controller/input/led----select corresponding blocks and press ctrl+t");
    d.addLabel("select many controllers/input/led----Ctrl + left mouse clic");
    d.addLabel("set controller/input/led positions----drag (mouse left button) or use the spin boxes and press send postion");
    d.addLabel("Zoom----zoom in: H / zoom out: G");
    d.addLabel("set controller/input/led positions----drag (mouse left button) or use the spin boxes and press send postion");

    d.exec();
}
void QNEMainWindow::saveFile()
{
    bool exaustiveProject =false;
    int conditions = 0;
    foreach(QGraphicsItem *item, nodes_editor_scene->items())
    {
        if (item->type() == QNEBlock::Type)
        {
            foreach(QGraphicsItem *port_, item->childItems())
            {
                 QNEPort *port = (QNEPort*) port_;
                 QString pName = port->portName();
                 if (pName == "smart_panel" || pName == "plugin_setting")
                 {
                     conditions++;
                 }
                 if (conditions >= 2)
                 {
                     exaustiveProject =true;
                 }
            }
        }
    }
    if (exaustiveProject == true)
    {
        QString fname = QFileDialog::getSaveFileName();
        if (fname.isEmpty())
            return;
        savePanelFile(fname);
        if (fname.contains(".") == false)
        {
            fname.append(".nods");
        }
        QFile f(fname);
        f.open(QFile::WriteOnly);
        QDataStream ds(&f);
        nodesEditor->save(ds);
    }
    else
    {
        CustomDialog d("non exaustive project", this);
        d.addLabel("HELP:");
        d.addLabel("It seems that your project doesn't contain 'plugin-setting blocks' or 'smart_panel blocks'");
        d.exec();
    }
}

void QNEMainWindow::loadFile()
{
	QString fname = QFileDialog::getOpenFileName();
	if (fname.isEmpty())
		return;
	QFile f(fname);
	f.open(QFile::ReadOnly);
	QDataStream ds(&f);
	nodesEditor->load(ds);
    fname.truncate(fname.lastIndexOf(QChar('.')));
    loadPanelFile(fname);
    loadControllerParamVec();
    incrementBlockID();
}
void QNEMainWindow::incrementBlockID()
{
    int biggerID = 0;
    foreach(QGraphicsItem *item, nodes_editor_scene->items())
    {
        if (item->type() == QNEBlock::Type)
        {
            QNEBlock *BL = (QNEBlock*) item;
            if (BL->uniqueID() > biggerID)
            {
                biggerID = BL->uniqueID();
            }
        }
    }
    blockID  = biggerID + 1;
}
void QNEMainWindow::savePanelFile(QString name_)
{
    bool exaustiveProject =false;
    int conditions = 0;
    //QDir fileDir(QDir::currentPath());
    //fileDir.cd("Project");
    /*QString fileDirPath = fileDir.absoluteFilePath("Output");
    QFile file(fileDirPath);*/
    foreach(QGraphicsItem *item, nodes_editor_scene->items())
    {
        if (item->type() == QNEBlock::Type)
        {
            foreach(QGraphicsItem *port_, item->childItems())
            {
                 QNEPort *port = (QNEPort*) port_;
                 QString pName = port->portName();
                 if (pName == "smart_panel" || pName == "plugin_setting")
                 {
                     conditions++;
                 }
                 if (conditions >= 2)
                 {
                     exaustiveProject =true;
                 }
            }
        }
    }
    if (exaustiveProject == true)
    {
        if (name_.contains(".") == false)
        {
            name_.append(".pan");
        }
        else
        {
            name_.truncate(name_.lastIndexOf(QChar('.')));
            name_.append(".pan");
        }
        if (QFile(name_).exists())
        {
             QFile f_r(name_);
             f_r.remove();
        }
        QFile f(name_);
        f.resize(0);
        f.open(QFile::WriteOnly);
        QDataStream ds(&f);
        int count = 0;

        foreach(QGraphicsItem *item, panel_editor_scene->items())
        {
            if (item->toolTip() == "panel")
            {
                ds<<panelPngName;
                ds<<textFont;
                ds<<textColorR;
                ds<<textColorG;
                ds<<textColorB;
                ds<<textSize;
            }
            else if (item->toolTip() != "button" && item->toolTip() != "SpinBox_1" && item->toolTip() != "SpinBox_2"  && item->toolTip() != "myText")
            {
                count ++;
            }
        }
        ds<<count;
        foreach(QGraphicsItem *item, panel_editor_scene->items()  )
        {
            if (item->toolTip() != "panel" && item->toolTip() != "myText" )
            {

                QString param = item->toolTip();
                QString tt = item->toolTip();
                param.truncate(param.lastIndexOf(QChar(',')));
                int s = param.size()+1;
                QString pngName = tt.mid(s);
                ds<<item->scenePos().x();
                ds<<item->scenePos().y();
                ds<<param;
                ds<<pngName;
            }
        }
        f.close();
    }
    else
    {
        CustomDialog d("non exaustive project", this);
        d.addLabel("HELP:");
        d.addLabel("It seems that your project doesn't contain 'plugin-setting blocks' or 'smart_panel blocks'");
        d.exec();
    }

}
void QNEMainWindow::loadPanelFile(QString name_)
{
    foreach(QGraphicsItem *item, panel_editor_scene->items()  )
    {
       if (item->toolTip() != "SpinBox_1" && item->toolTip() != "SpinBox_2" && item->toolTip() != "button")
       {
           delete item;
       }
    }
    name_.append(".pan");
    QFile f(name_);
    f.open(QFile::ReadOnly);
    QDataStream ds(&f);
    QString panelPng;
    int count;
    ds>>panelPng;
    QString tf;
    QString cr;
    QString cg;
    QString cb;
    QString sizeT;
    ds>>tf;
    ds>>cr;
    ds>>cg;
    ds>>cb;
    ds>>sizeT;
    textFont = tf;
    textColorR = cr;
    textColorG = cg;
    textColorB = cb;
    textSize = sizeT;
    addPanelImage(panelPng);
    ds>>count;
    for (int i = 0; i < count ; i++)
    {
        float posx;
        float posy;
        QString param;
        QString pngName;
        ds>>posx;
        ds>>posy;
        ds>>param;
        ds>>pngName;
        addControllerImage(pngName, param, posx, posy);
    }
}

void QNEMainWindow::setNewItemCalling()
{
    newItemCalling = true;
}

void QNEMainWindow::addBlock(QAction* a)
{
    if (actCheck != a || newItemCalling == true)
    {
        newItemCalling = false;
        actCheck = a;
        // Add a block corresponding to the item selected in one of the submenus by using the name and type vectors (based on the model Files stored in BlockModels)load on program starting (see constructor)
        QString blockName = a->statusTip();
        blockmodel *readBlock = new blockmodel();
        int bmIndex[4] = {1000, 1000, 1000, 1000};
        string name_;
        string  currentDefVal_s;
        float  currentDefVal_f;
        int  currentDefVal_i;
        int type_ = 0;
        float minValue_ = 0 ;
        float maxValue_ = 0;
        string value_s_ = "0";
        int numericParam = 0;
        for( int i = 0 ; i<loadedBlockNameVec.size(); i++)
        {
            if (blockName == loadedBlockNameVec.at(i))
            {
                readBlock = blockModelVec.at(i);
                QStringList min_max = blockPropertiesLimitsVec.at(i);
                QVector<QString> namesVec = readBlock->getNameVector();
                QVector<QString> defValVec = readBlock->getDefaultValueVector();
                QVector<int> typesVec = readBlock->getTypeVector();
                //blockID ++;
                QNEBlock *b = new QNEBlock(blockID, 0);
                blockID ++;

                    nodes_editor_scene->addItem(b);
                    for( int j = 0; j < namesVec.size(); j++)
                    {
                        QString currentDefVal;
                        if (j >= 2)
                        {
                            currentDefVal = defValVec.at(j-2);
                        }
                        else
                        {
                            currentDefVal = "NAP";
                        }
                        int isMainType = 1;
                        int currentType = typesVec.at(j);
                        QString currentName = namesVec.at(j);
                        name_= currentName.toStdString();
                        currentDefVal_s =currentDefVal.toStdString();
                        currentDefVal_f = currentDefVal.toFloat();
                        currentDefVal_i = currentDefVal.toInt();
                        type_ = currentType-3;
                        if ((currentType == 3 || currentType == 4 || currentType == 5) && j>1)
                        {
                            isMainType = 0;
                            if (currentType == 4 || currentType == 5)
                            {
                                minValue_ =  min_max[numericParam].toFloat();
                                maxValue_ =  min_max[numericParam+1].toFloat();
                            }
                            else
                            {
                                minValue_ =  0;
                                maxValue_ =  0;
                            }
                            b->addProperty(currentName, type_, minValue_, maxValue_, currentDefVal_s, currentDefVal_f, currentDefVal_i);
                        }
                        switch(currentType)
                        {
                            case 0:
                            b->addInputPort(currentName);
                            break;
                            case 1:
                            b->addOutputPort(currentName);
                            break;
                            case 2:
                            if (i<bmIndex[0])
                            {
                                bmIndex[0] = i;
                            }
                            b->addPort(currentName, false, QNEPort::NamePort, 1, isMainType, 1);
                            break;
                            case 3:
                            if (i<bmIndex[1])
                            {
                                bmIndex[1] = i;
                            }
                            b->addPort(currentName, false, QNEPort::TypePort, 2, isMainType, 2);
                            break;
                            case 4:
                            if (i<bmIndex[2])
                            {
                                bmIndex[2] = i;
                            }
                            b->addPort(currentName, false, QNEPort::TypePort, 3, isMainType, 3);
                            numericParam = numericParam + 2;
                            break;
                            case 5:
                            if (i<bmIndex[3])
                            {
                                bmIndex[3] = i;
                            }
                            b->addPort(currentName, false, QNEPort::TypePort, 4, isMainType, 4);
                            numericParam = numericParam + 2;
                            break;
                        }
                    }

              }
         }

    }

}
void QNEMainWindow::addPanelImage(QString fileName)
{
    panelPngName = fileName;
    foreach(QGraphicsItem *item, panel_editor_scene->items()  )
    {
       if (item->toolTip() == "panel")
       {
           delete item;
       }
    }
    QDir dir(QDir::currentPath());
    dir.cd("Images");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    QString fileDirPath = dir.absoluteFilePath(fileName);
    Panel *p = new Panel(0);
    QGraphicsSvgItem* l = p->panItem(fileDirPath);
    float panelWidth = p->getWidth();
    int pw = int(panelWidth);
    foreach(QGraphicsItem *item, nodes_editor_scene->items())
    {
        QNEBlock *BL = (QNEBlock*) item;

        if (item->type() == QNEBlock::Type)
        {
            if (BL->blockName() == "smart_panel")
            {

                BL->setParamI(pw, 1);
                int pNum = 0;
                foreach(QGraphicsItem *port_, item->childItems())
                {
                     QNEPort *port = (QNEPort*) port_;
                     QString pName = port->portName();
                     if (pNum == 3)
                     {
                         QString pwS = QString::number(pw);
                         port->setName(pwS, true);
                     }
                     pNum++;
                }
            }
        }

    }
    l->setZValue(-0.01);
    panel_editor_scene->addItem(l);
}

void QNEMainWindow::addControllerImage(QString fileName, QString name_, float posx, float posy)
{
    QString fiName = "";
    foreach(QGraphicsItem *item, panel_editor_scene->items()  )
    {
       QString ttp = item->toolTip();
       ttp.truncate(ttp.lastIndexOf(QChar(',')));
       if (ttp == name_)
       {
           delete item;
       }
    }
    QDir dir(QDir::currentPath());
    dir.cd("Images");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();

    QString fileDirPath = dir.absoluteFilePath(fileName);
    Controller *p = new Controller(0);
    QGraphicsSvgItem* l = p->controllerItem(fileDirPath, name_, fileName, posx, posy);
    panel_editor_scene->addItem(l);
    if (fileName.contains("screen"))
    {
        QGraphicsTextItem* t = new QGraphicsTextItem("text example ", l);
        t->setPos(0, -10);
        //t->setFlag(QGraphicsItem::ItemIsMovable);
        //t->setFlag(QGraphicsItem::ItemIsSelectable);
        //QFont font("Fonts/" + textFont );


        QString f = "Fonts/" + textFont;
        int id = QFontDatabase::addApplicationFont(f);
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont monospace(family);
        int r = textColorR.toInt();
        int g = textColorG.toInt();
        int b = textColorB.toInt();
        QColor *col = new QColor(r, g, b, 255);
        t->setDefaultTextColor(*col);
        t->setToolTip("myText");
        int s = textSize.toInt();
        monospace.setPointSize(s/1.25);
        t->setFont(monospace);
        panel_editor_scene->addItem(t);
    }

    //l->toolTip()


}

void QNEMainWindow::loadBlocksFiles()
{
    // use the block files in the "BlockModels" folder to generate blockmodel objects and stored these in a vector called blockModelVec.
    FileReader fr;
    QFileInfoList blockFileList = fr.detectFiles("BlockModels");
    for (int i = 0; i < blockFileList.size(); ++i)
    {
        QFileInfo blockFile = blockFileList.at(i);
        QString blockFileName = blockFile.fileName();
        QStringList paramList = fr.readBlockLine(blockFileName, "BlockModels");
        QStringList paramPropList = fr.readPropertiesLine(blockFileName, "BlockModels");
        QStringList defaultValList = fr.readPropertiesLine(blockFileName, "BlockModels");
        QStringList defaultnumParamList;
        defaultnumParamList.append("no");
        defaultnumParamList.append("num");
        if (paramPropList.isEmpty())
        {
            paramPropList = defaultnumParamList;
        }
        blockmodel *qv = new blockmodel;
        qv->setNameVector(paramList);
        qv->setTypeVector(paramList);
        qv->setDefaultValuesVector(paramList);
        blockModelVec.append(qv);
        blockPropertiesLimitsVec.append(paramPropList);
        //defaultValuesVec.append();
        //QStringList paramNameList;
        QVector<int> vecT = qv->getTypeVector();
        QVector<QString> vecN = qv->getNameVector();
        QVector<QString> vecD = qv->getDefaultValueVector();
        QStringList currentNameList;
        QStringList currentDefValList;

        for (int j = 0; j <vecT.size(); j ++)
        {
            if ((vecT.at(j) == 3 || vecT.at(j) == 4 || vecT.at(j) == 5) && i != 3)
            {
                    QString currentName = vecN.at(j);
                    currentNameList.append(currentName);
            }
            else
            {
                    currentNameList.append("0");
            }
            if (j >= 2)
            {
                QString currentDefVal = vecD.at(j - 2);
                currentDefValList.append(currentDefVal);
            }
        }
        blockPropertiesNamesVec.append(currentNameList);
        defaultValuesVec.append(currentDefValList);
    }
}

void QNEMainWindow::loadControllerParamVec()
{
    panelControllerParams.clear();
    foreach(QGraphicsItem *pItem, panel_editor_scene->items())
    {
        QString param_name = pItem->toolTip();
        param_name.truncate(param_name.lastIndexOf(QChar(',')));
        panelControllerParams.append(param_name);
    }
}

void QNEMainWindow::keyPressEvent(QKeyEvent *event)
{
    /*if (event->key() != Qt::Key_P && ctrl == false)
    {
        foreach(QGraphicsItem *item, nodes_editor_scene->items())
        {
            QNEBlock *BL = (QNEBlock*) item;
            if (item->type() == QNEBlock::Type)
            {
                if (item->isSelected() && nodesEditor->isPropActive() == true && propVisible == true)
                {
                    BL->scanProperties();
                }
            }
        }
    }*/
    // erase a block
    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete)
    {
        deleteBlock();
    }
    if (event->key() == Qt::Key_H && ctrl == false)
    {
        zoomIn();
    }
    if (event->key() == Qt::Key_G && ctrl == false)
    {
        zoomOut();
    }
    if (event->key() == Qt::Key_P && ctrl == false)
    {
        setPropVisible();
    }
    if (event->key() == Qt::Key_Control)
    {
        ctrl = true;
        nodesEditor->controlPressed(ctrl);
        //nodesEditor->propActive(false);
    }
    if (event->key() == Qt::Key_T && ctrl == true)
    {
       displayOnPanel();
    }
    if (event->key() == Qt::Key_Return && ctrl == false)
    {
        /*foreach(QGraphicsItem *item, nodes_editor_scene->items())
        {
            QNEBlock *BL = (QNEBlock*) item;
            if (item->type() == QNEBlock::Type)
            {
                if (item->isSelected())
                {
                    BL->openDialog();
                }
            }
        }*/
    }
    if (event->key() == Qt::Key_Q && ctrl == true)
    {
       exportCode();
    }

    if (event->key() == Qt::Key_D && ctrl == true)
    {
        duplicate();
    }
}

void QNEMainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
    {
        ctrl = false;
        nodesEditor->controlPressed(ctrl);
        /*
        int b = 0;
        foreach(QGraphicsItem *item, nodes_editor_scene->items())
        {
            if (item->type() == QNEBlock::Type)
            {
                if (item->isSelected())
                {
                   b++;
                }
            }
        }
        if (b <= 1)
        {
             nodesEditor->propActive(true);
        }*/
    }
}
