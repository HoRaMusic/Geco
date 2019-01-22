#-------------------------------------------------
#
# Project created by QtCreator 2012-04-16T14:33:05
#
#-------------------------------------------------

QT       += core gui
QT += svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Geco
TEMPLATE = app


SOURCES += Sources/main.cpp\
        Sources/qnemainwindow.cpp \
    Sources/qneblock.cpp \
    Sources/qneport.cpp \
    Sources/qneconnection.cpp \
    Sources/qnodeseditor.cpp \
    Sources/customdialog.cpp \
    Sources/blockfilereader.cpp \
    Sources/blockmodel.cpp \
    Sources/paneleditor.cpp \
    Sources/panel.cpp \
    Sources/property.cpp \
    Sources/controller.cpp \
    Sources/controllerstore.cpp \
    Sources/writerutil.cpp \
    Sources/canvawrite.cpp \
    Sources/modulecanvawriter.cpp \
    Sources/externalwriter.cpp \
    Sources/stepwriter.cpp

HEADERS  += Headers/qnemainwindow.h \
    Headers/qneblock.h \
    Headers/qneport.h \
    Headers/qneconnection.h \
    Headers/qnodeseditor.h \
    Headers/customdialog.h \
    Headers/blockfilereader.h \
    Headers/blockmodel.h \
    Headers/paneleditor.h \
    Headers/panel.h \
    Headers/property.h \
    Headers/controller.h \
    Headers/controllerstore.h \
    Headers/writerutil.h \
    Headers/canvawrite.h \
    Headers/modulecanvawriter.h \
    Headers/externalwriter.h \
    Headers/stepwriter.h \
    Headers/customlineedit.h \
    Headers/customspinbox.h \
    Headers/customspinboxf.h

