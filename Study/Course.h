#ifndef COURSE_H
#define COURSE_H

#include <QWidget>
#include "GlobalInfo.h"

namespace Ui {
class Course;
}

class Course : public QWidget
{
    Q_OBJECT

public:
    explicit Course(QWidget *parent = 0);
    ~Course();
    void SetSynopsis(int nImage); //设置简介字体
    void SetCurricula(QString sImage); //设置每一课的背景图
    void HideKeGenDanLab(); //隐藏曲目欣赏界面的可跟弹标签
    void SetJCKC(); //设置界面样式为基础课程
    void SetQMXS(); //设置界面样式为曲目欣赏
    void UpdateJCKC_UI(const CLASS_INFO &ci); //更新基础课程界面
    void UpdateQMXS_UI(const MUSIC_INFO &mi); //更新曲目欣赏界面
    void InitUI();
    void SetScoringClok(QString sArg);
    void SetScoringUnLock(QString sArg);
	QPixmap PixmapToRound(const QPixmap &src, int radius);

    QString getCourseId() const;
    void setScore(const QString &score);

private:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void SetNetworkPic(const QString &sUrl); //获取给定url地址的图像
    void customEvent(QEvent * event);//获取课后习题模块提交的分数
private:
    Ui::Course *ui;
    QString _sCourseStyleSheet;  //界面背景样式
    QString _sSynopsisStyleSheet;
    QString _sKeGenDanStyleSheet; //是否可跟弹，1：可以，0：不可以
    CLASS_INFO _classInfo;
    MUSIC_INFO _musicInfo;
    QString _sScoringNumStyleSheet;

public:
    void setAutoFirst(const AUTO_FIRST auotofirst);
private:
    AUTO_FIRST _autofirst;
};

#endif // COURSE_H
