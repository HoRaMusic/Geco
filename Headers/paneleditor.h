#ifndef PANELEDITOR_H
#define PANELEDITOR_H
#include "Headers/panel.h"
#include <QObject>
#include <QPushButton>
#include <Headers/qneblock.h>
#include <Headers/qneport.h>
class QGraphicsScene;
class QGraphicsItem;
class QPointF;
class Panel;
class Controller;


class Paneditor: public QObject
{
    Q_OBJECT
public:
    explicit Paneditor(QObject *parent = 0);

    void install(QGraphicsScene *scene, QGraphicsScene *sNE);
    QVector<QString> deletedParamList();
    void eraseDeletedParamList(int index);
    bool eventFilter(QObject *, QEvent *);
    void eraseSelectedItem();
private slots:
    void setPosFromSB();

private:
    QGraphicsItem *itemAt(const QPointF&);

private:
    QGraphicsScene *scene;
    QGraphicsScene *sceneNE;
    QDoubleSpinBox* dsb_x;
    QDoubleSpinBox* dsb_y;
    QPushButton* sendPosition;
    QVector<QString> deletedParam;

    // QNEBlock *selBlock;
};

#endif // PANELEDITOR_H
