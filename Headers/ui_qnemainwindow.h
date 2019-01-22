/********************************************************************************
** Form generated from reading UI file 'qnemainwindow.ui'
**
** Created: Tue 17. Apr 16:01:26 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QNEMAINWINDOW_H
#define UI_QNEMAINWINDOW_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QGraphicsView>
#include <QHeaderView>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QtWidgets>

QT_BEGIN_NAMESPACE

class Ui_QNEMainWindow
{
public:
    QAction *action_Save;
    QAction *action_Load;
    QAction *action_Quit;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QToolBar *toolBar;

    void setupUi(QMainWindow *QNEMainWindow)
    {
        if (QNEMainWindow->objectName().isEmpty())
            QNEMainWindow->setObjectName(QString::fromUtf8("QNEMainWindow"));
        QNEMainWindow->resize(540, 389);
        action_Save = new QAction(QNEMainWindow);
        action_Save->setObjectName(QString::fromUtf8("action_Save"));
        action_Load = new QAction(QNEMainWindow);
        action_Load->setObjectName(QString::fromUtf8("action_Load"));
        action_Quit = new QAction(QNEMainWindow);
        action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
        centralWidget = new QWidget(QNEMainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        verticalLayout->addWidget(graphicsView);

        QNEMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QNEMainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 540, 20));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        QNEMainWindow->setMenuBar(menuBar);
        toolBar = new QToolBar(QNEMainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        QNEMainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menu_File->menuAction());
        menu_File->addAction(action_Save);
        menu_File->addAction(action_Load);
        menu_File->addSeparator();
        menu_File->addAction(action_Quit);

        retranslateUi(QNEMainWindow);

        QMetaObject::connectSlotsByName(QNEMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *QNEMainWindow)
    {
        QNEMainWindow->setWindowTitle(("QNodesEditor Demo (c) 2012 STANISLAW ADASZEWSKI, http://algoholic.eu"));
        action_Save->setText(("&Save"));
        action_Load->setText(("&Load"));
        action_Quit->setText(("&Quit"));
        menu_File->setTitle(("&File"));
        toolBar->setWindowTitle(("toolBar"));
    } // retranslateUi

};

namespace Ui {
    class QNEMainWindow: public Ui_QNEMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QNEMAINWINDOW_H
