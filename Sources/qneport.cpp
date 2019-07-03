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

#include "Headers/qneport.h"

#include <QGraphicsScene>
#include <QFontMetrics>

#include <QPen>

#include "Headers/qneconnection.h"

QNEPort::QNEPort(QGraphicsItem *parent):
    QGraphicsPathItem(parent)
{
	label = new QGraphicsTextItem(this);
    //label->setDefaultTextColor(QColor(205, 205, 205));
    radius_ = 5;
	margin = 2;

	QPainterPath p;
    p.addEllipse(-radius_, -radius_, 2*radius_, 2*radius_);
    setPath(p);

    setPen(QPen(Qt::darkGray));
    setBrush(Qt::white);

	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	m_portFlags = 0;
}

QNEPort::~QNEPort()
{
	foreach(QNEConnection *conn, m_connections)
		delete conn;
}

void QNEPort::setNEBlock(QNEBlock *b)
{
	m_block = b;
}

void QNEPort::setName(const QString &n, bool visible)
{
	name = n;
    QString subString = n.left(16);
    QStringList wordList;
   if (n.contains(":"))
    {
        wordList = n.split(':');
        subString = wordList.at(0);
    }
    if (n.contains("x__Op"))
    {
        subString = n.left(1);
    }

    label->setVisible(visible);
    label->setPlainText(subString);
    label->setDefaultTextColor(QColor(205, 205, 205));
}

void QNEPort::setIsOutput(bool o)
{
	isOutput_ = o;

    //QFontMetrics fm(scene()->font());
    //QRect r = fm.boundingRect(name);

    if (isOutput_)
        label->setPos(-radius_ - margin - label->boundingRect().width(), -label->boundingRect().height()/2);
    else
        label->setPos(radius_ + margin, (-label->boundingRect().height()/2));
    label->setDefaultTextColor(QColor(205, 205, 205));
}

int QNEPort::radius()
{
	return radius_;
}

bool QNEPort::isOutput()
{
	return isOutput_;
}

QVector<QNEConnection*>& QNEPort::connections()
{
	return m_connections;
}

void QNEPort::setPortFlags(int f)
{
	m_portFlags = f;

	if (m_portFlags & TypePort)
	{
		QFont font(scene()->font());
		font.setItalic(true);
		label->setFont(font);
        label->setDefaultTextColor(QColor(205, 205, 205));
        setPath(QPainterPath());
    }
    else if (m_portFlags & NamePort)
	{
		QFont font(scene()->font());
		font.setBold(true);
		label->setFont(font);
        label->setDefaultTextColor(QColor(255, 255, 255));
        setPath(QPainterPath());
	}
}
void QNEPort::setRed()
{
    label->setDefaultTextColor(QColor(200, 75, 57));
    setPath(QPainterPath());
}
QNEBlock* QNEPort::block() const
{
	return m_block;
}

quint64 QNEPort::ptr()
{
	return m_ptr;
}
int QNEPort::isMainType()
{
    return m_second_ptr;
}
int QNEPort::thirdPtr()
{
    return m_third_ptr;
}
void QNEPort::setPtr(quint64 p)
{
	m_ptr = p;
}
void QNEPort::setIsMainType(int p)
{
    m_second_ptr = p;
}
void QNEPort::setThirdPtr(int p)
{
    m_third_ptr = p;
}


bool QNEPort::isConnected(QNEPort *other)
{
	foreach(QNEConnection *conn, m_connections)
		if (conn->port1() == other || conn->port2() == other)
			return true;
	return false;
}

QVariant QNEPort::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemScenePositionHasChanged)
	{
		foreach(QNEConnection *conn, m_connections)
		{
			conn->updatePosFromPorts();
			conn->updatePath();
		}
	}
	return value;
}
