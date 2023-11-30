#pragma execution_character_set("UTF-8")
#include "ClientSocketThread.h"
#include <Windows.h>
#include <QHostAddress>
#include "Src/DataHandleModel/DataOperate.h"
#include "Src/ComHandleModel/ReflexObject.h"
#include "Src/DataHandleModel/HandleThread.h"

ClientSocketThread::ClientSocketThread(qintptr sockeDsc)
{
    this->setSocketDescriptor(sockeDsc);
    SetClientAddress(this->peerAddress().toString());
    SetClientPort(this->peerPort());

    qRegisterMetaType<TransferDataModel>("TransferDataModel&");
    qRegisterMetaType<TransferDataModel>("TransferDataModel");

    qDebug()<<"-------------ClientSocketThread::ClientSocketThread(int sockeDsc)----------";
    //客户端发送消息
    qDebug()<< connect(this,&QTcpSocket::readyRead, this,&ClientSocketThread::clientReadyReadSlots);
    //客户端连接
    qDebug()<< connect(this,&QTcpSocket::connected, this,&ClientSocketThread::clientConnectedSlots);
    //客户端断开
    qDebug()<< connect(this,&QTcpSocket::disconnected, this,&ClientSocketThread::clientDisconnectedSlots);
    //客户端出错
    qDebug()<< connect(this,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),this,&ClientSocketThread::clientErrorSlots);

    qDebug()<< connect(this,&ClientSocketThread::DisconnectSignals, this,&ClientSocketThread::DisconnectSlots);
    qDebug()<< connect(this,&ClientSocketThread::SendDataSignals, this,&ClientSocketThread::SendDataSlots);

}

ClientSocketThread::~ClientSocketThread()
{
    QString  clientInfo= QString("---------客户端 [%1:%2] 析构---------").arg(ClientAddress).arg(ClientPort);
    qDebug()<<clientInfo;
}

void ClientSocketThread::SetClientAddress(QString clientAddress)
{
    ClientAddress=clientAddress;
}

void ClientSocketThread::SetClientPort(quint16 clientPort)
{
    ClientPort=clientPort;
}

QString ClientSocketThread::GetIPAddress()
{
    return  ClientAddress;
}

quint64 ClientSocketThread::GetPort()
{
    return  ClientPort;
}

void ClientSocketThread::SendData(TransferDataModel &transferDataModel)
{
    emit SendDataSignals(transferDataModel);
}

void ClientSocketThread::Disconnect()
{
    emit DisconnectSignals();
}

void ClientSocketThread::workFinished()
{
    emit workFinishedSignals();
}

void ClientSocketThread::clientReadyReadSlots()
{
    QTcpSocket *pClient = (QTcpSocket*)this->sender();      //强转TCP类型 可转换 可不转换 本类就是一个QTcpSocket
    QString  clientInfo= QString("---------客户端[%1:%2] 说：---------").arg(pClient->peerAddress().toString()).arg(pClient->peerPort());
    //QString msg=QString::fromLocal8Bit(pClient->readAll());

    while (true) {
        if(ReceiveByteSiz==0){
            QByteArray sizeByteArray;
            if(pClient->bytesAvailable()>=2){
                QByteArray sizeByteArray=pClient->read(2);
                memcpy(&ReceiveByteSiz, sizeByteArray.data(),2);
                //qDebug()<<"outIntVar: "<<ReceiveByteSiz;
            }
            else {
                break;
            }
        }

        if(ReceiveByteSiz>0){
            if(ReceiveByteSiz<=pClient->bytesAvailable()){
                //法一读取（接收）：
                QByteArray dataByteArray=pClient->read(ReceiveByteSiz);
                TransferDataModel transferDataModel;
                QDataStream in(dataByteArray);
                in.setVersion(QDataStream::Qt_5_0);//最好设置数据保存版本
                in >>transferDataModel;

                ReceiveByteSiz=0;
                ReceiveMsgCount++;
            }
            else {
                break;
            }
        }
    }

    //qDebug()<<"ReceiveMsgCount: "<<ReceiveMsgCount;

    //法二读取（接收）：
    //    TransferDataModel transferDataModel;
    //    QDataStream in(pClient);
    //    in.setVersion(QDataStream::Qt_5_0);//最好设置数据保存版本
    //    in >> transferDataModel;

    //    std::thread t([=](){
    //    });
    //    t.detach();
}

void ClientSocketThread::clientConnectedSlots()
{
    QTcpSocket *pClient = (QTcpSocket*)this->sender();
    QString msg = QString("---------客户端[%1:%2] 连入！---------").arg(pClient->peerAddress().toString()).arg(pClient->peerPort());
    //qDebug()<<"---------客户端连入----------";
}

void ClientSocketThread::clientDisconnectedSlots()
{
    QTcpSocket *pClient = (QTcpSocket*)this->sender(); //强转TCP类型
    QString msg = QString("---------客户端[%1:%2]退出！---------").arg(pClient->peerAddress().toString()).arg(pClient->peerPort());
    qDebug()<<"---------客户端退出----------";
}

void ClientSocketThread::clientErrorSlots(QAbstractSocket::SocketError err)
{
    QTcpSocket *pClient = (QTcpSocket*)this->sender(); //强转TCP类型
    qDebug()<<"---------客户端出错----------";
    qDebug() << pClient->error();
    qDebug() << pClient->errorString();
    qDebug() << err;
}

void ClientSocketThread::SendDataSlots(TransferDataModel &transferDataModel)
{
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out <<transferDataModel;

    int intVar= byteArray.size();
    QByteArray sizeByteArray;
    int len_intVar = 2;
    sizeByteArray.resize(len_intVar);
    memcpy(sizeByteArray.data(), &intVar, len_intVar);

    QByteArray sendByteArray=sizeByteArray+byteArray;
    qDebug()<<"sendByteArray.size(): "<<sendByteArray.size();

    int ret = this->write(sendByteArray);
    if (ret <= 0)
        return;

    while (!this->waitForBytesWritten())
        ;
    //this->flush();

    //    //法二  写入（发送）
    //    QDataStream out(ClientInfoList.at(clientIndex).ClientSocket);
    //    out.setVersion(QDataStream::Qt_5_0);
    //    out << transferDataModel;//通过<<运算符直接将tcpData写入out连接的套接字中
    //    ClientInfoList.at(clientIndex).ClientSocket->flush();
}

void ClientSocketThread::DisconnectSlots()
{
    this->disconnectFromHost();
}
