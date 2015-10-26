#ifndef SYLLABUS_H
#define SYLLABUS_H

#include <QWidget>
#include <map>
#include <QVariant>
#include <vector>
#include "Course.h"
#include <map>
#include "MenuBtn.h"
#include "GlobalInfo.h"


using namespace std;

namespace Ui {
class SyllabusWidget;
}

class SyllabusWidget : public QWidget
{
    Q_OBJECT
signals:
    void SetLookToPage(int n); //通知_menuBtn观看到了第几页，n：按钮上的数字文本
public:
    explicit SyllabusWidget(QWidget *parent = 0);
    ~SyllabusWidget();
    void SetJCKC(); //设置界面样式为基础课程
    void SetQMXS(); //设置界面样式为曲目欣赏
    void GetClassInfo(QString sPressBtnName, QString sUrl); //获取制定按下按钮对应的课程信息,然后根据按下的按钮显示正确观看页和保存到正确的map中

private slots:
    void ReceiveStructInfo(int nCourseId, QVariant varStructInfo); //接收NetWork解析的每一课json信息
    void GetLookToPage(int n); //保存观看到了第几页，n：按钮上的数字文本
    void sUserscoreResponse(QVariant response);     //加载用户分数
private:
    void SetMaxPage(int n);
    void UpdateUI();
    void paintEvent(QPaintEvent *);
    void customEvent(QEvent * event);
protected:
    void resizeEvent(QResizeEvent *);
private:
    Ui::SyllabusWidget *ui;
    MenuBtn _menuBtn;
    QVector<Course*> _vecCourse; //八个课程大纲页面
    map<int, CLASS_INFO> _mapJCKC;  //基础课程的课程信息，键：课程号，值：课程信息结构体
    map<int, MUSIC_INFO> _mapQMXS; //曲目赏析，键：课程号，值：课程信息结构体
    map<QString, int> _mapLeftPressBtn; // 键：左侧按钮名称，值：左侧这个按钮选中时观看的是第几页
    AUTO_FIRST autoFirst;

private slots:
    void RecevieAutoFirst(QVariant Info);
};

#endif // SYLLABUS_H
