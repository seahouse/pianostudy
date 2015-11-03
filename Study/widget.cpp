#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include "LogonDlg.h"
#include "NetWork.h"
#include<QMouseEvent>
#include<QDesktopWidget>



double Widget::previousWidth=0.0;
double Widget::previousHeight=0.0;
int Widget::desktopX=0;
int Widget::desktopY=0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),_syllabus(this)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    //修改
    mark=false;
    isLeftPressDown = false;
    this->dir = NONE;
    this->setMinimumHeight(700);
    this->setMinimumWidth(1200);
    this->setMouseTracking(true);
    //

    //左侧基础课程，曲目欣赏的背景
    ui->_pLeftMenuWidget->setStyleSheet("QWidget{border-image:url(:/images/LeftMenuBack.png);}");
    //界面上方一条深绿色背景
    ui->_pCaptionWidget->setStyleSheet("QWidget{background-color:rgb(8,87,42);}");
    //界面右上角浅绿色背景
    ui->_pCaptionWidget2->setStyleSheet("QWidget{background-color:rgb(133,222,20);}");
    //右上角关闭和最小化按钮样式
    ui->_pCloseBtn->setStyleSheet("QPushButton{border-image:url(:/images/close.png);}");
    ui->_pSmallBtn->setStyleSheet("QPushButton{border-image:url(:/images/small.png);}");
    //右上角头像
    ui->_pHeadLab->setStyleSheet("QLabel{background-color:transparent;border-image: url(:/images/head.png);}");
    //右上角文本样式
    ui->_pHeadInfoLab->setStyleSheet("QLabel{font-family:宋体;font-weight:500;font-size:20px;color:rgb(0,61,28);}");
    ui->_pHeadInfoLab->setText(tr("欢迎XXX登陆\n您是成人登陆"));
    //下方样式
    ui->_pBottomLab->setStyleSheet("QLabel{color:white;font-size:14px;font-weight:500;border-image:url(:/images/bottom.png);}");

    //基础课程，曲目欣赏，按钮样式
    {
        _sLeftMenuBtnDefStyleSheet = "QPushButton{font-weight:500;font-size:25px;color:rgb(163,76,0);border-image: url(:/images/%1.png);}QPushButton:pressed{border-image: url(:/images/%2.png);}";
        _sLeftMenuBtnPressStyleSheet = "QPushButton{font-weight:500;font-size:25px;color:rgb(163,76,0);border-image: url(:/images/%1.png);}";
        ui->_pLeftMenuBtn_1->setStyleSheet(_sLeftMenuBtnPressStyleSheet.arg("jckc_press"));
        ui->_pLeftMenuBtn_2->setStyleSheet(_sLeftMenuBtnDefStyleSheet.arg("qmxs","qmxs_press"));
    }

    //乐谱自学，纠错陪练，在线教学，考试系统，老师点评，这些按钮样式
    {
        QString sCapBtnStyleSheet = "QPushButton{border-image: url(:/images/CapBtn%1.png);font-weight:500;font-size:17px;color:rgb(163,76,0);}";
        ui->_pCapBtn_1->setStyleSheet(sCapBtnStyleSheet.arg(1));
        ui->_pCapBtn_2->setStyleSheet(sCapBtnStyleSheet.arg(2));
        ui->_pCapBtn_3->setStyleSheet(sCapBtnStyleSheet.arg(3));
        ui->_pCapBtn_4->setStyleSheet(sCapBtnStyleSheet.arg(4));
        ui->_pCapBtn_5->setStyleSheet(sCapBtnStyleSheet.arg(5));
        ui->_pCapBtn_1->setCursor(QCursor(Qt::PointingHandCursor));
        ui->_pCapBtn_2->setCursor(QCursor(Qt::PointingHandCursor));
        ui->_pCapBtn_3->setCursor(QCursor(Qt::PointingHandCursor));
        ui->_pCapBtn_4->setCursor(QCursor(Qt::PointingHandCursor));
        ui->_pCapBtn_5->setCursor(QCursor(Qt::PointingHandCursor));
    }

    _syllabus.setGeometry(240,85, 940, 568);
    ui->_pBottomLab->move(0, this->height()-35);
    ui->_pLeftMenuBtn_1->setCursor(QCursor(Qt::PointingHandCursor));
    ui->_pLeftMenuBtn_2->setCursor(QCursor(Qt::PointingHandCursor));


    NetWork *pNetWork = NetWork::GetInstance();
    connect(pNetWork, SIGNAL(SendClassInfo(int,QVariant)), &_syllabus, SLOT(ReceiveStructInfo(int,QVariant)));
    connect(pNetWork,SIGNAL(SendAutoFirst(QVariant)),&_syllabus,SLOT(RecevieAutoFirst(QVariant)));


    LogonDlg *pDlg = new LogonDlg(this);
    connect(pDlg, SIGNAL(Go()), this, SLOT(on__pLeftMenuBtn_1_pressed()));
    pDlg->show();

//    LogonDlg2 *pDlg = new LogonDlg2(this);
//    connect(pDlg, SIGNAL(Go()), this, SLOT(on__pLeftMenuBtn_1_pressed()));
//    pDlg->show();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
 {
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), QPixmap(":/images/MainBack.png"));
    QPen oldpen(painter.pen());
    painter.setPen(QColor(8,87,42));
    painter.drawLine(0,0,this->width(),0);
    painter.setPen(oldpen);
}


void Widget::on__pLeftMenuBtn_1_pressed()
{
    ui->_pLeftMenuBtn_1->setStyleSheet(_sLeftMenuBtnPressStyleSheet.arg("jckc_press"));
    ui->_pLeftMenuBtn_2->setStyleSheet(_sLeftMenuBtnDefStyleSheet.arg("qmxs","qmxs_press"));

    _syllabus.SetJCKC();
}

void Widget::on__pLeftMenuBtn_2_pressed()
{
    ui->_pLeftMenuBtn_1->setStyleSheet(_sLeftMenuBtnDefStyleSheet.arg("jckc","jckc_press"));
    ui->_pLeftMenuBtn_2->setStyleSheet(_sLeftMenuBtnPressStyleSheet.arg("qmxs_press"));

    _syllabus.SetQMXS();
}

void Widget::on__pCapBtn_1_clicked()
{
  //  QMessageBox::information(this, tr("乐谱自学"), tr("这是乐谱自学"));
}

void Widget::on__pCapBtn_2_clicked()
{
   // QMessageBox::information(this, tr("纠错陪练"), tr("这是纠错陪练"));
}

void Widget::on__pCapBtn_3_clicked()
{
   // QMessageBox::information(this, tr("在线教学"), tr("这是在线教学"));
}

void Widget::on__pCapBtn_4_clicked()
{
   // QMessageBox::information(this, tr("考试系统"), tr("这是考试系统"));
}

void Widget::on__pCapBtn_5_clicked()
{
    //QMessageBox::information(this, tr("老师点评"), tr("这是老师点评"));
}



void Widget::on__pCloseBtn_clicked()
{
    this->close();
}

void Widget::on__pSmallBtn_clicked()
{
    this->showMinimized();
}



//修改
void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        isLeftPressDown = false;
        if(dir != NONE) {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
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

void Widget::mouseMoveEvent(QMouseEvent *event)
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

void Widget::region(const QPoint &cursorGlobalPoint)
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

void Widget::resizeEvent(QResizeEvent *event){
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


//桌面改变大小是发生槽调用
void Widget::resized(int){
    int newwidth=QApplication::desktop()->width();
    int newheight=QApplication::desktop()->height();
    this->resize(int(this->width()*newwidth/desktopX),
                 int(this->height()*newheight/desktopY));
    desktopX=newwidth;
    desktopY=newheight;
}


bool Widget::getMark() const
{
    return mark;
}


void Widget::setMark(bool value)
{
    mark = value;
}

