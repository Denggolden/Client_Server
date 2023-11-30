#pragma execution_character_set("UTF-8")
#include "ClientSocketThread.h"
#include <Windows.h>
#include <QHostAddress>
#include "Src/DataHandleModel/DataOperate.h"
#include "Src/ComHandleModel/ReflexObject.h"
#include "ServerMainWin.h"
#include "Src/TestHandleModel/InterfaceTestWin.h"
#include <QFileInfo>
#include "Src/FileHandleModel/FileManageWin.h"

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

void ClientSocketThread::SetUserName(QString userName)
{
    UserName=userName;
}

void ClientSocketThread::SetUserID(QString userID)
{
    UserID=userID;
}

void ClientSocketThread::SetUserPassword(QString userPassword)
{
    UserPassword=userPassword;
}

void ClientSocketThread::SetClientUserStatus(ClientUserStatus clientUser_Status)
{
    clientUserStatus=clientUser_Status;
}

QString ClientSocketThread::GetIPAddress()
{
    return  ClientAddress;
}

quint64 ClientSocketThread::GetPort()
{
    return  ClientPort;
}

QString ClientSocketThread::GetUser_Name()
{
    return UserName;
}

QString ClientSocketThread::GetUserID()
{
    return  UserID;
}

QString ClientSocketThread::GetUserPassword()
{
    return  UserPassword;
}

ClientUserStatus ClientSocketThread::GetClientUserStatus()
{
    return clientUserStatus;
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

void ClientSocketThread::MsgHandel(TransferDataModel &transferDataModel)
{
    CommandType commandType=transferDataModel.commandType;

    if(commandType==Register){//注册
        QString userName=transferDataModel.UserName;
        QString userID=transferDataModel.UserID;
        QString userPassword=transferDataModel.UserPassword;
        QByteArray userHeadImgByte=transferDataModel.FileByteArr;
        QString retMsg="";
        bool ret=DataOperate::Instance()->UserRegister(userName,userID,userPassword,userHeadImgByte,retMsg);

        TransferDataModel retTransferDataModel;
        retTransferDataModel.SetRegisterRetInfo(RegisterRet,ret,retMsg);
        SendData(retTransferDataModel);
    }
    else if (commandType==Login) {
        QString userID=transferDataModel.UserID;//利用userID添加登录列表即可  加不加登录列表看吧，本身这个就有保存登录用户信息
        QString userPassword=transferDataModel.UserPassword;
        QString retUserName="";
        QByteArray retUserHeadImgByte;
        QString retMsg="";
        bool ret=DataOperate::Instance()->UserLogOn(userID,userPassword,retUserName,retUserHeadImgByte,retMsg);
        if(ret==true){//登录成功设置
            SetUserName(retUserName);
            SetUserID(userID);
            SetUserPassword(userPassword);
            SetClientUserStatus(OnLine);

            ServerMainWin *pServerMainWin = dynamic_cast<ServerMainWin*>(ReflexObject::Instance()->GetObjectIns("ServerMainWin"));
            pServerMainWin->UpDateClientUserTableWidgetList();
        }

        TransferDataModel retTransferDataModel;
        retTransferDataModel.SetLoginRetInfo(LoginRet,ret,retMsg,userID,userPassword,retUserName,retUserHeadImgByte);
        SendData(retTransferDataModel);
    }
    else if (commandType==Logout) {
        QString userID=transferDataModel.UserID;//利用userID删除登录列表即可  加不加登录列表看吧，本身这个就有保存登录用户信息
        QString retMsg="注销成功！！！";//默认注销成功
        bool ret=true;//
        if(ret==true){//注销成功设置
            SetUserName("");
            SetUserID("");
            SetUserPassword("");
            SetClientUserStatus(OffLine);

            ServerMainWin *pServerMainWin = dynamic_cast<ServerMainWin*>(ReflexObject::Instance()->GetObjectIns("ServerMainWin"));
            pServerMainWin->UpDateClientUserTableWidgetList();
        }

        TransferDataModel retTransferDataModel;
        retTransferDataModel.SetModifyUserInfoRet(LogoutRet,ret,retMsg);
        SendData(retTransferDataModel);
    }
    else if (commandType==ModifyUserInfo){
        QString userName=transferDataModel.UserName;
        QString userID=transferDataModel.UserID;//利用userID删除登录列表即可  加不加登录列表看吧，本身这个就有保存登录用户信息
        QString userPassword=transferDataModel.UserPassword;
        QByteArray userHeadImgByte=transferDataModel.FileByteArr;
        QString retMsg="";
        bool ret=DataOperate::Instance()->UserModifyUserInfo(userName,userID,userPassword,userHeadImgByte,retMsg);
        if(ret==true){//修改成功设置
            SetUserName("");
            SetUserID("");
            SetUserPassword("");
            SetClientUserStatus(OffLine);

            ServerMainWin *pServerMainWin = dynamic_cast<ServerMainWin*>(ReflexObject::Instance()->GetObjectIns("ServerMainWin"));
            pServerMainWin->UpDateClientUserTableWidgetList();
        }

        TransferDataModel retTransferDataModel;
        retTransferDataModel.SetModifyUserInfoRet(ModifyUserInfoRet,ret,retMsg);
        SendData(retTransferDataModel);
    }
    //文件传输相关
    else if (commandType==SendFileReady){
        QString sendUserID=transferDataModel.SendUserID;
        QString targetUserID=transferDataModel.TargetUserID;
        QString filePath=transferDataModel.FilePath;
        qint64 allFileSize=transferDataModel.AllFileSize;
        qDebug()<<QString("SendFileReady-----sendUserID: %1---targetUserID: %2---filePath: %3").arg(sendUserID).arg(targetUserID).arg(filePath);
        if(targetUserID=="Server"){//如果文件是发给服务器的则服务端自行处理 （这里可以创建服务器接收窗口 显示实时进度）
            int curFileTransferBindID=0;
            DataOperate::Instance()->GetCurFileTransferBindID(curFileTransferBindID);
            transferDataModel.CurFileTransferBindID=curFileTransferBindID;
//            InterfaceTestWin *pInterfaceTestWin = dynamic_cast<InterfaceTestWin*>(ReflexObject::Instance()->GetObjectIns("InterfaceTestWin"));
//            pInterfaceTestWin->HandelOperate(transferDataModel);

            FileManageWin *pFileManageWin = dynamic_cast<FileManageWin*>(ReflexObject::Instance()->GetObjectIns("FileManageWin"));
            pFileManageWin->HandelOperate(transferDataModel);

            bool ret=true;//默认准备成功
            QString retMsg="";
            TransferDataModel retTransferDataModel;
            retTransferDataModel.SetSendFileReadyRetInfo(SendFileReadyRet,sendUserID,targetUserID,curFileTransferBindID,filePath,ret,retMsg);
            SendData(retTransferDataModel);
        }
        else if(targetUserID==sendUserID){//如果文件是发给自己的则不做任何处理 或看如何处理（直接原路返回）
            int curFileTransferBindID=0;
            bool ret=true;//默认准备成功
            QString retMsg="";
            TransferDataModel retTransferDataModel;
            retTransferDataModel.SetSendFileReadyRetInfo(SendFileReadyRet,sendUserID,targetUserID,curFileTransferBindID,filePath,ret,retMsg);
            SendData(retTransferDataModel);
            //            int curFileTransferBindID=0;
            //            DataOperate::Instance()->GetCurFileTransferBindID(curFileTransferBindID);
            //            qint64 allFileSize=transferDataModel.AllFileSize;
            //            TransferDataModel retTransferDataModel;
            //            retTransferDataModel.SetReceiveFileReadyInfo(ReceiveFileReady,sendUserID,targetUserID,curFileTransferBindID,filePath,allFileSize);
            //            DataOperate::Instance()->NotifyClientInfoList(targetUserID,retTransferDataModel);//通知接收者
        }
        else{//文件发给其他用户的 则通知其他用户准备接收文件
            int curFileTransferBindID=0;
            DataOperate::Instance()->GetCurFileTransferBindID(curFileTransferBindID);
            TransferDataModel retTransferDataModel;
            retTransferDataModel.SetReceiveFileReadyInfo(ReceiveFileReady,sendUserID,targetUserID,curFileTransferBindID,filePath,allFileSize);
            DataOperate::Instance()->NotifyClientInfoList(targetUserID,retTransferDataModel);//通知接收者
        }
    }
    else if (commandType==ReceiveFileReadyRet) {
        QString sendUserID=transferDataModel.SendUserID;
        QString targetUserID=transferDataModel.TargetUserID;
        int curFileTransferBindID=transferDataModel.CurFileTransferBindID;
        QString filePath=transferDataModel.FilePath;
        qDebug()<<QString("ReceiveFileReadyRet-----sendUserID: %1---targetUserID: %2---filePath: %3---curFileTransferBindID: %4").arg(sendUserID).arg(targetUserID).arg(filePath).arg(curFileTransferBindID);
        //注意要明白此时这个TcpSocket是谁和谁   别发错了通道
        if(sendUserID=="Server"){//如果文件发送者是服务器 接收的是其他客户端   已经连接成功可以直接发送了
            //SendFile(sendUserID,targetUserID,curFileTransferBindID,filePath);//发送给主TcpSocket了
        }
        else {//如果文件发送者是客户端 接收的是其他客户端
            bool ret=true;//默认准备成功
            QString retMsg="";
            TransferDataModel retTransferDataModel;
            retTransferDataModel.SetSendFileReadyRetInfo(SendFileReadyRet,sendUserID,targetUserID,curFileTransferBindID,filePath,ret,retMsg);
            DataOperate::Instance()->NotifyClientInfoList(sendUserID,retTransferDataModel);//通知发送者
        }
    }
    else if (commandType==FileSendTransferBind||commandType==FileRecTransferBind) {
        QString sendUserID=transferDataModel.SendUserID;
        QString targetUserID=transferDataModel.TargetUserID;
        QString fileTransferMyID=transferDataModel.FileTransferMyID;
        int curFileTransferBindID=transferDataModel.CurFileTransferBindID;
        //把发送方和接收方绑定在一起
        SetUserName("");
        if(commandType==FileSendTransferBind)//发送者标识绑定
            SetUserID(QString("%1_FileSender_%2").arg(sendUserID).arg(curFileTransferBindID));
        else if (commandType==FileRecTransferBind)//接收者标识绑定
            SetUserID(QString("%1_FileReceiver_%2").arg(targetUserID).arg(curFileTransferBindID));
        SetUserPassword("");
        SetClientUserStatus(Busy);
        if(sendUserID=="Server"&&commandType==FileRecTransferBind){//开始发送数据给客户端
            QString filePath=transferDataModel.FilePath;
            SendFile(sendUserID,targetUserID,curFileTransferBindID,filePath);
        }
        //这里应该加一个机制给文件发送者一个信号表明所有准备工作已经做好了  发送者可以开始发送数据了
        //然后发送者直接触发发送即可  得看时机

        ServerMainWin *pServerMainWin = dynamic_cast<ServerMainWin*>(ReflexObject::Instance()->GetObjectIns("ServerMainWin"));
        pServerMainWin->UpDateClientUserTableWidgetList();
        qDebug()<<QString("FileTransferBind-----sendUserID: %1---targetUserID: %2---fileTransferMyID: %3---curFileTransferBindID: %4").arg(sendUserID).arg(targetUserID).arg(fileTransferMyID).arg(curFileTransferBindID);
    }
    else if (commandType==SendFileMsg) {//这里处理的是客户端给客户端/客户端给服务器的 就是说进来的 不是出去的
        QString sendUserID=transferDataModel.SendUserID;
        QString targetUserID=transferDataModel.TargetUserID;
        int curFileTransferBindID=transferDataModel.CurFileTransferBindID;
        if(targetUserID=="Server"){//如果文件是发给服务器的则服务端自行处理
            QByteArray fileByteArr=transferDataModel.FileByteArr;
            QString fileName=transferDataModel.FileName;
            bool isSendFileFinish=transferDataModel.IsSendFileFinish;
            int fileByteArrSize=fileByteArr.size();

            if(pFile==nullptr){
                pFile=new QFile(fileName);
                pFile->open(QFile::WriteOnly);
            }
            //（这里可以刷新服务器接收窗口 显示实时进度）
            pFile->write(fileByteArr);
            if(isSendFileFinish==true){
                pFile->close();
                delete pFile;
                pFile=nullptr;

                //workFinished();不需要
            }
//            InterfaceTestWin *pInterfaceTestWin = dynamic_cast<InterfaceTestWin*>(ReflexObject::Instance()->GetObjectIns("InterfaceTestWin"));
//            pInterfaceTestWin->FileReceiveWinHandle(sendUserID,targetUserID,curFileTransferBindID,fileByteArrSize);

            FileManageWin *pFileManageWin = dynamic_cast<FileManageWin*>(ReflexObject::Instance()->GetObjectIns("FileManageWin"));
            pFileManageWin->FileReceiveWinHandle(sendUserID,targetUserID,curFileTransferBindID,fileByteArrSize);
        }
        else if(targetUserID==sendUserID){//如果文件是发给自己的则不做任何处理 或看如何处理（自己接收自己也行 既做发送者又作接收者）
            SendData(transferDataModel);
            //QString fileReceiver=QString(QString("%1_FileReceiver_%2").arg(targetUserID).arg(curFileTransferBindID));
            //DataOperate::Instance()->NotifyClientInfoList(fileReceiver,transferDataModel);//通知文件接收者
            //qDebug()<<QString("SendFileMsg-----sendUserID: %1---targetUserID: %2---fileReceiver: %3---curFileTransferBindID: %4").arg(sendUserID).arg(targetUserID).arg(fileReceiver).arg(curFileTransferBindID);
        }
        else{//文件发给其他用户的 则通知其他用户准备接收文件
            QString fileReceiver=QString(QString("%1_FileReceiver_%2").arg(targetUserID).arg(curFileTransferBindID));
            DataOperate::Instance()->NotifyClientInfoList(fileReceiver,transferDataModel);//通知文件接收者
            qDebug()<<QString("SendFileMsg-----sendUserID: %1---targetUserID: %2---fileReceiver: %3---curFileTransferBindID: %4").arg(sendUserID).arg(targetUserID).arg(fileReceiver).arg(curFileTransferBindID);
        }
    }
}

void ClientSocketThread::SendFile(QString sendUserID,QString targetUserID,int curFileTransferBindID,QString filePath)
{
    //InterfaceTestWin *pInterfaceTestWin = dynamic_cast<InterfaceTestWin*>(ReflexObject::Instance()->GetObjectIns("InterfaceTestWin"));
    FileManageWin *pFileManageWin = dynamic_cast<FileManageWin*>(ReflexObject::Instance()->GetObjectIns("FileManageWin"));
    QFile file(filePath);
    // 获取文件信息
    QFileInfo fileInfo(filePath);
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
            transferDataModel.SetSendFileMsg(SendFileMsg,sendUserID,targetUserID,curFileTransferBindID,fileByteArr,fileName,allFileSize,sendFileSize,true);
            SendData(transferDataModel);
            pFileManageWin->FileSendWinHandle(sendUserID,targetUserID,curFileTransferBindID,sendFileSize);
            //emit SetProgressBarCurNum(sendFileSize);
            break;
        }
        else {
            transferDataModel.SetSendFileMsg(SendFileMsg,sendUserID,targetUserID,curFileTransferBindID,fileByteArr,fileName,allFileSize,sendFileSize,false);
            SendData(transferDataModel);
            pFileManageWin->FileSendWinHandle(sendUserID,targetUserID,curFileTransferBindID,sendFileSize);
            //emit SetProgressBarCurNum(sendFileSize);
        }
    }

    //    if(SendUserID!=TargetUserID)
    //        workFinished();
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

                //                CommandType commandType=transferDataModel.commandType;
                //                if(commandType==Register||commandType==Login)
                //                    transferDataModel.SetClientAddressAndClientPort(clientAddress,clientPort);

                if(transferDataModel.commandType!=SendFileMsg){//传输文件就别疯狂输出了 UI压力大 存在卡死的可能
                    ServerMainWin *pServerMainWin = dynamic_cast<ServerMainWin*>(ReflexObject::Instance()->GetObjectIns("ServerMainWin"));
                    pServerMainWin->AppendTextEditLogText(transferDataModel);
                }

                MsgHandel(transferDataModel);

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

    QString clientAddress=pClient->peerAddress().toString();
    quint16 clientPort=pClient->peerPort();
    DataOperate::Instance()->DeleteClientInfoList(clientAddress,clientPort);

    ServerMainWin *pServerMainWin = dynamic_cast<ServerMainWin*>(ReflexObject::Instance()->GetObjectIns("ServerMainWin"));
    pServerMainWin->UpDateClientUserTableWidgetList();
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
