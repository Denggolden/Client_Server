#pragma execution_character_set("UTF-8")
#include "ServerMainWin.h"
#include "ui_ServerMainWin.h"
#include "Src/NetHandleModel/ServerSocket.h"
#include "Src/ComHandleModel/ReflexObject.h"
#include "../../Common/CommonUISet.h"
#include "../../Common/StyleData.h"
#include <QPainter>
#include <QBitmap>
#include <Windows.h>
#include <windowsx.h>
#include <QStyleOption>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QDateTime>
#include "Src/DataHandleModel/DataOperate.h"
#include "Src/NetHandleModel/ClientSocketThread.h"
#include <QFileInfo>
#include <QFileDialog>
#include "Src/TestHandleModel/InterfaceTestWin.h"
#include "Src/FileHandleModel/FileManageWin.h"

#pragma comment(lib,"user32")

ServerMainWin::ServerMainWin(QWidget *parent)
    : WidgetBase(parent)
    , ui(new Ui::ServerMainWin)
{
    ui->setupUi(this);
}

ServerMainWin::~ServerMainWin()
{
    delete ui;
}

void ServerMainWin::InitClass()
{
    InitGroupBox();
    InitTableWidget(ui->tableWidget);
    InitPushButton();
    InitlineEdit();
    InitLabel();
    InitComboBox();
    InitTextEdit();

    this->setMinimumWidth(1300);
    this->setMinimumHeight(800);
    //设置无边框
    this->setWindowFlags(Qt::FramelessWindowHint);

    qRegisterMetaType<TransferDataModel>("TransferDataModel&");
    qRegisterMetaType<TransferDataModel>("TransferDataModel");

    connect(this,&ServerMainWin::UpDateTextEditLogText, this,&ServerMainWin::UpDateTextEditLogTextSlots);
    connect(this,&ServerMainWin::UpDateClientUserList, this,&ServerMainWin::UpDateClientUserListSlots);
}

void ServerMainWin::InitGroupBox()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->groupBox->setFont(font);
    ui->groupBox_2->setFont(font);
    ui->groupBox_3->setFont(font);
}

void ServerMainWin::InitTableWidget(QTableWidget *tableWidget)
{
    //设置表头
    QStringList header;
    header << tr("编号") << tr("IP地址") << tr("Port(端口)")<< tr("用户昵称")<< tr("用户ID") << tr("用户密码")<< tr("状态");
    //设置表头比例系数
    int ratio[]={1,5,2,5,3,3,1};
    int hHeaderHeight=30;//设置表头高度
    int hHeaderMinWidth=761;//设置最小表格宽度
    CommonUISet CommonUISet;
    CommonUISet.InitTableWidgetSet(tableWidget,header,ratio,hHeaderHeight,hHeaderMinWidth,true);

    connect(tableWidget, static_cast<void (QTableWidget::*)(int,int)>(&QTableWidget::cellClicked), this, [this](int row, int col)
    {
        QTableWidget*TableWidget = qobject_cast<QTableWidget*>(sender());
        //qDebug()<<"tableWidget: "<<TableWidget->item(row,col)->text();
    });
}

void ServerMainWin::InitPushButton()
{
    PushButtonList.clear();
    PushButtonList<<ui->CloseWinBtn<<ui->ConnectBtn<<ui->FileSendBtn<<ui->SendMsgBtn;
    int PushButtonListSize=PushButtonList.size();
    for (int index=0;index<PushButtonListSize;index++) {
        PushButtonList.at(index)->setMinimumSize(80, 30);        //设置最小尺寸
        PushButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
        PushButtonList.at(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); //QPushButton的高度自动适应文本
        if(PushButtonList.at(index)->objectName()=="AddBtn"){ //设置样式 Qt设置QPushButton文字加图片
            PushButtonList.at(index)->setStyleSheet(ImgBtnQss);
        }else {
            PushButtonList.at(index)->setStyleSheet(NormalBtnQss);
        }
        //绑定槽函数
        connect(PushButtonList.at(index),SIGNAL(clicked()),this,SLOT(PushButtonSlots()));
    }
}

void ServerMainWin::InitlineEdit()
{
    QFont font = QFont("Microsoft YaHei",10,QFont::Bold,false);
    ui->lineEdit->setFont(font);
    ui->lineEdit_2->setFont(font);

    ui->lineEdit->setStyleSheet("QLineEdit{border:3px solid blue;border-radius: 5px;}");
    ui->lineEdit_2->setStyleSheet("QLineEdit{border:3px solid blue;border-radius: 5px;}");

    //    ui->lineEdit->setEnabled(false);
    //    ui->lineEdit_2->setEnabled(false);
}

void ServerMainWin::InitLabel()
{
    QFont font = QFont("Microsoft YaHei",10,QFont::Bold,false);
    ui->label->setFont(font);
    ui->label_2->setFont(font);
    ui->label_3->setFont(font);
}

void ServerMainWin::InitComboBox()
{
    ui->comboBox->setStyleSheet("border: 2px solid blue;border-radius: 10px;");
    ui->comboBox->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
    //ui->comboBox->lineEdit()->setAlignment(Qt::AlignCenter);
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexChangedSlots(int)));
}

void ServerMainWin::InitTextEdit()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->textEdit->setFont(font);
    ui->textEdit->clear();
    ui->textEdit->setStyleSheet("QTextEdit{border:3px solid blue;border-radius: 5px;}");
}

void ServerMainWin::UpDateClientUserTableWidgetList()
{
    emit UpDateClientUserList();
}

void ServerMainWin::UpDateClientUserTableWidgetList(QTableWidget *tableWidget)
{
    while(tableWidget->rowCount()!=0){
        tableWidget->removeRow(0);
    }
    QList<ClientSocketThread *> clientInfoList;clientInfoList.clear();
    DataOperate::Instance()->GetClientInfoList(clientInfoList);

    int clientInfoListSize=clientInfoList.size();
    for (int index=0;index< clientInfoListSize;index++ ) {
        int curRow=tableWidget->rowCount();
        tableWidget->insertRow(curRow);

        tableWidget->setItem(curRow,0,new QTableWidgetItem(QString("%1").arg(index+1)));
        tableWidget->setItem(curRow,1,new QTableWidgetItem(QString("%1").arg(clientInfoList.at(index)->GetIPAddress())));
        tableWidget->setItem(curRow,2,new QTableWidgetItem(QString("%1").arg(clientInfoList.at(index)->GetPort())));

        tableWidget->setItem(curRow,3,new QTableWidgetItem(QString("%1").arg(clientInfoList.at(index)->GetUser_Name())));//用户昵称
        tableWidget->setItem(curRow,4,new QTableWidgetItem(QString("%1").arg(clientInfoList.at(index)->GetUserID())));//用户ID
        tableWidget->setItem(curRow,5,new QTableWidgetItem(QString("%1").arg(clientInfoList.at(index)->GetUserPassword())));//用户密码

        if(clientInfoList.at(index)->GetClientUserStatus()==OnLine)
            tableWidget->setItem(curRow,6,new QTableWidgetItem("在线"));
        else if (clientInfoList.at(index)->GetClientUserStatus()==Busy)
            tableWidget->setItem(curRow,6,new QTableWidgetItem("繁忙"));
        else if (clientInfoList.at(index)->GetClientUserStatus()==DoNotDisturb)
            tableWidget->setItem(curRow,6,new QTableWidgetItem("请勿打扰"));
        else if (clientInfoList.at(index)->GetClientUserStatus()==Stealth)
            tableWidget->setItem(curRow,6,new QTableWidgetItem("隐身"));
        else if (clientInfoList.at(index)->GetClientUserStatus()==OffLine)
            tableWidget->setItem(curRow,6,new QTableWidgetItem("离线"));
    }
}

void ServerMainWin::UpDateServerIPAddressList(QList<QString> iPAddressList)
{
    ui->comboBox->clear();
    ui->comboBox->addItem("Any");
    ui->comboBox->addItem("LocalHost");

    int iPAddressListSize=iPAddressList.size();
    for (int index=0;index<iPAddressListSize;index++) {
        ui->comboBox->addItem(iPAddressList.at(index));
    }
    if(ui->comboBox->count()>0)
        ui->lineEdit->setText(ui->comboBox->currentText());
}

void ServerMainWin::UpDateServerPort(QString port)
{
    ui->lineEdit_2->setText(port);
}

void ServerMainWin::AppendTextEditLogText(QString logInfo)
{
    QDateTime dateTime(QDateTime::currentDateTime());
    QString dateStr = dateTime.toString("yyyy-MM-dd hh:mm:ss");

    QString logMsgInfo=QString("[%1]:%2").arg(dateStr,logInfo);
    ui->textEdit->append(logMsgInfo);
}

void ServerMainWin::AppendTextEditLogText(TransferDataModel transferDataModel)
{
    emit UpDateTextEditLogText(transferDataModel);
}

void ServerMainWin::ConnectOrDisConnect(bool isConnect)
{
    if(isConnect==true){//连接
        QString serverIPAddress=ui->lineEdit->text();
        QString serverPort=ui->lineEdit_2->text();
        ServerSocket *pServerSocket = dynamic_cast<ServerSocket*>(ReflexObject::Instance()->GetObjectIns("ServerSocket"));
        if(pServerSocket->StartServerListen(serverIPAddress,serverPort)==true){
            ui->comboBox->setEnabled(false);
            ui->lineEdit->setEnabled(false);
            ui->lineEdit_2->setEnabled(false);
        }
    }
    else{//断开
        ServerSocket *pServerSocket = dynamic_cast<ServerSocket*>(ReflexObject::Instance()->GetObjectIns("ServerSocket"));
        pServerSocket->StopServerListen();

        ui->comboBox->setEnabled(true);
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
    }
}

void ServerMainWin::FileSend()
{
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    int row=-1;
    if(items.empty()){
        //QMessageBox::warning(this,"警告","请先选择一行再进行操作");
        return;
    }
    //int count = items.count();
    QTableWidgetItem *item = items.at(0);
    row=item->row();
    QString fileName = QFileDialog::getOpenFileName(
                this,tr("open a file."),"D:/Users/DJL/Desktop/chat-9.16",
                tr("all file(*)"));
    if (fileName.isEmpty()){
        return;
    }

    QString filePath=fileName;
    QFileInfo fileInfo(filePath);

    QString sendUserID="Server";
    QString targetUserID=ui->tableWidget->item(row,4)->text();
    qint64 allFileSize=fileInfo.size();
    int curFileTransferBindID=0;
    DataOperate::Instance()->GetCurFileTransferBindID(curFileTransferBindID);
    TransferDataModel transferDataModel;
    transferDataModel.SetReceiveFileReadyInfo(ReceiveFileReady,sendUserID,targetUserID,curFileTransferBindID,filePath,allFileSize);
    DataOperate::Instance()->NotifyClientInfoList(targetUserID,transferDataModel);//通知接收者

//    InterfaceTestWin *pInterfaceTestWin = dynamic_cast<InterfaceTestWin*>(ReflexObject::Instance()->GetObjectIns("InterfaceTestWin"));
//    pInterfaceTestWin->HandelOperate(transferDataModel);

    FileManageWin *pFileManageWin = dynamic_cast<FileManageWin*>(ReflexObject::Instance()->GetObjectIns("FileManageWin"));
    pFileManageWin->HandelOperate(transferDataModel);
}

void ServerMainWin::PushButtonSlots()
{
    QPushButton*PushButton = qobject_cast<QPushButton*>(sender());
    qDebug()<<PushButton->objectName()<<"  "<<PushButton->text();

    if(PushButton->objectName()=="CloseWinBtn"){
        ServerSocket *pServerSocket = dynamic_cast<ServerSocket*>(ReflexObject::Instance()->GetObjectIns("ServerSocket"));
        pServerSocket->StopServerListen();
        qApp->exit();
    }
    else if (PushButton->objectName()=="ConnectBtn") {
        if(IsConnect==false){
            ui->ConnectBtn->setText("断开");
            IsConnect=true;
            ConnectOrDisConnect(IsConnect);
        }
        else {
            ui->ConnectBtn->setText("连接");
            IsConnect=false;
            ConnectOrDisConnect(IsConnect);
        }
    }
    else if (PushButton->objectName()=="FileSendBtn") {
        FileSend();
    }
    else if (PushButton->objectName()=="SendMsgBtn") {

    }
}

void ServerMainWin::currentIndexChangedSlots(int index)
{
    QComboBox*ComboBox = qobject_cast<QComboBox*>(sender());
    qDebug()<<ComboBox->objectName()<<"  "<<ComboBox->currentText();

    if(index>=0)
        ui->lineEdit->setText(ui->comboBox->currentText());
}

void ServerMainWin::UpDateTextEditLogTextSlots(TransferDataModel transferDataModel)
{
    QDateTime dateTime(QDateTime::currentDateTime());
    QString dateStr = dateTime.toString("yyyy-MM-dd hh:mm:ss");

    QString logInfo="";
    CommandType commandType=transferDataModel.commandType;

    if(commandType==Register){
        logInfo=QString("[注册---用户昵称：%1 用户名：%2 密码：%3 IP：%4 Port：%5]").arg(transferDataModel.UserName).arg(transferDataModel.UserID).arg(transferDataModel.UserPassword)
                .arg(transferDataModel.ClientAddress).arg(transferDataModel.ClientPort);
    }
    else if (commandType==RegisterRet) {
        logInfo=QString("[注册返回---结果：%1 消息：%2 IP：%3 Port：%4]").arg(transferDataModel.IsSeverRetSuccess).arg(transferDataModel.TextMsg)
                .arg(transferDataModel.ClientAddress).arg(transferDataModel.ClientPort);
    }
    else if (commandType==Login) {
        logInfo=QString("[登录---用户名：%1 密码：%2 IP：%3 Port：%4]").arg(transferDataModel.UserID).arg(transferDataModel.UserPassword)
                .arg(transferDataModel.ClientAddress).arg(transferDataModel.ClientPort);
    }
    else if (commandType==LoginRet) {
        logInfo=QString("[登录返回---结果：%1 消息：%2 IP：%3 Port：%4 用户昵称：%5 用户名：%6 用户密码：%7]").arg(transferDataModel.IsSeverRetSuccess).arg(transferDataModel.TextMsg)
                .arg(transferDataModel.ClientAddress).arg(transferDataModel.ClientPort).arg(transferDataModel.UserName).arg(transferDataModel.UserID).arg(transferDataModel.UserPassword);
    }

    else if (commandType==SendFileMsg) {
        double percentage=(double)transferDataModel.SendFileSize/(double)transferDataModel.AllFileSize*100;
        logInfo=QString("[文件传输---文件名：%1 总大小：%2 字节 已传输：%3 字节 百分比：%4]").arg(transferDataModel.FileName).arg(transferDataModel.AllFileSize).arg(transferDataModel.SendFileSize)
                .arg(QString::number(percentage,'f', 2));
    }

    QString logMsgInfo=QString("[%1]:%2").arg(dateStr,logInfo);
    ui->textEdit->append(logMsgInfo);
}

void ServerMainWin::UpDateClientUserListSlots()
{
    UpDateClientUserTableWidgetList(ui->tableWidget);
}

void ServerMainWin::resizeEvent(QResizeEvent *event)
{
    CommonUISet CommonUISet;

    int ratio1[]={1,5,2,5,3,3,1};
    int columnCount1=7;
    CommonUISet.TableWidgetDynamicSet(ui->tableWidget,ratio1,columnCount1);

    //qDebug()<<ui->tableWidget->horizontalHeader()->width();
}

void ServerMainWin::paintEvent(QPaintEvent *paint)
{
    //绘制样式
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);//绘制样式

    QBitmap bmp(this->size());
    bmp.fill();
    QPainter painter(&bmp);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(bmp.rect(), 10, 10);
    setMask(bmp);
}

bool ServerMainWin::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_NCHITTEST) {
        *result = 0;
        const int BORDERWIDTH = 8; // 拉伸边框的宽度
        const int MoveHeight = 20; // 调整拖动生效的范围
        RECT winrect;
        GetWindowRect(reinterpret_cast<HWND>(winId()), &winrect);
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        int PosY = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
        if(PosY < MoveHeight){//拖拽生效区域
            *result = HTCAPTION;
        }
        // 判断鼠标位置是否在拉伸区域内
        if (y < winrect.top + BORDERWIDTH) {
            *result = HTTOP;
        }
        if (y > winrect.bottom - BORDERWIDTH) {
            *result = HTBOTTOM;
        }
        if (x < winrect.left + BORDERWIDTH) {
            *result = HTLEFT;
        }
        if (x > winrect.right - BORDERWIDTH) {
            *result = HTRIGHT;
        }
        if (y < winrect.top + BORDERWIDTH && x < winrect.left + BORDERWIDTH) {
            *result = HTTOPLEFT;
        }
        if (y < winrect.top + BORDERWIDTH && x > winrect.right - BORDERWIDTH) {
            *result = HTTOPRIGHT;
        }
        if (y > winrect.bottom - BORDERWIDTH && x < winrect.left + BORDERWIDTH) {
            *result = HTBOTTOMLEFT;
        }
        if (y > winrect.bottom - BORDERWIDTH && x > winrect.right - BORDERWIDTH) {
            *result = HTBOTTOMRIGHT;
        }
        if (*result != 0) {
            return true;
        }
    }

    return QWidget::nativeEvent(eventType, message, result);
}

