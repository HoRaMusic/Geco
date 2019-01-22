#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H
#include <QLineEdit>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "qneblock.h"
class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    CustomLineEdit(QWidget *parent = nullptr) : QLineEdit(parent){}
    void setScene(QGraphicsScene *gs)
    {
        nodes_editor_scene = gs;
    }
protected:
    QGraphicsScene *nodes_editor_scene;

    void focusOutEvent(QFocusEvent *event)
    {
        QLineEdit::focusOutEvent(event);
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
        QLineEdit::keyPressEvent(event);
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
#endif // CUSTOMLINEEDIT_H
