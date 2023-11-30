#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QTcpSocket>
//服务器+客户端数据模型 或许可以精简

enum CommandType{
    Register=0,RegisterRet,//注册  注册返回
    Login,LoginRet,        //登录  登录返回
    Logout,LogoutRet,      //注销  注销返回
    ModifyUserInfo,ModifyUserInfoRet,//修改个人信息  修改个人信息返回
    //文件收发相关
    SendFileReady,SendFileReadyRet,  //发送文件准备  发送文件准备返回
    ReceiveFileReady,ReceiveFileReadyRet,//接收文件准备  接收文件准备返回
    FileSendTransferBind,FileRecTransferBind,//文件发送接收传输绑定
    SendFileMsg,//正式发送文件
    //添加/删除好友相关
    //添加
    SearchFriends,SearchFriendsRet,//搜索好友  搜索好友返回
    AddFriends,AddFriendsRet,//添加好友  添加好友返回
    NotifyAddFriends,NotifyAddFriendsRet,//通知添加好友  通知添加好友返回
    RecAddFriends,RecAddFriendsRet,//接受添加好友  接受添加好友返回
//    AddFriends,
//    SendMsgToSelf,SendMsgToOtherUser,SendMsgToServer
};

enum ClientUserStatus{
    OnLine=0,Busy,DoNotDisturb,Stealth,OffLine
};

enum SendMsgType{
    TextMsg=0,FileMsg
};

typedef struct ClientInfo{
    ClientInfo(){ClientSocket=nullptr;UserName="";UserID="";UserPassword="";ClientUserStatus=OffLine;};
    ClientInfo(QTcpSocket* clientSocket,QString userName,QString userID,QString userPassword,ClientUserStatus clientUserStatus){ClientSocket=clientSocket;UserName=userName;UserID=userID;UserPassword=userPassword;ClientUserStatus=clientUserStatus;};
    QTcpSocket* ClientSocket;
    QString UserName;
    QString UserID;
    QString UserPassword;
    ClientUserStatus ClientUserStatus;
}ClientInfo;

typedef struct RegisterUserInfo{
    RegisterUserInfo(){UserName="",UserID="";UserPassword="";};
    RegisterUserInfo(QString userName,QString userID,QString userPassword){UserName=userName;UserID=userID;UserPassword=userPassword;};
    QString UserName;
    QString UserID;
    QString UserPassword;
}UserInfo ;

typedef struct WaitSendMsg{//归为同一类消息，只有可能是发给不同设备的信息才可能进入等待发送消息
    //用户发给其他用户的 服务器发给其他用户的  //Msg解析出消息即可
    WaitSendMsg(){TargetUserID="";SendUserID="";Msg="";};
    WaitSendMsg(QString sendUserID,QString targetUserID,QString msg){SendUserID=sendUserID;TargetUserID=targetUserID;Msg=msg;};
    QString SendUserID;
    QString TargetUserID;
    QString Msg;
    //原因是removeOne();或者其它删除操作,括号中的值实际上要去list中去判等操作,自定义结构体没有重载 ==符号,只要重载即可.
    bool operator==(const WaitSendMsg& t){
        return (TargetUserID == t.TargetUserID) && (SendUserID == t.SendUserID) &&(Msg == t.Msg);
    }
}WaitSendMsg;

class DataModel
{
public:
    DataModel();
};

#endif // DATAMODEL_H
