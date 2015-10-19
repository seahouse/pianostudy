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
	file->open(QIODevice::WriteOnly);//只读方式打开文件
	accessManager=new QNetworkAccessManager(this);
	request.setUrl(url);
	/******************设置http的header***********************/
	// request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
	// request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
	// request.setRawHeader("Content-Disposition","form-data;name='doc';filename='a.txt'");
	//request.setHeader(QNetworkRequest::ContentLengthHeader,post_data.length());



	reply=accessManager->get(request);//通过发送数据，返回值保存在reply指针里.
	connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//finish为manager自带的信号，replyFinished是自定义的
	connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT( onDownloadProgress(qint64 ,qint64 )));//download文件时进度
	connect((QObject *)reply, SIGNAL(readyRead()),this, SLOT(onReadyRead()));
	progressBar->setValue(0);
	progressBar->show();




}
/***************响应结束**************************/
void Dialog::replyFinished(QNetworkReply *reply){
	//获取响应的信息，状态码为200表示正常
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	QByteArray bytes = reply->readAll();  //获取字节
	QString result(bytes);  //转化为字符串
	qDebug()<<result;

	//无错误返回
	if(reply->error() == QNetworkReply::NoError)
	{
		download_Btn->setText("success");
		QByteArray bytes = reply->readAll();  //获取字节
		QString result(bytes);  //转化为字符串

		qDebug()<<result;
	}
	else
	{
		//处理错误
		download_Btn->setText("failed");
	}

	reply->deleteLater();//要删除reply，但是不能在repyfinished里直接delete，要调用deletelater;

}

/***********更新进度条*************/
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
		reply->abort();//响应终止
		httpRequestAbort=true;
		stop_Btn->setText("start");
	}
	else
	{   httpDownload();
	httpRequestAbort=false;
	stop_Btn->setText("stop");
	}


}
