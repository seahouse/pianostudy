#include "PraxisDlg.h"
#include "ui_PraxisDlg.h"
#include "NetWork.h"
#include "CustomEvent.h"
#include <QApplication>
#include<QMouseEvent>
#include<QDesktopWidget>







double PraxisDlg::previousWidth=0.0;
double PraxisDlg::previousHeight=0.0;
int PraxisDlg::desktopX=0;
int PraxisDlg::desktopY=0;

PraxisDlg::PraxisDlg(QString sCourseId, QWidget *parent, QObject *pCourseObj) :
    QDialog(parent),ready(false),
    ui(new Ui::PraxisDlg),_menuBtn(178, 6),_b(true)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->move(0, 0);
    this->setStyleSheet("QDialog{border-image:url(:/images/PraxisDlg/back.png);}");
    this->setObjectName("课程习题");
    _sCourseId = sCourseId;
    _pCourseObj = pCourseObj;

    int nX[7] = {21,21,21,628,628,628,628};
    int nY[7] = {180,313,446,38,180,313,446};
    for(int i=0; i<7; ++i)
    {
        Praxis *p = new Praxis(this);
        p->move(nX[i], nY[i]);
        _vecPraxis.push_back(p);
    }

    //关闭按钮
    ui->_pCloseBtn->setStyleSheet("QPushButton{border-image:url(:/images/SyllabusBasicDlg/close.png);}");
    //提交按钮
    ui->_pSbumitBtn->setStyleSheet("QPushButton{border-image: url(:/images/PraxisDlg/submit.png);}");
    //按钮栏
    _menuBtn.setParent(ui->_pMenuBtnParentWidget);
    connect(this, SIGNAL(SetLookToPage(int)), &_menuBtn, SLOT(GetLookToPage(int)));
    _menuBtn.SetUrl("http://120.55.119.93/course/index.php?m=Api&c=Question&a=questionList&page=%1&rows=7&course_id=%2");
    //下边按钮栏的背景样式
    ui->_pMenuBtnParentWidget->setStyleSheet("QWidget{border-image: url(:/images/MenuBtnBack.png);}");

    //下方样式
    ui->_pBottomLab->setStyleSheet("QLabel{color:white;font-size:14px;font-weight:500;border-image:url(:/images/bottom.png);}");
    ui->_pBottomLab->move(0, this->height()-35);
    //修改
    mark=false;
    isLeftPressDown = false;
    this->dir = NONE;
    this->setMouseTracking(true);
    //
}

PraxisDlg::~PraxisDlg()
{
    delete ui;
}


void PraxisDlg::customEvent(QEvent *event)
{
   switch( event->type() )
   {
   case PRAXIS_SET_MAX_PAGE:
   {
       CustomEvent *e = static_cast<CustomEvent*>(event);
       SetMaxPage(e->_nMaxPage);
       e->accept();
       break;
   }
   case PRAXIS_UPDATE_UI:
   {
       CustomEvent *e = static_cast<CustomEvent*>(event);
       UpdateUI();
       e->accept();
       break;
   }
   default:
       QObject::event(event);
   }
}
 //connect(p, SIGNAL(SendAnswer(QString,QString,bool)), this, SLOT(ReceiveAnswer(QString,QString,bool)));
void PraxisDlg::on__pSbumitBtn_clicked()
{
    int n = _mapAnswer.size(); //总题数
    int nOK = 0; //答对的题数
    map<QString, ANSWER>::iterator Iter = _mapAnswer.begin();
    while( Iter != _mapAnswer.end() )
    {
        if( Iter->second.bIsOK )
        {
            ++nOK;
        }
        ++Iter;
    }
    int nScore = 0;
    if( n > 0 )
    {
        nScore = 100/n*nOK;
    }

    CustomEvent *e = new CustomEvent(QEvent::Type(UPDATE_COURSE_SCORE));
    e->_nScore = nScore;
    e->_sCourseId = _sCourseId;
    QCoreApplication::sendEvent(_pCourseObj, e); //将得分设置到课程列表界面
 //   this->close();
    this->hide();
}

void PraxisDlg::on__pCloseBtn_clicked()
{
  //  this->close();
    this->hide();
}

void PraxisDlg::ReceiveStructInfo(int nId, QVariant varStructInfo)
{
    PRAXIS_INFO pi = varStructInfo.value<PRAXIS_INFO>();
    _mapPraxis[nId] = pi;

     map<QString, ANSWER>::iterator Iter = _mapAnswer.find(pi.sQuestionId);
     if( Iter == _mapAnswer.end() )
     {
         ANSWER answer = {"", false};
         _mapAnswer[pi.sQuestionId] = answer;
     }
}

void PraxisDlg::ReceiveAnswer(QString sQuestionID, QString sAnswer, bool bIsOK)
{
    ANSWER answer;
    answer.sAnswer = sAnswer;
    answer.bIsOK = bIsOK;
    _mapAnswer[sQuestionID] = answer;
}

void PraxisDlg::SetMaxPage(int n)
{
    //根据传入的页数初始化下排翻页按钮
    _menuBtn.SetMaxPage(n);
    if(_b)
    {
         SetLookToPage(1);
        _b = false;
    }
}

void PraxisDlg::Start()
{
    _menuBtn.PressNumBtn("1", _sCourseId);
}

void PraxisDlg::UpdateUI()
{
    //初始化界面
    this->setEnabled(false);
    for(int i=0; i<_vecPraxis.size(); ++i)
    {
        _vecPraxis.at(i)->InitUI();
    }
    //将内容显示到界面
    map<int ,PRAXIS_INFO>::const_iterator Iter = _mapPraxis.begin();
    for(int i=0; i<_vecPraxis.size(); ++i)
    {
        if( Iter != _mapPraxis.end() )
        {
            _vecPraxis.at(i)->UpdateUI(Iter->second);

            map<QString, ANSWER>::iterator AnswerIter = _mapAnswer.find(Iter->second.sQuestionId);
            if( AnswerIter != _mapAnswer.end() )
            {
                if( AnswerIter->second.sAnswer == "A" )
                {
                     _vecPraxis.at(i)->ClickA();
                }
                else if( AnswerIter->second.sAnswer == "B" )
                {
                    _vecPraxis.at(i)->ClickB();
                }
                else if( AnswerIter->second.sAnswer == "C" )
                {
                    _vecPraxis.at(i)->ClickC();
                }
                else if( AnswerIter->second.sAnswer == "D" )
                {
                    _vecPraxis.at(i)->ClickD();
                }
            }
            _vecPraxis.at(i)->setMark(true);
            ++Iter;
        }
        else
        {
            _vecPraxis.at(i)->setMark(false);
        }
    }
    this->show();
    for(int i=0; i<_vecPraxis.size(); ++i)
    {
        Praxis *p=_vecPraxis.at(i);
        if(p->getMark()){
            p->show();
        }else
            p->hide();
    }

    if(!ready){
        for(int i=0; i<_vecPraxis.size(); ++i)
            connect(_vecPraxis.at(i), SIGNAL(SendAnswer(QString,QString,bool)), this, SLOT(ReceiveAnswer(QString,QString,bool)));
        ready=true;
    }
    _mapPraxis.clear();
    this->setEnabled(true);
}

void PraxisDlg::on__pSmallBtn_clicked()
{

}





//修改
void PraxisDlg::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        isLeftPressDown = false;
        if(dir != NONE) {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

void PraxisDlg::mousePressEvent(QMouseEvent *event)
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

void PraxisDlg::mouseMoveEvent(QMouseEvent *event)
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

void PraxisDlg::region(const QPoint &cursorGlobalPoint)
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

void PraxisDlg::resizeEvent(QResizeEvent *event){
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

void PraxisDlg::resized(int){
    int newwidth=QApplication::desktop()->width();
    int newheight=QApplication::desktop()->height();
    this->resize(int(this->width()*newwidth/desktopX),
                 int(this->height()*newheight/desktopY));
    desktopX=newwidth;
    desktopY=newheight;
}

bool PraxisDlg::getMark() const
{
    return mark;
}

void PraxisDlg::setMark(bool value)
{
    mark = value;
}

bool PraxisDlg::getReady() const
{
    return ready;
}

void PraxisDlg::setReady(bool value)
{
    ready = value;
}



