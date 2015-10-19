#ifndef SYLLABUSMUSICDLG_H
#define SYLLABUSMUSICDLG_H

#include <QDialog>
#include"GlobalInfo.h"
#include "VideoPlayerWidget.h"

namespace Ui {
class SyllabusMusicDlg;
}

class SyllabusMusicDlg : public QDialog
{
    Q_OBJECT

public:
    SyllabusMusicDlg(MUSIC_INFO musicInfo, QWidget *parent = 0);
    ~SyllabusMusicDlg();
private slots:
    void on__pCloseBtn_clicked();

private:
    void paintEvent(QPaintEvent *e);
private:
    Ui::SyllabusMusicDlg *ui;

protected:
    //修改
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void region(const QPoint &cursorGlobalPoint);
    void resizeEvent(QResizeEvent *event);
public slots:
    void resized(int);
    void adjust();

public:
    static int desktopX;
    static int desktopY;
signals:
    void setAllWindow(QObject*parent,double scalex,double scaley);
private:
    bool isLeftPressDown;  // 判断左键是否按下
    QPoint dragPosition;   // 窗口移动拖动时需要记住的点
    Direction dir;        // 窗口大小改变时，记录改变方向
public:
    static double previousWidth;
    static double previousHeight;
    bool getMark() const;
    void setMark(bool value);
private:
    bool mark;
    VideoPlayerWidget _pVideoPlayerWidget;
    MUSIC_INFO _musicInfo;
};

#endif // SYLLABUSMUSICDLG_H
