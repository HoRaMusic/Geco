#include "controller.h"
#include <QGraphicsScene>

Controller::Controller(QGraphicsItem *parent): QGraphicsPathItem(parent)
{

}

QGraphicsSvgItem* Controller::controllerItem(QString Filepath, QString name, QString fileName, float posx, float posy)
{
    QGraphicsSvgItem *svgControl = new QGraphicsSvgItem(Filepath);
    svgControl->setToolTip(name + "," + fileName);
    svgControl->setPos(posx, posy);
    svgControl->setFlag(QGraphicsItem::ItemIsMovable);
    svgControl->setFlag(QGraphicsItem::ItemIsSelectable);
    return svgControl;
}

