#include "widget.h"
#include <QApplication>
#include <QTextCodec>
#include <QSettings>
#include "NetWork.h"
#include"allwidget.h"
#include<QDesktopWidget>
#include <QUrl>
#include <QFile>
#include <QDir>

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

/// 日志记录, by seahouse, 2015-12-23
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QDir dir(".");
    if (!dir.exists("logs"))
        dir.mkdir("logs");
    QFile file("logs/" + QDate::currentDate().toString("yyyy-MM-dd") + ".txt");
    if (file.open(QIODevice::Append))
    {
        QTextStream out(&file);
        QByteArray localMsg = msg.toLocal8Bit();
        switch (type) {
        case QtDebugMsg:
            out << QString("Debug: %1 (%2:%3, %4)\n")
                   .arg(localMsg.constData())
                   .arg(context.file)
                   .arg(QString::number(context.line))
                   .arg(context.function);
//            fprintf(stdout, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            fprintf(stdout, "%s\n", localMsg.constData());
            fflush(stdout);
            break;
        case QtInfoMsg:
            out << QString("Info: %1 (%2:%3, %4)\r\n")
                   .arg(localMsg.constData())
                   .arg(context.file)
                   .arg(QString::number(context.line))
                   .arg(context.function);
//            fprintf(stdout, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            fprintf(stdout, "%s\n", localMsg.constData());
            fflush(stdout);
            break;
        case QtFatalMsg:
            out << QString("Fatal: %1 (%2:%3, %4)\n")
                   .arg(localMsg.constData())
                   .arg(context.file)
                   .arg(QString::number(context.line))
                   .arg(context.function);
            fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtSystemMsg:
            out << QString("Critical: %1 (%2:%3, %4)\n")
                   .arg(localMsg.constData())
                   .arg(context.file)
                   .arg(QString::number(context.line))
                   .arg(context.function);
            fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        default:
            break;
        }

        file.close();
    }

}

int main(int argc, char *argv[])
{
    ReadSettings();
    qInstallMessageHandler(myMessageOutput);

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
