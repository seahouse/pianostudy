#ifndef KEYKNOWLEDGE_H
#define KEYKNOWLEDGE_H

#include <QWidget>
#include <QUrl>

#include "GlobalInfo.h"

namespace Ui {
class KeyKnowledge;
}

class KeyKnowledge : public QWidget
{
    Q_OBJECT

public:
    explicit KeyKnowledge(QWidget *parent = 0);
    ~KeyKnowledge();

    void setHtml(const QString& html, const QUrl& baseUrl = QUrl());

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    void region(const QPoint &cursorGlobalPoint);
    virtual void resizeEvent(QResizeEvent *event);

private slots:
    void on_pbnClose_clicked();

private:
    Ui::KeyKnowledge *ui;

    bool isLeftPressDown;  // 判断左键是否按下
    QPoint dragPosition;   // 窗口移动拖动时需要记住的点
    Direction dir;        // 窗口大小改变时，记录改变方向
    bool mark;
    static double previousWidth;
    static double previousHeight;
};

#endif // KEYKNOWLEDGE_H
