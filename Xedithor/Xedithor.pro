#-------------------------------------------------
#
# Project created by QtCreator 2011-11-15T09:38:19
#
#-------------------------------------------------

QT       += core gui

TARGET = Xedithor
TEMPLATE = app


SOURCES += src\main.cpp\
        src\mainwindow.cpp \
    src/xscene.cpp \
    src/rectselectionitem.cpp \
    src/moduletablemodel.cpp \
    src/hlinegraphicsitem.cpp \
    src/graphwidgets.cpp \
    src/editwindow.cpp \
    src/moduleslist.cpp

HEADERS  += mainwindow.h \
    src/xscene.h \
    src/rowdata.h \
    src/rectselectionitem.h \
    src/moduletablemodel.h \
    src/mainwindow.h \
    src/hlinegraphicsitem.h \
    src/graphwidgets.h \
    src/globalconstant.h \
    src/editwindow.h \
    src/moduleslist.h


FORMS    += mainwindow.ui \
    editwindow.ui

RESOURCES += \
    XedithorResources.qrc

OTHER_FILES += \
    temp.txt





































