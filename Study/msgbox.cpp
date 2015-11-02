#include "msgbox.h"
#include "ui_msgbox.h"

Msgbox::Msgbox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Msgbox)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    ui->lblTitle->setObjectName("title");
    ui->lblIcon->setText("");

    setFixedSize(sizeHint());
}

Msgbox::~Msgbox()
{
    delete ui;
}

void Msgbox::setInfo(const QString &title, const QString &info)
{
    ui->lblTitle->setText(" " + title);
    ui->lblInfo->setText(info);
}
