#-------------------------------------------------
#
# Project created by QtCreator 2015-08-20T21:52:41
#
#-------------------------------------------------

QT += core gui core xml webkit webkitwidgets sql
QT += network
QT += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Study
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    CapBtn.cpp \
    MenuBtn.cpp \
    PraxisDlg.cpp \
    NetWork.cpp \
    Course.cpp \
    CustomEvent.cpp \
    Praxis.cpp \
    SyllabusMusicDlg.cpp \
    SyllabusBasicDlg.cpp \
    SyllabusWidget.cpp \
    LogonDlg.cpp \
    VideoPlayerWidget.cpp \
    MainWidgetTitle.cpp \
    mytextbrower.cpp \
    allwidget.cpp \
    autofirstdialog.cpp \
    keyknowledge.cpp \
    GlobalInfo.cpp \
    msgbox.cpp

HEADERS  += widget.h \
    CapBtn.h \
    MenuBtn.h \
    PraxisDlg.h \
    NetWork.h \
    Course.h \
    GlobalInfo.h \
    CustomEvent.h \
    Praxis.h \
    SyllabusMusicDlg.h \
    SyllabusBasicDlg.h \
    SyllabusWidget.h \
    LogonDlg.h \
    VideoPlayerWidget.h \
    MainWidgetTitle.h \
    mytextbrower.h \
    allwidget.h \
    autofirstdialog.h \
    keyknowledge.h \
    msgbox.h

FORMS    += widget.ui \
    MenuBtn.ui \
    PraxisDlg.ui \
    Course.ui \
    Praxis.ui \
    SyllabusMusicDlg.ui \
    SyllabusBasicDlg.ui \
    SyllabusWidget.ui \
    LogonDlg.ui \
    VideoPlayerWidget.ui \
    autofirstdialog.ui \
    keyknowledge.ui \
    msgbox.ui

RESOURCES += \
    rc.qrc

DISTFILES +=

OTHER_FILES += qss/global.qss
