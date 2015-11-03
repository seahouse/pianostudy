#ifndef PRAXISDLG_H
#define PRAXISDLG_H

#include <QDialog>
#include <QVector>
#include <QVariant>
#include <map>
#include "GlobalInfo.h"
#include "MenuBtn.h"
#include "Praxis.h"

using namespace std;

class QLabel;

namespace Ui {
class PraxisDlg;
}

class PraxisDlg : public QDialog
{
    Q_OBJECT

    struct ANSWER
    {
        QString sAnswer; //用户选择的答案，A,B,C,D
        bool bIsOK; //true：回答正确，false：回答错误
    };

signals:
    void SetLookToPage(int n); //通知_menuBtn观看到了第几页，n：按钮上的数字文本
public:
    explicit PraxisDlg(QString sCourseID, int iScore, QWidget *parent, QObject *pCourseObj);
    ~PraxisDlg();
    void Start(); //开始选择第一页
private slots:
    void on__pSbumitBtn_clicked();
    void on__pCloseBtn_clicked();
    void ReceiveStructInfo(int nId, QVariant varStructInfo); //接收NetWork解析的每一个json习题信息
    void ReceiveAnswer(QString sQuestionID, QString sAnswer, bool bIsOK);
    void on__pSmallBtn_clicked();

    void sTimeout();

private:
    void SetMaxPage(int n);
    void UpdateUI();
    void customEvent(QEvent * event);
private:
    Ui::PraxisDlg *ui;
    MenuBtn _menuBtn;
    QVector<Praxis*> _vecPraxis;  //习题
    QString _sCourseId;
    map<QString, ANSWER> _mapAnswer; //所有习题的回答情况和答题情况，键：sQuestionID，课程ID。值：回答情况
    map<int, PRAXIS_INFO> _mapPraxis;
    bool _b; //临时的以后修改menubtn类就会删掉
    QObject *_pCourseObj;
    int _total;     // 习题总个数，用于计算分数
    int _score;     // 分数



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
    bool getReady() const;
    void setReady(bool value);

private:
    bool mark;
private:
    bool ready;

    QTimer *_timer;
    QLabel *_lblGif;
};

#endif // PRAXISDLG_H
