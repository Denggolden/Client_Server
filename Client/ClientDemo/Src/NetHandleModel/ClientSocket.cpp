#pragma execution_character_set("utf-8")
#include "ClientSocket.h"
#include <QTcpSocket>
#include <QDebug>
#include <QHostAddress>
#include <QTimer>
#include <Windows.h>
#include <QTextCodec>
#include "Src/ComHandleModel/ReflexObject.h"
#include "NetAssistantWin.h"
#include <QNetworkInterface>
#include "Src/DataHandleModel/HandleThread.h"
#include "Src/DataHandleModel/DataOperate.h"

ClientSocket::ClientSocket(QObject *parent) : ObjectBase(parent)
{
}

ClientSocket::~ClientSocket()
{

}

void ClientSocket::InitClass()
{
    pClientSocket=nullptr;
    ConnectTimeOut=false;
    IsConnectSucceed=false;
    ConnectCount=0;

    InitSocket();
    InitKeepAliveTimer();

    ForEachNetWorkInfo();

    connect(this,SIGNAL(ConnectToServerFuncSignal(QString)),this,SLOT(ConnectToServerFunc(QString)));

    AutoConnectToServerFunc();
}

void ClientSocket::InitSocket()
{
    //分配空间
    pClientSocket = new QTcpSocket();
    QString clientSocketName = "";
    pClientSocket->setObjectName(clientSocketName);
    //ClientSocket->setProxy(QNetworkProxy::NoProxy);

    //连接成功
    connect(pClientSocket, static_cast<void (QTcpSocket::*)()>(&QTcpSocket::connected), this,&ClientSocket::ConnectSuccessSlots);
    //读取服务器信息数据  收数据
    connect(pClientSocket, static_cast<void (QTcpSocket::*)()>(&QTcpSocket::readyRead), this,&ClientSocket::ReceiveServerDataSlots);
    //服务器断开
    connect(pClientSocket, static_cast<void (QTcpSocket::*)()>(&QTcpSocket::disconnected), this,&ClientSocket::ServerDisconnectedSlots);
    //出错函数
    connect(pClientSocket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, [this](QAbstractSocket::SocketError err)
    {
        qDebug() << pClientSocket->error();
        qDebug() << pClientSocket->errorString();
        qDebug() << err;

        ConnectTimeOutMutex.lock();IsConnectSucceedMutex.lock();
        if(IsConnectSucceed==true){//连接成功但是下位异常退出的时候也需要重置
            ConnectCountMutex.lock();
            ConnectCount=0;
            ConnectCountMutex.unlock();
        }
        ConnectTimeOut=true;IsConnectSucceed=false;
        ConnectTimeOutMutex.unlock();IsConnectSucceedMutex.unlock();

        NetAssistantWin *pNetAssistantWin= dynamic_cast<NetAssistantWin*>(ReflexObject::Instance()->GetObjectIns("NetAssistantWin"));
        pNetAssistantWin->ModifyConnectStatus("服务器已断开",0);

        pKeepAliveTimer->stop();//停止定时器
    });
    //发数据
    //connect(ui->SendBtn, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, &SocketNetCom::SendDataSlots);

    qRegisterMetaType<TransferDataModel>("TransferDataModel&");
    qRegisterMetaType<TransferDataModel>("TransferDataModel");

    connect(this,&ClientSocket::SendMsg, this,&ClientSocket::SendMsgSlots);

}

void ClientSocket::InitKeepAliveTimer()
{
    pKeepAliveTimer = new QTimer();
    pKeepAliveTimer->setInterval(15000);//15s的延时
    connect(pKeepAliveTimer,SIGNAL(timeout()),this,SLOT(OnKeepAliveTimeOut()));
    //    pKeepAliveTimer->stop();//停止定时器
    //pKeepAliveTimer->start();//启动定时器
}

void ClientSocket::AutoConnectToServerFunc()
{
    //ui->ConnectBtn->setEnabled(false);
    std::thread t([=](){
        while(true){
            ConnectCountMutex.lock();
            ConnectCount++;
            ConnectCountMutex.unlock();
            emit ConnectToServerFuncSignal(QString("连接中...第[%1]次").arg(ConnectCount));
            ConnectTimeOutMutex.lock();
            ConnectTimeOut=false;
            ConnectTimeOutMutex.unlock();
            while(true){
                ConnectTimeOutMutex.lock();IsConnectSucceedMutex.lock();
                if(ConnectTimeOut==false&&IsConnectSucceed==false){
                }
                else {
                    qDebug()<<"break1";
                    break;
                }
                ConnectTimeOutMutex.unlock();IsConnectSucceedMutex.unlock();
            }
            ConnectTimeOutMutex.unlock();IsConnectSucceedMutex.unlock();

            while(true){
                IsConnectSucceedMutex.lock();
                if(IsConnectSucceed==true){
                }
                else {
                    break;
                }
                IsConnectSucceedMutex.unlock();
            }
            qDebug()<<"break2";
            IsConnectSucceedMutex.unlock();
            Sleep(1500);//这里可以控制连接频率
        }
    });
    t.detach();
}

void ClientSocket::DisConnectToServerFunc()
{
    pClientSocket->disconnectFromHost();
}

void ClientSocket::ForEachNetWorkInfo()
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

    ServeIPAdress = DataOperate::Instance()->GetServerIP();//"192.168.0.87";//"192.168.43.196";//"192.168.0.87";//IPAddressList.at(0);
    ServePort =DataOperate::Instance()->GetServerPort();//"8520";

    NetAssistantWin *pNetAssistantWin = dynamic_cast<NetAssistantWin*>(ReflexObject::Instance()->GetObjectIns("NetAssistantWin"));
    pNetAssistantWin->UpDateServerIPAddressList(IPAddressList);
    pNetAssistantWin->UpDateServerPort(ServePort);
}

void ClientSocket::SendData(QString message)
{
    QString msg = message;
    //int ret = ClientSocket->write(QByteArray::fromHex(msg.toLatin1()));
    //int ret = ClientSocket->write(QByteArray::fromHex(msg.toUtf8()));
    QTextCodec *tc = QTextCodec::codecForName("GBK");
    int ret = pClientSocket->write(tc->fromUnicode(message));
    if (ret <= 0){
        return;
    }
    pClientSocket->flush();
}

void ClientSocket::SendData(const TransferDataModel &transferDataModel)
{
    std::thread t([=](){
        SendMsgMutex.lock();
        emit SendMsg(transferDataModel);
        Sleep(2);//3毫秒的延时
        SendMsgMutex.unlock();
    });
    t.detach();

//    //法一  写入（发送）

//    QByteArray byteArray;
//    QDataStream out(&byteArray, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_0);
//    out <<transferDataModel;
//    int ret = pClientSocket->write(byteArray);
//    if (ret <= 0){
//        return;
//    }


    //    //法二  写入（发送）
    //    QDataStream out(pClientSocket);
    //    out.setVersion(QDataStream::Qt_5_0);
    //    out << transferDataModel;//通过<<运算符直接将tcpData写入out连接的套接字中
    //    pClientSocket->flush();
}

void ClientSocket::SetServeIPAdressAndServePort(QString serveIPAdress, QString servePort)
{
    ServeIPAdressMutex.lock();ServePortMutex.lock();
    ServeIPAdress=serveIPAdress;ServePort=servePort;
    ServeIPAdressMutex.unlock();ServePortMutex.unlock();
}

bool ClientSocket::JudgeIPAddressIsCorrect(QString ipAddress)
{
    QRegExp regExpIp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    if (!regExpIp.exactMatch(ipAddress)) {
        //QMessageBox::information(this, "提示", "ip格式错误");
        return false;
    }
    return true;
}

bool ClientSocket::JudgePortIsCorrect(QString port)
{
    QRegExp regExpPort("([0-9]|[1-9]\\d{1,3}|[1-5]\\d{4}|6[0-4]\\d{4}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5])");
    if (!regExpPort.exactMatch(port)) {
        //QMessageBox::information(this, "提示", "端口格式错误");
        return false;
    }
    return true;
}

QString ClientSocket::GetClientSocketAdress()
{
    return  pClientSocket->peerAddress().toString();
}

quint16 ClientSocket::GetClientSocketPort()
{
    return pClientSocket->peerPort();
}

void ClientSocket::ConnectSuccessSlots()
{
    QString msg = QString("连上服务器了！");
    qDebug()<<msg;

    IsConnectSucceedMutex.lock();
    IsConnectSucceed=true;
    IsConnectSucceedMutex.unlock();
    ConnectCountMutex.lock();
    ConnectCount=0;
    ConnectCountMutex.unlock();

    NetAssistantWin *pNetAssistantWin= dynamic_cast<NetAssistantWin*>(ReflexObject::Instance()->GetObjectIns("NetAssistantWin"));
    pNetAssistantWin->ModifyConnectStatus("服务器已连接",2);
}

void ClientSocket::ReceiveServerDataSlots()
{
    QTcpSocket *pClient = (QTcpSocket*)this->sender();      //强转TCP类型
    QString  clientInfo= QString("---------客户端[%1:%2] 说：---------").arg(pClient->peerAddress().toString()).arg(pClient->peerPort());
    //QString msg=QString::fromLocal8Bit(pClient->readAll());
    //qDebug()<<"clientInfo: "<<clientInfo;
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

                HandleThread *pHandleThread = dynamic_cast<HandleThread*>(ReflexObject::Instance()->GetObjectIns("HandleThread"));
                pHandleThread->ReceiveMsg(transferDataModel);

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
//    //法一读取（接收）：
//    QByteArray byteArray=pClientSocket->readAll();
//    TransferDataModel transferDataModel;
//    QDataStream in(byteArray);
//    in.setVersion(QDataStream::Qt_5_0);//最好设置数据保存版本
//    in >>transferDataModel;

//    //法二读取（接收）：
//    //    TransferDataModel transferDataModel;
//    //    QDataStream in(pClient);
//    //    in.setVersion(QDataStream::Qt_5_0);//最好设置数据保存版本
//    //    in >> transferDataModel;

//    HandleThread *pHandleThread = dynamic_cast<HandleThread*>(ReflexObject::Instance()->GetObjectIns("HandleThread"));
//    pHandleThread->ReceiveMsg(transferDataModel);
}

void ClientSocket::ServerDisconnectedSlots()
{
    QString msg = "服务器退出 ";
    //QDateTime dateTime(QDateTime::currentDateTime());
    //QString dateStr = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    //QString Msg=dateStr+" : "+msg;
    qDebug()<<msg;

    IsConnectSucceedMutex.lock();ConnectTimeOutMutex.lock();
    IsConnectSucceed=false;ConnectTimeOut=true;
    IsConnectSucceedMutex.unlock();ConnectTimeOutMutex.unlock();
    ConnectCountMutex.lock();
    ConnectCount=0;
    ConnectCountMutex.unlock();

    NetAssistantWin *pNetAssistantWin= dynamic_cast<NetAssistantWin*>(ReflexObject::Instance()->GetObjectIns("NetAssistantWin"));
    pNetAssistantWin->ModifyConnectStatus("服务器已断开",0);

    pKeepAliveTimer->stop();//停止定时器
}

void ClientSocket::OnKeepAliveTimeOut()
{
    pClientSocket->disconnectFromHost();//主动断开
    qDebug()<<"OnKeepAliveTimeOut";
}

void ClientSocket::ConnectToServerFunc(QString msg)
{
    //    QString serveIP ="192.168.43.196";//ui->IPLineEdit->text();//获取服务器IP
    //    QString servePort = "8080";//ui->PortLineEdit->text();//获取服务器端口号
    ServeIPAdressMutex.lock();ServePortMutex.lock();
    pClientSocket->connectToHost(QHostAddress(ServeIPAdress), ServePort.toInt());//连接服务器
    NetAssistantWin *pNetAssistantWin= dynamic_cast<NetAssistantWin*>(ReflexObject::Instance()->GetObjectIns("NetAssistantWin"));
    pNetAssistantWin->ModifyConnectStatus(msg,1);
    ServeIPAdressMutex.unlock();ServePortMutex.unlock();
}

void ClientSocket::SendMsgSlots(const TransferDataModel &transferDataModel)
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

    int ret = pClientSocket->write(sendByteArray);
    if (ret <= 0){
        return;
    }

    while (!pClientSocket->waitForBytesWritten()) {

    }

    //pClientSocket->flush();
}
