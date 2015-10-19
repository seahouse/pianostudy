#include "autofirstdialog.h"
#include "ui_autofirstdialog.h"
#include<QTimer>
#include<QUrl>
#include<QPainter>
#include<QNetworkRequest>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QDebug>
#include<QFile>
#include <QProgressBar>
#include <QMessageBox>
#include <QDir>

AutoFirstDialog::AutoFirstDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AutoFirstDialog),
    _NetworkReplyErro(false)
{
    ui->setupUi(this);

}

AutoFirstDialog::~AutoFirstDialog()
{
    delete ui;
}


AutoFirstDialog::AutoFirstDialog(AUTO_FIRST autofirst, QWidget *parent):
    QDialog(parent),
    ui(new Ui::AutoFirstDialog),autofirst(autofirst),pixmap(NULL),
    _NetworkReplyErro(false)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(close()));

    _progressBar = new QProgressBar(this);
    _progressBar->hide();

    player=new QMediaPlayer(this);
    player->setMedia(QUrl(autofirst.sURL));
    player->play();
    timer->start(5000);
}

AutoFirstDialog::AutoFirstDialog(CLASS_INFO classInfo, QWidget *parent):
    QDialog(parent),
    ui(new Ui::AutoFirstDialog),_classInfo(classInfo),pixmap(NULL),ways(1),autopic(false),autopicsss(false),
    _NetworkReplyErro(false)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);

    _progressBar = new QProgressBar(this);
    _progressBar->hide();

    player=new QMediaPlayer(this);
    connect(this,SIGNAL(rejected()),player,SLOT(stop()));
    player->setMedia(QUrl(_classInfo.sVideoUrl));
    player->play();
    this->hide();
    SetNetworkPic(_classInfo.sAudio_pic,"1");
}

AutoFirstDialog::AutoFirstDialog(MUSIC_INFO musicInfo, QWidget *parent):
    QDialog(parent),
    ui(new Ui::AutoFirstDialog),_musicInfo(musicInfo),pixmap(NULL),ways(2),autopic(false),autopicsss(false),
    _NetworkReplyErro(false)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);

    _progressBar = new QProgressBar(this);
    _progressBar->resize(200, 20);
    _progressBar->hide();

    player=new QMediaPlayer(this);
    connect(this,SIGNAL(rejected()),player,SLOT(stop()));

    setVideoFile(_musicInfo.sVideoUrl);
    setPlayVideo();

//    player->setMedia(QUrl(_musicInfo.sVideoUrl));
//    player->play();
    this->hide();
    SetNetworkPic(_musicInfo.sAudio_pic,"2");
}

void AutoFirstDialog::paintEvent(QPaintEvent *){
   QPainter painter(this);
   if(pixmap==NULL){
       return;
   }
   if(pixmap->isNull()){
       return;
   }
   if(autopic){

//       if(!autopicsss)
       {
           if(pixmap->width()<this->width()&&pixmap->height()<this->height())
           {
               painter.setBrush(QBrush(qRgb(255,251,240)));
               painter.drawRect(rect());
               painter.drawPixmap((this->width()-pixmap->width())/2,(this->height()-pixmap->height())/2,*pixmap);
           }
           else{
               painter.drawPixmap(0,0,pixmap->width(),pixmap->height(),*pixmap);
               if (!autopicsss)
               {
                   setMinimumWidth(pixmap->width());
                   setMinimumHeight(pixmap->height());
               }
               autopicsss=true;
            }
       }
   }
   else{
       painter.drawPixmap(rect(),*pixmap);
   }
}
bool AutoFirstDialog::getAutopicsss() const
{
    return autopicsss;
}

void AutoFirstDialog::setAutopicsss(bool value)
{
    autopicsss = value;
}


bool AutoFirstDialog::getAutopic() const
{
    return autopic;
}

void AutoFirstDialog::setAutopic(bool value)
{
    autopic = value;
}


void AutoFirstDialog::setWays(const QString &value)
{
    ways = value;
}


void AutoFirstDialog::SetNetworkPic(const QString &sUrl, QString ways)
{
    QString s =sUrl;
    QUrl url(s);
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    //请求结束并下载完成后，退出子事件循环
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环
    loop.exec();
    QByteArray jpegData = reply->readAll();

    pixmap=new QPixmap();
    pixmap->loadFromData(jpegData);
    if(pixmap->isNull()){
        if(ways=="2")
            pixmap->load(":/images/SyllabusMusicDlg/img.png");
        else
            pixmap->load(":/images/SyllabusBasicDlg/video.png");
        autopic=false;
    }else{
        autopic=true;
    }
    this->show();
//    player->play();
}

void AutoFirstDialog::setVideoFile(const QString& fileName)
{
    m_fileName = fileName;
}

void AutoFirstDialog::setPlayVideo()
{
    if (m_fileName.isEmpty())
    {
        QMessageBox::warning(NULL,"","视频地址为空");
    }
    else
    {

        if (_NetworkReplyErro)
        {
            return;
        }
        QUrl url(m_fileName);
        fileInfo= url.path();

        _strcachePath = QObject::tr("C:\\Videocache\\");
        QDir dir(_strcachePath);

        //如果缓存路径不存在，则创建目录
        if (!dir.exists())
        {
            dir.mkpath(_strcachePath);
        }

        //判断是否存在文件
        QString strfile = _strcachePath+fileInfo.fileName();
        QFileInfo videofileInfo(strfile);
        if(videofileInfo.exists())
        {
            //如果文件大小为0，重新下载
            QFile file(strfile);
            if (!file.open(QIODevice::ReadOnly)) return;
            int isize = file.size();
            if (isize==0)
            {
                enabelDownLoad();
//                startPlay();
            }
            else
            {
                startPlay();//直接播放
            }
        }
        else
        {
            enabelDownLoad();
        }
    }
}

void AutoFirstDialog::enabelDownLoad()
{
    //启动下载线程
    _downloadThread = new DownloadThread(this,m_fileName,_strcachePath);
    _downloadThread->start();

    _progressBar->setValue(0);
    _progressBar->show();
}

void AutoFirstDialog::startPlay()
{
    QUrl url(m_fileName);
    fileInfo= url.path();
    QString localFilename = _strcachePath+fileInfo.fileName();
    player->setMedia(QUrl::fromLocalFile(localFilename));
    player->play();
}

void AutoFirstDialog::onDownloadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if (!_NetworkReplyErro)
    {
        _progressBar->setMaximum(bytesTotal);
        _progressBar->setValue(bytesSent);
    }
}

void AutoFirstDialog::onStartPlay()
{
    if(_downloadThread->gethttpRequestAbort())
    {
        _downloadThread->setstop(true);
        _downloadThread->stop();
        _downloadThread->wait();

        _progressBar->hide();
        startPlay();
    }
}
