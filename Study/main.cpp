#include "widget.h"
#include <QApplication>
#include <QTextCodec>
#include <QSettings>
#include "NetWork.h"
#include"allwidget.h"
#include<QDesktopWidget>
#include <QUrl>
#include <QFile>

/*
    组织数据用的是信号槽
    设置最大页和更新UI用的是事件
*/

QString g_sUrlHead = "";

void WriteSettings()
{
    QSettings settings("Option.ini", QSettings::IniFormat); // 当前目录的INI文件
    settings.beginGroup("Option");
    settings.setValue("UrlHead", "http://120.55.119.93/course/");
    settings.endGroup();
}

void ReadSettings()
{
    QSettings settings("Option.ini", QSettings::IniFormat);
    g_sUrlHead = settings.value("Option/UrlHead").toString();
    qDebug() << "IP" << g_sUrlHead;
}



int main(int argc, char *argv[])
{
    ReadSettings();

    QApplication a(argc, argv);
    QFile file(":/qss/global.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString strStyle = file.readAll();
        if (!strStyle.isEmpty())
            a.setStyleSheet(strStyle);
    }
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("gbk"));
    setlocale(LC_CTYPE, "zh_CN.GB2312");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    NetWork::GetInstance();
    QDesktopWidget* desktopWidget = QApplication::desktop();
    Widget::desktopX=desktopWidget->width();
    Widget::desktopY=desktopWidget->height();
    AllWidget allwidget;




    Widget w;
    QApplication::connect(&w,SIGNAL(saveWindowsInfo(QObject*)),&allwidget,SLOT(saveRectList(QObject*)));
    QApplication::connect(&w,SIGNAL(setAllWindow(QObject*,double,double)),&allwidget,SLOT(setAllCorrectLocal(QObject*,double,double)));
    QApplication::connect(desktopWidget,SIGNAL(resized(int)),&w,SLOT(resized(int)));
    w.show();

    return a.exec();
}
