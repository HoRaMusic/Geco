
#include "Headers/paneleditor.h"
#include <QGraphicsScene>
#include <QEvent>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

Paneditor::Paneditor(QObject *parent) :
    QObject(parent)
{

};

void Paneditor::install(QGraphicsScene *s, QGraphicsScene *sNE)
{
    s->installEventFilter(this);
    scene = s;
    sceneNE = sNE;
    dsb_x = new QDoubleSpinBox(0);
    dsb_x->setRange(0, 1000);
    dsb_x->setValue(0);
    dsb_x->setSingleStep(0.01);
    dsb_x->setDecimals(2);
    QGraphicsProxyWidget* pProxyWidget_x = scene->addWidget(dsb_x);
    pProxyWidget_x->setPos(2, 380);
    pProxyWidget_x->setToolTip("SpinBox_1");

    dsb_y = new QDoubleSpinBox(0);
    dsb_y->setRange(0, 1000);
    dsb_y->setValue(0);
    dsb_y->setSingleStep(0.01);
    dsb_y->setDecimals(2);
    QGraphicsProxyWidget* pProxyWidget_y = scene->addWidget(dsb_y);
    pProxyWidget_y->setPos(80, 380);
    pProxyWidget_y->setToolTip("SpinBox_2");

    sendPosition = new QPushButton("&send position", 0);
    QGraphicsProxyWidget* button = scene->addWidget(sendPosition);
    button->setPos(50, 410);
    button->setToolTip("button");
    connect( sendPosition, SIGNAL(clicked()), this, SLOT(setPosFromSB()));
}

QGraphicsItem* Paneditor::itemAt(const QPointF &pos)
{
    QList<QGraphicsItem*> items = scene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

    foreach(QGraphicsItem *item, items)

            return item;
    return 0;
}
void Paneditor::setPosFromSB()
{
        foreach(QGraphicsItem *item, scene->items())
        {
            if (item->isSelected())
            {
                item->setPos(dsb_x->value(),dsb_y->value());
            }
        }
}

QVector<QString>  Paneditor::deletedParamList()
{
    return deletedParam;
}
void  Paneditor::eraseDeletedParamList(int index)
{
    deletedParam.remove(index);
}
void Paneditor::eraseSelectedItem()
{
    foreach(QGraphicsItem *item, scene->items())
    {
        if (item->isSelected())
        {
            delete item;
        }
    }
}
bool Paneditor::eventFilter(QObject *o, QEvent *e)
{
    QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*) e;
    switch ((int) e->type())
    {
        case QEvent::GraphicsSceneMousePress:
        {
            if (me->button() == Qt::RightButton &&  itemAt(me->scenePos()) != NULL)
            {
                QGraphicsItem *item = itemAt(me->scenePos());
                QString param_name = item->toolTip();
                param_name.truncate(param_name.lastIndexOf(QChar(',')));
                foreach(QGraphicsItem *Nitem, sceneNE->items())
                {
                    if (Nitem->type() == QNEBlock::Type)
                    {
                        QNEBlock *BL = (QNEBlock*) Nitem;
                        foreach(QGraphicsItem *port_, Nitem->childItems())
                        {
                             QNEPort *port = (QNEPort*) port_;
                             QString pName = port->portName();
                             if (pName == param_name)
                             {
                                 BL->IsOnPanel(false);
                                 double x = BL->pos().x();
                                 double y = BL->pos().y();
                                 BL->setPos( x + 1, y + 1);
                                 deletedParam.append(pName);
                             }
                        }
                    }
                }
                delete item;
            }
            break;
        }
        case QEvent::GraphicsSceneMouseMove:
        {
            break;
        }
        case QEvent::GraphicsSceneMouseRelease:
        {
            if (me->button() == Qt::LeftButton)
            {
                QGraphicsItem *item = itemAt(me->scenePos());
                if (item != 0)
                {
                    if (item->isSelected())
                    {
                        float xt = item->pos().x();
                        float yt = item->pos().y();
                        dsb_x->setValue(xt);
                        dsb_y->setValue(yt);
                    }
                    else
                    {
                       //setPosFromSB();
                    }
                }
                break;
            }
        }
    }
    return QObject::eventFilter(o, e);
}
