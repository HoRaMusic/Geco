#ifndef CUSTOMSPINBOXF_H
#define CUSTOMSPINBOXF_H
#include <QDoubleSpinBox>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "qneblock.h"
class CustomSpinBoxF : public QDoubleSpinBox
{
    Q_OBJECT
public:
    CustomSpinBoxF(QWidget *parent = nullptr) : QDoubleSpinBox(parent){}
    void setScene(QGraphicsScene *gs)
    {
        nodes_editor_scene = gs;
    }
protected:
    QGraphicsScene *nodes_editor_scene;

    void focusOutEvent(QFocusEvent *event)
    {
        QDoubleSpinBox::focusOutEvent(event);
            foreach(QGraphicsItem *item, nodes_editor_scene->items())
            {
                QNEBlock *BL = (QNEBlock*) item;
                if (item->type() == QNEBlock::Type)
                {
                    if (item->isSelected())
                    {
                        BL->scanProperties();
                    }
                }
            }
    }
    void keyPressEvent(QKeyEvent *event)
    {
        QDoubleSpinBox::keyPressEvent(event);
            foreach(QGraphicsItem *item, nodes_editor_scene->items())
            {
                QNEBlock *BL = (QNEBlock*) item;
                if (item->type() == QNEBlock::Type)
                {
                    if (item->isSelected())
                    {
                        BL->scanProperties();
                    }
                }
            }
    }
    void mousePressEvent(QMouseEvent *event)
    {
        QDoubleSpinBox::mousePressEvent(event);
            foreach(QGraphicsItem *item, nodes_editor_scene->items())
            {
                QNEBlock *BL = (QNEBlock*) item;
                if (item->type() == QNEBlock::Type)
                {
                    if (item->isSelected())
                    {
                        BL->scanProperties();
                    }
                }
            }
    }
};
#endif // CUSTOMSPINBOXF_H
