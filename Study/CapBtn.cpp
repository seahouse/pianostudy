#include "CapBtn.h"
#include <QStylePainter>
#include <QStyleOptionButton>
#include <QPainter>

CapBtn::CapBtn(QWidget * parent):QPushButton(parent)
{

}

CapBtn::~CapBtn()
{

}

void CapBtn::paintEvent(QPaintEvent*event)
{
    QStylePainter p(this);
    QStyleOptionButton option;
    initStyleOption(&option);
    option.text=QString();
    p.drawControl(QStyle::CE_PushButton,option);
    p.drawText(rect().adjusted(5,5,-15,-12),Qt::AlignRight|Qt::AlignBottom,this->text());
}
