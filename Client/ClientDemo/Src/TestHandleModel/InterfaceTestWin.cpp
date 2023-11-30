#include "InterfaceTestWin.h"
#include "ui_InterfaceTestWin.h"
#include <QFileDialog>
#include "Src/FileHandleModel/FileSendWin.h"
#include "../../Common/StyleData.h"
#include <QDebug>
#include "Src/NetHandleModel/ClientSocket.h"
#include "Src/ComHandleModel/ReflexObject.h"
#include "Src/FileHandleModel/FileReceiveWin.h"

InterfaceTestWin::InterfaceTestWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::InterfaceTestWin)
{
    ui->setupUi(this);
}

InterfaceTestWin::~InterfaceTestWin()
{
    delete ui;
}

void InterfaceTestWin::InitClass()
{
    InitPushButton();

    qRegisterMetaType<TransferDataModel>("TransferDataModel&");
    qRegisterMetaType<TransferDataModel>("TransferDataModel");

    qDebug()<< connect(this,&InterfaceTestWin::HandelOperateSignal, this,&InterfaceTestWin::HandelOperateSolts);
}

void InterfaceTestWin::InitPushButton()
{
    PushButtonList.clear();
    PushButtonList<<ui->SendFileBtn;
    int PushButtonListSize=PushButtonList.size();
    for (int index=0;index<PushButtonListSize;index++) {
        PushButtonList.at(index)->setMinimumSize(60, 40);        //设置最小尺寸
        PushButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
        PushButtonList.at(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); //QPushButton的高度自动适应文本
        PushButtonList.at(index)->setStyleSheet(NormalBtnQss);
        //绑定槽函数
        connect(PushButtonList.at(index),SIGNAL(clicked()),this,SLOT(PushButtonSlots()));
    }
}

void InterfaceTestWin::ReadySendFile()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,tr("open a file."),"D:/Users/DJL/Desktop/chat-9.16",
                tr("all file(*)"));
    if (fileName.isEmpty()){
        return;
    }
    QString sendUserID=ui->lineEdit->text().trimmed();
    QString targetUserID=ui->lineEdit_2->text().trimmed();
    QString filePath=fileName;

    QFileInfo fileInfo(filePath);
    qint64 allFileSize=fileInfo.size();

    TransferDataModel transferDataModel;
    transferDataModel.SetSendFileReadyInfo(SendFileReady,sendUserID,targetUserID,filePath,allFileSize);
    ClientSocket *pClientSocket = dynamic_cast<ClientSocket*>(ReflexObject::Instance()->GetObjectIns("ClientSocket"));
    pClientSocket->SendData(transferDataModel);
}

void InterfaceTestWin::HandelOperate(TransferDataModel &transferDataModel)
{
    emit HandelOperateSignal(transferDataModel);
}

void InterfaceTestWin::PushButtonSlots()
{
    QPushButton*PushButton = qobject_cast<QPushButton*>(sender());
    qDebug()<<PushButton->objectName()<<"  "<<PushButton->text();
    if(PushButton->objectName()=="SendFileBtn"){
        ReadySendFile();
    }
}

void InterfaceTestWin::HandelOperateSolts(TransferDataModel &transferDataModel)
{
    CommandType commandType=transferDataModel.commandType;
    if(commandType==SendFileReadyRet){
        QString sendUserID=transferDataModel.SendUserID;
        QString targetUserID=transferDataModel.TargetUserID;
        int curFileTransferBindID=transferDataModel.CurFileTransferBindID;
        QString filePath=transferDataModel.FilePath;

        qDebug()<<QString("SendFileReadyRet-----sendUserID: %1---targetUserID: %2---filePath: %3---curFileTransferBindID: %4").arg(sendUserID).arg(targetUserID).arg(filePath).arg(curFileTransferBindID);

        FileSendWin *pFileSendWin=new FileSendWin(sendUserID,targetUserID,curFileTransferBindID,filePath);
        pFileSendWin->show();
    }
    else if (commandType==ReceiveFileReady) {
        QString sendUserID=transferDataModel.SendUserID;
        QString targetUserID=transferDataModel.TargetUserID;
        int curFileTransferBindID=transferDataModel.CurFileTransferBindID;
        QString filePath=transferDataModel.FilePath;
        qint64 allFileSize=transferDataModel.AllFileSize;

        qDebug()<<QString("ReceiveFileReady-----sendUserID: %1---targetUserID: %2---filePath: %3---curFileTransferBindID: %4---allFileSize: %5").arg(sendUserID).arg(targetUserID).arg(filePath).arg(curFileTransferBindID).arg(allFileSize);

        FileReceiveWin *pFileReceiveWin=new FileReceiveWin(sendUserID,targetUserID,curFileTransferBindID,filePath,allFileSize);
        pFileReceiveWin->show();

        //接收文件返回 ReceiveFileReadyRet
        bool ret=true;//默认准备成功
        QString retMsg="";
        TransferDataModel retTransferDataModel;
        retTransferDataModel.SetReceiveFileReadyRetInfo(ReceiveFileReadyRet,sendUserID,targetUserID,curFileTransferBindID,filePath,allFileSize,ret,retMsg);
        ClientSocket *pClientSocket = dynamic_cast<ClientSocket*>(ReflexObject::Instance()->GetObjectIns("ClientSocket"));
        pClientSocket->SendData(retTransferDataModel);

    }
}

