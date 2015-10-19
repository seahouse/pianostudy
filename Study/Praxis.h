#ifndef PRAXIS_H
#define PRAXIS_H

#include <QWidget>
#include "GlobalInfo.h"
#include "GlobalInfo.h"

namespace Ui {
class Praxis;
}

class Praxis : public QWidget
{
    Q_OBJECT
signals:
    void SendAnswer(QString sQuestionID, QString sAnswer, bool bIsOK);
public:
    explicit Praxis(QWidget *parent = 0);
    ~Praxis();
    void InitUI();
    void UpdateUI(const PRAXIS_INFO &pi);
    QString GetAnswer();  //返回选择的答案
    void ClickA();
    void ClickB();
    void ClickC();
    void ClickD();

    bool getMark() const;
    void setMark(bool value);

private slots:
    void SetAnswer();
private:
    Ui::Praxis *ui;
    PRAXIS_INFO _pi;
    QString _sAnswer; //正确答案
    bool mark;
};

#endif // PRAXIS_H
