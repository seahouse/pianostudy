#include "NetWork.h"
#include <QtNetwork/QNetworkRequest>
#include <QNetworkReply>
#include <QDataStream>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QEvent>
#include <QCoreApplication>
#include "CustomEvent.h"
#include <QTextDocumentFragment>
#include<QFile>
extern QString g_sUrlHead;

NetWork::NetWork(QObject *parent) : QObject(parent)
{
    _pNetwork = new QNetworkAccessManager(this);
    connect(_pNetwork, SIGNAL(finished(QNetworkReply*)), this, SLOT(ReceiveMsg(QNetworkReply*)));
}

NetWork::~NetWork()
{

}

NetWork* NetWork::GetInstance()
{
    static NetWork nw;
    return &nw;
}

void NetWork::SetUrl(QObject *pRecvObj, QString sParseUiName, QString sUrl)
{
    _pRecvObj = pRecvObj;
    _sParseUiName = sParseUiName;
    _pNetwork->get(QNetworkRequest(QUrl(sUrl)));
}

void NetWork::post(const QString &sUrl, const QString &data)
{
    _pNetwork->post(QNetworkRequest(QUrl(sUrl)), data.toLatin1());
}

void NetWork::customEvent(QEvent * event)
{
//   if(event->type() == SYLLABUS)
//   {
//        CustomEvent *e = static_cast<CustomEvent*>(event);
//        e->accept();
//   }
//   else
//   {
//       QObject::event(event);
//   }
}

void NetWork::ReceiveMsg(QNetworkReply *reply)
{
   // QVariant var = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
   // qDebug() << "ReturnCode:" << QString::number(var.toInt());
    if( reply->error() == QNetworkReply::NoError )
    {
        QByteArray b(reply->readAll());
        // QString s(b);
        // qDebug() << s;
        //解析获取到
        ParseJson(b);
    }
    else
    {
        qDebug() << reply->errorString();
    }
    reply->deleteLater();
}

QString NetWork::GetFieldValue(QJsonObject &jsonObj ,QString sKey)
{
    if(jsonObj.contains(sKey))
    {
        QJsonValue jsonValue = jsonObj.take(sKey);
        if(jsonValue.isString())
        {
            QString sValue = jsonValue.toString();
            return sValue;
        }
        else if (jsonValue.isArray())
        {
            QStringList sValueList;
            QJsonArray valueArray = jsonValue.toArray();
            foreach (QJsonValue jv, valueArray) {
                sValueList.append(jv.toString());
            }
            return sValueList.join("&&&");
        }
        else
        {
            return "ERROR";
        }
    }
    else
    {
        return "ERROR";
    }
}

void NetWork::ParseJCKC(const QJsonValue &jsonValue)
{
    if( jsonValue.isArray() )
    {
        QJsonArray jsonArray = jsonValue.toArray();
        for(int i=0; i < jsonArray.size(); ++i)
        {
            QJsonValue jsonValue = jsonArray.at(i);
            QJsonObject jsonObj = jsonValue.toObject();
            CLASS_INFO ci;
            ci.sCourseId = GetFieldValue(jsonObj, "course_id");
            ci.sCourseName = GetFieldValue(jsonObj, "course_name");
			std::string sCourseName = ci.sCourseName.toStdString();
            ci.sTitle = GetFieldValue(jsonObj, "title");
			std::string sTitle = ci.sTitle.toStdString();
            ci.sType = GetFieldValue(jsonObj, "type");
            ci.sPicUrl = GetFieldValue(jsonObj, "pic_url");
			std::string surl = ci.sPicUrl.toStdString();
            ci.sNote = GetFieldValue(jsonObj, "note");
            ci.sContent = GetFieldValue(jsonObj, "content");
            ci.sVideoUrl = g_sUrlHead+GetFieldValue(jsonObj, "video_url");
            ci.sCreateDate = GetFieldValue(jsonObj, "create_at");

            ci.sCtype=GetFieldValue(jsonObj,"ctype");
            ci.sAudio_pic=g_sUrlHead+GetFieldValue(jsonObj, "audio_pic");


            QVariant var;
            var.setValue(ci);
            emit SendClassInfo(ci.sCourseId.toInt(), var);
        }
    }
}

void NetWork::ParseQMXS(const QJsonValue &jsonValue)
{
    if( jsonValue.isArray() )
    {
        QJsonArray jsonArray = jsonValue.toArray();
        for(int i=0; i < jsonArray.size(); ++i)
        {
            QJsonValue jsonValue = jsonArray.at(i);
            QJsonObject jsonObj = jsonValue.toObject();
            QString sMusicId = GetFieldValue(jsonObj, "music_id");
            MUSIC_INFO mi;
            mi.sTitle = GetFieldValue(jsonObj, "title");
            mi.sType = GetFieldValue(jsonObj, "type");
            mi.sPicUrl = GetFieldValue(jsonObj, "pic_url");
            mi.sIsFollow = GetFieldValue(jsonObj, "is_follow");
            mi.sVideoUrl = g_sUrlHead+GetFieldValue(jsonObj, "video_url");
            mi.sCreateDate = GetFieldValue(jsonObj, "create_at");

            mi.sCtype=GetFieldValue(jsonObj,"ctype");
            mi.sAudio_pic=g_sUrlHead+GetFieldValue(jsonObj, "audio_pic");

            QVariant var;
            var.setValue(mi);
            emit SendClassInfo(sMusicId.toInt(), var);
        }
    }
}

void NetWork::ParsePraxis(const QJsonValue &jsonValue)
{
    if( jsonValue.isArray() )
    {
        QJsonArray jsonArray = jsonValue.toArray();
        for(int i=0; i < jsonArray.size(); ++i)
        {
            QJsonValue jsonValue = jsonArray.at(i);
            QJsonObject jsonObj = jsonValue.toObject();
            PRAXIS_INFO pi;
            pi.sCourseId = GetFieldValue(jsonObj, "course_id");
            pi.sQuestionId = GetFieldValue(jsonObj, "question_id");
            pi.sQuestion = GetFieldValue(jsonObj, "question");
            qDebug() << pi.sQuestion;

            QString s = GetFieldValue(jsonObj, "answer");//"答案1&答案2&答案3&答案4";
            qDebug() << s;

            QStringList sList = s.split("&&&");
            if( sList.size() == 1 )
            {
                pi.ANSWER.sAnswerA = sList.at(0);
            }
            else if( sList.size() == 2 )
            {
                pi.ANSWER.sAnswerA = sList.at(0);
                pi.ANSWER.sAnswerB = sList.at(1);
            }
            else if( sList.size() == 3 )
            {
                pi.ANSWER.sAnswerA = sList.at(0);
                pi.ANSWER.sAnswerB = sList.at(1);
                pi.ANSWER.sAnswerC = sList.at(2);
            }
            else if( sList.size() == 4 )
            {
                pi.ANSWER.sAnswerA = sList.at(0);
                pi.ANSWER.sAnswerB = sList.at(1);
                pi.ANSWER.sAnswerC = sList.at(2);
                pi.ANSWER.sAnswerD = sList.at(3);
            }

            pi.sRightAnswer = GetFieldValue(jsonObj, "right_answer");
            QVariant var;
            var.setValue(pi);
            emit SendClassInfo(pi.sQuestionId.toInt(), var);
        }
    }
}

void NetWork::parseLogin(const QJsonObject &jsonObject)
{
    LOGIN_RESPONSE loginResponse;
    if (jsonObject.contains("status"))
    {
        qDebug() << "status:" << jsonObject.value("status").toString();
        loginResponse.sStatus = jsonObject.value("status").toString();

    }
    if (jsonObject.contains("msg"))
    {
        loginResponse.sMsg = jsonObject.value("msg").toString();
    }
    if (jsonObject.contains("username"))
    {
        loginResponse.sUsername = jsonObject.value("username").toString();
    }
    if (jsonObject.contains("personType"))
    {
        loginResponse.iPersonType = jsonObject.value("personType").toInt();
    }

    QVariant var;
    var.setValue(loginResponse);
    emit sLoginResponse(var);
}

void NetWork::parseReg(const QJsonObject &jsonObject)
{
    REG_RESPONSE loginResponse;
    if (jsonObject.contains("status"))
    {
        qDebug() << "status:" << jsonObject.value("status").toString();
        loginResponse.sStatus = jsonObject.value("status").toString();

    }
    if (jsonObject.contains("msg"))
    {
        loginResponse.sMsg = jsonObject.value("msg").toString();
    }
    if (jsonObject.contains("personType"))
    {
        loginResponse.iPersonType = jsonObject.value("personType").toInt();
    }

    QVariant var;
    var.setValue(loginResponse);
    emit regResponse(var);
}

void NetWork::parseUserScore(const QJsonArray &jsonArray)
{
    QList<USER_SCORE> dataResponse;
    for (int i = 0; i < jsonArray.size(); i++)
    {
        QJsonObject jsonObject = jsonArray.at(i).toObject();

        USER_SCORE userScore;
        userScore.sCourseId = jsonObject.value("course_id").toString();
        userScore.sScore = jsonObject.value("score").toString();

        dataResponse.append(userScore);
    }
//    if (jsonObject.contains("status"))
//    {
//        dataResponse.sStatus = jsonObject.value("status").toString();

//    }
//    if (jsonObject.contains("msg"))
//    {
//        dataResponse.sMsg = jsonObject.value("msg").toString();
//    }

    QVariant var;
    var.setValue(dataResponse);
    emit userscoreResponse(var);
}

void NetWork::ParseJson(QByteArray &byteArray)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoucment = QJsonDocument::fromJson(byteArray, &jsonError);
    if(jsonError.error == QJsonParseError::NoError)
    {
        if( jsonDoucment.isObject() )
        {
            //解析总页数并发送出去
            QJsonObject jsonObj = jsonDoucment.object();
            QString sMaxPage = GetFieldValue(jsonObj, "total");



            //发送最大页数
            CustomEvent *ceSetMaxPage = NULL;
            if( _sParseUiName == "基础课程" || _sParseUiName == "曲目欣赏" )
            {
                ceSetMaxPage = new CustomEvent(QEvent::Type(SYLLABUS_SET_MAX_PAGE));
                ceSetMaxPage->_nMaxPage = sMaxPage.toInt()/8+1;
            }
            else if( _sParseUiName == "课程习题" )
            {
                ceSetMaxPage = new CustomEvent(QEvent::Type(PRAXIS_SET_MAX_PAGE));
                ceSetMaxPage->_total = sMaxPage.toInt();
                ceSetMaxPage->_nMaxPage = sMaxPage.toInt()/7+1;
            }
            if (ceSetMaxPage != NULL)
                QCoreApplication::sendEvent(_pRecvObj, ceSetMaxPage);

            if(jsonObj.contains("rows"))
            {
                QJsonValue jsonValue = jsonObj.take("rows");
                //解析课程
                if( _sParseUiName == "基础课程" )
                {
                   ParseJCKC(jsonValue);
                }
                else if( _sParseUiName == "曲目欣赏" )
                {
                   ParseQMXS(jsonValue);
                }
                else if( _sParseUiName == "课程习题" )
                {
                    qDebug() << byteArray;
                    ParsePraxis(jsonValue);
                }
            }
            else
            {
                if (_sParseUiName == "LogonDlg" || _sParseUiName == "LogonDlg2")  /// 解析登录
                {
                    parseLogin(jsonObj);
                    return;
                }
                else if (_sParseUiName == "LogonDlg_reg" || _sParseUiName == "LogonDlg2_reg")   /// 解析注册
                {
                    parseReg(jsonObj);
                    return;
                }
            }

            //修改
            if(jsonObj.contains("ctype")){
                QString ctype = jsonObj.take("ctype").toString();
                QString audio_pic = jsonObj.take("audio_pic").toString();
                AUTO_FIRST auto_first;
                auto_first.sCtype=ctype;
                auto_first.sURL=audio_pic;
                QVariant var;
                var.setValue(auto_first);
                emit SendAutoFirst(var);
            }
            //发送完数据后更新UI
            CustomEvent *ceUpdateUI = NULL;
            if( _sParseUiName == "基础课程" || _sParseUiName == "曲目欣赏" )
            {
                ceUpdateUI = new CustomEvent(QEvent::Type(SYLLABUS_UPDATE_UI));
            }
            else if( _sParseUiName == "课程习题" )
            {
                ceUpdateUI = new CustomEvent(QEvent::Type(PRAXIS_UPDATE_UI));
            }
            if (ceUpdateUI != NULL)
                QCoreApplication::sendEvent(_pRecvObj, ceUpdateUI);
        }
        else if (jsonDoucment.isArray())
        {
            QJsonArray jsonArray = jsonDoucment.array();
            if (_sParseUiName == tr("基础课程_score"))  /// 解析用户分数
            {
                parseUserScore(jsonArray);
            }
        }
    }
    else
    {
        qDebug() << jsonError.errorString();
    }
}

