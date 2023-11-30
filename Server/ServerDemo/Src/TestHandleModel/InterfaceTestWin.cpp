#include "InterfaceTestWin.h"
#include "ui_InterfaceTestWin.h"

#include "Src/FileHandleModel/FileReceiveWin.h"
#include "Src/FileHandleModel/FileSendWin.h"

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
    FileReceiveWinList.clear();
    FileSendWinList.clear();

    qRegisterMetaType<TransferDataModel>("TransferDataModel&");
    qRegisterMetaType<TransferDataModel>("TransferDataModel");

    qDebug()<< connect(this,&InterfaceTestWin::HandelOperateSignal, this,&InterfaceTestWin::HandelOperateSolts);
}

void InterfaceTestWin::HandelOperate(TransferDataModel &transferDataModel)
{
    emit HandelOperateSignal(transferDataModel);
}

void InterfaceTestWin::FileReceiveWinHandle(QString sendUserID,QString targetUserID,int curFileTransferBindID,int fileByteArrSize)
{

    //    QString filePath=transferDataModel.FilePath;
    //    qint64 allFileSize=transferDataModel.AllFileSize;
    FileReceiveWinListMutex.lock();
    int FileReceiveWinListSize=FileReceiveWinList.size();
    for (int index=0;index<FileReceiveWinListSize ;index++ ) {
        if(FileReceiveWinList.at(index)->GetSendUserID()==sendUserID&&FileReceiveWinList.at(index)->GetTargetUserID()==targetUserID
                &&FileReceiveWinList.at(index)->GetCurFileTransferBindID()==curFileTransferBindID){
            FileReceiveWinList.at(index)->SetProgressBarCurNumSlots(fileByteArrSize);
        }
    }
    FileReceiveWinListMutex.unlock();
}

void InterfaceTestWin::FileSendWinHandle(QString sendUserID, QString targetUserID, int curFileTransferBindID, int fileByteArrSize)
{
    FileSendWinListMutex.lock();
    int FileSendWinListSize=FileSendWinList.size();
    for (int index=0;index<FileSendWinListSize ;index++ ) {
        if(FileSendWinList.at(index)->GetSendUserID()==sendUserID&&FileSendWinList.at(index)->GetTargetUserID()==targetUserID
                &&FileSendWinList.at(index)->GetCurFileTransferBindID()==curFileTransferBindID){
            FileSendWinList.at(index)->SetProgressBarCurNumSlots(fileByteArrSize);
        }
    }
    FileSendWinListMutex.unlock();
}

void InterfaceTestWin::PushButtonSlots()
{
//    QPushButton*PushButton = qobject_cast<QPushButton*>(sender());
//    qDebug()<<PushButton->objectName()<<"  "<<PushButton->text();
}

void InterfaceTestWin::HandelOperateSolts(TransferDataModel &transferDataModel)
{
    CommandType commandType=transferDataModel.commandType;
    if(commandType==SendFileReady){
        QString sendUserID=transferDataModel.SendUserID;
        QString targetUserID=transferDataModel.TargetUserID;
        QString filePath=transferDataModel.FilePath;
        qint64 allFileSize=transferDataModel.AllFileSize;
        int curFileTransferBindID=transferDataModel.CurFileTransferBindID;

        FileReceiveWin *pFileReceiveWin=new FileReceiveWin(sendUserID,targetUserID,curFileTransferBindID,filePath,allFileSize);
        pFileReceiveWin->show();
        FileReceiveWinListMutex.lock();
        FileReceiveWinList.append(pFileReceiveWin);
        FileReceiveWinListMutex.unlock();

        //        QString sendUserID=transferDataModel.SendUserID;
        //        QString targetUserID=transferDataModel.TargetUserID;
        //        int curFileTransferBindID=transferDataModel.CurFileTransferBindID;
        //        QString filePath=transferDataModel.FilePath;

        //        qDebug()<<QString("SendFileReadyRet-----sendUserID: %1---targetUserID: %2---filePath: %3---curFileTransferBindID: %4").arg(sendUserID).arg(targetUserID).arg(filePath).arg(curFileTransferBindID);

        //        FileSendWin *pFileSendWin=new FileSendWin(sendUserID,targetUserID,curFileTransferBindID,filePath);
        //        pFileSendWin->show();
    }
    else if (commandType==ReceiveFileReady) {
        QString sendUserID=transferDataModel.SendUserID;
        QString targetUserID=transferDataModel.TargetUserID;
        int curFileTransferBindID=transferDataModel.CurFileTransferBindID;
        QString filePath=transferDataModel.FilePath;

        //qDebug()<<QString("SendFileReadyRet-----sendUserID: %1---targetUserID: %2---filePath: %3---curFileTransferBindID: %4").arg(sendUserID).arg(targetUserID).arg(filePath).arg(curFileTransferBindID);

        FileSendWin *pFileSendWin=new FileSendWin(sendUserID,targetUserID,curFileTransferBindID,filePath);
        pFileSendWin->show();
        FileSendWinListMutex.lock();
        FileSendWinList.append(pFileSendWin);
        FileSendWinListMutex.unlock();

        //        //接收文件返回 ReceiveFileReadyRet
        //        bool ret=true;//默认准备成功
        //        QString retMsg="";
        //        TransferDataModel retTransferDataModel;
        //        retTransferDataModel.SetReceiveFileReadyRetInfo(ReceiveFileReadyRet,sendUserID,targetUserID,curFileTransferBindID,filePath,allFileSize,ret,retMsg);
        //        ClientSocket *pClientSocket = dynamic_cast<ClientSocket*>(ReflexObject::Instance()->GetObjectIns("ClientSocket"));
        //        pClientSocket->SendData(retTransferDataModel);

    }
}
