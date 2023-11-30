#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QTcpServer>
#include <QQueue>
#include <mutex>
#include <QList>
#include "../../Common/TransferDataModel.h"


class ClientSocketThread;

class ServerSocket : public QTcpServer//public QObject
{
    Q_OBJECT
public:
    explicit ServerSocket();//(QObject *parent = nullptr);
    ~ServerSocket();

public:
    void InitTcpServer();
    void InitConnect();

public:
    void HandleWaitSendMsgListLoop();
    void ForEachNetWorkInfo();

    bool JudgeIPAddressIsCorrect(QString ipAddress);
    bool JudgePortIsCorrect(QString port);

    void SetServerIPAddress(QString serverIPAddress);
    void SetServerPort(QString serverPort);
    bool StartServerListen(QString serverIPAddress,QString serverPort);
    void StopServerListen();

public:
    QString ServerIPAddress ="";
    QString ServerPort = "8520";
    QList<QString> IPAddressList;

    QList<WaitSendMsg> WaitSendMsgList; //等待发送给未登录用户的消息  积压的消息
    std::mutex WaitSendMsgListMutex;

public:
    //客户端通知相关
//    void RegisterRetNotify(TransferDataModel transferDataModel);
//    void LoginRetNotify(TransferDataModel transferDataModel);

signals:
    void NotifyOtherClientInfoList(QString targetUserID,QString sendUserID,QString msg);

public slots:
    //服务器相关
    void newConnectionSlots();
    void acceptErrorSlots(QAbstractSocket::SocketError err);


//    void NotifyClientLoginSuccessSlots(QString clientAddress,quint16 clientPort,QString userID,QString userPassword,QString userName,QString msg);
//    void NotifyClientSelfInfoListSlots(QString clientAddress,quint16 clientPort,QString msg);
//    void NotifyOtherClientInfoListSlots(QString sendUserID,QString targetUserID,QString msg);
//    void NotifyClientInfoListLogOutSlots(QString userID,QString msg);

protected:
    virtual void incomingConnection(qintptr handle) override;

};

#endif // SERVERSOCKET_H
