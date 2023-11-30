#pragma execution_character_set("UTF-8")
#include "DataBase.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlField>
#include <QCoreApplication>

#include <QDateTime>

DataBase::DataBase()
{

}

DataBase::~DataBase()
{

}

bool DataBase::ConnectDataBase(QSqlDatabase *db)
{
    if(QSqlDatabase::contains("default"))
        (*db) = QSqlDatabase::database("default");
    else
        (*db) = QSqlDatabase::addDatabase("QSQLITE","default");
    (*db).setDatabaseName("DBFile.db");//新增数据库
    bool isOk = (*db).open();//打开数据表
    if(!isOk){
        qDebug()<<"error info :"<<(*db).lastError();
        return false;
    }
    return true;
}

bool DataBase::UserRegister(QString userName, QString userID, QString userPassword, QByteArray userHeadImgByte, QString &retMsg)
{
    if(JudgeUserIsExists(userID)==true){
        retMsg="注册失败---用户已被注册！！！";
        return false;
    }

    InsertUserTable(userName,userID,userPassword,userHeadImgByte);
    retMsg="注册成功！！！";
    return true;
}

bool DataBase::JudgeUserIsExists(QString userID)
{
    bool isExists=false;
    std::lock_guard<std::mutex> mtx_locker(DBMutex);
    {
        QSqlDatabase db;
        if(ConnectDataBase(&db)==false)
            return true;

        //开启事务增加效率
        db.transaction();

        QSqlQuery query(db);
        QString selectSql = QString("SELECT EXISTS(SELECT 1 FROM UserTable WHERE UserID = '%1')").arg(userID);
        query.prepare(selectSql);
        if(!query.exec()){
            qDebug() << "Error: Fail to select table." << query.lastError();
        }
        else{
            qDebug() << "Table select!";
        }

        while(query.next())
        {
            isExists=query.value(0).toBool();//0/1
        }

        db.commit();
        query.clear();
        db.close();
    }
    QSqlDatabase::removeDatabase("default");
    return isExists;
}

void DataBase::InsertUserTable(QString userName, QString userID, QString userPassword, QByteArray userHeadImgByte)
{
    std::lock_guard<std::mutex> mtx_locker(DBMutex);
    {
        QSqlDatabase db;
        if(ConnectDataBase(&db)==false)
            return;

        //开启事务增加效率
        db.transaction();

        QSqlQuery query(db);
        if(db.tables().contains(QString("UserTable"))) {
            //qDebug() << "table already exists!";
            InsertUserTablePart(userName,userID,userPassword,userHeadImgByte,&query);
        } else {
            //表格不存在，则创建表格
            QString createSql = "create table UserTable (UserName varchar(30),UserID varchar(30),UserPassword varchar(30),UserHeadImgByte BLOB)";
            query.prepare(createSql);
            if(!query.exec()){
                qDebug() << "Error: Fail to create table." << query.lastError();
            }
            else{
                qDebug() << "Table created!";
            }
            InsertUserTablePart(userName,userID,userPassword,userHeadImgByte,&query);
        }

        db.commit();
        query.clear();
        db.close();
    }
    QSqlDatabase::removeDatabase("default");
}

bool DataBase::UserLogOn(QString userID, QString userPassword, QString &retUserName, QByteArray &retUserHeadImgByte, QString &retMsg)
{
    if(JudgeUserIDAndPassword(userID,userPassword,retUserName,retUserHeadImgByte)==false){
        retMsg="登录失败---用户名或密码错误或不存在！！！";
        return false;
    }

    retMsg="登录成功！！！";
    return true;
}

bool DataBase::JudgeUserIDAndPassword(QString userID, QString userPassword,QString &retUserName,QByteArray &retUserHeadImgByte)
{
    bool isExists=false;
    std::lock_guard<std::mutex> mtx_locker(DBMutex);
    {
        QSqlDatabase db;
        if(ConnectDataBase(&db)==false)
            return false;

        //开启事务增加效率
        db.transaction();

        QSqlQuery query(db);
        QString selectSql = QString("SELECT UserName,UserHeadImgByte FROM UserTable WHERE UserID='%1' AND UserPassword='%2'").arg(userID).arg(userPassword);
        query.prepare(selectSql);
        if(!query.exec()){
            qDebug() << "Error: Fail to select table." << query.lastError();
        }
        else{
            qDebug() << "Table select!";
        }

        while(query.next())
        {
            isExists=true;
            retUserName=query.value(0).toString();
            retUserHeadImgByte=query.value(1).toByteArray();
        }

        db.commit();
        query.clear();
        db.close();
    }
    QSqlDatabase::removeDatabase("default");
    return isExists;
}

bool DataBase::UserModifyUserInfo(QString userName, QString userID, QString userPassword, QByteArray userHeadImgByte, QString &retMsg)
{
    std::lock_guard<std::mutex> mtx_locker(DBMutex);
    {
        QSqlDatabase db;
        if(ConnectDataBase(&db)==false){
            retMsg="修改失败！！！";
            return false;
        }

        //开启事务增加效率
        db.transaction();

        QSqlQuery query(db);

        QString insertSql = QString("update UserTable set UserName = :UserName,UserPassword = :UserPassword,UserHeadImgByte = :UserHeadImgByte where UserID = :UserID;");
        query.prepare(insertSql);
        query.bindValue(":UserName",userName);query.bindValue(":UserID",userID);query.bindValue(":UserPassword",userPassword);query.bindValue(":UserHeadImgByte",userHeadImgByte);
        if(!query.exec()){
            qDebug() << "Error: Fail to update table." << query.lastError();
        }
        else{
            //qDebug() << "Table insert!";
        }

        db.commit();
        query.clear();
        db.close();
    }
    QSqlDatabase::removeDatabase("default");
    retMsg="修改成功！！！";
    return true;
}

void DataBase::LoadUserTable()
{
    std::lock_guard<std::mutex> mtx_locker(DBMutex);
    {
        QSqlDatabase db;
        if(ConnectDataBase(&db)==false)
            return;

        //开启事务增加效率
        db.transaction();

        QSqlQuery query(db);
        QString selectSql = "select* from UserTable";
        query.prepare(selectSql);
        if(!query.exec()){
            qDebug() << "Error: Fail to select table." << query.lastError();
        }
        else{
            qDebug() << "Table select!";
        }

        while(query.next())
        {
            qDebug()<<"query.value(0).toString(): "<<query.value(0).toString();
            qDebug()<<"query.value(1).toString(): "<<query.value(1).toString();
            qDebug()<<"query.value(2).toString(): "<<query.value(2).toString();
            qDebug()<<"query.value(3).toByteArray().size(): "<<query.value(3).toByteArray().size();
        }

        db.commit();
        query.clear();
        db.close();
    }
    QSqlDatabase::removeDatabase("default");
}

void DataBase::InsertTest()
{
    std::lock_guard<std::mutex> mtx_locker(DBMutex);
    {
        QSqlDatabase db;
        if(ConnectDataBase(&db)==false)
            return;

        //开启事务增加效率
        db.transaction();

        QDateTime beginTime = QDateTime::currentDateTime();//获取系统现在的时间
        QSqlQuery query(db);
        for(int index=1;index<=10;index++){
            QString dateTime= beginTime .addDays(0-index).toString("yyyy-MM-dd hh:mm:ss");
            if(db.tables().contains(QString("TestTable"))) {
                //qDebug() << "table already exists!";
                InsertTest(index,dateTime,&query);
            } else {
                //表格不存在，则创建表格
                QString createSql = "create table TestTable (DiskNumber int,OpenBottleTime datetime)";
                query.prepare(createSql);
                if(!query.exec()){
                    qDebug() << "Error: Fail to create table." << query.lastError();
                }
                else{
                    qDebug() << "Table created!";
                }
                InsertTest(index,dateTime,&query);
            }
        }
        db.commit();
        query.clear();
        db.close();
    }
    QSqlDatabase::removeDatabase("default");
}

void DataBase::SelectTest()
{
    std::lock_guard<std::mutex> mtx_locker(DBMutex);
    {
        QSqlDatabase db;
        if(ConnectDataBase(&db)==false)
            return;

        //开启事务增加效率
        db.transaction();

        QSqlQuery query(db);
        QString selectSql = QString("select* from TestTable where OpenBottleTime between '%1' and '%2'")
                .arg("2023-08-25 09:09:00").arg("2023-08-29 09:54:59");
        //        QString selectSql = QString("select* from TestTable where OpenBottleTime >='%1' and OpenBottleTime<= '%2'")
        //                .arg("2023-08-25 09:09:00").arg("2023-08-29 09:47:59");
        //均统一成 datetime 并且查询条件格式为 年/月/日 时/分/...
        //开始时间的秒为 00 结束时间的秒为 59 即可
        //生成时间不带 \n的读取出来的时候query.value(1).toString().replace(" "," \n "); 即可空格替换成 空格\n空格即可
        //即可实现表格日期时间换行显示了
        query.prepare(selectSql);
        if(!query.exec()){
            qDebug() << "Error: Fail to select table." << query.lastError();
        }
        else{
            qDebug() << "Table select!";
        }

        while(query.next())
        {
            qDebug()<<"query.value(0).toString(): "<<query.value(0).toString();
            qDebug()<<"query.value(1).toString(): "<<query.value(1).toString().replace(" "," \n ");;
        }

        db.commit();
        query.clear();
        db.close();
    }
    QSqlDatabase::removeDatabase("default");
}

void DataBase::InsertUserTablePart(QString userName, QString userID, QString userPassword, QByteArray userHeadImgByte, QSqlQuery *query)
{
    QString insertSql = QString("insert into UserTable (UserName,UserID,UserPassword,UserHeadImgByte) values (:UserName,:UserID,:UserPassword,:UserHeadImgByte)");
    (*query).prepare(insertSql);
    (*query).bindValue(":UserName",userName);(*query).bindValue(":UserID",userID);(*query).bindValue(":UserPassword",userPassword);(*query).bindValue(":UserHeadImgByte",userHeadImgByte);
    if(!(*query).exec()){
        qDebug() << "Error: Fail to insert table." << (*query).lastError();
    }
    else{
        //qDebug() << "Table insert!";
    }
}

void DataBase::InsertTest(int count, QString dateTime, QSqlQuery *query)
{
    QString insertSql = QString("insert into TestTable values (%1,'%2')")
            .arg(count).arg(dateTime);
    (*query).prepare(insertSql);
    if(!(*query).exec()){
        qDebug() << "Error: Fail to insert table." << (*query).lastError();
    }
    else{
        //qDebug() << "Table insert!";
    }
}
