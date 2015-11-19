#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "SyllabusWidget.h"
#include "GlobalInfo.h"

namespace Ui {
class Widget;
}



class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void sLoginGo();

    void on__pLeftMenuBtn_1_pressed();

    void on__pLeftMenuBtn_2_pressed();

    void on__pCapBtn_1_clicked();

    void on__pCapBtn_2_clicked();

    void on__pCapBtn_3_clicked();

    void on__pCapBtn_4_clicked();

    void on__pCapBtn_5_clicked();
    void on__pCloseBtn_clicked();

    void on__pSmallBtn_clicked();

private:
    void paintEvent(QPaintEvent *);
protected:
    //修改
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void region(const QPoint &cursorGlobalPoint);
    void resizeEvent(QResizeEvent *event);
public slots:
    void resized(int);
public:
    static int desktopX;
    static int desktopY;
signals:
    void setAllWindow(QObject*parent,double scalex,double scaley);
    void saveWindowsInfo(QObject*root);
private:
    bool isLeftPressDown;  // 判断左键是否按下
    QPoint dragPosition;   // 窗口移动拖动时需要记住的点
    Direction dir;        // 窗口大小改变时，记录改变方向
    //
private:
    Ui::Widget *ui;
    SyllabusWidget _syllabus;
    QString _sLeftMenuBtnDefStyleSheet;
    QString _sLeftMenuBtnPressStyleSheet;
public:
    static double previousWidth;
    static double previousHeight;
    bool getMark() const;
    void setMark(bool value);
private:
    bool mark;
};

#endif // WIDGET_H
