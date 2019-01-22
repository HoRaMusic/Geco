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

#include "qnodeseditor.h"

#include <QGraphicsScene>
#include <QEvent>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include "qneport.h"
#include "qneconnection.h"
#include "qneblock.h"

QNodesEditor::QNodesEditor(QObject *parent) :
    QObject(parent)
{
	conn = 0;
    widgetFocus = false;
    propMenuActive = true;
    propMenuVisible = true;
    ctrlPressed = false;
    mouseOverPanel = false;
}

void QNodesEditor::propVisible(bool isVisible)
{
    propMenuVisible = isVisible;
}
void QNodesEditor::controlPressed(bool _CtrlPressed)
{
    ctrlPressed = _CtrlPressed;
}
void QNodesEditor::install(QGraphicsScene *s)
{
	s->installEventFilter(this);
	scene = s;
}

QGraphicsItem* QNodesEditor::itemAt(const QPointF &pos)
{
	QList<QGraphicsItem*> items = scene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

	foreach(QGraphicsItem *item, items)
		if (item->type() > QGraphicsItem::UserType)
			return item;

	return 0;
}
void QNodesEditor::propActive(bool p_active)
{
    propMenuActive = p_active;
    QList<QGraphicsItem*> items = scene->items();
    foreach(QGraphicsItem *wd, items)
    {
        if (wd->isWidget())
        {
            delete wd;
        }
    }
}
void QNodesEditor::addSpinBoxI(QString name, int limitMin, int limitMax, int param_i, int minScale, QString defaultValue)
{

}
void QNodesEditor::addTextEdit(QString name, QString param_s, QString defaultValue)
{

}
bool QNodesEditor::eventFilter(QObject *o, QEvent *e)
{
	QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*) e;
    QList<QGraphicsItem*> items = scene->items();
    bool BlockSelected = false;
    int blSelNum = 0;
    foreach(QGraphicsItem *bl, items)
    {
        if (bl->type() == QNEBlock::Type)
        {
            if (bl->isSelected())
            {
                BlockSelected = true;
                blSelNum ++;
            }
        }
    }
    /*QString hh = QString::number(blSelNum);
    QByteArray s = hh.toUtf8();
    qDebug(s);*/
    if (BlockSelected == false)
    {
        propMenuActive = true;
        foreach(QGraphicsItem *wd, items)
        {
            if (wd->isWidget())
            {
                delete wd;
            }
        }
    }
    widgetFocus = false;
    mouseOverPanel = false;
    foreach(QGraphicsItem *wd, items)
    {
        if (wd->isWidget())
        {
            if (wd->focusItem())
            {
                widgetFocus = true;
            }
            if (wd->isUnderMouse())
            {
                mouseOverPanel = true;
            }
        }
    }
	switch ((int) e->type())
	{
	case QEvent::GraphicsSceneMousePress:
	{

            switch ((int) me->button())
            {
                case Qt::LeftButton:
                {

                    QGraphicsItem *item = itemAt(me->scenePos());
                  // bool itemPanelDisplayed = true;
                    if (item && item->type() == QNEPort::Type)
                    {
                        conn = new QNEConnection(0);
                        scene->addItem(conn);
                        conn->setPort1((QNEPort*) item);
                        conn->setPos1(item->scenePos());
                        conn->setPos2(me->scenePos());
                        conn->updatePath();
                        return true;
                    }
                    else if (item && item->type() == QNEBlock::Type )
                    {
                       // QByteArray s = "newSel";
                        int d = 0;
                       /* foreach(QGraphicsItem *wd, items)
                        {
                            if (wd->isWidget() && wd->toolTip() == ((QNEBlock*) item)->blockName())
                            {
                                itemPanelDisplayed = false;
                            }
                        }*/
                        if (mouseOverPanel == false)
                        {
                            foreach(QGraphicsItem *wd, items)
                            {
                                if (wd->isWidget())
                                {
                                    delete wd;
                                    d++;
                                }
                            }
                        }
                        if (blSelNum < 2)
                        {
                            //qDebug(s);
                            if (propMenuActive == true && propMenuVisible == true)
                            {

                                if (ctrlPressed == false)
                                {

                                    if (mouseOverPanel == false)
                                    {
                                        foreach(QGraphicsItem *bl, items)
                                        {
                                            if (bl->type() == QNEBlock::Type)
                                            {
                                                if (bl->isSelected())
                                                {
                                                    bl->setSelected(false);
                                                }
                                            }
                                        }
                                        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                                        item->setSelected(true);
                                        ((QNEBlock*) item)->openPermDialog();
                                        ((QNEBlock*) item)->scanProperties();
                                    }
                                    else
                                    {
                                        if (item->isSelected() == false)
                                        {
                                            item->setFlag(QGraphicsItem::ItemIsSelectable, false);
                                        }
                                    }

                                }

                            }
                        }
                    }
                    break;
                }
                case Qt::RightButton:
                {
                    QGraphicsItem *item = itemAt(me->scenePos());
                    if (item && item->type() == QNEConnection::Type)
                    {
                        delete item;
                    }
                    break;
                }
            break;
            }


	}
	case QEvent::GraphicsSceneMouseMove:
    {
        QGraphicsItem *item = itemAt(me->scenePos());

		if (conn)
		{
			conn->setPos2(me->scenePos());
			conn->updatePath();
			return true;
		}
		break;
	}
	case QEvent::GraphicsSceneMouseRelease:
	{
		if (conn && me->button() == Qt::LeftButton)
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && item->type() == QNEPort::Type)
			{
				QNEPort *port1 = conn->port1();
				QNEPort *port2 = (QNEPort*) item;

				if (port1->block() != port2->block() && port1->isOutput() != port2->isOutput() && !port1->isConnected(port2))
				{
					conn->setPos2(port2->scenePos());
					conn->setPort2(port2);
					conn->updatePath();
					conn = 0;
					return true;
				}
			}

			delete conn;
			conn = 0;
			return true;
		}
		break;
	}
	}
	return QObject::eventFilter(o, e);
}

void QNodesEditor::save(QDataStream &ds)
{
	foreach(QGraphicsItem *item, scene->items())
		if (item->type() == QNEBlock::Type)
		{
			ds << item->type();
			((QNEBlock*) item)->save(ds);
		}

	foreach(QGraphicsItem *item, scene->items())
		if (item->type() == QNEConnection::Type)
		{
			ds << item->type();
			((QNEConnection*) item)->save(ds);
		}
}

void QNodesEditor::load(QDataStream &ds)
{
    scene->clear(); // vide la scene actuel

	QMap<quint64, QNEPort*> portMap;

    while (!ds.atEnd()) // tant qu'on est pas a la fin du fichier
	{
		int type;
        ds >> type;
        if (type == QNEBlock::Type) // si c'est un block
		{
            QNEBlock *block = new QNEBlock(0);
            scene->addItem(block); // ajout d'un block
            block->load(ds, portMap); // chargement des ports et propriétés du bloc
        }
        else if (type == QNEConnection::Type)
		{
            QNEConnection *conn = new QNEConnection(0);
            scene->addItem(conn);
			conn->load(ds, portMap);
		}
	}
}
bool QNodesEditor::isPropActive()
{
    return propMenuActive;
}
