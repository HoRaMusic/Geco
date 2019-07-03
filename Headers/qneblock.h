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

#ifndef QNEBLOCK_H
#define QNEBLOCK_H

#include <QGraphicsPathItem>
#include <QSpinBox>
#include <QVector>
#include <QGraphicsProxyWidget>
#include "Headers/property.h"
#include "Headers/customdialog.h"

class QNEPort;

class QNEBlock : public QGraphicsPathItem
{
public:
	enum { Type = QGraphicsItem::UserType + 3 };

    QNEBlock(int uniqueBlockID, QGraphicsItem *parent = 0);
    ~QNEBlock();
    QNEPort* addPort(const QString &name, bool isOutput, int flags, int ptr, int secondPtr, int thirdPtr);
	void addInputPort(const QString &name);
	void addOutputPort(const QString &name);
	void addInputPorts(const QStringList &names);
	void addOutputPorts(const QStringList &names);
	void save(QDataStream&);
    void load(QDataStream &ds, QMap<quint64, QNEPort*> &portMap);
    void getPos(QDataStream&, QMap<quint64, QNEPort*> &portMap);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void addPropertyFromClone(QString name, int type, float minValue, float maxValue, string value_s, float value_f, int value_i);
    QNEPort* addPortOnLoading (const QString &name, bool isOutput, int flags, int ptr, int secondPtr, int thirdPtr);
    void addProperty(QString name, int type, float minValue, float maxValue, string value_s, float value_f, int value_i);
    void setPortState(QVector<bool> toBeSet);
    int uniqueID();
    void setParamI(int value, int index);
    QNEBlock* clone(int IBlockID);
	QVector<QNEPort*> ports();
	int type() const { return Type; }
    void setWidth(int width_);
    int getWidth();
    void openDialog();
    void openPermDialog();
    void IsOnPanel(bool is_on_panel);
    QString blockName();
    QString blockType();
    int numberOfInputs();
    int numberOfOutputs();
    void scanProperties();
    void resetPanelPosition();
    void setPortsColor();

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    /*void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);*/
private:
	int horzMargin;
	int vertMargin;
	int width;
	int height;
    bool isFLoat;
    bool isInt;
    int originalWidth;
    QVector<Property*> propVector;
    QVector<string*> param_s;
    QVector<float*> param_f;
    QVector<int*> param_i;
    QVector<string*> defaultParam_s;
    QVector<float*> defaultParam_f;
    QVector<int*> defaultParam_i;
    bool o_width_set;
    QColor qc;
    bool onPanel;
    QString block_name;
    QString block_type;
    int number_of_inputs;
    int number_of_outputs;
    int numberOfCreatedPorts;
    int blockID;
    QVector<QDoubleSpinBox*> widVectorF;
    QVector<QSpinBox*> widVectorI;
    QVector<QLineEdit*> widVectorS;
    QVector<bool> hasToBeSet;
    QVector<bool> visible;
    QGraphicsScene *panel_editor_scene;
    //QGroupBox *groupBox;
    //QVector<QDoubleSpinBox> dsbV;
    //QVector<QSpinBox> sbV;
    //QVector<QLineEdit> leV;



    //QVector<QSpinBox*> widVectorI;
};

#endif // QNEBLOCK_H
