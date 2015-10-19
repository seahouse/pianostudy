#ifndef MENUBTN_H
#define MENUBTN_H

#include <QWidget>
#include <QPushButton>
#include <QVector>

namespace Ui {
class MenuBtn;
}

class MenuBtn : public QWidget
{
    Q_OBJECT
signals:
    void SetLookToPage(int n); //通知父窗体观看到了第几页，n：按钮上的数字文本
public:
    explicit MenuBtn(int nPageSpace, int nBtnSpace, QWidget *parent = 0);
    ~MenuBtn();

public:
    void SetWidgetStyleSheet(QString s);
    void SetMaxPage(int n);
    void PressNumBtn(QString sBtnText, QString sArg = ""); //按下第几个按钮,nBtnText从1开始,并设置按钮为按下状态, sArg是附加参数，供需要多个url参数的模块用
    void SetUrl(QString sUrl); //设置翻页时使用哪个url连接
private slots:
    void on__pUpPageBtn_clicked();
    void on__pDownPageBtn_clicked();
    void PageBtnClick(); //页按钮
    void GetLookToPage(int n);  //主窗体通知看到第几页，然后更新相应按钮样式，n：按钮上的数字文本
private:
    void resizeEvent(QResizeEvent *);
    /*****************************************************************
    功能：
             创建按钮栏的按钮，包括样式，位置，关联信号槽等。
    参数：
             无。
    返回值：
             无。
    说明：
             无。
    *****************************************************************/
    void CreateBtns();
    void TurnPage(); //翻页
private:
    Ui::MenuBtn *ui;
    int _nMaxPage; //最大页数
    int _nBeginPage; //起始页
    QVector<QPushButton*> _vecBtn; //创建的10个功能按钮都存到这里
    int _nPageSpace; //上下页按钮距离两侧边框的间距
    int _nBtnSpace; //10个按钮的间距
    QString _sNumBtnDefStyhleSheet;   //数字按钮默认样式
    QString _sNumBtnPressStyhleSheet; //数字按钮按下样式
    QString _sUrl; //链接网址
    QString _sArg; //课程习题用到的url参数，以后模块多了这个部分可能重新设计
};

#endif // MENUBTN_H
