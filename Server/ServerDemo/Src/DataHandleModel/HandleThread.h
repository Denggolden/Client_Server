#ifndef HANDLETHREAD_H
#define HANDLETHREAD_H

//#include <QObject>
#include "../../Common/ObjectBase.h"
#include <QQueue>
#include <mutex>
#include "../../Common/TransferDataModel.h"

class HandleThread : public ObjectBase
{
    Q_OBJECT
public:
    explicit HandleThread(QObject *parent = nullptr);
    ~HandleThread();

    typedef  struct UserMsgInfo{
        UserMsgInfo(){ClientAddress="";ClientPort=0;Msg="";};
        UserMsgInfo(QString clientAddress,quint16 clientPort,QString msg){ClientAddress=clientAddress;ClientPort=clientPort;Msg=msg;};
        QString ClientAddress;
        quint16 ClientPort;
        QString Msg;
    }UserMsgInfo;

public:
    virtual void InitClass() override;

public:
    void ReceiveMsg(TransferDataModel transferDataModel);

    void HandleMsgLoop();
    void HandleMsg(TransferDataModel &transferDataModel);


public:
    QQueue<TransferDataModel> UserMsgInfoQueue;
    std::mutex MsgQueueMutex;

    QList<QFile*> FileList;

signals:
//    void RegisterRetNotify(TransferDataModel transferDataModel);
//    void NotifyClientLoginSuccess(QString clientAddress,quint16 clientPort,QString userID,QString userPassword,QString userName,QString msg);
//    void NotifyClientSelfInfoList(QString clientAddress,quint16 clientPort,QString msg);
//    void NotifyOtherClientInfoList(QString sendUserID,QString targetUserID,QString msg);
//    void NotifyClientInfoListLogOut(QString userID,QString msg);
};

#endif // HANDLETHREAD_H
