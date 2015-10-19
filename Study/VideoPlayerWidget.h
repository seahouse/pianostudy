#ifndef VIDEOPLAYERWIDGET_H
#define VIDEOPLAYERWIDGET_H

#include <QWidget>
#include <QIcon>
#include <QProcess>
#include <QPushButton>
#include <QSlider>
#include <QTextEdit>
#include <QTimer>
#include <QLabel>
#include <QEvent>
#include <QDialog>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QIODevice>
#include <QFileInfo>
#include <QProgressBar>

#include <QThread>


//namespace Ui {
//class VideoPlayerWidget;
//}

class DownloadThread : public QThread
{
	     Q_OBJECT
	public:
        DownloadThread(QWidget* parentwidgte,QString strUrl,QString strcachePath);
		 void stop() {stopped = true;}
		 
		 void setstop(bool isstop) {stopped = isstop;}
         bool gethttpRequestAbort() {return httpRequestAbort;}

	 protected:
		 void run(); 

		 void enabeldownload();
		 
	private:
		  bool stopped;
		  QString _strUrl;
          QString _strcachePath;

		  QProgressBar *_progressBar;
		  QWidget* _parentwidgte;
		  QFile *file;
		  QFileInfo fileInfo;
		  QNetworkAccessManager *accessManager;
		  QNetworkRequest request;
		  QNetworkReply *reply;

		  bool httpRequestAbort; //请求停止的标志位,false表示允许请求
 };

class VideoPlayerWidget : public QWidget
{
    Q_OBJECT

public:

	enum PlayState{Playing, Paused, Stopped};

    explicit VideoPlayerWidget(QWidget *parent = 0);
    ~VideoPlayerWidget();

	void closeEvent(QCloseEvent *event);

	void setVideoFile(const QString& fileName);

	void setPlayVideo();

private:
    void paintEvent(QPaintEvent *event);
private:
    //Ui::VideoPlayerWidget *ui;
	QWidget *videoWidget;
	//QPushButton *openButton;
	QPushButton *playButton;
	QIcon playIcon;
	QIcon pauseIcon;
	QPushButton *stopButton;
	QPushButton *slowSpeedButton;
	QPushButton *highSpeedButton;
	//QPushButton *frameStepButton;
	QPushButton *muteButton;
	QSlider *volumeSlider;
	QSlider *seekSlider;
	QProcess *process;
	QTimer *poller;
	QLabel *timeLabel;
    QLabel *downloadlabel;

	PlayState playState;
	QTextEdit *outEdit;
	float m_speed;
	bool m_bMute;
	bool m_bFrameStep;
	float m_maxTime;
	float m_curTime;
	QString m_fileName;
    QString _strcachePath;
	int m_volume;

	QString m_stotalTime;
	QString m_scurTime;

	void updateUi();
	QString convertToTime(int secs);

	void enabelDownLoad();


	QProgressBar *progressBar;
	DownloadThread* _pdownloadthread;
	QFile *file;
	QFileInfo fileInfo;
	QNetworkAccessManager *accessManager;
	QNetworkRequest request;
	QNetworkReply *reply;

	bool _NetworkReplyErro;
	bool httpRequestAbort; //请求停止的标志位,false表示允许请求

	//视频下载
	public slots:
		void httpDownload();
		void replyFinished(QNetworkReply*reply);
		void onDownloadProgress(qint64 bytesSent,qint64 bytesTotal);
		void onReadyRead();//
		void stop();//暂停，重新下载
        void onStartPlay();
		void getNetworkError(QNetworkReply::NetworkError error);

	private slots:
		void openFile();
		void startPlay();
		void stopPlay();
		void playOrPause();
		void slowSpeed();
		void highSpeed();
		void frameStep();
		void mute();
		void setVolume(int volume);
		void seekSliderChanged(int pos);
		void pollCurrentTime();
		void mplayerEnded(int exitCode, QProcess::ExitStatus exitStatus);
		void messageProcessing();
};

#endif // VIDEOPLAYERWIDGET_H
