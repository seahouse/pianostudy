#include "mytextbrower.h"

#include <QFile>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QDebug>

MyTextBrower::MyTextBrower(QWidget * parent) :
    QTextBrowser(parent)
{
    _pixmap = new QPixmap;

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MyTextBrower::~MyTextBrower()
{
    if (_pixmap)
        delete _pixmap;
}

QVariant MyTextBrower::loadResource(int type, const QUrl &name)
{
    if (type == QTextDocument::ImageResource)
    {
//        static int i = 0;
//        if (i == 0)
        {
            QNetworkAccessManager manager;
            QEventLoop loop;
            QNetworkReply *reply = manager.get(QNetworkRequest(name));
            //请求结束并下载完成后，退出子事件循环
            qDebug() << "111";
            QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            //开启子事件循环
            loop.exec();
            qDebug() << "222";
            QByteArray jpegData = reply->readAll();
//            QPixmap pixmap;
//            i++;
            if (_pixmap->loadFromData(jpegData))
            {
                qDebug() << "true";
                return *_pixmap;
            }
        }

    }
    return QTextBrowser::loadResource(type, name);
}


void MyTextBrower::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << "mouseReleaseEvent";
    emit mouseReleased();
    QTextBrowser::mouseReleaseEvent(ev);
}

void MyTextBrower::appendTextPlainText(const QString &text)
{
    QTextCursor textCursor = this->textCursor();
    if (!textCursor.atEnd())
    {
        textCursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        setTextCursor(textCursor);
    }
    insertPlainText(text);
}
