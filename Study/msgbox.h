#ifndef MSGBOX_H
#define MSGBOX_H

#include <QDialog>

namespace Ui {
class Msgbox;
}

class Msgbox : public QDialog
{
    Q_OBJECT

public:
    explicit Msgbox(QWidget *parent = 0);
    ~Msgbox();

    void setInfo(const QString &title, const QString &info);

private:
    Ui::Msgbox *ui;
};

#endif // MSGBOX_H
