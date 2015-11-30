#include "LogonDlg.h"
#include "ui_LogonDlg.h"

#include "NetWork.h"
#include <QMessageBox>
#include <QCalendarWidget>
#include "msgbox.h"

LogonDlg::LogonDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogonDlg)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->move(0, 0);

    ui->_pParentWidget->setStyleSheet("QWidget{border-image: url(:/images/Logon/loginBg.png);}");

    //登录、注册 标题按钮样式
    {
        _sCapBtnStyleSheet = "QPushButton{border-image: url(:/images/Logon/%1.png);font-weight:500;font-size:15px;color:black;}";
        ui->_pCapLoginBtn->setStyleSheet(_sCapBtnStyleSheet.arg("Login_On"));
        ui->_pCapRegisterBtn->setStyleSheet(_sCapBtnStyleSheet.arg("Reg_Off"));
    }

    QString sEditStyleSheet = "QLabel{border-image: url(:/images/Logon/edit.png);}";
    //登录界面样式
    {
        ui->_pLoginIdLab->setStyleSheet("QLabel{border-image: url(:/images/Logon/telIcon.png);}");
        ui->_pLoginIdEdit->setStyleSheet(sEditStyleSheet);
        ui->_pLoginPasswordLab->setStyleSheet("QLabel{border-image: url(:/images/Logon/passwordIcon.png);}");
        ui->_pLoginPasswordEdit->setStyleSheet(sEditStyleSheet);
        ui->_pLoginBtn->setStyleSheet("QPushButton{border-image:url(:/images/Logon/loginBtn.png);}");
        ui->_pLoginExitBtn->setStyleSheet("QPushButton{border-image:url(:/images/Logon/cancelBtn.png);}");
    }
    //注册界面样式
    {
        ui->_pRegisterPhoneEdit->setStyleSheet(sEditStyleSheet);
        ui->_pRegisterPasswordEdit->setStyleSheet(sEditStyleSheet);
        ui->_pRegisterNameEdit->setStyleSheet(sEditStyleSheet);
        ui->_pRegisterAddressEdit->setStyleSheet(sEditStyleSheet);
        ui->_pRegisterDateLab->setStyleSheet(sEditStyleSheet);
        ui->_pRegisterBtn->setStyleSheet("QPushButton{border-image:url(:/images/Logon/registerBtn.png);}");
        ui->_pRegisterExitBtn->setStyleSheet("QPushButton{border-image:url(:/images/Logon/cancelBtn.png);}");
        ui->_pRegisterUserLab->setStyleSheet("QLabel{color:red;}");
    }



    //移动注册界面控件到合适的位置
    {
        ui->_pRegisterUserLab->move(310, 195);
//        ui->_pCalendarWidget->hide();
    }

    //移动各个子界面到合适状态
    {
        ui->_pParentWidget->move(372, 210);
        ui->_pRegisterWidget->setParent(ui->_pParentWidget);
        ui->_pRegisterWidget->move(3, 83);
        ui->_pRegisterWidget->hide();
        ui->_pRegisterWidget->setStyleSheet("QWidget{border-image:non;}");
        ui->_pLoginWidget->setParent(ui->_pParentWidget);
        ui->_pLoginWidget->move(3, 83);
        ui->_pLoginWidget->setStyleSheet("QWidget{border-image:non;}");
    }

    ui->lblMsg->setObjectName("warning");
    ui->lblMsg->setText("");

    ui->deBirthday->setDate(QDate::currentDate());
//    connect(ui->_pRegisterDateBtn, SIGNAL(clicked(bool)), this, SLOT(sSelectDate()));
}

LogonDlg::~LogonDlg()
{
    delete ui;
}

void LogonDlg::on__pCapLoginBtn_clicked()
{
    ui->_pParentWidget->setStyleSheet("QWidget{border-image: url(:/images/Logon/loginBg.png);}");
    ui->_pCapLoginBtn->setStyleSheet(_sCapBtnStyleSheet.arg("Login_On"));
    ui->_pCapRegisterBtn->setStyleSheet(_sCapBtnStyleSheet.arg("Reg_Off"));
    ui->_pParentWidget->setGeometry(372, 210, 456, 280);
    ui->_pLoginWidget->show();
    ui->_pRegisterWidget->hide();
}

void LogonDlg::on__pCapRegisterBtn_clicked()
{
    ui->_pParentWidget->setStyleSheet("QWidget{border-image: url(:/images/Logon/registerBg.png);}");
    ui->_pCapLoginBtn->setStyleSheet(_sCapBtnStyleSheet.arg("Login_Off"));
    ui->_pCapRegisterBtn->setStyleSheet(_sCapBtnStyleSheet.arg("Reg_On"));
    ui->_pParentWidget->setGeometry(372, 150, 456, 400);
    ui->_pRegisterWidget->show();
    ui->_pLoginWidget->hide();
}


void LogonDlg::on__pLoginBtn_clicked()
{
    QString tel = ui->_pLoginIdEdit->text().trimmed();
    QString pwd = ui->_pLoginPasswordEdit->text().trimmed();
    QString sUrl = QString("http://120.55.119.93/course/index.php?m=Api&c=user&a=login&tel=%1&pwd=%2")
            .arg(tel).arg(pwd);

    NetWork *pNetWork = NetWork::GetInstance();
    connect(pNetWork, SIGNAL(sLoginResponse(QVariant)), this, SLOT(sLoginResponse(QVariant)));
    pNetWork->SetUrl(this, objectName(), sUrl);
//    emit Go();
//    this->close();
}

void LogonDlg::on__pLoginExitBtn_clicked()
{
    qApp->exit();
}

void LogonDlg::on__pRegisterBtn_clicked()
{
    QString username = ui->_pRegisterNameEdit->text().trimmed();
    QString pwd = ui->_pRegisterPasswordEdit->text().trimmed();
    QString tel = ui->_pRegisterPhoneEdit->text().trimmed();
    QString addr = ui->_pRegisterAddressEdit->text().trimmed();
//    QString birthday = ui->_pRegisterDateEdit->text().trimmed();
    QString birthday = ui->deBirthday->text().trimmed();
    QString sUrl = QString("http://120.55.119.93/course/index.php?m=Api&c=user&a=reg&username=%1&pwd=%2&tel=%3&addr=%4&birthday=%5")
            .arg(username).arg(pwd).arg(tel).arg(addr).arg(birthday);

    NetWork *pNetWork = NetWork::GetInstance();
    connect(pNetWork, SIGNAL(regResponse(QVariant)), this, SLOT(sRegResponse(QVariant)));
    pNetWork->SetUrl(this, objectName()+"_reg", sUrl);
//    emit Go();
//    this->close();
}

void LogonDlg::on__pRegisterExitBtn_clicked()
{
    qApp->exit();
}

void LogonDlg::sLoginResponse(QVariant loginResponse)
{
    NetWork *pNetWork = NetWork::GetInstance();
    disconnect(pNetWork, SIGNAL(sLoginResponse(QVariant)), this, SLOT(sLoginResponse(QVariant)));

    LOGIN_RESPONSE data = loginResponse.value<LOGIN_RESPONSE>();
    if (data.sStatus == "1")
    {
        g_uid = data.sMsg;
        g_username = data.sUsername;
        emit Go();
        this->close();
    }
    else
    {
        g_uid = "-1";
        ui->lblMsg->setText(tr("密码或手机号错误。"));
//        ui->lblMsg->setFixedSize(ui->lblMsg->sizeHint());

//        Msgbox mb;
//        mb.setInfo(tr("登录失败"), tr("密码或手机号错误。"));
//        mb.exec();

    }
}

void LogonDlg::sRegResponse(QVariant response)
{
    NetWork *pNetWork = NetWork::GetInstance();
    disconnect(pNetWork, SIGNAL(regResponse(QVariant)), this, SLOT(sRegResponse(QVariant)));

    REG_RESPONSE data = response.value<REG_RESPONSE>();
    if (data.sStatus == "1")
    {
        Msgbox mb;
        mb.setInfo(tr("信息"), tr("注册成功。"));
        mb.exec();
        emit Go();
        this->close();
    }
    else
    {
        Msgbox mb;
        mb.setInfo(tr("信息"), tr("注册失败。"));
        mb.exec();
    }
}

void LogonDlg::sSelectDate()
{
//    ui->_pCalendarWidget->move(ui->_pRegisterDateBtn->geometry().x(), ui->_pRegisterDateBtn->geometry().y());
//    ui->_pCalendarWidget->show();
}
