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

#include "Headers/qneblock.h"
#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>
#include "Headers/qneport.h"
#include "Headers/property.h"
#include "Headers/customdialog.h"
#include "Headers/customlineedit.h"
#include "Headers/customspinbox.h"
#include "Headers/customspinboxf.h"
#include <QColor>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsView>
#include <QScrollBar>
#include <QDockWidget>
#include <QDesktopWidget>
#include "Headers/customdialog.h"

QNEBlock::QNEBlock(int uniqueBlockID, QGraphicsItem *parent) : QGraphicsPathItem(parent)
{
    number_of_outputs = 0;
    onPanel = false;
    isFLoat = false;
    isInt = false;
    QPainterPath p;
    /*QGraphicsView *view = new QGraphicsView(this->scene());
    QPoint globalCursorPos = QCursor::pos();
    int mouseScreen = qApp->desktop()->screenNumber(globalCursorPos);
    QRect mouseScreenGeometry = qApp->desktop()->screen(mouseScreen)->geometry();
    QPoint localCursorPos = globalCursorPos - mouseScreenGeometry.topLeft();
    setPos(localCursorPos.x()-50, localCursorPos.y()-15);*/
    p.addRoundedRect(-50, -15, 100, 30, 5, 5);
    p.addRoundedRect(-50, -15, 15, 30, 5, 5);
	setPath(p);
    setPen(QPen(Qt::black));
    setBrush(Qt::white);
    // utilisation des flag pour pouvoir selectionner bouger les blocks
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	horzMargin = 20;
	vertMargin = 5;
	width = horzMargin;
	height = vertMargin;
    numberOfCreatedPorts = 0;
    blockID = uniqueBlockID;
}

QNEBlock::~QNEBlock()
{
}

void QNEBlock::addProperty(QString name, int type, float minValue, float maxValue, string value_s, float value_f, int value_i)
{
    if (name.contains("*", Qt::CaseSensitive))
    {
        hasToBeSet.append(true);
    }
    else
    {
        hasToBeSet.append(false);
    }
    if (name.contains("/", Qt::CaseSensitive))
    {
        visible.append(false);
    }
    else
    {
        visible.append(true);
    }
   Property *prop = new Property(name, type, minValue, maxValue);
   propVector.append(prop);
   float *vf = new float(value_f);
   param_f.append (vf);
   defaultParam_f.append (vf);
   int *vi = new int(value_i);
   param_i.append (vi);
   defaultParam_i.append (vi);
   string *vs = new string(value_s);
   if (block_name != "plugin_setting" && block_name != "smart_panel" && block_name != "add_in_step" && block_name != "add_in_widget" && block_name != "add_in_struct" && block_name != "add_in_module_CPP" && block_name != "add_include"
          && block_name != "(x*y)" && block_name != "(x/y)" && block_name != "(x-y)" && block_name != "(x+y)" && block_name != "(x%y)" && block_name != "x<y" && block_name != "x>y" && block_name != "x<=y" && block_name != "x>=y" && block_name != "x=y" && block_name != "x||y" && block_name != "x and y")
   {
       if (propVector.size() == 1)
       {
           if (block_name == "knob" || block_name == "switch" ||  block_name == "rotary_switch" ||  block_name == "button")
           {
                   QString substring = block_name.left(3);
                   QString bi = QString::number(blockID);
                   QString varName = substring + "_" + bi + "_PARAM";
                   string varNameC = varName.toStdString();
                   string *varName_s = new string(varNameC);
                   param_s.append(varName_s);
                   defaultParam_s.append (vs);
           }
           else if ( block_name == "module_input")
           {
                   QString substring = block_name.left(3);
                   QString bi = QString::number(blockID);
                   QString varName = substring + "_" + bi + "_INPUT";
                   string varNameC = varName.toStdString();
                   string *varName_s = new string(varNameC);
                   param_s.append(varName_s);
                   defaultParam_s.append (vs);
           }
           else if ( block_name == "module_output")
           {
                   QString substring = block_name.left(3);
                   QString bi = QString::number(blockID);
                   QString varName = substring  + "_" +  bi + "_OUTPUT";
                   string varNameC = varName.toStdString();
                   string *varName_s = new string(varNameC);
                   param_s.append(varName_s);
                   defaultParam_s.append (vs);
           }
           else if ( block_name == "led")
           {
                   QString substring = block_name.left(3);
                   QString bi = QString::number(blockID);
                   QString varName = substring  + "_" +  bi + "_LIGHT";
                   string varNameC = varName.toStdString();
                   string *varName_s = new string(varNameC);
                   param_s.append(varName_s);
                   defaultParam_s.append (vs);
           }
           else
           {
               QString substring = block_name.left(3);
               QString bi = QString::number(blockID);
               QString varName = substring + bi;
               string varNameC = varName.toStdString();
               string *varName_s = new string(varNameC);
               param_s.append(varName_s);
               defaultParam_s.append (vs);
           }
       }
       else
       {
           param_s.append (vs);
           defaultParam_s.append (vs);
       }
   }
    else
   {
       param_s.append (vs);
       defaultParam_s.append (vs);
   }
}

void QNEBlock::addPropertyFromClone(QString name, int type, float minValue, float maxValue, string value_s, float value_f, int value_i)
{
    /*if (name.contains("*", Qt::CaseSensitive))
    {
        hasToBeSet.append(true);
    }
    else
    {
        hasToBeSet.append(false);
    }*/
    if (name.contains("/", Qt::CaseSensitive))
    {
        visible.append(false);
    }
    else
    {
        visible.append(true);
    }
   Property *prop = new Property(name, type, minValue, maxValue);
   propVector.append(prop);
   float *vf = new float(value_f);
   param_f.append (vf);
   int *vi = new int(value_i);
   param_i.append (vi);
   string *vs = new string(value_s);
   if (block_name != "plugin_setting" && block_name != "smart_panel" && block_name != "add_in_step" && block_name != "add_in_widget" && block_name != "add_in_struct" && block_name != "add_in_module_CPP" && block_name != "add_include"
          && block_name != "(x*y)" && block_name != "(x/y)" && block_name != "(x-y)" && block_name != "(x+y)" && block_name != "(x%y)" && block_name != "x<y" && block_name != "x>y" && block_name != "x<=y" && block_name != "x>=y" && block_name != "x=y" && block_name != "x||y" && block_name != "x and y")
   {
       if (propVector.size() == 1)
       {
           if (block_name == "knob" || block_name == "switch" ||  block_name == "rotary_switch" ||  block_name == "button")
           {
                   QString substring = block_name.left(3);
                   QString bi = QString::number(blockID);
                   QString varName = substring + "_" + bi + "_PARAM";
                   string varNameC = varName.toStdString();
                   string *varName_s = new string(varNameC);
                   param_s.append(vs);
           }
           else if ( block_name == "module_input")
           {
                   QString substring = block_name.left(3);
                   QString bi = QString::number(blockID);
                   QString varName = substring + "_" + bi + "_INPUT";
                   string varNameC = varName.toStdString();
                   string *varName_s = new string(varNameC);
                   param_s.append(vs);
           }
           else if ( block_name == "module_output")
           {
                   QString substring = block_name.left(3);
                   QString bi = QString::number(blockID);
                   QString varName = substring  + "_" +  bi + "_OUTPUT";
                   string varNameC = varName.toStdString();
                   string *varName_s = new string(varNameC);
                   param_s.append(vs);
           }
           else if ( block_name == "led")
           {
                   QString substring = block_name.left(3);
                   QString bi = QString::number(blockID);
                   QString varName = substring  + "_" +  bi + "_LIGHT";
                   string varNameC = varName.toStdString();
                   string *varName_s = new string(varNameC);
                   param_s.append(vs);
           }
           else
           {
               QString substring = block_name.left(3);
               QString bi = QString::number(blockID);
               QString varName = substring + bi;
               string varNameC = varName.toStdString();
               string *varName_s = new string(varNameC);
               param_s.append(vs);
           }
       }
       else
       {
           param_s.append (vs);
       }
   }
    else
   {
       param_s.append (vs);
   }
}
void QNEBlock::setPortsColor()
{
    int paramIndex = 0;
    foreach(QGraphicsItem *port_, childItems()) {

        if (port_->type() != QNEPort::Type)
            continue;

        QNEPort *port = (QNEPort*) port_;
        if (port->isMainType() == 0)
        {
            if (port->thirdPtr() == 2)
            {

                if (hasToBeSet.at(paramIndex) == true)
                {
                    port->setRed();
                }

                paramIndex++;
            }
            if (port->thirdPtr() == 3)
            {

                if (hasToBeSet.at(paramIndex) == true)
                {
                    port->setRed();
                }
                paramIndex++;
            }
            if (port->thirdPtr() == 4)
            {

                if (hasToBeSet.at(paramIndex) == true)
                {
                    port->setRed();
                }
                paramIndex++;
            }
        }
    }
}
void QNEBlock::scanProperties()
{
    for(int i = 0; i <propVector.size() ; i++)
    {
        QString name = propVector.at(i)->getName();
        if (propVector.at(i)->getType() == 1)
        {
            for(int j = 0; j <widVectorF.size(); j++)
            {
                if (widVectorF.at(j)->toolTip() == name)
                {
                    if (widVectorF.at(j)->value() != *param_f[i])
                    {
                        float *wv = new float(widVectorF.at(j)->value());
                        param_f[i] = wv;
                    }
                    float *hh = param_f[i];
                    float *bb = defaultParam_f[i];
                    float jj =  float(*hh) ;
                    float ll =  float(*bb) ;

                    if (jj == ll && hasToBeSet.at(i) == true)
                    {
                        widVectorF.at(j)->setStyleSheet(
                        "QWidget:disabled{color: #64B2D1;}"
                        "QWidget{color: red;}");
                    }
                    else
                    {
                       widVectorF.at(j)->setStyleSheet("QTDark.stylesheet");
                    }
                }
            }
        }
        else if (propVector.at(i)->getType() == 2 )
        {
            for(int j = 0; j <widVectorI.size(); j++)
            {
                if (widVectorI.at(j)->toolTip() == name)
                {
                    if (widVectorI.at(j)->value() != *param_i[i])
                    {
                        int *wv = new int(widVectorI.at(j)->value());
                        param_i[i] = wv;
                    }
                    int *hh = param_i[i];
                    int *bb = defaultParam_i[i];
                    int jj =  int(*hh) ;
                    int ll =  int(*bb) ;
                    if (ll == jj && hasToBeSet.at(i) == true)
                    {
                        widVectorI.at(j)->setStyleSheet(
                        "QWidget:disabled{color: #64B2D1;}"
                        "QWidget{color: red;}");
                    }
                    else
                    {
                       widVectorI.at(j)->setStyleSheet("QTDark.stylesheet");
                    }
                }
            }
        }
        else if (propVector.at(i)->getType() == 0)
        {
          // QLayoutItem *gg = groupBox->layout()->itemAt(i);
           for(int j = 0; j <widVectorS.size(); j++)
            {

               QString hh = QString::number(j);
               QLineEdit *qs = widVectorS.at(j);
               QString gg = qs->objectName();
               QByteArray s = gg.toUtf8();
               //qDebug(s);
               //if (qs->isModified() == false)
               if (i == 2 )
               {
                   if (block_name == "knob" || block_name == "switch" ||  block_name == "rotary_switch" ||  block_name == "button" || block_name == "module_input" || block_name == "module_output" || block_name == "led")
                   {
                       if (onPanel)
                       {
                           if (qs->text().toStdString() != *param_s[i])
                           {
                               qs->setStyleSheet(
                               "QWidget:disabled{color: #64B2D1;}"
                               "QWidget{color: grey;}");
                           }
                       }
                       else
                       {
                           string *image_Name = param_s[i];
                           QString imName =  QString::fromStdString(*image_Name);
                           QString substring;
                           if ( block_name == "rotary_switch" )
                           {
                               substring = imName.left(imName.size() - 4) + "snap";
                           }
                           else
                           {
                               substring = imName.left(imName.size() - 4);
                           }

                           string *imageName = new string(substring.toStdString());
                           param_s[i-1] = imageName;
                       }
                   }
                   if (qs->toolTip() == name)
                   {

                       if (onPanel)
                       {
                           if (qs->text().toStdString() != *param_s[i])
                           {
                               qs->setStyleSheet(
                               "QWidget:disabled{color: #64B2D1;}"
                               "QWidget{color: grey;}");
                           }
                       }
                       else
                       {
                           if (qs->text().toStdString() != *param_s[i])
                           {
                               string *wv = new string(qs->text().toStdString());
                               param_s[i] = wv;
                           }
                           string *hh = param_s[i];
                           string *bb = defaultParam_s[i];
                           QString jj =  QString::fromStdString(*hh) ;
                           QString ll =  QString::fromStdString(*bb) ;

                           if (jj == ll && hasToBeSet.at(i) == true)
                           {
                               qs->setStyleSheet(
                               "QWidget:disabled{color: #64B2D1;}"
                               "QWidget{color: red;}");
                           }
                           else
                           {
                              qs->setStyleSheet("QTDark.stylesheet");
                           }
                       }
                   }
               }
               else if (qs->toolTip() == name)
               {
                   if (i == 0)
                   {
                       if (block_name == "knob" || block_name == "switch" ||  block_name == "rotary_switch" ||  block_name == "button" || block_name == "module_input" || block_name == "module_output" || block_name == "led")
                       {
                           if (onPanel)
                           {
                               if (qs->text().toStdString() != *param_s[i])
                               {
                                   qs->setStyleSheet(
                                   "QWidget:disabled{color: #64B2D1;}"
                                   "QWidget{color: grey;}");
                               }
                           }
                           else
                           {
                               if (qs->text().toStdString() != *param_s[i])
                               {
                                   string *wv = new string(qs->text().toStdString());
                                   param_s[i] = wv;
                               }
                           }
                       }
                       else
                       {
                           if (qs->text().toStdString() != *param_s[i])
                           {
                               string *wv = new string(qs->text().toStdString());
                               param_s[i] = wv;
                           }
                           string *hh = param_s[i];
                           string *bb = defaultParam_s[i];
                           QString jj =  QString::fromStdString(*hh) ;
                           QString ll =  QString::fromStdString(*bb) ;

                           if (jj == ll && hasToBeSet.at(i) == true)
                           {
                               qs->setStyleSheet(
                               "QWidget:disabled{color: #64B2D1;}"
                               "QWidget{color: red;}");
                           }
                           else
                           {
                              qs->setStyleSheet("QTDark.stylesheet");
                           }
                       }
                   }
                   else
                   {
                       if (qs->text().toStdString() != *param_s[i])
                       {
                           string *wv = new string(qs->text().toStdString());
                           param_s[i] = wv;
                       }
                       string *hh = param_s[i];
                       string *bb = defaultParam_s[i];
                       QString jj =  QString::fromStdString(*hh) ;
                       QString ll =  QString::fromStdString(*bb) ;

                       if (jj == ll && hasToBeSet.at(i) == true)
                       {
                           qs->setStyleSheet(
                           "QWidget:disabled{color: #64B2D1;}"
                           "QWidget{color: red;}");
                       }
                       else
                       {
                          qs->setStyleSheet("QTDark.stylesheet");
                       }
                   }
                }
            }
        }
    }

    QFontMetrics fm(scene()->font());
    int paramIndex = 0;
    int w = 0;
    foreach(QGraphicsItem *port_, childItems()) {

        if (port_->type() != QNEPort::Type)
            continue;

        QNEPort *port = (QNEPort*) port_;
        if (port->isMainType() == 0)
        {
            if (port->thirdPtr() == 2)
            {
                string *paramValue_s = param_s[paramIndex]; 

                QString ps = QString::fromStdString(*paramValue_s);
                port->setName(ps, visible.at(paramIndex));
                int textWidth = fm.width(ps);
                if (w<textWidth)
                {
                   w = fm.width(ps);
                }
                string *hh = param_s[paramIndex];
                string *bb = defaultParam_s[paramIndex];
                QString jj =  QString::fromStdString(*hh) ;
                QString ll =  QString::fromStdString(*bb) ;
                if (ll == jj && hasToBeSet.at(paramIndex) == true)
                {
                    port->setRed();
                }

                paramIndex++;
            }
            if (port->thirdPtr() == 3)
            {
                float *paramValue_f = param_f[paramIndex];
                QString pName_f = QString::number(*paramValue_f);
                port->setName(pName_f, visible.at(paramIndex));
                int textWidth_f = fm.width(pName_f);
                if (w<textWidth_f)
                {
                   w = fm.width(textWidth_f);
                }
                float *hh = param_f[paramIndex];
                float *bb = defaultParam_f[paramIndex];
                float jj =  float(*hh) ;
                float ll =  float(*bb) ;
                if (ll == jj && hasToBeSet.at(paramIndex) == true)
                {
                    port->setRed();
                }
                paramIndex++;
            }
            if (port->thirdPtr() == 4)
            {
                 int *paramName_i = param_i[paramIndex];
                QString pName_i = QString::number(*paramName_i);
                port->setName(pName_i, visible.at(paramIndex));
                int textWidth_i = fm.width(pName_i);
                if (w<textWidth_i)
                {
                   w = fm.width(textWidth_i);
                }
                int *hh = param_i[paramIndex];
                int *bb = defaultParam_i[paramIndex];
                int jj =  int(*hh) ;
                int ll =  int(*bb) ;
                if (jj == ll && hasToBeSet.at(paramIndex) == true)
                {
                    port->setRed();
                }
                paramIndex++;
            }
        }
        else
        {
           QString pName_i = port->portName();
           int wt = fm.width(pName_i);
           if (w<wt)
           {
              w = wt;
           }
        }
    }
    setWidth(w + 25);
}
void QNEBlock::openPermDialog()
{
    QGraphicsView *view = new QGraphicsView(this->scene());
    QGroupBox *groupBox = new QGroupBox(block_type +"->" + block_name);
    QVBoxLayout *vbox = new QVBoxLayout;
    widVectorI.clear();
    widVectorS.clear();
    widVectorF.clear();
    for(int i = 0; i <propVector.size() ; i++)
    {
        QString name = propVector.at(i)->getName();
        if (propVector.at(i)->getType() == 0)
        {
            //d.addLineEdit (name, param_s[i], "text field");
            QLabel *label = new QLabel(0);
            if (name.contains("/"))
            {
                QString subString = name.left(name.size() - 1);
                label->setText(subString);
            }
            else
            {
               label->setText(name);
            }
            vbox->addWidget(label);
            CustomLineEdit *le = new CustomLineEdit(0);
            le->setScene(this->scene());
            le->setText(QString::fromStdString(*param_s[i]));
            le->setToolTip(name);
            le->setObjectName(name);
            string *hh = param_s[i];
            string *bb = defaultParam_s[i];
            QString jj =  QString::fromStdString(*hh) ;
            QString ll =  QString::fromStdString(*bb) ;
            if (jj == ll && (hasToBeSet.at(i) == true))
            {
                le ->setStyleSheet(
                "QWidget:disabled{color: #64B2D1;}"
                "QWidget{color: red;}");
            }
            else
            {
                hasToBeSet[i] = false;
            }
            vbox->addWidget(le);
            widVectorS.append(le);
        }
        else
        {
            string *defS = new string("0");
            param_s[i] = defS;
        }
        if (propVector.at(i)->getType() == 1)
        {
            float limitMin = propVector.at(i)->getMinValue();
            float limitMax = propVector.at(i)->getMaxValue();
            QLabel *label = new QLabel(0);
            if (name.contains("/"))
            {
                QString subString = name.left(name.size() - 1);
                label->setText(subString);
            }
            else
            {
               label->setText(name);
            }
            label->setFrameStyle(QFrame::Panel | QFrame::Plain);
            label->setLineWidth(5);
            vbox->addWidget(label);
            CustomSpinBoxF *dsb = new CustomSpinBoxF(0);
            dsb->setScene(this->scene());
            dsb->setRange(limitMin, limitMax);
            dsb->setValue(*param_f[i]);
            dsb->setSingleStep(0.01);
            dsb->setDecimals(10);
            dsb->setToolTip(name);
            vbox->addWidget(dsb);
            float *hh = param_f[i];
            float *bb = defaultParam_f[i];
            float jj =  float(*hh) ;
            float ll =  float(*bb) ;
            if (ll == jj && (hasToBeSet.at(i) == true))
            {
                dsb ->setStyleSheet(
                "QWidget:disabled{color: #64B2D1;}"
                "QWidget{color: red;}");
            }
            else
            {
                hasToBeSet[i] = false;
            }
            widVectorF.append(dsb);
        }
        else
        {
            float *defF = new float(0);
            param_f[i] = defF;
        }
        if (propVector.at(i)->getType() == 2)
        {
            float limitMin = propVector.at(i)->getMinValue();
            float limitMax = propVector.at(i)->getMaxValue();
            QLabel *label = new QLabel(0);
            if (name.contains("/"))
            {
                QString subString = name.left(name.size() - 1);
                label->setText(subString);
            }
            else
            {
               label->setText(name);
            }
            label->setFrameStyle(QFrame::Panel | QFrame::Raised);
            label->setLineWidth(2);
            vbox->addWidget(label);
            CustomSpinBox *sb = new CustomSpinBox(0);
            sb->setScene(this->scene());
            sb->setRange(limitMin, limitMax);
            sb->setValue(*param_i[i]);
            sb->setSingleStep(1);
            sb->setToolTip(name);
            vbox->addWidget(sb);
            int *hh = param_i[i];
            int *bb = defaultParam_i[i];
            int jj =  int(*hh) ;
            int ll =  int(*bb) ;
            if (ll == jj && (hasToBeSet.at(i) == true))
            {
                sb ->setStyleSheet(
                "QWidget:disabled{color: #64B2D1;}"
                "QWidget{color: red;}");
            }
            else
            {
                hasToBeSet[i] = false;
            }
            widVectorI.append(sb);
        }
        else
        {
            int *defI = new int(0);
            param_i[i] = defI;
        }
    }
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    QGraphicsProxyWidget* gbox = this->scene()->addWidget(groupBox);
    gbox->setZValue(0.01);

    //gbox ->setPos(this->pos().x() + 75, (this->pos().y() -40));
    QPoint global = view->mapToGlobal(QPoint(0,0));
    //this-Application.Current.MainWindow.Left
    QPoint globalR = groupBox->mapFromGlobal(global);
    gbox->setPos(globalR.x() + 5, globalR.y() + 115);
    //gbox->setPos(this->pos());
    gbox ->setToolTip(block_name);
    QFontMetrics fm(scene()->font());
    int paramIndex = 0;
    int w = 0;
    foreach(QGraphicsItem *port_, childItems()) {

        if (port_->type() != QNEPort::Type)
            continue;

        QNEPort *port = (QNEPort*) port_;
        if (port->isMainType() == 0)
        {
            if (port->thirdPtr() == 2)
            {
                string *paramValue_s = param_s[paramIndex];
                QString ps = QString::fromStdString(*paramValue_s);
                port->setName(ps, visible.at(paramIndex));
                int textWidth = fm.width(ps);
                if (w<textWidth)
                {
                   w = fm.width(ps);
                }
                paramIndex++;
            }
            if (port->thirdPtr() == 3)
            {
                float *paramValue_f = param_f[paramIndex];
                QString pName_f = QString::number(*paramValue_f);
                port->setName(pName_f, visible.at(paramIndex));
                int textWidth_f = fm.width(pName_f);
                if (w<textWidth_f)
                {
                   w = fm.width(textWidth_f);
                }
                paramIndex++;
            }
            if (port->thirdPtr() == 4)
            {
                 int *paramName_i = param_i[paramIndex];
                QString pName_i = QString::number(*paramName_i);
                port->setName(pName_i, visible.at(paramIndex));
                int textWidth_i = fm.width(pName_i);
                if (w<textWidth_i)
                {
                   w = fm.width(textWidth_i);
                }
                paramIndex++;
            }
        }
        else
        {
           QString pName_i = port->portName();
           int wt = fm.width(pName_i);
           if (w<wt)
           {
              w = wt;
           }
        }
    }
    setWidth(w + 25);
}


QNEPort* QNEBlock::addPort(const QString &name, bool isOutput, int flags, int ptr, int isMainType, int thirdPtr)
{
	QNEPort *port = new QNEPort(this);
    if (block_name == "(x*y)" || block_name == "(x/y)" || block_name == "(x-y)" || block_name == "(x+y)" || block_name == "(x%y)" || block_name == "x<y" || block_name == "x>y" || block_name == "x<=y" || block_name == "x>=y" || block_name == "x=y" || block_name == "x||y" || block_name == "x and y" )
    {
        if (numberOfCreatedPorts == 2)
        {
            port->setName("x__Op" + QString::number(blockID), true);
        }
        else
        {
            if ((name.contains("/")  && name != "(x/y)")|| (flags == QNEPort::TypePort && isMainType == 1))
            {
                QString subString = name.left(name.size() - 1);
                port->setName(name, false);
            }
            else
            {
                port->setName(name, true);
            }
        }
    }
    else if (block_name != "plugin_setting" && block_name != "smart_panel" && block_name != "add_in_step" && block_name != "add_in_widget" && block_name != "add_in_struct" && block_name != "add_in_module_CPP" && block_name != "add_include")
    {
        if ((name.contains("/")  && name != "(x/y)")|| (flags == QNEPort::TypePort && isMainType == 1))
        {
            QString subString = name.left(name.size() - 1);
            port->setName(name, false);
        }
        else
        {
            if (numberOfCreatedPorts == 2)
            {
                QString subString = block_name.left(3);
                port->setName(subString + QString::number(blockID), true);
            }
            else
            {
                port->setName(name, true);
            }
        }
    }
    else
    {
        if ((name.contains("/")  && name != "(x/y)")|| (flags == QNEPort::TypePort && isMainType == 1))
        {
            QString subString = name.left(name.size() - 1);
            port->setName(name, false);
        }
        else
        {
                port->setName(name, true);
        }
    } 
    numberOfCreatedPorts ++;
	port->setIsOutput(isOutput);
	port->setNEBlock(this);
	port->setPortFlags(flags);
	port->setPtr(ptr);
    port->setIsMainType(isMainType);
    port->setThirdPtr(thirdPtr);

    if (flags == QNEPort::NamePort)
    {
        block_name = name;

    }
    if (isMainType == 1)
    {
        block_type = name;
    }
	QFontMetrics fm(scene()->font());
	int w = fm.width(name);
	int h = fm.height();
	if (w > width - horzMargin)
        width = w*1.15 + horzMargin;

    if ((name.contains("/") == false  || name == "(x/y)")&& (isMainType != 1 || flags == QNEPort::NamePort))
    {
        height += h;
    }
    QPainterPath p;
    p.addRoundedRect(-width/2, -height/2, width, height, 5, 5);

	setPath(p);

    float y = -height / 2 + vertMargin + port->radius();

    foreach(QGraphicsItem *port_, childItems()) {
		if (port_->type() != QNEPort::Type)
			continue;

		QNEPort *port = (QNEPort*) port_;
		if (port->isOutput())
        {
            port->setPos(width/2 + port->radius(), y);
            number_of_outputs ++;
        }
		else
        {
            port->setPos(-width/2 - port->radius(), y);
        }
        if (((port->portName().contains("/") == false || name == "(x/y)")&& port->isMainType() != 1 )|| port->portFlags() == QNEPort::NamePort)
        {
        y += h;
        }
	}
    if (o_width_set == false)
    {
        originalWidth = width;
    }
    /*QGraphicsView *view = new QGraphicsView(this->scene());
    QPoint global = view->mapToGlobal(QPoint(0,0));
    QPoint globalR = this-()->mapFromGlobal(global);*/
    if (this->scene()->selectedItems().size() > 0)
    {
        QPointF globalR = this->scene()->selectedItems().at(0)->pos();
        this->setPos(QPointF(globalR.x() + 40 , globalR.y() + 40));
    }

    this->ensureVisible();

    if (hasToBeSet.size() > numberOfCreatedPorts && numberOfCreatedPorts > 1 )
    {
        string *hh;
        hh = defaultParam_s.at(numberOfCreatedPorts - 1);
        QString jj =  QString::fromStdString(*hh) ;
        string *xx;
        xx = param_s.at(numberOfCreatedPorts - 1);
        /*QString ss =  QString::fromStdString(*hh) ;
        QByteArray s = ss.toUtf8();
        QByteArray j = jj.toUtf8();
        qDebug(s);
        qDebug(j);*/
        if (hasToBeSet.at(numberOfCreatedPorts - 1))
        {
            port->setRed();
        }
    }
    setPortsColor();
	return port;
}

QNEPort* QNEBlock::addPortOnLoading(const QString &name, bool isOutput, int flags, int ptr, int isMainType, int thirdPtr)
{
    QNEPort *port = new QNEPort(this);
    if (block_name == "(x*y)" || block_name == "(x/y)" || block_name == "(x-y)" || block_name == "(x+y)" || block_name == "(x%y)" || block_name == "x<y" || block_name == "x>y" || block_name == "x<=y" || block_name == "x>=y" || block_name == "x=y" || block_name == "x||y" || block_name == "x and y" )
    {
        if (numberOfCreatedPorts == 2)
        {
            port->setName("x__Op" + QString::number(blockID), true);
        }
        else
        {
            if ((name.contains("/") && name != "(x/y)")|| (flags == QNEPort::TypePort && isMainType == 1))
            {
                //QString subString = name.left(name.size() - 1);
                port->setName(name, false);
            }
            else
            {
                port->setName(name, true);
            }
        }
    }
    else if (block_name != "plugin_setting" && block_name != "smart_panel" && block_name != "add_in_step" && block_name != "add_in_widget" && block_name != "add_in_struct" && block_name != "add_in_module_CPP" && block_name != "add_include")
    {
        if ((name.contains("/") && name != "(x/y)") || (flags == QNEPort::TypePort && isMainType == 1))
        {
            //QString subString = name.left(name.size() - 1);
            port->setName(name, false);
        }
        else
        {
                port->setName(name, true);
        }
    }
    else
    {
        if ((name.contains("/") && name != "(x/y)") || (flags == QNEPort::TypePort && isMainType == 1))
        {
            //QString subString = name.left(name.size() - 1);
            port->setName(name, false);
        }
        else
        {
                port->setName(name, true);
        }
    }
    numberOfCreatedPorts ++;
    port->setIsOutput(isOutput);
    port->setNEBlock(this);
    port->setPortFlags(flags);
    port->setPtr(ptr);
    port->setIsMainType(isMainType);
    port->setThirdPtr(thirdPtr);

    if (flags == QNEPort::NamePort)
    {
        block_name = name;

    }
    if (isMainType == 1)
    {
        block_type = name;
    }
    QFontMetrics fm(scene()->font());
    int w = fm.width(name);
    int h = fm.height();
    if (w > width - horzMargin)
        width = w*1.15 + horzMargin;

    if (((name.contains("/") == false || name == "(x/y)")&& (isMainType != 1 || flags == QNEPort::NamePort)))
    {
        height += h;
    }
    QPainterPath p;
    p.addRoundedRect(-width/2, -height/2, width, height, 5, 5);

    setPath(p);

    float y = -height / 2 + vertMargin + port->radius();

    foreach(QGraphicsItem *port_, childItems()) {
        if (port_->type() != QNEPort::Type)
            continue;

        QNEPort *port = (QNEPort*) port_;
        if (port->isOutput())
        {
            port->setPos(width/2 + port->radius(), y);
            number_of_outputs ++;
        }
        else
        {
            port->setPos(-width/2 - port->radius(), y);
        }
        if (((port->portName().contains("/") == false || name == "(x/y)")&& port->isMainType() != 1 )|| port->portFlags() == QNEPort::NamePort)
        {
            y += h;
        }
    }
    if (o_width_set == false)
    {
        originalWidth = width;
    }
    /*QGraphicsView *view = new QGraphicsView(this->scene());
    QPoint global = view->mapToGlobal(QPoint(0,0));
    QPoint globalR = this-()->mapFromGlobal(global);*/
    if (this->scene()->selectedItems().size() > 0)
    {
        QPointF globalR = this->scene()->selectedItems().at(0)->pos();
        this->setPos(QPointF(globalR.x() + 40 , globalR.y() + 40));
    }

    this->ensureVisible();

    if (hasToBeSet.size() > numberOfCreatedPorts && numberOfCreatedPorts > 1 )
    {
        string *hh;
        hh = defaultParam_s.at(numberOfCreatedPorts - 1);
        QString jj =  QString::fromStdString(*hh) ;
        string *xx;
        xx = param_s.at(numberOfCreatedPorts - 1);
        QString ss =  QString::fromStdString(*hh) ;
        QByteArray s = ss.toUtf8();
        QByteArray j = jj.toUtf8();
        qDebug(s);
        qDebug(j);
        if (hasToBeSet.at(numberOfCreatedPorts - 1))
        {
            port->setRed();
        }
    }
    setPortsColor();
    return port;
}

void QNEBlock::setWidth(int width_)
{
    int newWidth;
    if (width_ > originalWidth)
    {
           newWidth = width_;
    }
    else
    {
           newWidth = originalWidth;
    }
    if (newWidth>105)
    {
        newWidth = 105;
    }
    QPainterPath p;
    //p.addRoundedRect(-newWidth/2, -height/2, newWidth, 18, 5, 0);
    p.addRoundedRect(-newWidth/2, -(height)/2, newWidth, height+5, 5, 5);

    setPath(p);
    QFontMetrics fm(scene()->font());
    int h = fm.height();
    int y = -height / 2 + vertMargin + 5;

    foreach(QGraphicsItem *port_, childItems()) {
        if (port_->type() != QNEPort::Type)
            continue;

        QNEPort *port = (QNEPort*) port_;
        if (port->isOutput())
        {
            port->setX(newWidth/2 + port->radius());
        }
        else
        {
            port->setX(-newWidth/2 - port->radius());
        }
        if ((port->portName().contains("/") == false && port->isMainType() != 1 )|| port->portFlags() == QNEPort::NamePort )
        {
            y += h;
        }
    }
}
int QNEBlock::getWidth()
{
    return width;
}

void QNEBlock::addInputPort(const QString &name)
{
    addPort(name, false,0, 6, 11, 6);
    number_of_inputs ++;
}

void QNEBlock::addOutputPort(const QString &name)
{
    addPort(name, true,0, 6, 11, 6);
}

void QNEBlock::addInputPorts(const QStringList &names)
{
	foreach(QString n, names)
    addInputPort(n);
}

void QNEBlock::addOutputPorts(const QStringList &names)
{
	foreach(QString n, names)
    addOutputPort(n);
}

QNEBlock* QNEBlock::clone(int NBlockID)
{
    QNEBlock *b = new QNEBlock(NBlockID, 0);
    this->scene()->addItem(b);
    int i = 0;
    foreach(QGraphicsItem *port_, childItems())
    {
        if (port_->type() == QNEPort::Type)
        {
            QNEPort *port = (QNEPort*) port_;
            QString pName = port->portName();
            QVector<QNEConnection*> connections = port->connections();
            if ((port->thirdPtr() == 2 || port->thirdPtr() == 3 || port->thirdPtr() == 4) && port->isMainType() == 0)
            {
                QString prName = propVector.at(i)->getName();
                if (visible.at(i) == false)
                {
                    pName = pName + "/";
                    prName = prName + "/";
                }

                b->addProperty(prName, propVector.at(i)->getType(), propVector.at(i)->getMinValue(), propVector.at(i)->getMaxValue(), *param_s.at(i), *param_f.at(i), *param_i.at(i));
                i++;
            }
            b->addPort(pName, port->isOutput(), port->portFlags(), port->ptr(),  port->isMainType(), port->thirdPtr());
        }
    }
    b->setPortState(hasToBeSet);
    return b;
}
void QNEBlock::setPortState(QVector<bool> toBeSet)
{
    hasToBeSet.clear();
    for (int i = 0; i < toBeSet.size(); i ++)
    {
        hasToBeSet.append(toBeSet.at(i));
    }
}

void QNEBlock::save(QDataStream &ds)
{
	ds << pos();
	int count(0);
    ds << originalWidth;
    ds << onPanel;
    ds << blockID;
    ds << block_name;
    ds << block_type;
    ds << number_of_inputs;
    ds << propVector.size();
    for (int i = 0 ; i<propVector.size(); i++)
    {
        QString prName = propVector.at(i)->getName();
        if (visible.at(i) == false)
        {
            //prName = prName + "/";
        }
        ds << prName;
        ds << propVector.at(i)->getType();
        ds << propVector.at(i)->getMinValue();
        ds << propVector.at(i)->getMaxValue();
        string *s = param_s.at(i);
        QString qs = QString::fromStdString(*s);
        ds << qs;
        ds << *param_f.at(i);
        ds << *param_i.at(i);
        ds << hasToBeSet.at(i);
        string *dst = defaultParam_s.at(i);
        QString qdst = QString::fromStdString(*dst);
        ds << qdst;
        int *dInt = defaultParam_i.at(i);
        int dIntR = int(*dInt);
        ds << dIntR;
        float *dFloat = defaultParam_f.at(i);
        float dFloatR = float(*dFloat);
        ds << dFloatR;
    }

    foreach(QGraphicsItem *port_, childItems())
    {
        if (port_->type() != QNEPort::Type)
            continue;
            count++;
    }

    ds << count;
    int j = 0;
    foreach(QGraphicsItem *port_, childItems())
    {
        if (port_->type() != QNEPort::Type)
            continue;
        QNEPort *port = (QNEPort*) port_;
        QString pName = port->portName();
        if ((port->thirdPtr() == 2 || port->thirdPtr() == 3 || port->thirdPtr() == 4) && port->isMainType() == 0)
        {
            if (visible.at(j) == false && pName.contains("/") == false)
            {
                pName = pName + "/";
            }
            j++;
        }
        ds << (quint64) port;
        ds << port->isMainType();
        ds << port->thirdPtr();
        ds << pName;
        ds << port->isOutput();
        ds << port->portFlags();
    }
}

void QNEBlock::load(QDataStream &ds, QMap<quint64, QNEPort*> &portMap)
{
	QPointF p;
	ds >> p;
	setPos(p);
    int origWidth;
    bool OP;
    QString b_name;
    QString b_type;
    int b_ID;
    int inputsNum;
    ds >> origWidth;
    ds >> OP;
    ds >> b_ID;
    ds >> b_name;
    ds >> b_type;
    ds >> inputsNum;
    block_name = b_name;
    block_type = b_type;
    blockID = b_ID;
    originalWidth = origWidth;
    onPanel = OP;
    number_of_inputs = inputsNum;
    int propNum;
    ds >> propNum;
    if (hasToBeSet.size() > 0)
    {
        hasToBeSet.clear();
    }
    if (defaultParam_f.size() > 0)
    {
        defaultParam_f.clear();
    }
    if (defaultParam_s.size() > 0)
    {
        defaultParam_s.clear();
    }
    if (defaultParam_i.size() > 0)
    {
        defaultParam_i.clear();
    }
    for (int j = 0; j<propNum; j++)
    {
        QString pName;
        int pType;
        float minValue;
        float maxValue;
        QString sValue;
        float fValue;
        int iValue;
        bool toBeSet;
        int defI;
        float defF;
        QString defS;
        ds >> pName;
        ds >> pType;
        ds >> minValue;
        ds >> maxValue;
        ds >> sValue;
        ds >> fValue;
        ds >> iValue;
        ds >> toBeSet;
        ds >> defS;
        ds >> defI;
        ds >> defF;
        string *defSValueStd = new string (defS.toStdString());
        defaultParam_s.append(defSValueStd);
        int *defIP = new int (defI);
        defaultParam_i.append(defIP);
        float *defFP = new float (defF);
        defaultParam_f.append(defFP);
        hasToBeSet.append(toBeSet);
        string sValueStd = sValue.toStdString();
        addPropertyFromClone(pName, pType, minValue, maxValue, sValueStd, fValue, iValue);
    }

    int count;
    ds >> count;
    for (int i = 0; i < count; i++)
    {
        QString name;
        bool output;
        int flags;
        quint64 ptr;
        int ptr2;
        int ptr3;
        ds >> ptr;
        ds >> ptr2;
        ds >> ptr3;
        ds >> name;
        ds >> output;
        ds >> flags;
        o_width_set = true;
        portMap[ptr] = addPortOnLoading(name, output, flags, (int)ptr, ptr2, ptr3);
    }
}

void QNEBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	if (isSelected()) {

        painter->setPen(QPen(Qt::black));
        painter->setBrush(QColor(50,50,50));
	} else {
        painter->setBrush(QColor(0,0,3));

        foreach(QGraphicsItem *port_, childItems())
        {
            QNEPort *port = (QNEPort*) port_;
            if (port_->type() == QNEPort::Type)
            {
                if (port->portName() == "Controllers"|| port->portName() == "I/O" || port->portName() == "screw")
                {
                    if (onPanel == false)
                    {
                        painter->setPen(QColor(154,80,80));
                        qc = QColor(253,200,200);
                        painter->setBrush(QColor(180,74,58));
                    }
                    else
                    {
                        painter->setPen(QColor(154,80,80));
                        qc = QColor(255,255,200);
                    }
                }
                else if (port->portName() == "Processors")
                {
                    painter->setPen(QColor(135,150,243));
                    qc = QColor(195,200,243);
                }
                else if (port->portName() == "Generators")
                {
                    painter->setPen(QColor(150,200,180));
                    qc = QColor(200,230,200);
                }
                else if (port->portName() == "Arrays"  || port->portName() == "Counters/Seq" || port->portName() == "Numbers" )
                {
                    painter->setPen(QColor(243,160,103));
                    qc = QColor(243,226,173);
                }
                else if (port->portName() == "Maths" || port->portName() == "Comparators")
                {
                    painter->setPen(QColor(243,120,63));
                    qc = QColor(243,200,163);
                }
                else if (port->portName() == "Others")
                {
                    painter->setPen(QColor(210,103,120));
                    qc = QColor(210,183,200);
                }
            }
        }
	}
	painter->drawPath(path());
}

QVector<QNEPort*> QNEBlock::ports()
{
	QVector<QNEPort*> res;
	foreach(QGraphicsItem *port_, childItems())
	{
        //if (port_->type() == QNEPort::Type)
			res.append((QNEPort*) port_);
	}
	return res;
}
void QNEBlock::IsOnPanel(bool is_on_panel)
{
    onPanel = is_on_panel;
}
QVariant QNEBlock::itemChange(GraphicsItemChange change, const QVariant &value)
{
    Q_UNUSED(change);
	return value;
}
// seq 18, drum 24, mix 18, modulation 18,vco vcf, vca 24,

QString QNEBlock::blockName()
{
    return block_name;
    //return "xx";
}
int QNEBlock::uniqueID()
{
    return blockID;
}
QString QNEBlock::blockType()
{
    return block_type;
}
int QNEBlock::numberOfInputs()
{
    return number_of_inputs;
}
int QNEBlock::numberOfOutputs()
{
    return number_of_outputs;
}
void QNEBlock::setParamI(int value, int index)
{
    int *p = new int(value);
    param_i[index]= p;
}


