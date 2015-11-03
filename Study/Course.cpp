#include "Course.h"
#include "ui_Course.h"
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QUrl>
#include <QEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QBitmap>
#include "widget.h"
#include "CustomEvent.h"
#include "SyllabusBasicDlg.h"
#include "SyllabusMusicDlg.h"
#include<QTime>
#include"autofirstdialog.h"
#include<QDesktopWidget>
#include"allwidget.h"

extern QString g_sUrlHead;


Course::Course(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Course),
    _locked(true)
{
    ui->setupUi(this);
    this->setCursor((QCursor(Qt::PointingHandCursor)));

    _sCourseStyleSheet = "QWidget{border-image: url(:/images/%1.png);}";

    QString sHeadStyleSheet = "QLabel{background-color:transparent;border-image: non/*url(:/images/head1.png)*/;}";
    QString sNumStyleSheet = "QLabel{font-weight:500;font-family: 微软雅黑;font-size:14px;color:red;border-image:url(:/images/kechengNum.png);}";
    QString sNameStyleSheet = "QLabel{font-family: 微软雅黑;font-size:15px;color:rgb(163,76,0);background-color:transparent;border-image:non;}";
    _sSynopsisStyleSheet = "QLabel{font-family: 宋体;font-size:%1px;color:rgb(163,76,0);background-color:transparent;border-image:non;}";
    QString sScoringStyleSheet = "QLabel{font-weight:400;font-family: 微软雅黑;font-size:14px;color:black;background-color:transparent;border-image:non;}";
    _sScoringNumStyleSheet = "QLabel{font-family: 微软雅黑;font-size:30px;color:red;background-color:transparent;border-image:%1;}";
    QString sEvaluatingStyleSheet = "QLabel{font-family: 微软雅黑;font-size:12px;color:white;background-color:transparent;border-image:non;}";
    QString sEvaluatingNumStyleSheet = "QLabel{font-family: 微软雅黑;font-size:14px;color:white;border-image:url(:/images/pinglunNum.png);}";
    _sKeGenDanStyleSheet = "QLabel{border-image: url(:/images/kegendan_%1.png);}";
    SetSynopsis(15);

    ui->_pHeadLab->setStyleSheet(sHeadStyleSheet);
    ui->_pNumLab->setStyleSheet(sNumStyleSheet);
    ui->_pNameLab->setStyleSheet(sNameStyleSheet);
    ui->_pScoringLab->setStyleSheet(sScoringStyleSheet);
    ui->_pScoringNumLab->setStyleSheet(_sScoringNumStyleSheet.arg("non"));
    ui->_pEvaluatingLab->setStyleSheet(sEvaluatingStyleSheet);
    ui->_pEvaluatingNumLab->setStyleSheet(sEvaluatingNumStyleSheet);
    ui->_pKeGenDanLab->setStyleSheet(_sKeGenDanStyleSheet.arg("1"));

    InitUI();
}

Course::~Course()
{
    delete ui;
}

void Course::SetScoringClok(QString sArg)
{
    ui->_pScoringLab->setText("");
    ui->_pScoringNumLab->setGeometry(349, 22, 58,57);
    ui->_pScoringNumLab->setStyleSheet(_sScoringNumStyleSheet.arg(sArg));
    ui->_pScoringNumLab->setText("");
    _locked = true;
}

void Course::SetScoringUnLock(QString sArg, const QString &score)
{
    ui->_pScoringLab->setText("我的分数");
    ui->_pScoringNumLab->setGeometry(362, 46, 33, 24);
    ui->_pScoringNumLab->setStyleSheet(_sScoringNumStyleSheet.arg(sArg));
    ui->_pScoringNumLab->setText(score);
    _locked = false;

    // 将分数记录到课程结构体
    if (score.toInt() > 0)
        _classInfo.iScore = score.toInt();
}

void Course::paintEvent(QPaintEvent *event)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter ppp(this);
     this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &ppp, this);
}

void Course::SetCurricula(QString sImage)
{
    this->setStyleSheet(_sCourseStyleSheet.arg(sImage));
}

void Course::SetSynopsis(int nImage)
{
    ui->_pSynopsisLab->setStyleSheet(_sSynopsisStyleSheet.arg(nImage));
}

void Course::HideKeGenDanLab()
{
    ui->_pKeGenDanLab->hide();
}

void Course::customEvent(QEvent * event)
{
   if(event->type() == UPDATE_COURSE_SCORE)
   {
        CustomEvent *e = static_cast<CustomEvent*>(event);
        if( e->_sCourseId == _classInfo.sCourseId )
        {
            ui->_pScoringNumLab->setText(QString::number(e->_nScore));
            /// 根据分数确定是否开放下一课程
            if (e->_nScore >= 90)
                emit unlockNext();
        }
        e->accept();
   }
   else
   {
       QObject::event(event);
   }
}

void Course::SetJCKC()
{
    ui->_pNumLab->show();
    ui->_pNameLab->show();
    ui->_pScoringLab->show();
    ui->_pScoringNumLab->show();
    ui->_pEvaluatingLab->show();
    ui->_pEvaluatingNumLab->show();
    ui->_pKeGenDanLab->hide();
    this->setStyleSheet(_sCourseStyleSheet.arg("classBg1"));
    ui->_pSynopsisLab->setGeometry(119, 47, 170, 58);
}

void Course::SetQMXS()
{
    ui->_pNumLab->hide();
    ui->_pNameLab->hide();
    ui->_pScoringLab->hide();
    ui->_pScoringNumLab->hide();
    ui->_pEvaluatingLab->hide();
    ui->_pEvaluatingNumLab->hide();
    ui->_pKeGenDanLab->show();
    this->setStyleSheet(_sCourseStyleSheet.arg("classBg2"));
    ui->_pSynopsisLab->setGeometry(123, 43, 161, 33);
}

void Course::InitUI()
{
    ui->_pNumLab->setText("");
    ui->_pNameLab->setText("");
    ui->_pSynopsisLab->setText("");
    ui->_pKeGenDanLab->hide();
}

void Course::UpdateJCKC_UI(const CLASS_INFO &ci)
{
    ui->_pNumLab->setText(ci.sCourseName);
    ui->_pNameLab->setText(ci.sTitle);
    ui->_pSynopsisLab->setText(ci.sNote);
    _classInfo = ci;
    if( ci.sCourseId != "12" )
    {
        SetScoringClok("url(:/images/lock.png)");
    }
    else
    {
        SetScoringUnLock("non");
    }

    SetNetworkPic(ci.sPicUrl);
}

void Course::UpdateQMXS_UI(const MUSIC_INFO &mi)
{
    ui->_pSynopsisLab->setText(mi.sTitle);
    ui->_pKeGenDanLab->setStyleSheet(_sKeGenDanStyleSheet.arg(mi.sIsFollow));
    ui->_pKeGenDanLab->show();
    _musicInfo = mi;
    SetNetworkPic(mi.sPicUrl);
}

void Course::mousePressEvent(QMouseEvent *event)
{
    if( this->parent()->objectName() == "基础课程" )
    {
        // 如果没有上锁，才能打开学习
        if (!_locked)
        {
            SyllabusBasicDlg sbd( _classInfo, (Widget*)this->parent()->parent(), this );
            AllWidget *alwidget=new AllWidget;
            connect(&sbd,SIGNAL(setAllWindow(QObject*,double,double)),alwidget,SLOT(setAllCorrectLocalTHREE(QObject*,double,double)));
            connect(QApplication::desktop(),SIGNAL(resized(int)),&sbd,SLOT(resized(int)));
            sbd.exec();
        }
    }
    else if( this->parent()->objectName() == "曲目欣赏" )
    {
        SyllabusMusicDlg smd(_musicInfo);
        AllWidget *awidget=new AllWidget;
        connect(&smd,SIGNAL(setAllWindow(QObject*,double,double)),awidget,SLOT(setAllCorrectLocalTWO(QObject*,double,double)));
        connect(QApplication::desktop(),SIGNAL(resized(int)),&smd,SLOT(resized(int)));
        smd.exec();
    }
}

void Course::SetNetworkPic(const QString &sUrl)
{
    QString s = g_sUrlHead+sUrl;
    QUrl url(s);
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    //请求结束并下载完成后，退出子事件循环
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环
    loop.exec();
    QByteArray jpegData = reply->readAll();
    QPixmap pixmap;
    pixmap.loadFromData(jpegData);
    ui->_pHeadLab->setPixmap(PixmapToRound(pixmap, 45));
}

//一个圆角半径等于边长一半的等边矩形
QPixmap Course::PixmapToRound(const QPixmap &src, int radius)
{
   if (src.isNull()) {
        return QPixmap();
    }

    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);

    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}

//修改
void Course::setAutoFirst(const AUTO_FIRST autofirst){
    _autofirst=autofirst;
}

QString Course::getCourseId() const
{
    return _classInfo.sCourseId;
}

int Course::score() const
{
    return ui->_pScoringNumLab->text().toInt();
}
