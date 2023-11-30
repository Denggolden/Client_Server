#include "FileManageWin.h"
#include "ui_FileManageWin.h"
#include "../../Common/StyleData.h"

#include <QDebug>
#include "FileSendWin.h"
#include "Src/NetHandleModel/ClientSocket.h"
#include "Src/ComHandleModel/ReflexObject.h"
#include "FileReceiveWin.h"

FileManageWin::FileManageWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::FileManageWin)
{
    ui->setupUi(this);
}

FileManageWin::~FileManageWin()
{
    delete ui;
}

void FileManageWin::InitClass()
{
    InitTabWidget();
    InitPushButton();
    InitGroupBox();
    InitListWidget();

    this->setWindowTitle("客户端");
    this->setMinimumWidth(500);

    qRegisterMetaType<TransferDataModel>("TransferDataModel&");
    qRegisterMetaType<TransferDataModel>("TransferDataModel");

    qDebug()<< connect(this,&FileManageWin::HandelOperateSignal, this,&FileManageWin::HandelOperateSolts);
}

void FileManageWin::InitTabWidget()
{
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(TabWidgetcurrentChanged(int)));
    //connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(TabWidgetcurrentChanged(int)));
    connect(ui->tabWidget,SIGNAL(tabBarClicked(int)),this,SLOT(TabWidgetcurrentChanged(int)));

    ui->tabWidget->widget(0)->layout()->setContentsMargins(0, 0, 0, 0);

    //标题栏红色背景，tab选中蓝色，未选中灰色
    ui->tabWidget->setStyleSheet("QTabWidget#tabWidget{background-color:rgb(255,255,255);}\
                                 QTabBar::tab{background-color:rgb(30,144,255);color:rgb(255,255,255);font: bold 10pt 'Microsoft YaHei'}\
                                 QTabBar::tab::selected{background-color:rgb(255,255,255);color:rgb(30,144,255);font: bold 10pt 'Microsoft YaHei'}\
                                 QTabBar::tab{width:90}QTabBar::tab{height:30}\
                                 QTabBar::tab{border: 3px solid white; border-radius: 8px;}");
}

void FileManageWin::InitPushButton()
{
    PushButtonList.clear();
    PushButtonList<<ui->ClearAllBtn;
    int PushButtonListSize=PushButtonList.size();
    for (int index=0;index<PushButtonListSize;index++) {
        PushButtonList.at(index)->setMinimumSize(60, 30);        //设置最小尺寸
        PushButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
        PushButtonList.at(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); //QPushButton的高度自动适应文本
        PushButtonList.at(index)->setStyleSheet(NormalBtnQss);
        //绑定槽函数
        connect(PushButtonList.at(index),SIGNAL(clicked()),this,SLOT(PushButtonSlots()));
    }
}

void FileManageWin::InitGroupBox()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->groupBox->setFont(font);
    ui->groupBox_2->setFont(font);

    //ui->groupBox->layout()->setContentsMargins(0, 0, 0, 0);
    ui->groupBox_2->layout()->setContentsMargins(0, 0, 0, 0);
}

void FileManageWin::InitListWidget()
{

}

void FileManageWin::HandelOperate(TransferDataModel &transferDataModel)
{
    emit HandelOperateSignal(transferDataModel);
}

void FileManageWin::CreateFileSendWinItem(TransferDataModel &transferDataModel)
{
    QListWidgetItem *item=new QListWidgetItem;
    int count=ui->listWidget->count()+1;
    item->setText(QString("%1").arg(count));
    //item->setTextColor(QColor(0,0,0,0));
    item->setSizeHint(QSize(100,200));

    QString sendUserID=transferDataModel.SendUserID;
    QString targetUserID=transferDataModel.TargetUserID;
    int curFileTransferBindID=transferDataModel.CurFileTransferBindID;
    QString filePath=transferDataModel.FilePath;

    qDebug()<<QString("SendFileReadyRet-----sendUserID: %1---targetUserID: %2---filePath: %3---curFileTransferBindID: %4").arg(sendUserID).arg(targetUserID).arg(filePath).arg(curFileTransferBindID);

    FileSendWin *pFileSendWin=new FileSendWin(sendUserID,targetUserID,curFileTransferBindID,filePath);
//    pFileSendWin->show();

    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item,pFileSendWin);
    //connect(ui->listWidget,&QListWidget::itemClicked,this,&ClientMainWin::itemClickedSlots);
//    connect(ui->listWidget,&QListWidget::itemSelectionChanged,this,&ClientMainWin::itemSelectionChangedSlots);
}

void FileManageWin::CreateFileReceiveWinItem(TransferDataModel &transferDataModel)
{
    QListWidgetItem *item=new QListWidgetItem;
    int count=ui->listWidget->count()+1;
    item->setText(QString("%1").arg(count));
    //item->setTextColor(QColor(0,0,0,0));
    item->setSizeHint(QSize(100,200));

    QString sendUserID=transferDataModel.SendUserID;
    QString targetUserID=transferDataModel.TargetUserID;
    int curFileTransferBindID=transferDataModel.CurFileTransferBindID;
    QString filePath=transferDataModel.FilePath;
    qint64 allFileSize=transferDataModel.AllFileSize;

    qDebug()<<QString("ReceiveFileReady-----sendUserID: %1---targetUserID: %2---filePath: %3---curFileTransferBindID: %4---allFileSize: %5").arg(sendUserID).arg(targetUserID).arg(filePath).arg(curFileTransferBindID).arg(allFileSize);

    FileReceiveWin *pFileReceiveWin=new FileReceiveWin(sendUserID,targetUserID,curFileTransferBindID,filePath,allFileSize);
    //pFileReceiveWin->show();

    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item,pFileReceiveWin);
    //connect(ui->listWidget,&QListWidget::itemClicked,this,&ClientMainWin::itemClickedSlots);
//    connect(ui->listWidget,&QListWidget::itemSelectionChanged,this,&ClientMainWin::itemSelectionChangedSlots);

    //接收文件返回 ReceiveFileReadyRet
    bool ret=true;//默认准备成功
    QString retMsg="";
    TransferDataModel retTransferDataModel;
    retTransferDataModel.SetReceiveFileReadyRetInfo(ReceiveFileReadyRet,sendUserID,targetUserID,curFileTransferBindID,filePath,allFileSize,ret,retMsg);
    ClientSocket *pClientSocket = dynamic_cast<ClientSocket*>(ReflexObject::Instance()->GetObjectIns("ClientSocket"));
    pClientSocket->SendData(retTransferDataModel);
}

void FileManageWin::TabWidgetcurrentChanged(int index)
{
    qDebug()<<ui->tabWidget->widget(index)->objectName()<<"  "<<ui->tabWidget->tabText(index);
    qDebug()<<ui->tabWidget->currentWidget()->objectName();
}

void FileManageWin::PushButtonSlots()
{
    QPushButton*PushButton = qobject_cast<QPushButton*>(sender());
    qDebug()<<PushButton->objectName()<<"  "<<PushButton->text();

    if(PushButton->objectName()=="ClearAllBtn"){

    }
}

void FileManageWin::HandelOperateSolts(TransferDataModel &transferDataModel)
{
    CommandType commandType=transferDataModel.commandType;
    if(commandType==SendFileReadyRet){
        CreateFileSendWinItem(transferDataModel);
        this->show();
    }
    else if (commandType==ReceiveFileReady) {
        CreateFileReceiveWinItem(transferDataModel);
        this->show();
    }
}

void FileManageWin::closeEvent(QCloseEvent *event)
{
    this->hide();
}
