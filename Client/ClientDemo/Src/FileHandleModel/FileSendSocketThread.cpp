#pragma execution_character_set("UTF-8")
#include "FileSendSocketThread.h"
#include <Windows.h>
#include <QHostAddress>
#include "Src/DataHandleModel/DataOperate.h"
#include "Src/ComHandleModel/ReflexObject.h"
#include "Src/DataHandleModel/HandleThread.h"
#include <QFile>
#include <QFileInfo>

FileSendSocketThread::FileSendSocketThread(QString serveIPAdress,QString servePort,QString sendUserID,QString targetUserID,int curFileTransferBindID,QString filePath)
    :ServeIPAdress(serveIPAdress),ServePort(servePort),SendUserID(sendUserID),TargetUserID(targetUserID),CurFileTransferBindID(curFileTransferBindID),FilePath(filePath)
{
    this->connectToHost(QHostAddress(ServeIPAdress), ServePort.toInt());//连接服务器
    if(!this->waitForConnected()){
        qDebug()<<"FileSendSocketThread 连接失败！！！";
    }

    qRegisterMetaType<TransferDataModel>("TransferDataModel&");
    qRegisterMetaType<TransferDataModel>("TransferDataModel");

    qDebug()<<"-------------ClientSocketThread::ClientSocketThread(int sockeDsc)----------";
    //客户端发送消息
    qDebug()<< connect(this,&QTcpSocket::readyRead, this,&FileSendSocketThread::clientReadyReadSlots);
    //客户端连接
    qDebug()<< connect(this,&QTcpSocket::connected, this,&FileSendSocketThread::clientConnectedSlots);
    //客户端断开
    qDebug()<< connect(this,&QTcpSocket::disconnected, this,&FileSendSocketThread::clientDisconnectedSlots);
    //客户端出错
    qDebug()<< connect(this,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),this,&FileSendSocketThread::clientErrorSlots);

    qDebug()<< connect(this,&FileSendSocketThread::DisconnectSignals, this,&FileSendSocketThread::DisconnectSlots);
    qDebug()<< connect(this,static_cast<void(FileSendSocketThread::*)()>(&FileSendSocketThread::SendDataSignals)
                       , this,static_cast<void(FileSendSocketThread::*)()>(&FileSendSocketThread::SendDataSlots));
    qDebug()<< connect(this,static_cast<void(FileSendSocketThread::*)(TransferDataModel &)>(&FileSendSocketThread::SendDataSignals)
                       , this,static_cast<void(FileSendSocketThread::*)(TransferDataModel &)>(&FileSendSocketThread::SendDataSlots));
}

FileSendSocketThread::~FileSendSocketThread()
{
    QString  clientInfo= QString("--------- [FileSendSocketThread::~FileSendSocketThread()] 析构---------");//.arg(ClientAddress).arg(ClientPort);
    qDebug()<<clientInfo;
}

void FileSendSocketThread::SendData(TransferDataModel &transferDataModel)
{
    emit SendDataSignals(transferDataModel);
}

//void FileSendSocketThread::SetClientAddress(QString clientAddress)
//{
//    ClientAddress=clientAddress;
//}
//void FileSendSocketThread::SetClientPort(quint16 clientPort)
//{
//    ClientPort=clientPort;
//}
//QString FileSendSocketThread::GetIPAddress()
//{
//    return  ClientAddress;
//}
//quint64 FileSendSocketThread::GetPort()
//{
//    return  ClientPort;
//}

void FileSendSocketThread::SendFileData()
{
    emit SendDataSignals();
}

void FileSendSocketThread::SendFileData(TransferDataModel &transferDataModel)
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

void FileSendSocketThread::ReceiveFileData(TransferDataModel &transferDataModel)
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

void FileSendSocketThread::Disconnect()
{
    emit DisconnectSignals();
}

void FileSendSocketThread::workFinished()
{
    emit workFinishedSignals();
}

void FileSendSocketThread::clientReadyReadSlots()
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

                if(SendUserID==TargetUserID)
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

void FileSendSocketThread::clientConnectedSlots()
{
    QTcpSocket *pClient = (QTcpSocket*)this->sender();
    QString msg = QString("---------客户端[%1:%2] 连入！---------").arg(pClient->peerAddress().toString()).arg(pClient->peerPort());
    //qDebug()<<"---------客户端连入----------";
}

void FileSendSocketThread::clientDisconnectedSlots()
{
    QTcpSocket *pClient = (QTcpSocket*)this->sender(); //强转TCP类型
    QString msg = QString("---------客户端[%1:%2]退出！---------").arg(pClient->peerAddress().toString()).arg(pClient->peerPort());
    qDebug()<<"---------客户端退出----------";
}

void FileSendSocketThread::clientErrorSlots(QAbstractSocket::SocketError err)
{
    QTcpSocket *pClient = (QTcpSocket*)this->sender(); //强转TCP类型
    qDebug()<<"---------客户端出错----------";
    qDebug() << pClient->error();
    qDebug() << pClient->errorString();
    qDebug() << err;
}

void FileSendSocketThread::SendDataSlots(TransferDataModel &transferDataModel)
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

void FileSendSocketThread::SendDataSlots()
{
    QFile file(FilePath);
    // 获取文件信息
    QFileInfo fileInfo(FilePath);
    QString fileName=fileInfo.fileName();
    file.open(QFile::ReadOnly);
    qint64 allFileSize=fileInfo.size();
    qint64 sendFileSize=0;
    qDebug()<<"fileName: "<<fileName;
    while(true){
        TransferDataModel transferDataModel;
        //QByteArray fileByteArr = file.readLine();//太慢了
        file.seek(sendFileSize);//偏移
        QByteArray fileByteArr = file.read(60000);//减少IO次数
        qDebug()<<"fileByteArr.size(): "<<fileByteArr.size();
        sendFileSize+=fileByteArr.size();
        if(file.atEnd()==true){
            transferDataModel.SetSendFileMsg(SendFileMsg,SendUserID,TargetUserID,CurFileTransferBindID,fileByteArr,fileName,allFileSize,sendFileSize,true);
            SendFileData(transferDataModel);
            emit SetProgressBarCurNum(sendFileSize);
            break;
        }
        else {
            transferDataModel.SetSendFileMsg(SendFileMsg,SendUserID,TargetUserID,CurFileTransferBindID,fileByteArr,fileName,allFileSize,sendFileSize,false);
            SendFileData(transferDataModel);
            emit SetProgressBarCurNum(sendFileSize);
        }
    }

    if(SendUserID!=TargetUserID)
        workFinished();
}

void FileSendSocketThread::DisconnectSlots()
{
    this->disconnectFromHost();
}
