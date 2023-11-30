#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

//#include <QObject>
#include "../../Common/ObjectBase.h"
#include "../../Common/TransferDataModel.h"
#include <mutex>

class QTcpSocket;
class QTimer;

class ClientSocket : public ObjectBase
{
    Q_OBJECT
public:
    explicit ClientSocket(QObject *parent = nullptr);
    ~ClientSocket();

public:
    virtual void InitClass() override;

public:
    void InitSocket();

    void InitKeepAliveTimer();

    void AutoConnectToServerFunc();
    void DisConnectToServerFunc();

    void ForEachNetWorkInfo();

    void SendData(QString message);//向外提供的接口 发送通信数据

    void SendData(const TransferDataModel &transferDataModel);

    void SetServeIPAdressAndServePort(QString serveIPAdress,QString servePort);

    bool JudgeIPAddressIsCorrect(QString ipAddress);
    bool JudgePortIsCorrect(QString port);

    QString GetClientSocketAdress();
    quint16 GetClientSocketPort();

public:
    QTcpSocket *pClientSocket=nullptr; //通信套接字
    QTimer *pTimer=nullptr;
    QTimer *pKeepAliveTimer=nullptr;

    bool ConnectTimeOut;
    bool IsConnectSucceed;
    int ConnectCount;
    std::mutex ConnectTimeOutMutex;
    std::mutex IsConnectSucceedMutex;
    std::mutex ConnectCountMutex;

    QString ServeIPAdress ="";
    QString ServePort ="8520";
    std::mutex ServeIPAdressMutex;
    std::mutex ServePortMutex;

    bool IsSendSuccess=false;
    std::mutex IsSendSuccessMutex;

    QList<QString> IPAddressList;

    std::mutex SendMsgMutex;

    int ReceiveByteSiz=0;
    int ReceiveMsgCount=0;

signals:
    void ConnectToServerFuncSignal(QString msg);
    void SendMsg(const TransferDataModel &transferDataModel);

public slots:
    void ConnectSuccessSlots();
    void ReceiveServerDataSlots();
    void ServerDisconnectedSlots();
    void OnKeepAliveTimeOut();
    void ConnectToServerFunc(QString msg);

    void SendMsgSlots(const TransferDataModel &transferDataModel);

};

#endif // CLIENTSOCKET_H
