#ifndef DIALOG_H
#define DIALOG_H

#include <QtWidgets/QDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QFile>
#include <QFile>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QIODevice>
#include <QFileInfo>
#include <QtWidgets/QProgressBar>

/********************HTTP上传数据******************************/
/*************************************************************/
/*************************************************************/
/*************************************************************/
class Dialog : public QDialog
{
	Q_OBJECT

public:
	Dialog(QWidget *parent = 0);
	~Dialog();
	protected slots:
		void httpDownload();
		void replyFinished(QNetworkReply*reply);
		void onDownloadProgress(qint64 bytesSent,qint64 bytesTotal);
		void onReadyRead();//
		void stop();//暂停，重新下载


private:
	QPushButton *download_Btn;
	QPushButton *stop_Btn;
	QProgressBar *progressBar;
	QFile *file;
	QFileInfo fileInfo;
	QNetworkAccessManager *accessManager;
	QNetworkRequest request;
	QNetworkReply *reply;
	QVBoxLayout *layout;
	bool httpRequestAbort; //请求停止的标志位,false表示允许请求



};

#endif // DIALOG_H
