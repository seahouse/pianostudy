#ifndef STRUCTINFO
#define STRUCTINFO

#include <QString>

#define NET_WORK QEvent::User+1
#define SYLLABUS_SET_MAX_PAGE QEvent::User+2
#define SYLLABUS_UPDATE_UI QEvent::User+3
#define PRAXIS_SET_MAX_PAGE QEvent::User+4
#define PRAXIS_UPDATE_UI QEvent::User+5
#define UPDATE_COURSE_SCORE QEvent::User+6

struct CLASS_INFO //课程信息
{
    QString sCourseId; //课程ID
    QString sCourseName; //课程名
    QString sTitle; //标题
    QString sType; //类别，1：成人，2：儿童
    QString sPicUrl; //主图片
    QString sNote;   //简介
    QString sContent; //详细内容
    QString sVideoUrl; //视频链接//或音频
    QString sCreateDate; //创建日期
    QString sCtype;//视频还是yingping
    QString sAudio_pic;//音频tupian
    int iScore;         //分数
};

struct MUSIC_INFO
{
    QString sTitle;  //标题
    QString sType;  //类别，1：成人，2：儿童
    QString sPicUrl; //主图片
    QString sIsFollow; //是否跟弹  1:是，2：否
    QString sVideoUrl; //视频链接//或音频
    QString sCreateDate; //创建日期
    QString sCtype;//视频还是yingping
    QString sAudio_pic;//音频tupian
};

struct PRAXIS_INFO
{
    QString sQuestionId; //问题ID
    QString sCourseId; //课程ID
    QString sQuestion; //问题
    QString sRightAnswer; //正确答案
    struct
    {
        QString sAnswerA;
        QString sAnswerB;
        QString sAnswerC;
        QString sAnswerD;
    }ANSWER;// 答案
};

struct AUTO_FIRST
{
    QString sCtype;
    QString sURL;

};

struct LOGIN_RESPONSE
{
    QString sStatus;
    QString sMsg;
};

struct REG_RESPONSE
{
    QString sStatus;
    QString sMsg;
};

struct USER_SCORE
{
    QString sCourseId;
    QString sScore;
};

#define PADDING 2
enum Direction { UP=0, DOWN=1, LEFT, RIGHT, LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP, NONE };

extern QString g_uid;

#endif // STRUCTINFO

