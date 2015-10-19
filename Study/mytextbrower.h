#ifndef MYTEXTBROWER_H
#define MYTEXTBROWER_H

#include <QTextBrowser>

class MyTextBrower : public QTextBrowser
{
    Q_OBJECT

public:
    explicit MyTextBrower(QWidget * parent = 0);
    ~MyTextBrower();

    void appendTextPlainText(const QString &text);

signals:
    void mouseReleased();

protected:
    virtual QVariant loadResource(int type, const QUrl & name);
//    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);
private:
    QPixmap *_pixmap;
};

#endif // MYTEXTBROWER_H
