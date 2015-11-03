#include "SyllabusBasicDlg.h"
#include "ui_SyllabusBasicDlg.h"
#include "PraxisDlg.h"
#include <QPalette>
#include <QPainter>
#include <NetWork.h>
#include <QBitmap>
#include <QFile>
#include <QMessageBox>
#include <QTextCodec>
#include<QMouseEvent>
#include<QDesktopWidget>
#include"allwidget.h"
#include <fstream>
#include"autofirstdialog.h"
#include "keyknowledge.h"

double SyllabusBasicDlg::previousWidth=0.0;
double SyllabusBasicDlg::previousHeight=0.0;
int SyllabusBasicDlg::desktopX=0;
int SyllabusBasicDlg::desktopY=0;

SyllabusBasicDlg::SyllabusBasicDlg(CLASS_INFO classInfo, QWidget *parent, QObject *pCourseObj) :
    QDialog(parent),
    ui(new Ui::SyllabusBasicDlg),_bIsExtend(true),pd(NULL)

{

    //this->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    _classInfo = classInfo;
    _pCourseObj = pCourseObj;

    //修改
    mark=false;
    isLeftPressDown = false;
    this->dir = NONE;
    this->setMouseTracking(true);
    //

    QFile file(QString("green.css"));
    file.open(QFile::ReadOnly);
    QString qss = QLatin1String(file.readAll());
    ui->_pScrollArea->setStyleSheet(qss);

    //QPalette pal = palette();
    //pal.setBrush(QPalette::Background, QBrush(QPixmap(":/images/SyllabusBasicDlg/bg.png")));
    //this->setPalette(pal);

    //this->setStyleSheet("QDialog{border-image:url(:/images/SyllabusBasicDlg/bg.png);}");
    ui->_pBtn1->setStyleSheet("QPushButton{border-image:url(:/images/SyllabusBasicDlg/button1.png);}");
    ui->_pBtn2->setStyleSheet("QPushButton{border-image:url(:/images/SyllabusBasicDlg/button2.png);}");
    ui->_pCloseBtn->setStyleSheet("QPushButton{border-image:url(:/images/SyllabusBasicDlg/close.png);}");
    ui->_pSubmitBtn->setStyleSheet("QPushButton{border-image:url(:/images/SyllabusBasicDlg/submit.png);}");
    ui->_pCriticismLab->setStyleSheet("QLabel{border-image:url(:/images/SyllabusBasicDlg/commentIcon.png);}");
    ui->_pTextEditWidget->setStyleSheet("QWidget{border-image:url(:/images/SyllabusBasicDlg/textarea.png);}");
    ui->_pTextEdit->setStyleSheet("QTextEdit{border-image: non;}");

    ui->_pTableWidget->setStyleSheet("QTableWidget{background-color: rgb(203,241,200);}");
    ui->_pTableWidget->setColumnCount(1);
    ui->_pTableWidget->horizontalHeader()->hide();
    ui->_pTableWidget->verticalHeader()->hide();
    ui->_pTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->_pTableWidget->setFocusPolicy(Qt::NoFocus);
    ui->_pTableWidget->horizontalHeader()->setStretchLastSection(true); //自适应宽度

//    _pWebviewhtml = new QWebView(this);
//    _pWebviewhtml->setWindowFlags(Qt::Dialog);
//    _pWebviewhtml->setWindowTitle("重要知识点");
//    _pWebviewhtml->setStyleSheet("QWebView{font-size:25px; background-color: rgb(203,241,202);}");

    _keyKnowledge = new KeyKnowledge(this);
    _keyKnowledge->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    QTextCodec *codec = QTextCodec::codecForName("Unicode");
    QTextCodec::setCodecForLocale(codec);
    creatHTML(classInfo.sContent);

//  QString strcontent;
    QString strcontenthtml;
    strcontenthtml  += "<!DOCTYPE HTML>\n";
    strcontenthtml  += "<html>\n";
    strcontenthtml  += "<head>\n";
    strcontenthtml  += "<style type=\"text/css\">\n";
    strcontenthtml  += "::-webkit-scrollbar {width: 10px; height: 10px;}\n";
    strcontenthtml  += "::-webkit-scrollbar-thumb { background:green; border-radius:15px;}\n";
    strcontenthtml  += "</style>\n";
    strcontenthtml  += "</head>\n";
    strcontenthtml  += "<body>\n";
    strcontenthtml  += classInfo.sContent ;
    strcontenthtml  += "\n";
    strcontenthtml  += "</body>\n";
    strcontenthtml  += "</html>";
//    _pWebviewhtml->setHtml(strcontenthtml);
//    _pWebviewhtml->hide();

    _keyKnowledge->setHtml(strcontenthtml);

//      _pWebviewhtml->load(QString("file:///C:\\Videocache\\content.html"));
    //ui->_pcontentHtml->setHtml(classInfo.sContent);
    //ui->_pcontentHtml->setText(ui->_pZhiShiDianTextEdit->toPlainText());



    //创建视频播放器
    //_pVideoPlayerWidget = new VideoPlayerWidget();


    if(_classInfo.sCtype=="a"){
        AutoFirstDialog *dlg=new AutoFirstDialog(_classInfo);
        connect(this,SIGNAL(rejected()),dlg,SLOT(close()));
        ui->_pScrollArea->setWidget(dlg);
        return;
    }
    QRect widgetcontentrect = ui->scrollAreaWidgetContents->geometry();
    _pVideoPlayerWidget.setParent(ui->scrollAreaWidgetContents);
    _pVideoPlayerWidget.setGeometry(widgetcontentrect);
    _pVideoPlayerWidget.setVideoFile(_classInfo.sVideoUrl);
    _pVideoPlayerWidget.setPlayVideo();


  //  if(pd==NULL){
 /*      pd =new PraxisDlg(_classInfo.sCourseId, this, _pCourseObj);
        AllWidget *alllwidget=new AllWidget;
        connect(pd,SIGNAL(setAllWindow(QObject*,double,double)),alllwidget,SLOT(setAllCorrectLocalTWO(QObject*,double,double)));
        connect(QApplication::desktop(),SIGNAL(resized(int)),pd,SLOT(resized(int)));
        pd->hide();
        pd->Start();
        NetWork *pNetWork = NetWork::GetInstance();
        connect(pNetWork, SIGNAL(SendClassInfo(int,QVariant)), pd, SLOT(ReceiveStructInfo(int,QVariant)));
 //       return;
   // }
   */
     //   pd->exec();
}

SyllabusBasicDlg::~SyllabusBasicDlg()
{
    delete ui;
}

void SyllabusBasicDlg::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.drawPixmap(rect(),QPixmap(":/images/SyllabusBasicDlg/bg.png"));
//  p.fillRect(rect(), QBrush(QPixmap(":/images/SyllabusBasicDlg/bg.png")));


//    QBitmap bmp(this->size());
//    bmp.fill();
//    QPainter p(&bmp);
//    p.setRenderHint(QPainter::Antialiasing);

//    int arcR = 40;
//    QRect rect = this->rect();
//    QPainterPath path;
//    //逆时针
//    path.moveTo(arcR, 0);
//    path.arcTo(0, 0, arcR * 2, arcR * 2, 90.0f, 90.0f);
//    path.lineTo(0, rect.height() - arcR);
//    path.arcTo(0, rect.height() - 2 * arcR, arcR * 2, arcR * 2, 180.0f, 90.0f);
//    path.lineTo(rect.width() - arcR, rect.height());
//    path.arcTo(rect.width() - 2 * arcR, rect.height() - 2 * arcR, arcR * 2, arcR * 2, 270.0f, 90.0f);
//    path.lineTo(rect.width(), arcR);
//    path.arcTo(rect.width() - arcR * 2, 0, arcR * 2, arcR * 2, 0.0f, 90.0f);
//    path.lineTo(arcR, 0);
//    p.drawPath(path);
//    p.fillPath(path, QBrush(Qt::red)); //arm和windows平台没有这行代码将显示一个透明的空空的框
//    setMask(bmp);
}

void SyllabusBasicDlg::creatHTML( QString scontent)
{

//    QTextCodec *codec = QTextCodec::codecForName("unicode");
//    QTextCodec::setCodecForLocale(codec);
    QFile contenthtml(QString("C:\\Videocache\\content.html"));
    if (contenthtml.open(QFile::ReadWrite))
    {
        QTextStream htmlOutput(&contenthtml);
//        htmlOutput.setCodec("unicode");

        //写入新的内容到html文件中
        QString strcontenthtml;
        strcontenthtml  += "<!DOCTYPE HTML>\n";
        strcontenthtml  += "<html>\n";
        strcontenthtml  += "<body>\n";
        strcontenthtml  += scontent ;
        strcontenthtml  += "\n";
        strcontenthtml  += "</body>\n";
        strcontenthtml  += "</html>";

        htmlOutput << strcontenthtml << endl;
        contenthtml.close();
    }
}

void SyllabusBasicDlg::on__pCloseBtn_clicked()
{
    this->close();
}

void SyllabusBasicDlg::on__pSubmitBtn_clicked()
{
    QString sText = ui->_pTextEdit->toPlainText();
    if( sText.isEmpty() )
    {
        return;
    }
    ui->_pTextEdit->clear();
    ui->_pTableWidget->setRowCount(ui->_pTableWidget->rowCount()+1);

    //创建一行信息
    QWidget *pWidget = new QWidget;
    QLabel *pIconLab = new QLabel(pWidget);
    QLabel *pNameLab = new QLabel(pWidget);
    QLabel *pTextLab = new QLabel(pWidget);

    pWidget->setStyleSheet("QWidget{border-image:url(:/images/SyllabusBasicDlg/tabBg.png);}");
    pIconLab->setStyleSheet("QLabel{border-image:url(:/images/SyllabusBasicDlg/icon1.png);}");
    pNameLab->setStyleSheet("QLabel{font-size: 15px; color: rgb(20,111,5);}");
    pTextLab->setStyleSheet("QLabel{font-size: 13px;}");
    pWidget->setFixedSize(280, 79);
    pIconLab->setFixedSize(24, 24);
    pNameLab->setFixedHeight(17);
    pIconLab->move(11, 25);
    pNameLab->move(45, 29);
    pTextLab->move(13, 61);

    pNameLab->setText(tr("小李"));
    pTextLab->setText(sText);
    ui->_pTableWidget->setCellWidget(ui->_pTableWidget->rowCount()-1, 0, pWidget);
    ui->_pTableWidget->setRowHeight(ui->_pTableWidget->rowCount()-1, 79);
}

void SyllabusBasicDlg::on__pBtn1_clicked()
{
    if( _bIsExtend )
    {
//       _pWebviewhtml->show();
       _keyKnowledge->show();
    }
    else
    {
//       _pWebviewhtml->hide();
       _keyKnowledge->hide();
    }
    _bIsExtend = !_bIsExtend;
}

void SyllabusBasicDlg::on__pBtn2_clicked()
{
 /*   QString sUrl = "http://120.55.119.93/course/index.php?m=Api&c=Question&a=questionList&page=1&rows=7&course_id=%1";
    PraxisDlg pd(_classInfo.sCourseId, NULL, _pCourseObj);
    AllWidget *alllwidget=new AllWidget;
    connect(&pd,SIGNAL(setAllWindow(QObject*,double,double)),alllwidget,SLOT(setAllCorrectLocalTWO(QObject*,double,double)));
    connect(QApplication::desktop(),SIGNAL(resized(int)),&pd,SLOT(resized(int)));
    NetWork *pNetWork = NetWork::GetInstance();
    connect(pNetWork, SIGNAL(SendClassInfo(int,QVariant)), &pd, SLOT(ReceiveStructInfo(int,QVariant)));
    pd.Start();
    pd.exec();
    */
    //    QString sUrl = "http://120.55.119.93/course/index.php?m=Api&c=Question&a=questionList&page=1&rows=7&course_id=%1";
    if(pd==NULL){
        pd =new PraxisDlg(_classInfo.sCourseId, _classInfo.iScore, this, _pCourseObj);
        AllWidget *alllwidget=new AllWidget;
        connect(pd,SIGNAL(setAllWindow(QObject*,double,double)),alllwidget,SLOT(setAllCorrectLocalTWO(QObject*,double,double)));
        connect(QApplication::desktop(),SIGNAL(resized(int)),pd,SLOT(resized(int)));
        pd->hide();
        pd->Start();
        NetWork *pNetWork = NetWork::GetInstance();
        connect(pNetWork, SIGNAL(SendClassInfo(int,QVariant)), pd, SLOT(ReceiveStructInfo(int,QVariant)));
        return;
    }
    pd->show();
}









//修改
void SyllabusBasicDlg::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        isLeftPressDown = false;
        if(dir != NONE) {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

void SyllabusBasicDlg::mousePressEvent(QMouseEvent *event)
{
    switch(event->button()) {
    case Qt::LeftButton:
        isLeftPressDown = true;
        if(dir != NONE) {
            this->mouseGrabber();
        } else {
            dragPosition = event->globalPos() - this->frameGeometry().topLeft();
        }
        break;
    case Qt::RightButton:
        this->close();
        break;
    default:
        QWidget::mousePressEvent(event);
    }

}

void SyllabusBasicDlg::mouseMoveEvent(QMouseEvent *event)
{
    QPoint gloPoint = event->globalPos();
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    if(!isLeftPressDown) {
        this->region(gloPoint);
    } else {

        if(dir != NONE) {
            QRect rMove(tl, rb);

            switch(dir) {
            case LEFT:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                break;
            case RIGHT:
                rMove.setWidth(gloPoint.x() - tl.x());
                break;
            case UP:
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case DOWN:
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case LEFTTOP:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case RIGHTTOP:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setY(gloPoint.y());
                break;
            case LEFTBOTTOM:
                rMove.setX(gloPoint.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case RIGHTBOTTOM:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            default:
                break;
            }
            this->setGeometry(rMove);
        } else {
            move(event->globalPos() - dragPosition);
            event->accept();
        }
    }
    QWidget::mouseMoveEvent(event);
}

void SyllabusBasicDlg::region(const QPoint &cursorGlobalPoint)
{
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if(tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y) {
        // 左上角
        dir = LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
    } else if(x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        // 右下角
        dir = RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        //左下角
        dir = LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING) {
        // 右上角
        dir = RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x()) {
        // 左边
        dir = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if( x <= rb.x() && x >= rb.x() - PADDING) {
        // 右边
        dir = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }else if(y >= tl.y() && y <= tl.y() + PADDING){
        // 上边
        dir = UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else if(y <= rb.y() && y >= rb.y() - PADDING) {
        // 下边
        dir = DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }else {
        // 默认
        dir = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

void SyllabusBasicDlg::resizeEvent(QResizeEvent *event){
  if(!mark){
        previousWidth=this->width();
        previousHeight=this->height();
        mark=true;
        return;
    }
   if(this->width()==previousWidth&&this->height()==previousHeight){
       return;
   }
   double scaleX=double(this->width())/double(previousWidth);
   double scaleY=double(this->height())/double(previousHeight);
   emit setAllWindow(this,scaleX,scaleY);
   previousWidth=this->width();
   previousHeight=this->height();
 /*   if(this->width()==this->minimumWidth()&&this->height()==this->minimumHeight()){
        return;
    }
    double scaleX=double(this->width())/double(this->minimumWidth());
    double scaleY=double(this->height())/double(this->minimumHeight());
    emit saveWindowsInfo(this);
    emit setAllWindow(this,scaleX,scaleY);
    */

}


void SyllabusBasicDlg::resized(int){
    int newwidth=QApplication::desktop()->width();
    int newheight=QApplication::desktop()->height();
    this->resize(int(this->width()*newwidth/desktopX),
                 int(this->height()*newheight/desktopY));
    desktopX=newwidth;
    desktopY=newheight;
}


bool SyllabusBasicDlg::getMark() const
{
    return mark;
}


void SyllabusBasicDlg::setMark(bool value)
{
    mark = value;
}


