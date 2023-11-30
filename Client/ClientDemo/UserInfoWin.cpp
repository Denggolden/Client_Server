#include "UserInfoWin.h"
#include "ui_UserInfoWin.h"
#include "../../Common/StyleData.h"
#include <QDebug>
#include "Src/NetHandleModel/NetAssistantWin.h"
#include "Src/ComHandleModel/ReflexObject.h"
#include <QFileInfo>
#include "Src/NetHandleModel/ClientSocket.h"
#include <QFileDialog>
#include <Windows.h>
#include <QThread>
#include <QMouseEvent>
#include "Src/DataHandleModel/DataOperate.h"
#include <QMessageBox>
#include "LogOnWin.h"
#include "ClientMainWin.h"

UserInfoWin::UserInfoWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::UserInfoWin)
{
    ui->setupUi(this);
}

UserInfoWin::~UserInfoWin()
{
    delete ui;
}

void UserInfoWin::InitClass()
{
    InitGroupBox();
    InitPushButton();
    InitlineEdit();
    InitLable();


    qRegisterMetaType<TransferDataModel>("TransferDataModel&");
    qRegisterMetaType<TransferDataModel>("TransferDataModel");

    connect(this,&UserInfoWin::ModifyUserRetInfoFunSignal, this,&UserInfoWin::ModifyUserRetInfoFunSlots);
    connect(this,&UserInfoWin::LogoutUserRetFunSignal, this,&UserInfoWin::LogoutUserRetFunSignalSlots);
}

void UserInfoWin::InitGroupBox()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->groupBox->setFont(font);
    ui->groupBox_2->setFont(font);
}

void UserInfoWin::InitPushButton()
{
    PushButtonList.clear();
    PushButtonList<<ui->LogOffBtn<<ui->ModifyInfoBtn<<ui->ViewNetworkBtn;
    int PushButtonListSize=PushButtonList.size();
    for (int index=0;index<PushButtonListSize;index++) {
        PushButtonList.at(index)->setMinimumSize(80, 40);        //设置最小尺寸
        PushButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
        PushButtonList.at(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); //QPushButton的高度自动适应文本
        PushButtonList.at(index)->setStyleSheet(NormalBtnQss);
        //绑定槽函数
        connect(PushButtonList.at(index),SIGNAL(clicked()),this,SLOT(PushButtonSlots()));
    }
}

void UserInfoWin::InitlineEdit()
{
    QFont font = QFont("Microsoft YaHei",10,QFont::Bold,false);
    ui->lineEdit->setFont(font);
    ui->lineEdit_2->setFont(font);
    ui->lineEdit_3->setFont(font);

    ui->lineEdit->setStyleSheet("QLineEdit{border:3px solid blue;border-radius: 5px;}");
    ui->lineEdit_2->setStyleSheet("QLineEdit{border:3px solid blue;border-radius: 5px;}");
    ui->lineEdit_3->setStyleSheet("QLineEdit{border:3px solid blue;border-radius: 5px;}");

    //    ui->lineEdit->setEnabled(false);
    //    ui->lineEdit_2->setEnabled(false);
}

void UserInfoWin::InitLable()
{
    QFont font = QFont("Microsoft YaHei",10,QFont::Bold,false);
    ui->label->setFont(font);
    ui->label_2->setFont(font);
    ui->label_3->setFont(font);

    ui->label_4->installEventFilter(this);
}

void UserInfoWin::SetUserInfo(QString userName, QString userID, QString userPassword, QByteArray userHeadImgByte)
{
    ui->lineEdit->setText(userName);
    ui->lineEdit_2->setText(userID);
    ui->lineEdit_3->setText(userPassword);

    QPixmap userHeadPixmap;
    userHeadPixmap.loadFromData(userHeadImgByte);
    //显示头像
    ui->label_4->setPixmap(userHeadPixmap);

    UserName=userName;
    UserID=userID;
    UserPassword=userPassword;
    UserHeadImgByte=userHeadImgByte;
    TempUserHeadImgByte=userHeadImgByte;
}

void UserInfoWin::ChangeUserHeadImg()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,tr("open a file."),"D:/",
                tr("images(*.png *jpeg *bmp)"));
    if (fileName.isEmpty()){
        return;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)){
    }
    QByteArray tempUserHeadImgByte=file.readAll();
    if(tempUserHeadImgByte.size()>=60000){
        file.close();
        return;//头像超长了
    }
    TempUserHeadImgByte = tempUserHeadImgByte;
    file.close();

    QPixmap userHeadPixmap;
    userHeadPixmap.loadFromData(TempUserHeadImgByte);
    //显示头像
    ui->label_4->setPixmap(userHeadPixmap);
}

void UserInfoWin::ModifyUserInfoFun()
{
    QString userName=ui->lineEdit->text();
    QString userID=ui->lineEdit_2->text();
    QString userPassWord=ui->lineEdit_3->text();
    TransferDataModel transferDataModel;
    transferDataModel.SetModifyUserInfo(ModifyUserInfo,userName,userID,userPassWord,TempUserHeadImgByte);

    ClientSocket *pClientSocket = dynamic_cast<ClientSocket*>(ReflexObject::Instance()->GetObjectIns("ClientSocket"));
    pClientSocket->SendData(transferDataModel);
}

void UserInfoWin::ModifyUserRetInfoFun(TransferDataModel &transferDataModel)
{
    emit ModifyUserRetInfoFunSignal(transferDataModel);
}

void UserInfoWin::LogoutUser()
{
    QString userID=ui->lineEdit_2->text();
    TransferDataModel transferDataModel;
    transferDataModel.SetLogoutInfo(Logout,userID);

    ClientSocket *pClientSocket = dynamic_cast<ClientSocket*>(ReflexObject::Instance()->GetObjectIns("ClientSocket"));
    pClientSocket->SendData(transferDataModel);
}

void UserInfoWin::LogoutUserRetFun(TransferDataModel &transferDataModel)
{
    emit LogoutUserRetFunSignal(transferDataModel);
}

void UserInfoWin::writeFile(TransferDataModel &transferDataModel)
{
    QByteArray fileByteArr=transferDataModel.FileByteArr;
    QString fileName=transferDataModel.FileName;
    bool isSendFileFinish=transferDataModel.IsSendFileFinish;

    int FileListSize=FileList.size();
    if(FileListSize==0){
        QFile* file = new QFile(fileName);
        FileList.push_back(file);
        FileList[0]->open(QFile::WriteOnly);
        FileList[0]->write(fileByteArr);
        if(isSendFileFinish==true){
            FileList[0]->close();//并且完成后移除FileList完成的项目
            FileList.removeAt(0);
        }
    }

    for(int index=0;index<FileListSize;index++){
        if(FileList.at(index)->fileName()==fileName){
            FileList[index]->write(fileByteArr);
            if(isSendFileFinish==true){
                FileList[index]->close();//并且完成后移除FileList完成的项目
                FileList.removeAt(index);
            }
            break;//写完后跳出
        }
        if(FileList.at(index)->fileName()!=fileName&&index==(FileListSize-1)){
            QFile* file = new QFile(fileName);
            FileList.push_back(file);
            FileList[FileListSize]->open(QFile::WriteOnly);
            FileList[FileListSize]->write(fileByteArr);
            if(isSendFileFinish==true){
                FileList[FileListSize]->close();//并且完成后移除FileList完成的项目
                FileList.removeAt(FileListSize);
            }
        }
    }
}

void UserInfoWin::PushButtonSlots()
{
    QPushButton*PushButton = qobject_cast<QPushButton*>(sender());
    qDebug()<<PushButton->objectName()<<"  "<<PushButton->text();
    if(PushButton->objectName()=="LogOffBtn"){
        LogoutUser();
    }
    else if (PushButton->objectName()=="ModifyInfoBtn") {
        ModifyUserInfoFun();
    }
    else if (PushButton->objectName()=="ViewNetworkBtn") {
        NetAssistantWin *pNetAssistantWin= dynamic_cast<NetAssistantWin*>(ReflexObject::Instance()->GetObjectIns("NetAssistantWin"));
        pNetAssistantWin->show();
    }
}

void UserInfoWin::ModifyUserRetInfoFunSlots(TransferDataModel &transferDataModel)
{
    QString retTextMsg= transferDataModel.TextMsg;
    bool isSeverRetSuccess=transferDataModel.IsSeverRetSuccess;
    QMessageBox::information(this, "提示", retTextMsg);

    if(isSeverRetSuccess==true){//修改成功
        LogOnWin *pLogOnWin = dynamic_cast<LogOnWin*>(ReflexObject::Instance()->GetObjectIns("LogOnWin"));
        pLogOnWin->show();

        ClientMainWin *pClientMainWin = dynamic_cast<ClientMainWin*>(ReflexObject::Instance()->GetObjectIns("ClientMainWin"));
        pClientMainWin->hide();
    }
}

void UserInfoWin::LogoutUserRetFunSignalSlots(TransferDataModel &transferDataModel)
{
    QString retTextMsg= transferDataModel.TextMsg;
    bool isSeverRetSuccess=transferDataModel.IsSeverRetSuccess;
    QMessageBox::information(this, "提示", retTextMsg);

    if(isSeverRetSuccess==true){//注销成功
        LogOnWin *pLogOnWin = dynamic_cast<LogOnWin*>(ReflexObject::Instance()->GetObjectIns("LogOnWin"));
        pLogOnWin->show();

        ClientMainWin *pClientMainWin = dynamic_cast<ClientMainWin*>(ReflexObject::Instance()->GetObjectIns("ClientMainWin"));
        pClientMainWin->hide();
    }
}

bool UserInfoWin::eventFilter(QObject *object, QEvent *event)
{
    if(object ==ui->label_4){
        if(event->type()==QEvent::MouseButtonPress){
            QMouseEvent *mouseEvent = (QMouseEvent*)event;
            if(mouseEvent->buttons()&Qt::LeftButton){
                ChangeUserHeadImg();
            }
        }
    }
    return QWidget::eventFilter(object, event);
}
