#pragma execution_character_set("UTF-8")
#include "ServerSocket.h"
#include <QDebug>
#include <QTcpSocket>
#include <thread>
#include "Src/ComHandleModel/ReflexObject.h"
#include "Src/DataHandleModel/HandleThread.h"
#include "ServerMainWin.h"
#include <QNetworkInterface>
#include "../../Common/TransferDataModel.h"
#include <QMetaType>
#include "Src/DataHandleModel/DataOperate.h"
#include "ClientSocketThread.h"
#include <QThread>

ServerSocket::ServerSocket()//(QObject *parent) //: QObject(parent)
{
    WaitSendMsgList.clear();
    IPAddressList.clear();

    ForEachNetWorkInfo();

    InitTcpServer();
    InitConnect();

    HandleWaitSendMsgListLoop();
}

ServerSocket::~ServerSocket()
{
    StopServerListen();
}

void ServerSocket::InitTcpServer()
{
    //设置监听数量
    this->setMaxPendingConnections(1024);

    //connect(this,&QTcpServer::newConnection, this,&ServerSocket::newConnectionSlots);
    connect(this,&QTcpServer::acceptError, this, &ServerSocket::acceptErrorSlots);
}

void ServerSocket::InitConnect()
{
    qRegisterMetaType<TransferDataModel>("TransferDataModel&");
    qRegisterMetaType<TransferDataModel>("TransferDataModel");

    //connect(this,&ServerSocket::NotifyOtherClientInfoList, this,&ServerSocket::NotifyOtherClientInfoListSlots);
}

void ServerSocket::HandleWaitSendMsgListLoop()
{
    std::thread t([=](){
        while(true){
            WaitSendMsgListMutex.lock();
            foreach(auto waitSendMsgItem,WaitSendMsgList){

                WaitSendMsg waitSendMsg=waitSendMsgItem;
                if(DataOperate::Instance()->JudegUserIsLogIn(waitSendMsg.TargetUserID)==true){
                    emit NotifyOtherClientInfoList(waitSendMsg.TargetUserID,waitSendMsg.SendUserID,waitSendMsg.Msg);
                    WaitSendMsgList.removeOne(waitSendMsgItem);
                }

            }
            WaitSendMsgListMutex.unlock();
        }
    });
    t.detach();
}

void ServerSocket::ForEachNetWorkInfo()
{
    IPAddressList.clear();
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();//获取网络接口
    for (int i = 0; i < list.count(); i++){
        QNetworkInterface aInterface = list.at(i);
        qDebug() << "设备名称:" << aInterface.humanReadableName(); //显示每个接口的设备名称
        qDebug() << "硬件地址:" << aInterface.hardwareAddress(); //显示每个接口的硬件地址
        QList<QNetworkAddressEntry> entryList = aInterface.addressEntries();
        for (int j = 0; j < entryList.count(); j++){
            QNetworkAddressEntry aEntry = entryList.at(j);
            qDebug() << "j_index:    "<< j << "  IP地址：" + aEntry.ip().toString();

            if(JudgeIPAddressIsCorrect(aEntry.ip().toString())==true)
                IPAddressList.append(aEntry.ip().toString());
        }
    }

    ServerMainWin *pServerMainWin = dynamic_cast<ServerMainWin*>(ReflexObject::Instance()->GetObjectIns("ServerMainWin"));
    pServerMainWin->UpDateServerIPAddressList(IPAddressList);
    pServerMainWin->UpDateServerPort(ServerPort);
}

bool ServerSocket::JudgeIPAddressIsCorrect(QString ipAddress)
{
    QRegExp regExpIp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    if (!regExpIp.exactMatch(ipAddress)) {
        //QMessageBox::information(this, "提示", "ip格式错误");
        return false;
    }
    return true;
}

bool ServerSocket::JudgePortIsCorrect(QString port)
{
    QRegExp regExpPort("([0-9]|[1-9]\\d{1,3}|[1-5]\\d{4}|6[0-4]\\d{4}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5])");
    if (!regExpPort.exactMatch(port)) {
        //QMessageBox::information(this, "提示", "端口格式错误");
        return false;
    }
    return true;
}

void ServerSocket::SetServerIPAddress(QString serverIPAddress)
{
    ServerIPAddress=serverIPAddress;
}

void ServerSocket::SetServerPort(QString serverPort)
{
    ServerPort=serverPort;
}

bool ServerSocket::StartServerListen(QString serverIPAddress, QString serverPort)
{
    ServerIPAddress=serverIPAddress;
    ServerPort=serverPort;
    QString msg;
    bool ret=false;
    if(serverIPAddress=="Any")
        ret = this->listen(QHostAddress::Any, ServerPort.toInt());//服务器监听绑定
    else if (serverIPAddress=="LocalHost")
        ret = this->listen(QHostAddress::LocalHost, ServerPort.toInt());//服务器监听绑定
    else
        ret = this->listen(QHostAddress(ServerIPAddress), ServerPort.toInt());//服务器监听绑定
    if (!ret){
        msg = "---------绑定失败---------";
        qDebug()<<"msg: "<<msg;
        ServerMainWin *pServerMainWin = dynamic_cast<ServerMainWin*>(ReflexObject::Instance()->GetObjectIns("ServerMainWin"));
        pServerMainWin->AppendTextEditLogText(msg);
        return ret;
    }
    msg = "---------绑定成功---------";
    ServerMainWin *pServerMainWin = dynamic_cast<ServerMainWin*>(ReflexObject::Instance()->GetObjectIns("ServerMainWin"));
    pServerMainWin->AppendTextEditLogText(msg);
    return ret;
}

void ServerSocket::StopServerListen()
{
    if (this && this->isListening()){
        this->close();
        DataOperate::Instance()->AllClientInfoListClose();
        ServerMainWin *pServerMainWin = dynamic_cast<ServerMainWin*>(ReflexObject::Instance()->GetObjectIns("ServerMainWin"));
        pServerMainWin->AppendTextEditLogText("---------断开成功---------");
    }
}

void ServerSocket::newConnectionSlots()
{
//    QTcpSocket *pClient = this->nextPendingConnection();//客户端的连接，获取客户端描述符
//    QString  clientInfo= QString("---------客户端[%1:%2]已连接：---------").arg(pClient->peerAddress().toString()).arg(pClient->peerPort());
//    //客户端发送消息
//    connect(pClient,&QTcpSocket::readyRead, this,&ServerSocket::clientReadyReadSlots);
//    //客户端连接
//    connect(pClient,&QTcpSocket::connected, this,&ServerSocket::clientConnectedSlots);
//    //客户端断开
//    connect(pClient,&QTcpSocket::disconnected, this,&ServerSocket::clientDisconnectedSlots);
//    //客户端出错
//    connect(pClient,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),this,&ServerSocket::clientErrorSlots);
}

void ServerSocket::acceptErrorSlots(QAbstractSocket::SocketError err)
{
    QTcpServer *Server = (QTcpServer*)this->sender(); //强转TCP类型
    qDebug() << Server->errorString();
    qDebug() << err;
    ServerMainWin *pServerMainWin = dynamic_cast<ServerMainWin*>(ReflexObject::Instance()->GetObjectIns("ServerMainWin"));
    pServerMainWin->AppendTextEditLogText("---------服务器出错---------");
    pServerMainWin->AppendTextEditLogText(Server->errorString());
}

void ServerSocket::incomingConnection(qintptr sockeDesc)
{
    ClientSocketThread *pClientSocketThread=new ClientSocketThread(sockeDesc);
    QThread *pThread = new QThread;
    pClientSocketThread->moveToThread(pThread);
    pThread->start();//启动事件循环  用以处理信号对应的槽函数

    //connect(thread,SIGNAL(started()),task,SLOT(doWork()));//非必要
    //从Qt4.4开始，qthreads-no-longer-abstract，run 默认调用 QThread::exec(),即启动消息队列
    connect(pClientSocketThread,SIGNAL(workFinishedSignals()),pThread,SLOT(quit()));//回收标志
    //automatically delete thread and task object when work is done:
    connect(pThread,SIGNAL(finished()),pClientSocketThread,SLOT(deleteLater()));//回收 ClientSocketThread
    connect(pThread,SIGNAL(finished()),pThread,SLOT(deleteLater()));//回收 QThread This signal can be connected to QObject::deleteLater()，to free objects in that thread.

    DataOperate::Instance()->AddClientInfoList(pClientSocketThread);
    ServerMainWin *pServerMainWin = dynamic_cast<ServerMainWin*>(ReflexObject::Instance()->GetObjectIns("ServerMainWin"));
    pServerMainWin->UpDateClientUserTableWidgetList();

    qDebug()<<"pClientSocketThread: "<<pClientSocketThread->GetIPAddress();
    qDebug()<<"pClientSocketThread: "<<pClientSocketThread->GetPort();
}
