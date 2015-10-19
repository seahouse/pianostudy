#include "allwidget.h"
#include<QWidget>
#include"widget.h"
#include<QDebug>
AllWidget::AllWidget(QObject *parent) : QObject(parent),mark(false)
{
}

void AllWidget::SetRootWindow(QObject *root)
{
    this->root=root;
}

//遍历窗口树形，设置好所有位置；
void AllWidget::setAllCorrectLocal(QObject *parent, double scalex,double scaley)
{

        SetRootWindow(parent);
        adjustLocal(parent,scalex,scaley);
        Widget* widget=qobject_cast<Widget *>(parent);
        QObjectList objList=widget->children();
        QObject *obj=NULL;
        foreach(obj,objList){
            if(obj->objectName()=="_pCaptionWidget")
                break;
        }
        if(widget->width()>qobject_cast<QWidget *>(obj)->width()+10){
            Widget* widget=qobject_cast<Widget *>(parent);
            widget->setMark(false);
            widget->setGeometry(widget->x(),widget->y(),qobject_cast<QWidget *>(obj)->width(),widget->height());
        }
/*


    Widget* widget=qobject_cast<Widget *>(parent);
    QObjectList objList=widget->children();
    QObject *obj=NULL;
    foreach(obj,objList){
        if(obj->objectName()=="_pCaptionWidget")
            break;
    }
    if(widget->width()<qobject_cast<QWidget *>(obj)->width()+3){
        return;
    }
    for(int i=0;i<oList.length();i++){
        QWidget* widget=qobject_cast<QWidget *>(oList.at(i));
        QRect rect(*(rList.at(i)));
        widget->setGeometry(*(rList.at(i)));
        widget->setGeometry(rect.x()*scalex,rect.y()*scaley,rect.width()*scalex,rect.height()*scaley);
        qDebug()<<rect.x()*scalex<<rect.y()*scaley<<rect.width()*scalex<<rect.height()*scaley;
    }
    */
}

void AllWidget::adjustLocal(QObject *parent, double scalex,double scaley){
    QObjectList list=parent->children();
    if(list.empty()){
        return;
    }
    for(int i=0;i<list.length();i++){
        QWidget* widget=qobject_cast<QWidget *>(list.at(i));
        if(widget==NULL)
            continue;
        float x=widget->x()*scalex;
        float y=widget->y()*scaley;
        float width=widget->width()*scalex;
        float height=widget->height()*scaley;
        if(x-int(x)>0.5)
            x++;
        if(y-int(y)>0.5)
            y++;
        if(width-int(width)>0.5)
            width++;
        if(height-int(height)>0.5)
            height++;
        widget->setGeometry(x,y,width,height);
        adjustLocal(widget,scalex,scaley);
    }
}



//保存最初数据
void AllWidget::saveRectList(QObject *parent){
    if(!mark){
        addlistAll(parent);
        mark=true;
    }
}



void AllWidget::addlistAll(QObject *parent){
    QObjectList list=parent->children();
    if(list.empty()){
        return;
    }
    for(int i=0;i<list.length();i++){
        QWidget* widget=qobject_cast<QWidget *>(list.at(i));
        int x=widget->x();
        int y=widget->y();
        int width=widget->width();
        int height=widget->height();
        QRect rect(x,y,width,height);
        rList.append(&rect);
        oList.append(list.at(i));
        addlistAll(list.at(i));
    }
}




void AllWidget::setAllCorrectLocalTWO(QObject *parent, double scalex,double scaley)
{

        adjustLocal(parent,scalex,scaley);
}


void AllWidget::setAllCorrectLocalTHREE(QObject *parent, double scalex,double scaley){
    adjustLocal(parent,scalex,scaley);
}

