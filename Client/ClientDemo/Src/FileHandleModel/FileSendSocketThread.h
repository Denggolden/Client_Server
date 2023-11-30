#ifndef FILESENDSOCKETTHREAD_H
#define FILESENDSOCKETTHREAD_H

#include <QObject>
#include <QTcpSocket>
#include "../../Common/DataModel.h"
#include "../../Common/TransferDataModel.h"

//总的原则是自己管理自己需要上面调用的功能
//自己的事 自己做  别上层调用信号 触发这里的槽
//记住  触发对于socket的相关读写得用信号触发槽函数即可
//能本类做到的本类做 要不然信号触发槽函数很抽象


class FileSendSocketThread:public QTcpSocket
{
    Q_OBJECT
public:
    FileSendSocketThread(QString serveIPAdress,QString servePort,QString sendUserID,QString targetUserID,int curFileTransferBindID,QString filePath);
    ~FileSendSocketThread();

    QString GetIPAddress();
    quint64 GetPort();

    void SendData(TransferDataModel &transferDataModel);
    void SendFileData();
    void SendFileData(TransferDataModel &transferDataModel);
    void ReceiveFileData(TransferDataModel &transferDataModel);
    void Disconnect();
    void workFinished();

public:
    //构造函数初始化列表中变量的初始化顺序与变量的声明顺序关系
    QString ServeIPAdress="";
    QString ServePort=0;

    QString SendUserID="";
    QString TargetUserID="";
    int CurFileTransferBindID=0;
    QString FilePath="";

    QFile* pFile=nullptr;

    int ReceiveByteSiz=0;
    int ReceiveMsgCount=0;
    int ReceiveFileByteSize=0;

signals:
    void SendDataSignals(TransferDataModel &transferDataModel);
    void SendDataSignals();
    void DisconnectSignals();
    void workFinishedSignals();

signals:
    void SetProgressBarCurNum(int curNum);

public slots:
    //基本 客户端相关
    void clientReadyReadSlots();
    void clientConnectedSlots();
    void clientDisconnectedSlots();
    void clientErrorSlots(QAbstractSocket::SocketError err);

    //其他 客户端相关
    void SendDataSlots(TransferDataModel &transferDataModel);
    void SendDataSlots();
    void DisconnectSlots();
};

#endif // FILESENDSOCKETTHREAD_H
