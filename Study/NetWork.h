#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QByteArray>
#include <QJsonObject>
#include <QVariant>
#include "GlobalInfo.h"

Q_DECLARE_METATYPE(CLASS_INFO)
Q_DECLARE_METATYPE(MUSIC_INFO)
Q_DECLARE_METATYPE(PRAXIS_INFO)
Q_DECLARE_METATYPE(AUTO_FIRST)
Q_DECLARE_METATYPE(LOGIN_RESPONSE)
Q_DECLARE_METATYPE(REG_RESPONSE)
Q_DECLARE_METATYPE(USER_SCORE)

class NetWork : public QObject
{
    Q_OBJECT

signals:
    void SendClassInfo(int nId, QVariant varStructInfo); //将解析的json数据发送给相关类的map容器
    void UpdateUI(int nMaxPage);
    void sLoginResponse(QVariant loginResponse);     // 发送登录信息信号
    void regResponse(QVariant response);            // 注册的返回信息信号
    void userscoreResponse(QVariant response);            // 用户信息列表的返回信息信号
public:
    void SetUrl(QObject *pRecvObj, QString sParseUiName, QString sUrl);
    void post(const QString &sUrl, const QString &data);
    static NetWork *GetInstance();
private slots:
    void ReceiveMsg(QNetworkReply *reply);
private:
    void ParseJCKC(const QJsonValue &jsonValue);  //解析基础课程内容
    void ParseQMXS(const QJsonValue &jsonValue);  //解析曲目欣赏内容
    void ParsePraxis(const QJsonValue &jsonValue);  //解析课程习题的内容
    void parseLogin(const QJsonObject &jsonObject);   //解析登录的返回信息
    void parseReg(const QJsonObject &jsonObject);       //解析注册的返回信息
    void parseUserScore(const QJsonArray &jsonArray); //解析用户分数
    void customEvent(QEvent * event);
    void ParseJson(QByteArray &byteArray);
    QString GetFieldValue(QJsonObject &jsonObj ,QString sKey); //获取键对应的值
    explicit NetWork(QObject *parent = 0);
    ~NetWork();
private:
    QNetworkAccessManager *_pNetwork;
    QObject *_pRecvObj;
    QString _sParseUiName;  //解析哪个界面
//修改
signals:
    void SendAutoFirst(QVariant Info);

};

#endif // NETWORK_H
