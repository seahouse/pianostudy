#ifndef AUTOFIRSTDIALOG_H
#define AUTOFIRSTDIALOG_H

#include <QDialog>
#include<QString>
#include "GlobalInfo.h"
#include <QMediaPlayer>
#include<QPixmap>
#include "VideoPlayerWidget.h"

class QProgressBar;

namespace Ui {
class AutoFirstDialog;
}

class AutoFirstDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AutoFirstDialog(QWidget *parent = 0);
    ~AutoFirstDialog();
signals:
    void adjust();

private slots:
    void onDownloadProgress(qint64 bytesSent,qint64 bytesTotal);
    void onStartPlay();

public:
    AutoFirstDialog(AUTO_FIRST autofirst,QWidget *parent=0);
    AutoFirstDialog(CLASS_INFO classInfo,QWidget*parent=0);
    AutoFirstDialog(MUSIC_INFO musicInfo,QWidget*parent=0);
    void SetNetworkPic(const QString &sUrl,QString ways);
    void setWays(const QString &value);

    bool getAutopic() const;
    void setAutopic(bool value);

    bool getAutopicsss() const;
    void setAutopicsss(bool value);

    void setVideoFile(const QString& fileName);
    void setPlayVideo();
    void enabelDownLoad();
    void startPlay();

private:
    void paintEvent(QPaintEvent *);

private:
    Ui::AutoFirstDialog *ui;
    AUTO_FIRST autofirst;
    CLASS_INFO _classInfo;
    MUSIC_INFO _musicInfo;
    QMediaPlayer *player;
    QPixmap *pixmap;
    QString ways;
    bool autopic;
    bool autopicsss;

    QString m_fileName;
    QString _strcachePath;
    QProgressBar *_progressBar;
    DownloadThread *_downloadThread;
    bool _NetworkReplyErro;
    QFileInfo fileInfo;
};

#endif // AUTOFIRSTDIALOG_H
