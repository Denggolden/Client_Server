#ifndef DATAOPERATE_H
#define DATAOPERATE_H

#include <QObject>
#include "../../Common/DataModel.h"
#include <mutex>
#include "../../Common/TransferDataModel.h"

class ClientSocketThread;
class DataBase;

class DataOperate
{

public:
    DataOperate(DataOperate&)=delete;
    DataOperate& operator=(const DataOperate&)=delete;
    static DataOperate* Instance();


public:
    bool UserRegister(QString userName,QString userID,QString userPassword,QByteArray userHeadImgByte,QString &retMsg);
    bool UserLogOn(QString userID,QString userPassword,QString &retUserName,QByteArray &retUserHeadImgByte,QString &retMsg);
    bool UserModifyUserInfo(QString userName,QString userID,QString userPassword,QByteArray userHeadImgByte,QString &retMsg);


    void AddClientInfoList(ClientSocketThread* clientSocketThread);
    void DeleteClientInfoList(QString clientAddress,quint16 clientPort);
    void GetClientInfoList(QList <ClientSocketThread*> &clientInfoList);
    void AllClientInfoListClose();
    //还可能存在修改的情况 待定
    //登录成功时更新绑定
    void UpDateClientInfoListInfo(QString clientAddress,quint16 clientPort,TransferDataModel transferDataModel);
    //登录后更新 更新状态/修改用户昵称/修改用户密码/甚至用户头像...具体再看吧 待定
    void UpDateClientInfoListInfo(QString UserID,TransferDataModel transferDataModel);
    //按 ip port通知客户端
    void NotifyClientInfoList(QString clientAddress,quint16 clientPort,TransferDataModel transferDataModel);
    //按 客户端绑定的用户ID通知客户端 通知目标用户
    void NotifyClientInfoList(QString targetUserID,TransferDataModel transferDataModel);

    void GetCurFileTransferBindID(int &curFileTransferBindID);

    void AddLoginUserList(QString userID);
    void DeleteLoginUserList(QString userID);
    bool JudegUserIsLogIn(QString userID);

public:
    QList <ClientSocketThread*> ClientInfoList;   //保存客户端用户连接列表
    std::mutex ClientInfoListMutex;

    int CurFileTransferBindID=0;
    std::mutex CurFileTransferBindIDMutex;

    QList<QString>LoginUserList;//用户登录列表
    std::mutex LoginUserListMutex;

    DataBase *pDataBase;

private:
    DataOperate();
    static DataOperate Ins;
};

#endif // DATAOPERATE_H
