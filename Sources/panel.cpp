#include "panel.h"
#include <QGraphicsScene>

Panel::Panel(QGraphicsItem *parent) : QGraphicsPathItem(parent)
{

}
QGraphicsSvgItem* Panel::panItem(QString Filepath)
{
    QGraphicsSvgItem *svgPan = new QGraphicsSvgItem(Filepath);
    svgPan->setToolTip("panel");
    width = svgPan->boundingRect().width();
    return svgPan;
}
float Panel::getWidth()
{
    return width;
}
