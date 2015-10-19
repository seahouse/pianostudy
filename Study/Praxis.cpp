#include "Praxis.h"
#include "ui_Praxis.h"
#include<QTextCodec>
#include<QMutexLocker>
#include<QMutex>

#include <QDebug>

Praxis::Praxis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Praxis),_sAnswer(""),mark(false)
{
    ui->setupUi(this);
//    this->hide();
//    QString sQuestionStyleSheet = "QTextBrowser{font-size:18px;color:white;border-image:url(:/images/PraxisDlg/titBg.png);}";
    QString sQuestionStyleSheet = ".QWidget{border-image:url(:/images/PraxisDlg/titBg.png);}";
    ui->widget->setStyleSheet(sQuestionStyleSheet);
    ui->lblAnswer->setStyleSheet("QLabel{font-size:18px;}");
//    ui->_pQuestionLab->setStyleSheet(sQuestionStyleSheet);
//    ui->_pQuestionLab->setAlignment(Qt::AlignBottom);
//    ui->_pQuestionLab->setAttribute(Qt::WA_TranslucentBackground, false);
//    QString sAnswerStyleSheet = "QPushButton{font-size:15px;background-color:transparent;text-align:left;}";
    QString sAnswerStyleSheet = "QTextBrowser{font-size:15px;background-color:transparent;text-align:left;}";
    ui->_pQuestionBtn_1->setStyleSheet(sAnswerStyleSheet);
    ui->_pQuestionBtn_1->setFrameShape(QFrame::NoFrame);
    ui->_pQuestionBtn_1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->_pQuestionBtn_2->setStyleSheet(sAnswerStyleSheet);
    ui->_pQuestionBtn_2->setFrameShape(QFrame::NoFrame);
    ui->_pQuestionBtn_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->_pQuestionBtn_3->setStyleSheet(sAnswerStyleSheet);
    ui->_pQuestionBtn_3->setFrameShape(QFrame::NoFrame);
    ui->_pQuestionBtn_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->_pQuestionBtn_4->setStyleSheet(sAnswerStyleSheet);
    ui->_pQuestionBtn_4->setFrameShape(QFrame::NoFrame);
    ui->_pQuestionBtn_4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->_pQuestionBtn_1->setObjectName("A");
    ui->_pQuestionBtn_2->setObjectName("B");
    ui->_pQuestionBtn_3->setObjectName("C");
    ui->_pQuestionBtn_4->setObjectName("D");
    connect(ui->_pQuestionBtn_1, SIGNAL(mouseReleased()), this, SLOT(SetAnswer()));
    connect(ui->_pQuestionBtn_2, SIGNAL(mouseReleased()), this, SLOT(SetAnswer()));
    connect(ui->_pQuestionBtn_3, SIGNAL(mouseReleased()), this, SLOT(SetAnswer()));
    connect(ui->_pQuestionBtn_4, SIGNAL(mouseReleased()), this, SLOT(SetAnswer()));
    this->hide();
}

Praxis::~Praxis()
{
    delete ui;
}

void Praxis::InitUI()
{
    ui->_pQuestionLab->setText("");
    ui->lblAnswer->setText("");
    ui->_pQuestionBtn_1->setText("");
    ui->_pQuestionBtn_2->setText("");
    ui->_pQuestionBtn_3->setText("");
    ui->_pQuestionBtn_4->setText("");
}

void Praxis::UpdateUI(const PRAXIS_INFO &pi)
{
    _pi = pi;
    ui->_pQuestionLab->setHtml(pi.sQuestion);
    ui->_pQuestionBtn_1->setHtml(pi.ANSWER.sAnswerA);
    ui->_pQuestionBtn_1->insertPlainText("A:");
    ui->_pQuestionBtn_2->setHtml(pi.ANSWER.sAnswerB);
    ui->_pQuestionBtn_2->insertPlainText("B:");
    ui->_pQuestionBtn_3->setHtml(pi.ANSWER.sAnswerC);
    ui->_pQuestionBtn_3->insertPlainText("C:");
    ui->_pQuestionBtn_4->setHtml(pi.ANSWER.sAnswerD);
    ui->_pQuestionBtn_4->insertPlainText("D:");
}


QString Praxis::GetAnswer()
{
    return _sAnswer;
}

void Praxis::ClickA()
{
//    ui->_pQuestionBtn_1->clicked();
}

void Praxis::ClickB()
{
//    ui->_pQuestionBtn_2->clicked();
}

void Praxis::ClickC()
{
//    ui->_pQuestionBtn_3->clicked();
}

void Praxis::ClickD()
{
//    ui->_pQuestionBtn_4->clicked();
}


//修改
void Praxis::SetAnswer()
{
    MyTextBrower *pBtn = qobject_cast<MyTextBrower*>(QObject::sender());
    if( pBtn )
    {
        _sAnswer = pBtn->objectName();
        QString str;
        QString result;
        bool mark=false;

        QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
        QString qustion=ui->_pQuestionLab->toHtml();
        QStringList list=qustion.split("（A");
        if(list.length()>=2){
            foreach (str, list) {
                result.append(str);
                if(!mark)
                    result.append("（"+pBtn->objectName());
                mark=true;
            }
            ui->_pQuestionLab->setHtml(result);
            SendAnswer(_pi.sQuestionId, pBtn->objectName(), _pi.sRightAnswer == pBtn->objectName());
            return;
        }
        list=qustion.split("（B");
        if(list.length()>=2){
            foreach (str, list) {
                result.append(str);
                if(!mark)
                    result.append("（"+pBtn->objectName());
                mark=true;
            }
            ui->_pQuestionLab->setText(result);
            SendAnswer(_pi.sQuestionId, pBtn->objectName(), _pi.sRightAnswer == pBtn->objectName());
            return;
        }
        list=qustion.split("（C");
        if(list.length()>=2){
            foreach (str, list) {
                result.append(str);
                if(!mark)
                    result.append("（"+pBtn->objectName());
                mark=true;
            }
            ui->_pQuestionLab->setHtml(result);
            SendAnswer(_pi.sQuestionId, pBtn->objectName(), _pi.sRightAnswer == pBtn->objectName());
            return;
        }
        list=qustion.split("（D");
        if(list.length()>=2){
            foreach (str, list) {
                result.append(str);
                if(!mark)
                    result.append("（"+pBtn->objectName());
                mark=true;
            }
            ui->_pQuestionLab->setHtml(result);
            SendAnswer(_pi.sQuestionId, pBtn->objectName(), _pi.sRightAnswer == pBtn->objectName());
            return;
        }
        list=qustion.split("（");
        if(list.length()>=2){
                    foreach (str, list) {
                        result.append(str);
                        if(!mark)
                            result.append("（"+pBtn->objectName());
                        mark=true;
                    }
                    ui->_pQuestionLab->setHtml(result);
                    SendAnswer(_pi.sQuestionId, pBtn->objectName(), _pi.sRightAnswer == pBtn->objectName());
                    return;
                }
        list=qustion.split("(A");
        if(list.length()>=2){
            foreach (str, list) {
                result.append(str);
                if(!mark)
                    result.append("("+pBtn->objectName());
                mark=true;
            }
            ui->_pQuestionLab->setHtml(result);
            SendAnswer(_pi.sQuestionId, pBtn->objectName(), _pi.sRightAnswer == pBtn->objectName());
            return;
        }
        list=qustion.split("(B");
        if(list.length()>=2){
            foreach (str, list) {
                result.append(str);
                if(!mark)
                    result.append("("+pBtn->objectName());
                mark=true;
            }
            ui->_pQuestionLab->setText(result);
            SendAnswer(_pi.sQuestionId, pBtn->objectName(), _pi.sRightAnswer == pBtn->objectName());
            return;
        }
        list=qustion.split("(C");
        if(list.length()>=2){
            foreach (str, list) {
                result.append(str);
                if(!mark)
                    result.append("("+pBtn->objectName());
                mark=true;
            }
            ui->_pQuestionLab->setHtml(result);
            SendAnswer(_pi.sQuestionId, pBtn->objectName(), _pi.sRightAnswer == pBtn->objectName());
            return;
        }
        list=qustion.split("(D");
        if(list.length()>=2){
            foreach (str, list) {
                result.append(str);
                if(!mark)
                    result.append("("+pBtn->objectName());
                mark=true;
            }
            ui->_pQuestionLab->setHtml(result);
            SendAnswer(_pi.sQuestionId, pBtn->objectName(), _pi.sRightAnswer == pBtn->objectName());
            return;
        }
        list=qustion.split("(");
        if(list.length()>=2){
                    foreach (str, list) {
                        result.append(str);
                        if(!mark)
                            result.append("("+pBtn->objectName());
                        mark=true;
                    }
                    ui->_pQuestionLab->setHtml(result);
                    SendAnswer(_pi.sQuestionId, pBtn->objectName(), _pi.sRightAnswer == pBtn->objectName());
                    return;
                }
        ui->lblAnswer->setText("("+pBtn->objectName()+")");
        return;
    }
}

bool Praxis::getMark() const
{
    return mark;
}

void Praxis::setMark(bool value)
{
    mark = value;
}

