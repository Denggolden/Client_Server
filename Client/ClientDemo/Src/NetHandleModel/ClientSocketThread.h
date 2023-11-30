#ifndef CLIENTSOCKETTHREAD_H
#define CLIENTSOCKETTHREAD_H

#include <QObject>
#include <QTcpSocket>
#include "../../Common/DataModel.h"
#include "../../Common/TransferDataModel.h"

//总的原则是自己管理自己需要上面调用的功能
//自己的事 自己做  别上层调用信号 触发这里的槽
//记住  触发对于socket的相关读写得用信号触发槽函数即可
//能本类做到的本类做 要不然信号触发槽函数很抽象

class ClientSocketThread:public QTcpSocket
{
    Q_OBJECT
public:
    ClientSocketThread(qintptr sockeDsc);
    ~ClientSocketThread();

    void SetClientAddress(QString clientAddress);
    void SetClientPort(quint16 clientPort);

    QString GetIPAddress();
    quint64 GetPort();

    void SendData(TransferDataModel &transferDataModel);
    void Disconnect();
    void workFinished();

public:
    QString ClientAddress="";
    quint16 ClientPort=0;

    int ReceiveByteSiz=0;
    int ReceiveMsgCount=0;

signals:
    void SendDataSignals(TransferDataModel &transferDataModel);
    void DisconnectSignals();
    void workFinishedSignals();

public slots:
    //基本 客户端相关
    void clientReadyReadSlots();
    void clientConnectedSlots();
    void clientDisconnectedSlots();
    void clientErrorSlots(QAbstractSocket::SocketError err);

    //其他 客户端相关
    void SendDataSlots(TransferDataModel &transferDataModel);
    void DisconnectSlots();
};

#endif // CLIENTSOCKETTHREAD_H
