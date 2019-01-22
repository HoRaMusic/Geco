#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QGraphicsPathItem>
#include <QSpinBox>
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QLabel>
#include <QGraphicsSvgItem>
class Controller: public QGraphicsPathItem
{
public:
    enum { Type = QGraphicsItem::UserType + 3 };
    Controller(QGraphicsItem *parent = 0);
    QGraphicsSvgItem* controllerItem(QString Filepath, QString name , QString fileName, float posx, float posy);
    int type() const { return Type; }
};

#endif // CONTROLLER_H




