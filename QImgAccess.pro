#-------------------------------------------------
#
# Project created by QtCreator 2010-10-22T10:47:19
#
#-------------------------------------------------

QT       += core gui

TARGET = QImgAccess
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    imgchildwnd.cpp \
    QImgAccess_global.cpp \
    PixmapWidget.cpp \
    createimgdlg.cpp

HEADERS  += mainwindow.h \
    imgchildwnd.h \
    QImgAccess_global.h \
    PixmapWidget.h \
    createimgdlg.h

FORMS    += mainwindow.ui \
    createimgdlg.ui

RESOURCES += \
    QImgAccess.qrc

INCLUDEPATH += /usr/include/opencv/

LIBS += /usr/lib/libcxcore.so \
        /usr/lib/libcv.so \
        /usr/lib/libcvaux.so \
        /usr/lib/libhighgui.so
