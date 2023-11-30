#include "TransferDataModel.h"

TransferDataModel::TransferDataModel()
{

}

void TransferDataModel::SetRegisterInfo(CommandType command_Type, QString userName, QString userID, QString userPassword, QByteArray fileByteArr)
{
    commandType=command_Type;UserName=userName;UserID=userID;UserPassword=userPassword;FileByteArr=fileByteArr;
}

void TransferDataModel::SetRegisterRetInfo(CommandType command_Type, bool isSeverRetSuccess, QString textMsg)
{
    commandType=command_Type;IsSeverRetSuccess=isSeverRetSuccess;TextMsg=textMsg;
}

void TransferDataModel::SetLoginInfo(CommandType command_Type, QString userID, QString userPassword)
{
    commandType=command_Type;UserID=userID;UserPassword=userPassword;
}

void TransferDataModel::SetLoginRetInfo(CommandType command_Type,bool isSeverRetSuccess,QString textMsg,QString userID,QString userPassword,QString userName,QByteArray fileByteArr)
{
    commandType=command_Type;IsSeverRetSuccess=isSeverRetSuccess;TextMsg=textMsg;UserID=userID;UserPassword=userPassword;UserName=userName;FileByteArr=fileByteArr;
}

void TransferDataModel::SetModifyUserInfo(CommandType command_Type, QString userName, QString userID, QString userPassword, QByteArray fileByteArr)
{
    commandType=command_Type;UserName=userName;UserID=userID;UserPassword=userPassword;FileByteArr=fileByteArr;
}

void TransferDataModel::SetModifyUserInfoRet(CommandType command_Type, bool isSeverRetSuccess, QString textMsg)
{
    commandType=command_Type;IsSeverRetSuccess=isSeverRetSuccess;TextMsg=textMsg;
}

void TransferDataModel::SetLogoutInfo(CommandType command_Type, QString userID)
{
    commandType=command_Type;UserID=userID;
}

void TransferDataModel::SetLogoutRetInfo(CommandType command_Type, bool isSeverRetSuccess, QString textMsg)
{
    commandType=command_Type;IsSeverRetSuccess=isSeverRetSuccess;TextMsg=textMsg;
}

void TransferDataModel::SetSendAddFriendsInfo(CommandType command_Type, QString sendUserID, QString targetUserID)
{
    commandType=command_Type;SendUserID=sendUserID;TargetUserID=targetUserID;
}

void TransferDataModel::SetSendAddFriendsRetInfo(CommandType command_Type, bool isSeverRetSuccess, QString textMsg)
{
    commandType=command_Type;IsSeverRetSuccess=isSeverRetSuccess;TextMsg=textMsg;
}

void TransferDataModel::SetSendFileReadyInfo(CommandType command_Type, QString sendUserID, QString targetUserID, QString filePath,qint64 allFileSize)
{
    commandType=command_Type;SendUserID=sendUserID;TargetUserID=targetUserID;FilePath=filePath;AllFileSize=allFileSize;
}

void TransferDataModel::SetSendFileReadyRetInfo(CommandType command_Type, QString sendUserID, QString targetUserID,int curFileTransferBindID, QString filePath, bool isSeverRetSuccess, QString textMsg)
{
    commandType=command_Type;SendUserID=sendUserID;TargetUserID=targetUserID;CurFileTransferBindID=curFileTransferBindID;FilePath=filePath;IsSeverRetSuccess=isSeverRetSuccess;TextMsg=textMsg;
}

void TransferDataModel::SetReceiveFileReadyInfo(CommandType command_Type, QString sendUserID, QString targetUserID,int curFileTransferBindID, QString filePath,qint64 allFileSize)
{
    commandType=command_Type;SendUserID=sendUserID;TargetUserID=targetUserID;CurFileTransferBindID=curFileTransferBindID;FilePath=filePath;AllFileSize=allFileSize;
}

void TransferDataModel::SetReceiveFileReadyRetInfo(CommandType command_Type, QString sendUserID, QString targetUserID,int curFileTransferBindID, QString filePath, qint64 allFileSize, bool isSeverRetSuccess, QString textMsg)
{
    commandType=command_Type;SendUserID=sendUserID;TargetUserID=targetUserID;CurFileTransferBindID=curFileTransferBindID;FilePath=filePath;AllFileSize=allFileSize;IsSeverRetSuccess=isSeverRetSuccess;TextMsg=textMsg;
}

void TransferDataModel::SetFileTransferBindInfo(CommandType command_Type, QString sendUserID, QString targetUserID,int curFileTransferBindID,QString fileTransferMyID,QString filePath)
{
    commandType=command_Type;SendUserID=sendUserID;TargetUserID=targetUserID;CurFileTransferBindID=curFileTransferBindID;FileTransferMyID=fileTransferMyID;FilePath=filePath;
}

void TransferDataModel::SetSendFileMsg(CommandType command_Type,QString sendUserID,QString targetUserID, int curFileTransferBindID,QByteArray fileByteArr, QString fileName,qint64 allFileSize,qint64 sendFileSize, bool isSendFileFinish)
{
    commandType=command_Type;SendUserID=sendUserID;TargetUserID=targetUserID;CurFileTransferBindID=curFileTransferBindID;FileByteArr=fileByteArr;FileName=fileName;AllFileSize=allFileSize;SendFileSize=sendFileSize;IsSendFileFinish=isSendFileFinish;
}

QDataStream &operator<<(QDataStream &stream, const TransferDataModel &data){//序列化
    stream<<data.commandType<<data.clientUserStatus<<data.UserName<<data.UserID<<data.UserPassword<<data.ClientAddress<<data.ClientPort
         <<data.SendUserID<<data.TargetUserID<<data.FileTransferMyID<<data.CurFileTransferBindID<<data.TextMsg<<data.FileByteArr<<data.FileName<<data.FilePath<<data.AllFileSize<<data.SendFileSize
        <<data.IsSendFileFinish<<data.IsSeverRetSuccess;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, TransferDataModel &data){//反序列化
    int commandType=0,clientUserStatus=0;
    stream>>commandType>>clientUserStatus>>data.UserName>>data.UserID>>data.UserPassword>>data.ClientAddress>>data.ClientPort
            >>data.SendUserID>>data.TargetUserID>>data.FileTransferMyID>>data.CurFileTransferBindID>>data.TextMsg>>data.FileByteArr>>data.FileName>>data.FilePath>>data.AllFileSize>>data.SendFileSize
            >>data.IsSendFileFinish>>data.IsSeverRetSuccess;
    data.commandType=CommandType(commandType);
    data.clientUserStatus=ClientUserStatus(clientUserStatus);

    return stream;
}
