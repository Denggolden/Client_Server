#pragma execution_character_set("UTF-8")
#include "HandleThread.h"
#include <thread>
#include <QDebug>
#include "DataOperate.h"
#include "Src/NetHandleModel/ServerSocket.h"
#include "Src/ComHandleModel/ReflexObject.h"

HandleThread::HandleThread(QObject *parent) : ObjectBase(parent)
{
}

HandleThread::~HandleThread()
{
}

void HandleThread::InitClass()
{
    UserMsgInfoQueue.clear();
    FileList.clear();
    HandleMsgLoop();
}

void HandleThread::ReceiveMsg(TransferDataModel transferDataModel)
{
    std::thread t([=](){
        MsgQueueMutex.lock();
        UserMsgInfoQueue.enqueue(transferDataModel);
        MsgQueueMutex.unlock();
    });
    t.detach();
}

void HandleThread::HandleMsgLoop()
{
    std::thread t([=](){
        while(true){
            MsgQueueMutex.lock();
            while (!UserMsgInfoQueue.isEmpty()) {
                TransferDataModel userMsgInfo=UserMsgInfoQueue.dequeue();
                HandleMsg(userMsgInfo);
            }
            MsgQueueMutex.unlock();
        }
    });
    t.detach();
}

void HandleThread::HandleMsg(TransferDataModel &transferDataModel)
{

//    CommandType commandType=transferDataModel.commandType;

//    if(commandType==Register){//注册
//        QString userName=transferDataModel.UserName;
//        QString userID=transferDataModel.UserID;
//        QString userPassword=transferDataModel.UserPassword;
//        qDebug()<<"Register: "<<"userID: "<<userID <<"   userPassword: "<<userPassword;
//        QString retMsg="";
//        bool ret= DataOperate::Instance()->AddRegisterUserInfoList(userName,userID,userPassword,retMsg);

//        /*上层ServerSocket里面提供接口即可*/
//        /*其实这里仅仅只是数据罢了，没必要这样直接走数据操作中心*/
//        QString clientAddress=transferDataModel.ClientAddress;
//        quint16 clientPort=transferDataModel.ClientPort;

//        TransferDataModel retTransferDataModel;
//        retTransferDataModel.SetRegisterRetInfo(RegisterRet,ret,retMsg,clientAddress,clientPort);

//        //通知指定用户
//        ServerSocket *pServerSocket = dynamic_cast<ServerSocket*>(ReflexObject::Instance()->GetObjectIns("ServerSocket"));
//        //pServerSocket->RegisterRetNotify(retTransferDataModel);
//    }
//    else if(commandType==Login){//登录：具体同一用户ID重复登录看情况而定 看具体策略
//        QString userID=transferDataModel.UserID;
//        QString userPassword=transferDataModel.UserPassword;
//        qDebug()<<"Login: "<<"userID: "<<userID <<"   userPassword: "<<userPassword;
//        QString retMsg="";
//        QString userName="";//看情况吧
//        bool ret= DataOperate::Instance()->CheckUserIDAndUserPassword(userID,userPassword,userName,retMsg);

//        QString clientAddress=transferDataModel.ClientAddress;
//        quint16 clientPort=transferDataModel.ClientPort;

//        TransferDataModel retTransferDataModel;
//        retTransferDataModel.SetLoginRetInfo(LoginRet,ret,retMsg,clientAddress,clientPort,userName,userID,userPassword);

//        //通知用户
//        ServerSocket *pServerSocket = dynamic_cast<ServerSocket*>(ReflexObject::Instance()->GetObjectIns("ServerSocket"));
//        //pServerSocket->LoginRetNotify(retTransferDataModel);
//    }
//    else if (commandType==SendFileMsg) {
//        QByteArray fileByteArr=transferDataModel.FileByteArr;
//        QString fileName=transferDataModel.FileName;
//        bool isSendFileFinish=transferDataModel.IsSendFileFinish;

//        int FileListSize=FileList.size();
//        if(FileListSize==0){
//            QFile* file = new QFile(fileName);
//            FileList.push_back(file);
//            FileList[0]->open(QFile::WriteOnly);
//            FileList[0]->write(fileByteArr);
//            if(isSendFileFinish==true){
//                FileList[0]->close();//并且完成后移除FileList完成的项目
//                FileList.removeAt(0);
//            }
//        }

//        for(int index=0;index<FileListSize;index++){
//            if(FileList.at(index)->fileName()==fileName){
//                FileList[index]->write(fileByteArr);
//                if(isSendFileFinish==true){
//                    FileList[index]->close();//并且完成后移除FileList完成的项目
//                    FileList.removeAt(index);
//                }
//                break;//写完后跳出
//            }
//            if(FileList.at(index)->fileName()!=fileName&&index==(FileListSize-1)){
//                QFile* file = new QFile(fileName);
//                FileList.push_back(file);
//                FileList[FileListSize]->open(QFile::WriteOnly);
//                FileList[FileListSize]->write(fileByteArr);
//                if(isSendFileFinish==true){
//                    FileList[FileListSize]->close();//并且完成后移除FileList完成的项目
//                    FileList.removeAt(FileListSize);
//                }
//            }
//        }
//        //qDebug()<<"fileName: "<<fileName<<"    fileByteArr.size(): "<<fileByteArr.size();
//    }
//    //    else if (commandType==Logout) {//注销：命令号+注销的用户名（用户ID）
//    //        QString userID=ListInfo[1];
//    //        qDebug()<<"Logout: "<<userID;

//    //        QString retMsg="注销成功！！！";
//    //        emit NotifyClientInfoListLogOut(userID,retMsg);
//    //    }
//    //    else if (commandType==SendMsgToServer) {//发送消息到服务器：命令号+消息
//    //        QString sendMsgInfo=ListInfo[1];
//    //        qDebug()<<"SendMsgToServer: "<<sendMsgInfo;
//    //    }
//    //    else if (commandType==SendMsgToSelf) {//发送消息给自己：命令号+消息（发送的消息内容）
//    //        QString sendMsgInfo=ListInfo[1];
//    //        qDebug()<<"SendMsgToSelf: "<<sendMsgInfo;

//    //        QString clientAddress=userMsgInfo.ClientAddress;
//    //        quint16 clientPort=userMsgInfo.ClientPort;

//    //        emit NotifyClientSelfInfoList(clientAddress,clientPort,sendMsgInfo);
//    //    }
//    //    else if (commandType==SendMsgToOtherUser) {//发送消息给其他人/好友：命令号+发送用户名+目标用户名(用户ID)+消息
//    //        QString sendUserID=ListInfo[1];
//    //        QString targetUserID=ListInfo[2];
//    //        QString sendMsgInfo=ListInfo[3];
//    //        qDebug()<<"SendMsgToOtherUsers  ID: "<<targetUserID<<"  Msg: "<<sendMsgInfo;

//    //        emit NotifyOtherClientInfoList(sendUserID,targetUserID,sendMsgInfo);
//    //    }
//    else {//还有其他命令(注册  添加好友等命令)

//    }
}
