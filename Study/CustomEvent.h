#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H
#include <QEvent>
#include <QString>

class CustomEvent : public QEvent
{
public:
    CustomEvent(QEvent::Type type);
    ~CustomEvent();
public:
    int _nMaxPage; //最大页数
    QString _sCourseId; //课程ID
    int _nScore; //课程得分
};

#endif // CUSTOMEVENT_H
