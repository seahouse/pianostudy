#ifndef CAPBTN_H
#define CAPBTN_H
#include <QPushButton>

class CapBtn : public QPushButton
{
public:
    CapBtn(QWidget * parent = 0);
    ~CapBtn();



private:
    void paintEvent(QPaintEvent*event);

};

#endif // CAPBTN_H
