#include "SyllabusWidget.h"
#include "ui_SyllabusWidget.h"
#include <QPainter>
#include <QEvent>
#include "CustomEvent.h"
#include "NetWork.h"


SyllabusWidget::SyllabusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SyllabusWidget),_menuBtn(150, 5)
{
    ui->setupUi(this);
    _menuBtn.setParent(ui->_pMenuBtnParentWidget);
    connect(&_menuBtn, SIGNAL(SetLookToPage(int)), this, SLOT(GetLookToPage(int)));
    connect(this, SIGNAL(SetLookToPage(int)), &_menuBtn, SLOT(GetLookToPage(int)));
    //创建八个课程
    int x1 = 32;
    int x2 = 485;
    int nX[8] = {32, 485, 32, 485, 32, 485, 32, 485};
    int nY[8] = {13, 13, 141, 141, 270, 270, 398, 398};
    for(int i=0; i<8; ++i)
    {
        Course *pCourse = new Course(this);
        _vecCourse.push_back(pCourse);
        pCourse->SetCurricula("classBg1");
        pCourse->HideKeGenDanLab();
        pCourse->move(nX[i], nY[i]);
//        if( 0 != i )
//        {
//            pCourse->SetScoringNumStyle("url(:/images/lock.png)");
//        }

    }
    //下边按钮栏的背景样式
    ui->_pMenuBtnParentWidget->setStyleSheet("QWidget{border-image: url(:/images/MenuBtnBack.png);}");

    _mapLeftPressBtn["基础课程"] = 1;
    _mapLeftPressBtn["曲目欣赏"] = 1;
}

SyllabusWidget::~SyllabusWidget()
{
    delete ui;
}

void SyllabusWidget::customEvent(QEvent * event)
{
    switch(event->type())
    {
    case SYLLABUS_SET_MAX_PAGE:
    {
        CustomEvent *e = static_cast<CustomEvent*>(event);
        SetMaxPage(e->_nMaxPage);
        e->accept();
        break;
    }
    case SYLLABUS_UPDATE_UI:
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

void SyllabusWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if( this->objectName() == "基础课程" )
    {

       // painter.drawImage(0, 0,QImage(":/images/SyllabusBack.png"));
      // QPainter painter(this);
        painter.drawPixmap(0, 0, this->width(), this->height(), QPixmap(":/images/SyllabusBack.png"));
    }
    else if( this->objectName() == "曲目欣赏" )
    {
      //  painter.drawImage(0, 0,QImage(":/images/SyllabusBack2.png"));
        painter.drawPixmap(0, 0, this->width(), this->height(), QPixmap(":/images/SyllabusBack.png"));
    }
}

void SyllabusWidget::resizeEvent(QResizeEvent *event)
{
    _menuBtn.setGeometry(0, 0, ui->_pMenuBtnParentWidget->width(), ui->_pMenuBtnParentWidget->height());
}

void SyllabusWidget::SetJCKC()
{
    for(int i=0; i<_vecCourse.size(); ++i)
    {
        _vecCourse.at(i)->SetCurricula("classBg1");
        _vecCourse.at(i)->SetSynopsis(15);
        _vecCourse.at(i)->HideKeGenDanLab();
        _vecCourse.at(i)->SetJCKC();
        _vecCourse.at(i)->setAutoFirst(autoFirst);
    }

    GetClassInfo("基础课程", "http://120.55.119.93/course/index.php?m=Api&c=Course&a=courseList&page=1&rows=8");
    update();
}

void SyllabusWidget::SetQMXS()
{
    for(int i=0; i<_vecCourse.size(); ++i)
    {
        _vecCourse.at(i)->SetCurricula("classBg2");
        _vecCourse.at(i)->SetSynopsis(15);
        _vecCourse.at(i)->SetQMXS();
        _vecCourse.at(i)->setAutoFirst(autoFirst);
    }

    GetClassInfo("曲目欣赏", "http://120.55.119.93/course/index.php?m=Api&c=Music&a=musicList&page=1&rows=8");
    update();
}

void SyllabusWidget::GetClassInfo(QString sPressBtnName, QString sUrl)
{
    this->setObjectName(sPressBtnName);
    QString sUrlTemplet = "";
    if( sPressBtnName == "基础课程" )
    {
        sUrlTemplet = "http://120.55.119.93/course/index.php?m=Api&c=Course&a=courseList&page=%1&rows=8";
    }
    else if( sPressBtnName == "曲目欣赏" )
    {
        sUrlTemplet = "http://120.55.119.93/course/index.php?m=Api&c=Music&a=musicList&page=%1&rows=8";
    }

    _menuBtn.SetUrl(sUrlTemplet);
    map<QString, int>::iterator Iter = _mapLeftPressBtn.find(sPressBtnName);
    if( Iter != _mapLeftPressBtn.end() )
    {
        _menuBtn.PressNumBtn(QString::number(Iter->second));
    }
}

void SyllabusWidget::ReceiveStructInfo(int nCourseId, QVariant varStructInfo)
{
    if( this->objectName() == "基础课程" )
    {
        if( varStructInfo.canConvert<CLASS_INFO>() )
        {
            CLASS_INFO ci = varStructInfo.value<CLASS_INFO>();
			std::string s = ci.sPicUrl.toStdString();
            _mapJCKC[nCourseId] = ci;
        }
    }
    else if( this->objectName() == "曲目欣赏" )
    {
        MUSIC_INFO mi = varStructInfo.value<MUSIC_INFO>();
        _mapQMXS[nCourseId] = mi;
    }
}

void SyllabusWidget::SetMaxPage(int n)
{
    //根据传入的页数初始化下排翻页按钮
    _menuBtn.SetMaxPage(n);
}

void SyllabusWidget::UpdateUI()
{
    //初始化界面
    for(int i=0; i<_vecCourse.size(); ++i)
    {
        _vecCourse.at(i)->InitUI();
    }
    //将内容显示到界面
    if( this->objectName() == "基础课程" )
    {
        map<int ,CLASS_INFO>::const_iterator Iter = _mapJCKC.begin();
        for(int i=0; i<_vecCourse.size(); ++i)
        {
            if( Iter != _mapJCKC.end() )
            {
                _vecCourse.at(i)->UpdateJCKC_UI(Iter->second);
                _vecCourse.at(i)->show();
                ++Iter;
            }
            else
            {
                _vecCourse.at(i)->hide();

            }
        }
        _mapJCKC.clear();
    }
    else if( this->objectName() == "曲目欣赏"  )
    {
        map<int ,MUSIC_INFO>::const_iterator Iter = _mapQMXS.begin();
        for(int i=0; i<_vecCourse.size(); ++i)
        {
            if( Iter != _mapQMXS.end() )
            {
                _vecCourse.at(i)->UpdateQMXS_UI(Iter->second);
                _vecCourse.at(i)->show();
                ++Iter;
            }
            else
            {
                _vecCourse.at(i)->hide();
            }
        }
        _mapQMXS.clear();
    }

    //通知按钮栏,看到了第几页
    map<QString, int>::iterator Iter = _mapLeftPressBtn.find(this->objectName());
    if( Iter != _mapLeftPressBtn.end() )
    {
        emit SetLookToPage(Iter->second);
    }
}

void SyllabusWidget::GetLookToPage(int n)
{
    map<QString, int>::iterator Iter = _mapLeftPressBtn.find(this->objectName());
    if( Iter != _mapLeftPressBtn.end() )
    {
        Iter->second = n;
    }
}



void SyllabusWidget::RecevieAutoFirst(QVariant Info){
    if( Info.canConvert<AUTO_FIRST>() )
    {
        AUTO_FIRST auto_first = Info.value<AUTO_FIRST>();
        this->autoFirst.sCtype=auto_first.sCtype;
        this->autoFirst.sURL=auto_first.sURL;
    }
}
