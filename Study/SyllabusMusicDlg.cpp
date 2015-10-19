#include "SyllabusMusicDlg.h"
#include "ui_SyllabusMusicDlg.h"
#include <QPainter>
#include<QMouseEvent>
#include<QDesktopWidget>
#include"autofirstdialog.h"
#include<QStyle>



double SyllabusMusicDlg::previousWidth=0.0;
double SyllabusMusicDlg::previousHeight=0.0;
int SyllabusMusicDlg::desktopX=0;
int SyllabusMusicDlg::desktopY=0;

SyllabusMusicDlg::SyllabusMusicDlg(MUSIC_INFO musicInfo, QWidget *parent) :
    QDialog(parent),
    _musicInfo(musicInfo),
    ui(new Ui::SyllabusMusicDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
   // this->setAttribute(Qt::WA_TranslucentBackground, true);

 //   ui->_pWind->setStyle(QStyle::);
    //修改
    mark=false;
    isLeftPressDown = false;
    this->dir = NONE;
    this->setMouseTracking(true);

    ui->_pCloseBtn->setStyleSheet("QPushButton{border-image:url(:/images/SyllabusBasicDlg/close.png);}");
    ui->_pTitleBtn_1->setStyleSheet("QPushButton{border-image:url(:/images/SyllabusMusicDlg/nav1.png);}");
    ui->_pTitleBtn_2->setStyleSheet("QPushButton{border-image:url(:/images/SyllabusMusicDlg/nav2.png);}");
    ui->_pTitleBtn_3->setStyleSheet("QPushButton{border-image:url(:/images/SyllabusMusicDlg/nav3.png);}");
    ui->_pTitleBtn_4->setStyleSheet("QPushButton{border-image:url(:/images/SyllabusMusicDlg/nav4.png);}");

    if(_musicInfo.sCtype=="a"){
        AutoFirstDialog *dlg=new AutoFirstDialog(_musicInfo);
        connect(this,SIGNAL(rejected()),dlg,SLOT(close()));
        ui->_pWind->setWidget(dlg);

        QFile file(QString("green.css"));
        file.open(QFile::ReadOnly);
        QString qss = QLatin1String(file.readAll());
        ui->_pWind->setStyleSheet(qss);

        return;
    }
    _pVideoPlayerWidget.setParent(ui->scrollAreaWidgetContents);
    _pVideoPlayerWidget.setGeometry(ui->scrollAreaWidgetContents->geometry());
    _pVideoPlayerWidget.setVideoFile(_musicInfo.sVideoUrl);
    _pVideoPlayerWidget.setPlayVideo();

}

SyllabusMusicDlg::~SyllabusMusicDlg()
{
    delete ui;
}

void SyllabusMusicDlg::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
 //   p.fillRect(rect(), QBrush(qRgb(0,255,64)));
    p.setBrush(QBrush(qRgb(8,87,42)));
    p.drawRect(rect());
}

void SyllabusMusicDlg::on__pCloseBtn_clicked()
{
    this->close();
}



//修改
void SyllabusMusicDlg::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        isLeftPressDown = false;
        if(dir != NONE) {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}


void SyllabusMusicDlg::mousePressEvent(QMouseEvent *event)
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

void SyllabusMusicDlg::mouseMoveEvent(QMouseEvent *event)
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

void SyllabusMusicDlg::region(const QPoint &cursorGlobalPoint)
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

void SyllabusMusicDlg::resizeEvent(QResizeEvent *event){
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


void SyllabusMusicDlg::resized(int){
    int newwidth=QApplication::desktop()->width();
    int newheight=QApplication::desktop()->height();
    this->resize(int(this->width()*newwidth/desktopX),
                 int(this->height()*newheight/desktopY));
    desktopX=newwidth;
    desktopY=newheight;
}


bool SyllabusMusicDlg::getMark() const
{
    return mark;
}


void SyllabusMusicDlg::setMark(bool value)
{
    mark = value;
}

void SyllabusMusicDlg::adjust(){

}

