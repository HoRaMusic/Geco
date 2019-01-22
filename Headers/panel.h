#ifndef PANEL_H
#define PANEL_H
#include <QGraphicsPathItem>
#include <QSpinBox>
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QLabel>
#include <QGraphicsSvgItem>
class Panel: public QGraphicsPathItem
{
public:
    enum { Type = QGraphicsItem::UserType + 3 };
    Panel(QGraphicsItem *parent = 0);
    QGraphicsSvgItem* panItem(QString Filepath);
    float getWidth();
    //~Panel();
    int type() const { return Type; }
private:
    float width;
};

#endif // PANEL_H
