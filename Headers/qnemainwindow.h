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

#ifndef QNEMAINWINDOW_H
#define QNEMAINWINDOW_H
#include "canvawrite.h"
#include <QMainWindow>
#include <QtWidgets>
#include <QtGui>
#include <QKeyEvent>
#include <QMessageBox>
#include <QImage>
#include "Headers/qneblock.h"
#include "Headers/blockfilereader.h"
#include "Headers/blockmodel.h"
#include "Headers/customdialog.h"
#include "Headers/externalwriter.h"
#include "Headers/stepwriter.h"
#define LIGHT 0
#define FULL 1

#define VERSION 1 // uncomment for full version
//#define VERSION 0 // uncomment for free version


class QNodesEditor;
class Paneditor;

class QNEMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit QNEMainWindow(QWidget *parent = 0);
	~QNEMainWindow();

private slots:
    // slot qui seront déclenché seul quit n'est pas dendans car utilise qApp
    // ces slots sont utilisé pour ouvrir, sauver, ajouter un block lorsqu'on envoit
    // un signal en déclenchant un des bouttons dans le menu.
	void saveFile();
	void loadFile();
    void loadBlocksFiles();
    void duplicate();
    void zoomIn();
    void zoomOut();
    void exportCode();
    void displayOnPanel();
    void deleteBlock();
    void deleteController();
    void addBlock(QAction* a);
    void displayHelp();
    void addPanelImage(QString fileName);
    void addControllerImage(QString filePath, QString name_, float posx, float posy);
    void setNewItemCalling();
    void savePanelFile(QString name_);
    void loadPanelFile(QString name_);
    void loadControllerParamVec();
    void incrementBlockID();
    void setPropVisible();
protected :
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private:
    // on ajoute un attributs,n un pointeur vers QnodesEditor
    // avec nodesEditor = new QNodesEditor(this); on le fera directemtn hérité de mainwindow
	QNodesEditor *nodesEditor;
    Paneditor *paneleditor;
    QMenu *fileMenu;
    QMenu *addBlockMenu;
    QMenu *editMenu;
    QGraphicsView *nodes_editor_view;
    QGraphicsView *panel_editor_view;
    QGraphicsScene *nodes_editor_scene;
    QGraphicsScene *panel_editor_scene;
    string vName;
    int vValue_i;
    float vValue_f;
    bool ctrl;
    QVector<blockmodel*> blockModelVec;
    QVector<QStringList> blockPropertiesLimitsVec;
    QVector<QStringList> blockPropertiesNamesVec;
    QVector<QMenu*> QmenuVec;
    QVector<QAction*> QActionVec;
    QVector<QString*> loadedBlockNameVec;
    QVector<QStringList> defaultValuesVec;
    QVector<string*> vNameVec;
    QVector<float*> vNumVec;
    QVector<int*> vNumVecI;
    QAction* actCheck;
    bool newItemCalling;
    QString selectedBlock_name;
    QString panelPngName;
    QString textFont;
    QString textSize;
    QString textColorR;
    QString textColorG;
    QString textColorB;
    Canvawrite *canWriter ;
    ExternalWriter *extWriter ;
    StepWriter * stepWriter ;
    QVector<QString> portVec;
    QVector<QString> panelControllerParams;
    QAction* propOutAct;
    bool propVisible;
    int blockID;
};

#endif // QNEMAINWINDOW_H
