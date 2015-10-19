#ifndef LOGON_H
#define LOGON_H

#include <QDialog>
#include <QVariant>

namespace Ui {
class LogonDlg;
}

class LogonDlg : public QDialog
{
    Q_OBJECT

signals:
    void Go(); //点击登陆按钮

public:
    explicit LogonDlg(QWidget *parent = 0);
    ~LogonDlg();

private slots:
    void on__pCapLoginBtn_clicked();

    void on__pCapRegisterBtn_clicked();


    void on__pLoginBtn_clicked();

    void on__pLoginExitBtn_clicked();

    void on__pRegisterBtn_clicked();

    void on__pRegisterExitBtn_clicked();

    void sLoginResponse(QVariant loginResponse);
    void sRegResponse(QVariant response);

private:
    Ui::LogonDlg *ui;
    QString _sCapBtnStyleSheet; //界面上方登录和注册按钮样式
};

#endif // LOGON_H
