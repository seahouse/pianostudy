#ifndef SYLLABUSBASICDLG_H
#define SYLLABUSBASICDLG_H

#include <QDialog>
#include <QWebView>
#include <QtWebKitWidgets/QWebView>
#include "VideoPlayerWidget.h"
#include "GlobalInfo.h"
#include"PraxisDlg.h"

class KeyKnowledge;

namespace Ui {
class SyllabusBasicDlg;
}

class SyllabusBasicDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SyllabusBasicDlg(CLASS_INFO classInfo, QWidget *parent, QObject *pCourseObj);
    ~SyllabusBasicDlg();

private slots:
    void on__pCloseBtn_clicked();
    void on__pSubmitBtn_clicked();
    void on__pBtn1_clicked();
    void on__pBtn2_clicked();

private:
    void paintEvent(QPaintEvent *e);

    void creatHTML(QString scontent);

private:
    Ui::SyllabusBasicDlg *ui;
    VideoPlayerWidget _pVideoPlayerWidget;
	QWebView* _pWebviewhtml;
    bool _bIsExtend; //扩展：true，收起：false
    CLASS_INFO _classInfo;
    QObject *_pCourseObj;
    KeyKnowledge *_keyKnowledge;




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
    PraxisDlg*pd;

};

#endif // SYLLABUSBASICDLG_H
