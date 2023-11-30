#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <mutex>

class QSqlDatabase;
class QSqlQuery;

class DataBase
{
public:
    DataBase();
    ~DataBase();

    bool ConnectDataBase(QSqlDatabase *db);

public:
    //用户注册
    bool UserRegister(QString userName,QString userID,QString userPassword,QByteArray userHeadImgByte,QString &retMsg);
    bool JudgeUserIsExists(QString userID);//判断用户是否存在
    void InsertUserTable(QString userName,QString userID,QString userPassword,QByteArray userHeadImgByte);//插入新用户

    //用户登录
    bool UserLogOn(QString userID,QString userPassword,QString &retUserName,QByteArray &retUserHeadImgByte,QString &retMsg);
    bool JudgeUserIDAndPassword(QString userID,QString userPassword,QString &retUserName,QByteArray &retUserHeadImgByte);//判断用户是否可以登录

    //用户修改个人信息
    bool UserModifyUserInfo(QString userName,QString userID,QString userPassword,QByteArray userHeadImgByte,QString &retMsg);


    void LoadUserTable();
    void InsertTest();
    void SelectTest();




private:
    void InsertUserTablePart(QString userName,QString userID,QString userPassword,QByteArray userHeadImgByte,QSqlQuery *query);

    void InsertTest(int count,QString dateTime,QSqlQuery *query);

public:
    std::mutex DBMutex;
};

#endif // DATABASE_H
