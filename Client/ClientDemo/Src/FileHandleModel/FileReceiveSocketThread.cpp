#pragma execution_character_set("UTF-8")
#include "FileReceiveSocketThread.h"
#include <Windows.h>
#include <QHostAddress>
#include "Src/DataHandleModel/DataOperate.h"
#include "Src/ComHandleModel/ReflexObject.h"
#include "Src/DataHandleModel/HandleThread.h"
#include <QFile>
#include <QFileInfo>

FileReceiveSocketThread::FileReceiveSocketThread(QString serveIPAdress,QString servePort,QString sendUserID,QString targetUserID,QString fileName)
    :ServeIPAdress(serveIPAdress),ServePort(servePort),SendUserID(sendUserID),TargetUserID(targetUserID),FileName(fileName)
{
    this->connectToHost(QHostAddress(ServeIPAdress), ServePort.toInt());//连接服务器
    if(!this->waitForConnected()){
        qDebug()<<"FileSendSocketThread 连接失败！！！";
    }

    qRegisterMetaType<TransferDataModel>("TransferDataModel&");
    qRegisterMetaType<TransferDataModel>("TransferDataModel");

    qDebug()<<"-------------FileReceiveSocketThread::FileReceiveSocketThread----------";
    //客户端发送消息
    qDebug()<< connect(this,&QTcpSocket::readyRead, this,&FileReceiveSocketThread::clientReadyReadSlots);
    //客户端连接
    qDebug()<< connect(this,&QTcpSocket::connected, this,&FileReceiveSocketThread::clientConnectedSlots);
    //客户端断开
    qDebug()<< connect(this,&QTcpSocket::disconnected, this,&FileReceiveSocketThread::clientDisconnectedSlots);
    //客户端出错
    qDebug()<< connect(this,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),this,&FileReceiveSocketThread::clientErrorSlots);

    qDebug()<< connect(this,&FileReceiveSocketThread::DisconnectSignals, this,&FileReceiveSocketThread::DisconnectSlots);
    qDebug()<< connect(this,&FileReceiveSocketThread::SendDataSignals, this,&FileReceiveSocketThread::SendDataSlots);
}

FileReceiveSocketThread::~FileReceiveSocketThread()
{
    QString  clientInfo= QString("--------- [FileReceiveSocketThread::~FileReceiveSocketThread()] 析构---------");//.arg(ClientAddress).arg(ClientPort);
    qDebug()<<clientInfo;
}

void FileReceiveSocketThread::SendData(TransferDataModel &transferDataModel)
{
    emit SendDataSignals(transferDataModel);
}

//void FileReceiveSocketThread::SetClientAddress(QString clientAddress)
//{
//    ClientAddress=clientAddress;
//}
//void FileReceiveSocketThread::SetClientPort(quint16 clientPort)
//{
//    ClientPort=clientPort;
//}
//QString FileReceiveSocketThread::GetIPAddress()
//{
//    return  ClientAddress;
//}
//quint64 FileReceiveSocketThread::GetPort()
//{
//    return  ClientPort;
//}

void FileReceiveSocketThread::ReceiveFileData(TransferDataModel &transferDataModel)
{
    QByteArray fileByteArr=transferDataModel.FileByteArr;
    QString fileName=transferDataModel.FileName;
    bool isSendFileFinish=transferDataModel.IsSendFileFinish;
    ReceiveFileByteSize+=fileByteArr.size();
    if(pFile==nullptr){
        pFile=new QFile(fileName);
        pFile->open(QFile::WriteOnly);
    }
    pFile->write(fileByteArr);
    emit SetProgressBarCurNum(ReceiveFileByteSize);
    if(isSendFileFinish==true){
        pFile->close();
        delete pFile;
        pFile=nullptr;

        workFinished();
    }
}

void FileReceiveSocketThread::Disconnect()
{
    emit DisconnectSignals();
}

void FileReceiveSocketThread::workFinished()
{
    emit workFinishedSignals();
}

void FileReceiveSocketThread::clientReadyReadSlots()
{
    QTcpSocket *pClient = (QTcpSocket*)this->sender();      //强转TCP类型 可转换 可不转换 本类就是一个QTcpSocket
    QString  clientInfo= QString("---------服务器[%1:%2] 说：---------").arg(pClient->peerAddress().toString()).arg(pClient->peerPort());
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

                ReceiveFileData(transferDataModel);
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

void FileReceiveSocketThread::clientConnectedSlots()
{
    QTcpSocket *pClient = (QTcpSocket*)this->sender();
    QString msg = QString("---------客户端[%1:%2] 连入！---------").arg(pClient->peerAddress().toString()).arg(pClient->peerPort());
    //qDebug()<<"---------客户端连入----------";
}

void FileReceiveSocketThread::clientDisconnectedSlots()
{
    QTcpSocket *pClient = (QTcpSocket*)this->sender(); //强转TCP类型
    QString msg = QString("---------客户端[%1:%2]退出！---------").arg(pClient->peerAddress().toString()).arg(pClient->peerPort());
    qDebug()<<"---------客户端退出----------";
}

void FileReceiveSocketThread::clientErrorSlots(QAbstractSocket::SocketError err)
{
    QTcpSocket *pClient = (QTcpSocket*)this->sender(); //强转TCP类型
    qDebug()<<"---------客户端出错----------";
    qDebug() << pClient->error();
    qDebug() << pClient->errorString();
    qDebug() << err;
}

void FileReceiveSocketThread::SendDataSlots(TransferDataModel &transferDataModel)
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
}

void FileReceiveSocketThread::DisconnectSlots()
{
    this->disconnectFromHost();
}
