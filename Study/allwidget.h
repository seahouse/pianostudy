#ifndef ALLWIDGET_H
#define ALLWIDGET_H

#include <QObject>
#include <QList>
class AllWidget : public QObject
{
    Q_OBJECT
public:
    explicit AllWidget(QObject *parent = 0);
public:
    void SetRootWindow(QObject*root);
public slots:
    void setAllCorrectLocal(QObject*parent,double scalex,double scaley);
    void saveRectList(QObject *parent);
    void setAllCorrectLocalTWO(QObject *parent, double scalex,double scaley);
    void setAllCorrectLocalTHREE(QObject*,double,double);
private:
    QObject*root;
    QList<QRect*>rList;
    QList<QObject*>oList;
    bool mark;
private:
    void adjustLocal(QObject *parent, double scalex,double scaley);
    void addlistAll(QObject *parent);

};

#endif // ALLWIDGET_H
