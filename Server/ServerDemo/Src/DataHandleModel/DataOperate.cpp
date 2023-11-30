#pragma execution_character_set("UTF-8")
#include "DataOperate.h"
#include "Src/NetHandleModel/ClientSocketThread.h"
#include "DataBase.h"

DataOperate DataOperate::Ins;

DataOperate::DataOperate()
{
    ClientInfoList.clear();
    LoginUserList.clear();

    pDataBase=new DataBase();
}

DataOperate *DataOperate::Instance()
{
    return &Ins;
}

bool DataOperate::UserRegister(QString userName, QString userID, QString userPassword, QByteArray userHeadImgByte, QString &retMsg)
{
    if(pDataBase->UserRegister(userName,userID,userPassword,userHeadImgByte,retMsg)==false)
        return false;
    return true;
}

bool DataOperate::UserLogOn(QString userID, QString userPassword, QString &retUserName, QByteArray &retUserHeadImgByte, QString &retMsg)
{
    if(pDataBase->UserLogOn(userID,userPassword,retUserName,retUserHeadImgByte,retMsg)==false)
        return false;
    return true;
}

bool DataOperate::UserModifyUserInfo(QString userName, QString userID, QString userPassword, QByteArray userHeadImgByte, QString &retMsg)
{
    if(pDataBase->UserModifyUserInfo(userName,userID,userPassword,userHeadImgByte,retMsg)==false)
        return false;
    return true;
}

void DataOperate::AddClientInfoList(ClientSocketThread *clientSocketThread)
{
    ClientInfoListMutex.lock();
    ClientInfoList.append(clientSocketThread);
    ClientInfoListMutex.unlock();
}

void DataOperate::DeleteClientInfoList(QString clientAddress, quint16 clientPort)
{
    ClientInfoListMutex.lock();
    int ClientInfoListSize=ClientInfoList.size();
    for(int index=0;index<ClientInfoListSize;index++){
        if(ClientInfoList.at(index)->GetIPAddress()==clientAddress&&ClientInfoList.at(index)->GetPort()==clientPort){
            ClientInfoList.at(index)->workFinished();//释放资源 回收内存 防止内存泄漏
            ClientInfoList.removeAt(index);
            break;
        }
    }
    ClientInfoListMutex.unlock();
}

void DataOperate::GetClientInfoList(QList<ClientSocketThread *> &clientInfoList)
{
    ClientInfoListMutex.lock();
    clientInfoList=ClientInfoList;
    ClientInfoListMutex.unlock();
}

void DataOperate::AllClientInfoListClose()
{
    ClientInfoListMutex.lock();
    int ClientInfoListSize=ClientInfoList.size();
    for(int index=0;index<ClientInfoListSize;index++){
        ClientInfoList.at(index)->Disconnect();
    }
    ClientInfoList.clear();
    ClientInfoListMutex.unlock();
}

void DataOperate::UpDateClientInfoListInfo(QString clientAddress, quint16 clientPort, TransferDataModel transferDataModel)
{
    ClientInfoListMutex.lock();
    int ClientInfoListSize=ClientInfoList.size();
    for(int index=0;index<ClientInfoListSize;index++){
        if(ClientInfoList.at(index)->GetIPAddress()==clientAddress&&ClientInfoList.at(index)->GetPort()==clientPort){
            ClientInfoList.at(index)->SetUserID(transferDataModel.UserID);
            ClientInfoList.at(index)->SetUserName(transferDataModel.UserName);
            ClientInfoList.at(index)->SetUserPassword(transferDataModel.UserPassword);
            ClientInfoList.at(index)->SetClientUserStatus(transferDataModel.clientUserStatus);
            break;
        }
    }
    ClientInfoListMutex.unlock();
}

void DataOperate::UpDateClientInfoListInfo(QString UserID, TransferDataModel transferDataModel)
{

}

void DataOperate::NotifyClientInfoList(QString clientAddress, quint16 clientPort, TransferDataModel transferDataModel)
{
    ClientInfoListMutex.lock();
    int ClientInfoListSize=ClientInfoList.size();
    for(int index=0;index<ClientInfoListSize;index++){
        if(ClientInfoList.at(index)->GetIPAddress()==clientAddress&&ClientInfoList.at(index)->GetPort()==clientPort){
            ClientInfoList.at(index)->SendData(transferDataModel);
            break;
        }
    }
    ClientInfoListMutex.unlock();
}

void DataOperate::NotifyClientInfoList(QString targetUserID, TransferDataModel transferDataModel)
{
    ClientInfoListMutex.lock();
    int ClientInfoListSize=ClientInfoList.size();
    for(int index=0;index<ClientInfoListSize;index++){
        if(ClientInfoList.at(index)->GetUserID()==targetUserID){
            ClientInfoList.at(index)->SendData(transferDataModel);
            break;
        }
    }
    ClientInfoListMutex.unlock();
}

void DataOperate::GetCurFileTransferBindID(int &curFileTransferBindID)
{
    CurFileTransferBindIDMutex.lock();
    CurFileTransferBindID++;
    curFileTransferBindID=CurFileTransferBindID;
    CurFileTransferBindIDMutex.unlock();
}

void DataOperate::AddLoginUserList(QString userID)
{
    LoginUserListMutex.lock();
    LoginUserList.append(userID);
    LoginUserListMutex.unlock();
}

void DataOperate::DeleteLoginUserList(QString userID)
{
    LoginUserListMutex.lock();
    int LoginUserListSize=LoginUserList.size();
    for (int index = 0; index < LoginUserListSize; index++){
        if (LoginUserList.at(index) ==userID ){
            LoginUserList.removeAt(index);
            break;
        }
    }
    LoginUserListMutex.unlock();
}

bool DataOperate::JudegUserIsLogIn(QString userID)
{
    LoginUserListMutex.lock();
    int LoginUserListSize=LoginUserList.size();
    for (int index = 0; index < LoginUserListSize; index++){
        if (LoginUserList.at(index) ==userID ){
            LoginUserListMutex.unlock();
            return true;
        }
    }
    LoginUserListMutex.unlock();
    return false;
}


