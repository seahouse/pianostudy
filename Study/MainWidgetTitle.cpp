#include "MainWidgetTitle.h"
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

MainWidgetTitle::MainWidgetTitle(QWidget *parent) : QWidget(parent),_nX(0),_nY(0),_pParent(parent)
{

}

void MainWidgetTitle::mousePressEvent(QMouseEvent *event)
{
    _nX = event->pos().x();
    _nY = event->pos().y();
    qDebug() << "x=" << QString::number(_nX)<< "y=" << QString::number(_nY);
}

void MainWidgetTitle::mouseMoveEvent(QMouseEvent *event)
{

   _pParent->move( _nX+_pParent->pos().x()-_nX, _nY+_pParent->pos().y()-_nY );
   _pParent->move( _pParent->pos().x()+event->pos().x()-_nX, _pParent->pos().y()+event->pos().y()-_nY );
   qDebug() << "xMove=" << QString::number(_nX+event->pos().x()-_nX)<< "yMove=" << QString::number(_nY+event->pos().y()-_nY);
}

void MainWidgetTitle::paintEvent(QPaintEvent *event)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter ppp(this);
     this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &ppp, this);
}
