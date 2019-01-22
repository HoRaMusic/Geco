#ifndef CUSTOMSPINBOX_H
#define CUSTOMSPINBOX_H
#include <QSpinBox>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "qneblock.h"
class CustomSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    CustomSpinBox(QWidget *parent = nullptr) : QSpinBox(parent){}
    void setScene(QGraphicsScene *gs)
    {
        nodes_editor_scene = gs;
    }
protected:
    QGraphicsScene *nodes_editor_scene;

    void focusOutEvent(QFocusEvent *event)
    {
        QSpinBox::focusOutEvent(event);
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
        QSpinBox::keyPressEvent(event);
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
        QSpinBox::mousePressEvent(event);
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
#endif // CUSTOMSPINBOX_H
