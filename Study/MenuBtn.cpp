#include "MenuBtn.h"
#include "ui_MenuBtn.h"
#include <QDebug>
#include <QStyleOption>
#include <QPainter>
#include "NetWork.h"

MenuBtn::MenuBtn(int nPageSpace, int nBtnSpace, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuBtn),_nMaxPage(1),_nBeginPage(1),_sUrl(""),_sArg("")
{
    ui->setupUi(this);
    _nPageSpace = nPageSpace;
    _nBtnSpace = nBtnSpace;
    //上一页，下一页按钮样式
    QString sPageBtnStyleSheet = "QPushButton{border-image:non;background-color:rgb(67,144,30);border-radius:10px;font-weight:500;font-size:15px;color:white;}\n\
                            QPushButton:pressed{border-image:non;background-color:rgb(224,166,27);border-radius:10px;font-weight:500;font-size:15px;color:white;}";
    ui->_pUpPageBtn->setStyleSheet(sPageBtnStyleSheet);
    ui->_pDownPageBtn->setStyleSheet(sPageBtnStyleSheet);
    ui->_pUpPageBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->_pDownPageBtn->setCursor(QCursor(Qt::PointingHandCursor));

    _sNumBtnDefStyhleSheet = "QPushButton{border-image:url(:/images/pageNum_off.png);font-weight:500;font-size:15px;color:black;}\n\
            QPushButton:pressed{border-image:url(:/images/pageNum_on.png);font-weight:500;font-size:15px;color:black;}";
    _sNumBtnPressStyhleSheet = "QPushButton{border-image:url(:/images/pageNum_on.png);font-weight:500;font-size:15px;color:black;}\n\
            QPushButton:pressed{border-image:url(:/images/pageNum_on.png);font-weight:500;font-size:15px;color:black;}";

    CreateBtns();
}

MenuBtn::~MenuBtn()
{
    delete ui;
}

void MenuBtn::SetUrl(QString sUrl)
{
    _sUrl = sUrl;
}

void MenuBtn::PressNumBtn(QString sBtnText, QString sArg)
{
    if( _sArg == "" )
    {
        _sArg = sArg;
    }
    else
    {
        sArg = _sArg;
    }
    emit SetLookToPage(sBtnText.toInt());
    NetWork *pNetWork = NetWork::GetInstance();
    pNetWork->SetUrl(this->parent()->parent(), this->parent()->parent()->objectName(), _sUrl.arg(sBtnText, sArg));
    _vecBtn.at(sBtnText.toInt()-1)->setStyleSheet(_sNumBtnPressStyhleSheet);
}

void MenuBtn::resizeEvent(QResizeEvent *event)
{
   ui->_pUpPageBtn->move(_nPageSpace, (this->height()-26)/2-13);
   ui->_pDownPageBtn->move(this->width()-60-_nPageSpace, (this->height()-26)/2-13);

   //更新按钮位置和尺寸
   int nBtnWidth = 40;
   int nBeginPos = _nPageSpace+60+9+50; //创建按钮的起始位置
   for(int i=0; i<_vecBtn.size(); ++i)
   {
       _vecBtn.at(i)->setGeometry(nBeginPos + (i-1)*(_nBtnSpace+nBtnWidth),ui->_pUpPageBtn->pos().y(), nBtnWidth, 26);
   }
}

void MenuBtn::CreateBtns()
{
    static int nPos = 1;
    for(int i = nPos; i<=10; ++i)
    {
        QPushButton *pBtn = new QPushButton(this);
        pBtn->setText(QString::number(i));
        pBtn->hide();
        pBtn->setStyleSheet(_sNumBtnDefStyhleSheet);
        connect(pBtn, SIGNAL(pressed()), this, SLOT(PageBtnClick()));
       _vecBtn.push_back(pBtn);
       pBtn->setCursor(QCursor(Qt::PointingHandCursor));
    }
}

void MenuBtn::PageBtnClick()
{
    QPushButton *pBtn = qobject_cast<QPushButton*>(QObject::sender());
    if( pBtn )
    {
        PressNumBtn(pBtn->text());
        //更新按钮按下样式
        for(int i=0; i<10; ++i)
        {
            if( pBtn->text().toInt() != i+1 )
            {
                 _vecBtn.at(i)->setStyleSheet(_sNumBtnDefStyhleSheet);
            }
        }
    }
}

void MenuBtn::GetLookToPage(int n)
{
    _nBeginPage = n/10*2+1; //设置起始页
    for(int i=0; i<10; ++i)
    {
        if( (i+1) <= _nMaxPage )
        {
            _vecBtn.at(i)->show();
        }
        else
        {
            _vecBtn.at(i)->hide();
        }
        if( i != n-1 )
        {
            _vecBtn.at(i)->setStyleSheet(_sNumBtnDefStyhleSheet);
        }
        else
        {
            _vecBtn.at(i)->setStyleSheet(_sNumBtnPressStyhleSheet);
        }
    }
}

void MenuBtn::on__pUpPageBtn_clicked()
{
    int i=0;
    while( !_vecBtn.at(i)->isHidden() )
    {
        if( _sNumBtnPressStyhleSheet == _vecBtn.at(i)->styleSheet() )
        {
            if( 0 == i )
            {
                if( _nBeginPage > 10 )
                {
                    _vecBtn.at(i)->setStyleSheet(_sNumBtnDefStyhleSheet);
                    _nBeginPage -= 10;
                    TurnPage();
                    PressNumBtn(_vecBtn.at(9)->text());
                    emit SetLookToPage(_vecBtn.at(9)->text().toInt());
                }
            }
            else
            {
                _vecBtn.at(i)->setStyleSheet(_sNumBtnDefStyhleSheet);
                PressNumBtn(_vecBtn.at(i-1)->text());
                emit SetLookToPage(_vecBtn.at(i-1)->text().toInt());
            }
            break;
        }
        ++i;
    }
}

void MenuBtn::on__pDownPageBtn_clicked()
{
    int i=0;
    while( !_vecBtn.at(i)->isHidden() )
    {
        if( _sNumBtnPressStyhleSheet == _vecBtn.at(i)->styleSheet() )
        {
            if( i < 9 && !_vecBtn.at(i+1)->isHidden() )
            {
                _vecBtn.at(i)->setStyleSheet(_sNumBtnDefStyhleSheet);
                PressNumBtn(_vecBtn.at(i+1)->text());
                emit SetLookToPage(_vecBtn.at(i+1)->text().toInt());
            }
            else
            {
                if( (_nBeginPage+10) <= _nMaxPage )
                {
                    _vecBtn.at(i)->setStyleSheet(_sNumBtnDefStyhleSheet);
                    _nBeginPage += 10;
                    TurnPage();
                    PressNumBtn(_vecBtn.at(0)->text());
                    emit SetLookToPage(_vecBtn.at(0)->text().toInt());
                }
            }
            break;
        }
        ++i;
    }
}

void MenuBtn::SetWidgetStyleSheet(QString s)
{
    this->setStyleSheet(s);
}

void MenuBtn::SetMaxPage(int n)
{
    _nMaxPage = n;
}

void MenuBtn::TurnPage()
{
    for(int i=0; i<10; ++i)
    {
        if( (_nBeginPage+i) <= _nMaxPage )
        {
            _vecBtn.at(i)->setStyleSheet(_sNumBtnDefStyhleSheet);
            _vecBtn.at(i)->setText(QString::number(_nBeginPage+i));
            _vecBtn.at(i)->show();
        }
        else
        {
            _vecBtn.at(i)->hide();
        }
    }
}
