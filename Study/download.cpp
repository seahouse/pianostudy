#include "download.h"
#include <QtDebug>

Dialog::Dialog(QWidget *parent)
	: QDialog(parent)
{
	download_Btn=new QPushButton("download");
	stop_Btn=new QPushButton("stop");
	progressBar=new QProgressBar;
	progressBar->hide();

	layout=new QVBoxLayout();
	layout->addWidget(progressBar);
	layout->addWidget(download_Btn);
	layout->addWidget(stop_Btn);

	setLayout(layout);
	httpRequestAbort=false;
	connect(download_Btn,SIGNAL(clicked()),this,SLOT(httpDownload()));
	connect(stop_Btn,SIGNAL(clicked()),this,SLOT(stop()));

}

Dialog::~Dialog()
{

}
void Dialog::httpDownload(){

	//QUrl url("<a target=_blank href="http://mm.newhua.com/down/Samsung_USB_Drivers_for_Mobile_Phones_1.5.9.0.zip">http://mm.newhua.com/down/Samsung_USB_Drivers_for_Mobile_Phones_1.5.9.0.zip</a>");
	QUrl url("http://120.55.119.93/course/uploads/videos/1441192925.mp4");

	fileInfo= url.path();
	file=new QFile(fileInfo.fileName());
	file->open(QIODevice::WriteOnly);//ֻ����ʽ���ļ�
	accessManager=new QNetworkAccessManager(this);
	request.setUrl(url);
	/******************����http��header***********************/
	// request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
	// request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
	// request.setRawHeader("Content-Disposition","form-data;name='doc';filename='a.txt'");
	//request.setHeader(QNetworkRequest::ContentLengthHeader,post_data.length());



	reply=accessManager->get(request);//ͨ���������ݣ�����ֵ������replyָ����.
	connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//finishΪmanager�Դ����źţ�replyFinished���Զ����
	connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT( onDownloadProgress(qint64 ,qint64 )));//download�ļ�ʱ����
	connect((QObject *)reply, SIGNAL(readyRead()),this, SLOT(onReadyRead()));
	progressBar->setValue(0);
	progressBar->show();




}
/***************��Ӧ����**************************/
void Dialog::replyFinished(QNetworkReply *reply){
	//��ȡ��Ӧ����Ϣ��״̬��Ϊ200��ʾ����
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
	QString result(bytes);  //ת��Ϊ�ַ���
	qDebug()<<result;

	//�޴��󷵻�
	if(reply->error() == QNetworkReply::NoError)
	{
		download_Btn->setText("success");
		QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
		QString result(bytes);  //ת��Ϊ�ַ���

		qDebug()<<result;
	}
	else
	{
		//�������
		download_Btn->setText("failed");
	}

	reply->deleteLater();//Ҫɾ��reply�����ǲ�����repyfinished��ֱ��delete��Ҫ����deletelater;

}

/***********���½�����*************/
void Dialog::onDownloadProgress(qint64 bytesSent, qint64 bytesTotal){
	progressBar->setMaximum(bytesTotal);
	progressBar->setValue(bytesSent);

}

void Dialog::onReadyRead(){

	file->write(reply->readAll());
	download_Btn->setText("downloading");
}

void Dialog::stop(){
	if(!httpRequestAbort)
	{
		reply->abort();//��Ӧ��ֹ
		httpRequestAbort=true;
		stop_Btn->setText("start");
	}
	else
	{   httpDownload();
	httpRequestAbort=false;
	stop_Btn->setText("stop");
	}


}
