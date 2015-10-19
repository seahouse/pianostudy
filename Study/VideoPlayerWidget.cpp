#include "VideoPlayerWidget.h"
#include "ui_VideoPlayerWidget.h"
#include <QStyleOption>
#include <QPainter>
#include <QWebView>
#include <QUrl>
#include <QWebPage>
#include <QWebFrame>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>

DownloadThread::DownloadThread(QWidget* parentwidgte,QString strUrl,QString strcachePath)
	:_parentwidgte(parentwidgte)
	,_strUrl(strUrl)
    ,_strcachePath(strcachePath)
{
	stopped = false;
    httpRequestAbort = true;
}
void DownloadThread::run()
{
	if(!stopped)
	{
		//
		enabeldownload();
	}
	stopped = false;
}

void DownloadThread::enabeldownload()
{
	QNetworkAccessManager manager;
	QEventLoop loop;
	QNetworkReply *reply = manager.get(QNetworkRequest(_strUrl));
	//请求结束并下载完成后，退出子事件循环
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), _parentwidgte, SLOT(getNetworkError(QNetworkReply::NetworkError)));
    //QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), _parentwidgte, SLOT(replyFinished(QNetworkReply*)));//finish为manager自带的信号，replyFinished是自定义的
    QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)), _parentwidgte, SLOT( onDownloadProgress(qint64 ,qint64 )));//download文件时进度
    QObject::connect(reply, SIGNAL(finished()), _parentwidgte, SLOT(onStartPlay()));
    //QObject::connect(reply, SIGNAL(readyRead()),_parentwidgte, SLOT(onReadyRead()));
	loop.exec();
    QUrl url(_strUrl);
    fileInfo= url.path();
    file=new QFile(_strcachePath+fileInfo.fileName());
    file->open(QIODevice::WriteOnly);//只读方式打开文件
    file->write(reply->readAll());
    file->close();
    httpRequestAbort = false;
}

//////////////////////////////////////////////////////////////////////////
VideoPlayerWidget::VideoPlayerWidget(QWidget *parent  ) :
    QWidget(parent)
   // ui(new Ui::VideoPlayerWidget)
{
    //ui->setupUi(this);
   // this->setStyleSheet("QWidget{border-image:url(:/images/SyllabusBasicDlg/video.png);}");
   /* QWebView *webview = new QWebView(this);
    //webview->setFixedSize(600, 400);
    QString url = "http://120.55.119.93/course/uploads/videos/1441192925.mp4";
    webview->load(QUrl::fromUserInput("F:\\QtProject\\Study\\ckplayer6.7\\demo2.htm"));
    webview->setGeometry(0, 0, 785, 475);


     QPalette pal = webview->page()->palette();
     pal.setBrush(QPalette::Base, Qt::transparent);
     webview->setPalette(pal);


    webview->setContextMenuPolicy(Qt::NoContextMenu);
    webview->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    webview->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

    webview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    //设置Webview的选项

    QWebSettings::globalSettings()->setThirdPartyCookiePolicy(QWebSettings::AlwaysAllowThirdPartyCookies);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavaEnabled,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanOpenWindows,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::SpatialNavigationEnabled,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LinksIncludedInFocusChain,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::AcceleratedCompositingEnabled,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages,true);
*/

	setWindowTitle("Qt MPlayer");
	m_speed = 1;
	m_bMute = 0;
	m_bFrameStep = 0;

	videoWidget = new QWidget(NULL);
    videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//videoWidget->setStyleSheet("background-color:#999;");
	//videoWidget->setAttribute(Qt::WA_OpaquePaintEvent);

	QSize buttonSize(34, 28);

	//openButton = new QPushButton(this);
	//openButton->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));

	playButton = new QPushButton(this);
	playIcon = style()->standardIcon(QStyle::SP_MediaPlay);
	pauseIcon = style()->standardIcon(QStyle::SP_MediaPause);
	playButton->setIcon(playIcon);

	stopButton = new QPushButton(this);
	stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

	slowSpeedButton = new QPushButton(this);
	slowSpeedButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));

	highSpeedButton = new QPushButton(this);
	highSpeedButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));

	//frameStepButton = new QPushButton(this);
	//frameStepButton->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));

	muteButton = new QPushButton(this);
	muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

	progressBar=new QProgressBar;
	progressBar->hide();

    downloadlabel = new QLabel;
    downloadlabel->hide();

	//openButton->setMinimumSize(buttonSize);
	playButton->setMinimumSize(buttonSize);
	stopButton->setMinimumSize(buttonSize);
	slowSpeedButton->setMinimumSize(buttonSize);
	highSpeedButton->setMinimumSize(buttonSize);
	//frameStepButton->setMinimumSize(buttonSize);
	muteButton->setMinimumSize(buttonSize);

	//openButton->setToolTip(tr("打开"));
	playButton->setToolTip(tr("暂停/播放"));
	stopButton->setToolTip(tr("停止"));
	slowSpeedButton->setToolTip(tr("减慢"));
	highSpeedButton->setToolTip(tr("快进"));
	//frameStepButton->setToolTip(tr("单帧播放"));
	muteButton->setToolTip(tr("静音"));

	volumeSlider = new QSlider(Qt::Horizontal);
	volumeSlider->setFixedWidth(100);
	volumeSlider->setRange(0,50);
	volumeSlider->setPageStep(2);
	m_volume = 50;
	volumeSlider->setValue(m_volume);

	seekSlider = new QSlider(Qt::Horizontal);
	//seekSlider->setRange(0,100);
	seekSlider->setPageStep(1);

	timeLabel = new QLabel(this);
	timeLabel->setMinimumWidth(60);
	QHBoxLayout *timeLayout = new QHBoxLayout;
	timeLayout->addWidget(seekSlider);
	timeLayout->addWidget(timeLabel);

	//outEdit = new QTextEdit(this);
	//outEdit->setReadOnly(true);

	playButton->setEnabled(true);
	stopButton->setEnabled(true);
	slowSpeedButton->setEnabled(true);
	highSpeedButton->setEnabled(true);
	//frameStepButton->setEnabled(true);
	muteButton->setEnabled(true);
	volumeSlider->setEnabled(true);

	QHBoxLayout *btnsLayout = new QHBoxLayout;
	//btnsLayout->addWidget(openButton);
	btnsLayout->addWidget(playButton);
	btnsLayout->addWidget(stopButton);
	btnsLayout->addWidget(slowSpeedButton);
	btnsLayout->addWidget(highSpeedButton);
	//btnsLayout->addWidget(frameStepButton);
	btnsLayout->addWidget(progressBar);
    btnsLayout->addWidget(downloadlabel);
	btnsLayout->addStretch();
	btnsLayout->addWidget(muteButton);
	btnsLayout->addWidget(volumeSlider);

	poller = new QTimer(this);

	process = new QProcess;	
	process->setProcessChannelMode(QProcess::MergedChannels);
	connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(messageProcessing()));
	//当进程退出时会发出这个信号，包括了退出时的退出码和退出状态
	connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
		this, SLOT(mplayerEnded(int, QProcess::ExitStatus))); 
	//每隔1S中定时器poller就会发出timeout信号，在响应函数中我们继续给Mplayer发送获取视频当前时间的命令get_time_pos
	connect(poller, SIGNAL(timeout()), this, SLOT(pollCurrentTime()));
	//当我们拖动滑动条时，视频也应该跟着播放
	connect(seekSlider, SIGNAL(sliderMoved(int)), this, SLOT(seekSliderChanged(int)));

	//connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));
	connect(playButton, SIGNAL(clicked()), this, SLOT(playOrPause()));
	connect(stopButton, SIGNAL(clicked()), this, SLOT(stopPlay()));
	connect(slowSpeedButton, SIGNAL(clicked()), this, SLOT(slowSpeed()));
	connect(highSpeedButton, SIGNAL(clicked()), this, SLOT(highSpeed()));
	//connect(frameStepButton, SIGNAL(clicked()), this, SLOT(frameStep()));
	connect(muteButton, SIGNAL(clicked()), this, SLOT(mute()));
	connect(volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(videoWidget);
	mainLayout->addLayout(timeLayout);
	mainLayout->addLayout(btnsLayout);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addLayout(mainLayout);

	setLayout(layout);
	setMinimumSize(QSize(800, 400));

    QFile file(QString("green.css"));
    file.open(QFile::ReadOnly);
    QString qss = QLatin1String(file.readAll());
    this->setStyleSheet(qss);

	//httpRequestAbort=false;
	_NetworkReplyErro = false;
}

VideoPlayerWidget::~VideoPlayerWidget()
{
   // delete ui;
}

void VideoPlayerWidget::setVideoFile(const QString& fileName)
{
	m_fileName = fileName;
}

void VideoPlayerWidget::getNetworkError(QNetworkReply::NetworkError error)
{
//	QString str = tr("视频文件地址错误") ;
	_NetworkReplyErro = true;
//	QMessageBox::warning(NULL,"",str ,QMessageBox::Yes);
	progressBar->hide();
	downloadlabel->hide();
}

void VideoPlayerWidget::setPlayVideo()
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
				startPlay();
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



		//QNetworkAccessManager manager;
		//QEventLoop loop;
		//QNetworkReply *reply = manager.get(QNetworkRequest(m_fileName));
		////请求结束并下载完成后，退出子事件循环
		//QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

		////connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//finish为manager自带的信号，replyFinished是自定义的
		//connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT( onDownloadProgress(qint64 ,qint64 )));//download文件时进度
		////connect((QObject *)reply, SIGNAL(readyRead()),this, SLOT(onReadyRead()));

		//progressBar->setValue(0);
		//progressBar->show();

		////开启子事件循环
		//loop.exec();
		//QUrl url(m_fileName);
		//fileInfo= url.path();
		//file=new QFile(fileInfo.fileName());
		//file->open(QIODevice::WriteOnly);//只读方式打开文件
		//file->write(reply->readAll());

		//httpDownload();
	}
}

void VideoPlayerWidget::enabelDownLoad()
{
	//启动下载线程
	_pdownloadthread = new DownloadThread(this,m_fileName,_strcachePath);
	_pdownloadthread->start();

	progressBar->setValue(0);
	progressBar->show();
	downloadlabel->show();
	downloadlabel->setText( QObject::tr("正在下载文件") );
}

void VideoPlayerWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter ppp(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &ppp, this);
    QWidget::paintEvent(event);
}

void VideoPlayerWidget::openFile()
{

	//if (m_fileName.isEmpty())
	//	return ;

	//startPlay();
}

//////////////////////////////////////////////////////////////////////////
void VideoPlayerWidget::httpDownload()
{

	QUrl url(m_fileName);
	fileInfo= url.path();
	file=new QFile(fileInfo.fileName());
	file->open(QIODevice::WriteOnly);//只读方式打开文件

	QNetworkAccessManager manager;
	reply = manager.get(QNetworkRequest(m_fileName));
	////请求结束并下载完成后，退出子事件循环
	//QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	//accessManager=new QNetworkAccessManager(this);
	//request.setUrl(url);

	//reply=accessManager->get(request);//通过发送数据，返回值保存在reply指针里.
	connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//finish为manager自带的信号，replyFinished是自定义的
	connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT( onDownloadProgress(qint64 ,qint64 )));//download文件时进度
	connect((QObject *)reply, SIGNAL(readyRead()),this, SLOT(onReadyRead()));
	progressBar->setValue(0);
	progressBar->show();
	file->write(reply->readAll());

}

void VideoPlayerWidget::replyFinished(QNetworkReply *reply)
{
	//获取响应的信息，状态码为200表示正常
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	QByteArray bytes = reply->readAll();  //获取字节
	QString result(bytes);  //转化为字符串
	qDebug()<<result;

	if (reply->error() == QNetworkReply::NoError)
	{
		//QByteArray bytes = reply->readAll();
		//QString result(bytes);
        if(_pdownloadthread->gethttpRequestAbort())
        {
            _pdownloadthread->setstop(true);
            _pdownloadthread->stop();
            _pdownloadthread->wait();
        }
	}
	else
	{
		//download_Btn->setText("failed");
	}

	reply->deleteLater();//要删除reply，但是不能在repyfinished里直接delete，要调用deletelater;

}

void VideoPlayerWidget::onDownloadProgress(qint64 bytesSent, qint64 bytesTotal)
{
	if (!_NetworkReplyErro)
	{
		progressBar->setMaximum(bytesTotal);
		progressBar->setValue(bytesSent);
	}

}

void VideoPlayerWidget::onReadyRead()
{

	file->write(reply->readAll());
	//download_Btn->setText("downloading");
}

void VideoPlayerWidget::stop()
{
	//if(!httpRequestAbort)
	//{
	//	reply->abort();//响应终止
	//	httpRequestAbort=true;
	//	//stop_Btn->setText("start");
	//}
	//else
	//{   
	//	httpDownload();
	//httpRequestAbort=false;
	////stop_Btn->setText("stop");
	//}
}

void VideoPlayerWidget::onStartPlay()
{
    if(_pdownloadthread->gethttpRequestAbort())
    {
        _pdownloadthread->setstop(true);
        _pdownloadthread->stop();
        _pdownloadthread->wait();

        playState = Playing;
        startPlay();
        progressBar->hide();
        downloadlabel->hide();
    }
}

void VideoPlayerWidget::startPlay()
{
	//m_fileName = "http://120.55.119.93/course/uploads/videos/1441192925.mp4";
	// for windows7 vista
    QUrl url(m_fileName);
    fileInfo= url.path();
    QString common = "./mplayer/mplayer.exe -slave -quiet -vo direct3d -aspect 16:9 -zoom " + _strcachePath+fileInfo.fileName() + " -wid " + QString::number((ulong)videoWidget->winId()); //QString::number(reinterpret_cast<uint>(videoWidget->winId()));
	// for XP
	//QString common = "./mplayer/mplayer.exe -slave -quiet -vo directx:noaccel " + m_fileName + " -wid " + QString::number((ulong)videoWidget->winId()); 
	process->start(common);

	//m_volume = 50;
	//volumeSlider->setValue(m_volume);

	if(!process->waitForStarted(100))
	{
		qDebug("Error:Open MPlayer failed!\n");
		return;
	}

	//retrieve basic information
	//process->write("get_video_resolution\n");
	process->write("get_time_length\n");
	poller->start(1000);

	playState = Playing;
	updateUi();
}

void VideoPlayerWidget::frameStep()
{
	playState = Paused;
	process->write("get_time_pos\n");
	process->write("frame_step\n");
	m_bFrameStep = 1; // 单帧播放模式
	if (m_bFrameStep)
		poller->stop();
	updateUi();
}

void VideoPlayerWidget::playOrPause()
{
	if (playState == Playing){
		playState = Paused;
		if (poller->isActive())
			poller->stop();
	}
	else if (playState == Paused){
		playState = Playing;
		if (!poller->isActive())
			poller->start();
	}
	else if (playState == Stopped)
	{
		playState = Playing;
		startPlay();
		return ;
	}

	updateUi();
	process->write("pause\n");
}

void VideoPlayerWidget::slowSpeed()
{
	process->write("speed_mult 0.5\n");
}

void VideoPlayerWidget::highSpeed()
{
	process->write("speed_mult 2\n");
}

void VideoPlayerWidget::mute()
{
	if (m_bMute)
	{
		muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
		m_bMute = 0;
		process->write("mute 0\n");
	}
	else
	{
		muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
		m_bMute = 1;
		process->write("mute 1\n");
	}
}

void VideoPlayerWidget::setVolume(int volume)
{
	int gapVolume = volume - m_volume;
	m_volume = volume;
	process->write(QString("volume "+QString::number(gapVolume)+"\n").toUtf8());
}

void VideoPlayerWidget::pollCurrentTime()
{
	process->write("get_time_pos\n");
}

QString VideoPlayerWidget::convertToTime(int secs)
{
	int mins = secs / 60;
	int hours = mins / 60;

	QTime time(hours, mins%60, secs%60);

	QString timeFormat("mm:ss");
	if (hours)
	{
		timeFormat = "hh:mm:ss";
	}
	return time.toString(timeFormat);
}

void VideoPlayerWidget::messageProcessing()
{
	while (process->canReadLine())
	{
		QString buffer(process->readLine());
		//outEdit->append(buffer);
		if(buffer.startsWith("ANS_LENGTH"))
		{
			buffer.remove(0, 11); // vire ANS_LENGTH=
			m_maxTime = buffer.toFloat();

			m_stotalTime = convertToTime(static_cast<int>(m_maxTime));
			seekSlider->setMaximum(static_cast<int>(m_maxTime));
		}
		//response to get_time_pos:ANS_TIME_POSITION=xx.y
		else if(buffer.startsWith("ANS_TIME_POSITION"))
		{
			buffer.remove(0, 18); // vire ANS_TIME_POSITION=
			m_curTime = buffer.toFloat();
			seekSlider->setValue(static_cast<int>(m_curTime));
			if (m_curTime <= m_maxTime){
				m_scurTime = convertToTime(static_cast<int>(m_curTime));
				timeLabel->setText(m_scurTime+"/"+m_stotalTime);
			}
		}
	}
}

void VideoPlayerWidget::seekSliderChanged(int pos)
{
	process->write(QString("seek "+QString::number(pos)+" 2\n").toUtf8());
}

void VideoPlayerWidget::stopPlay()
{
	process->write("quit\n");
	if (process->waitForFinished(100))
	{
		qDebug("Error:Failed to waiting mplayer finish\n");
		return ;
	}
	playState = Stopped;
	updateUi();
}

void VideoPlayerWidget::mplayerEnded(int exitCode, QProcess::ExitStatus exitStatus)
{
	playState = Stopped;
	updateUi();
	poller->stop();
}

void VideoPlayerWidget::updateUi()
{
	if (playState == Playing)
	{
		playButton->setEnabled(true);
		playButton->setIcon(pauseIcon);
		stopButton->setEnabled(true);
		slowSpeedButton->setEnabled(true);
		highSpeedButton->setEnabled(true);
		//frameStepButton->setEnabled(true);
		muteButton->setEnabled(true);
		volumeSlider->setEnabled(true);
	}
	else if (playState == Paused)
	{
		playButton->setIcon(playIcon);
	}
	else if (playState == Stopped)
	{
		playButton->setEnabled(true);
		playButton->setIcon(playIcon);
		stopButton->setEnabled(false);
		slowSpeedButton->setEnabled(false);
		highSpeedButton->setEnabled(false);
		//frameStepButton->setEnabled(false);
		muteButton->setEnabled(false);
		volumeSlider->setEnabled(false);
	}
}

void VideoPlayerWidget::closeEvent(QCloseEvent *event)
{
	stopPlay();
	//event->accept();

}
