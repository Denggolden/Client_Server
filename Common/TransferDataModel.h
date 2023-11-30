#ifndef TRANSFERDATAMODEL_H
#define TRANSFERDATAMODEL_H

#include <QObject>
#include <QDataStream>
#include <QFile>
#include <QApplication>
#include "DataModel.h"


class TransferDataModel
{
public:
    TransferDataModel();

    CommandType commandType=Register;//命令类型...
    ClientUserStatus clientUserStatus=OffLine;
    //登录成功前
    QString UserName="";//注册+修改个人信息...
    QString UserID="";//注册+登录...
    QString UserPassword="";//注册+登录+修改个人信息...
    QString ClientAddress="";
    quint16 ClientPort=0;

    //登录成功后
    QString SendUserID="";//加好友+发送消息给其他人...
    QString TargetUserID="";//加好友+发送消息给其他人...
    QString FileTransferMyID="";//文件传输方的ID
    int CurFileTransferBindID=0;

    //公用消息
    //SendMsgType SendMsgType;
    QString TextMsg="";//发送消息给自己+给服务器+给好友...//通用消息

    QByteArray FileByteArr;
    QString FileName="";
    QString FilePath="";
    qint64 AllFileSize=0;
    qint64 SendFileSize=0;

    bool IsSendFileFinish=false;

    bool IsSeverRetSuccess=false;//服务器执行操作成功/失败的标志

    //注册调用
    void SetRegisterInfo(CommandType command_Type,QString userName,QString userID,QString userPassword,QByteArray fileByteArr);
    void SetRegisterRetInfo(CommandType command_Type,bool isSeverRetSuccess,QString textMsg);

    //登录调用
    void SetLoginInfo(CommandType command_Type,QString userID,QString userPassword);
    void SetLoginRetInfo(CommandType command_Type,bool isSeverRetSuccess,QString textMsg,QString userID,QString userPassword,QString userName,QByteArray fileByteArr);

    //修改个人信息调用
    void SetModifyUserInfo(CommandType command_Type,QString userName,QString userID,QString userPassword,QByteArray fileByteArr);
    void SetModifyUserInfoRet(CommandType command_Type,bool isSeverRetSuccess,QString textMsg);

    //注销调用
    void SetLogoutInfo(CommandType command_Type,QString userID);
    void SetLogoutRetInfo(CommandType command_Type,bool isSeverRetSuccess,QString textMsg);

    //加好友调用
    void SetSendAddFriendsInfo(CommandType command_Type,QString sendUserID,QString targetUserID);
    void SetSendAddFriendsRetInfo(CommandType command_Type,bool isSeverRetSuccess,QString textMsg);

    //发送文件调用
    void SetSendFileReadyInfo(CommandType command_Type,QString sendUserID,QString targetUserID,QString filePath,qint64 allFileSize);
    void SetSendFileReadyRetInfo(CommandType command_Type,QString sendUserID,QString targetUserID,int curFileTransferBindID,QString filePath,bool isSeverRetSuccess,QString textMsg);

    void SetReceiveFileReadyInfo(CommandType command_Type,QString sendUserID,QString targetUserID,int curFileTransferBindID,QString filePath,qint64 allFileSize);
    void SetReceiveFileReadyRetInfo(CommandType command_Type,QString sendUserID,QString targetUserID,int curFileTransferBindID,QString filePath,qint64 allFileSize,bool isSeverRetSuccess,QString textMsg);

    void SetFileTransferBindInfo(CommandType command_Type,QString sendUserID,QString targetUserID,int curFileTransferBindID,QString fileTransferMyID,QString filePath);

    void SetSendFileMsg(CommandType command_Type,QString sendUserID,QString targetUserID,int curFileTransferBindID,QByteArray fileByteArr,QString fileName,qint64 allFileSize,qint64 sendFileSize,bool isSendFileFinish);


    // 重写QDataStream& operator<<操作符，做数据序列化操作
    friend QDataStream& operator<<(QDataStream &stream, const TransferDataModel &data);
    // 重写QDataStream& operator>>操作符，做数据反序列化操作
    friend QDataStream& operator>>(QDataStream &stream, TransferDataModel &data);
};

#endif // TRANSFERDATAMODEL_H
