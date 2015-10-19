#ifndef MAINWIDGETTITLE_H
#define MAINWIDGETTITLE_H
#include <QWidget>


class MainWidgetTitle : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidgetTitle(QWidget *parent = 0);

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
public slots:

private:
    int _nX;
    int _nY;
    QWidget *_pParent;
};

#endif // MAINWIDGETTITLE_H
