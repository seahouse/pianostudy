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

/********************HTTP�ϴ�����******************************/
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
		void stop();//��ͣ����������


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
	bool httpRequestAbort; //����ֹͣ�ı�־λ,false��ʾ��������



};

#endif // DIALOG_H
