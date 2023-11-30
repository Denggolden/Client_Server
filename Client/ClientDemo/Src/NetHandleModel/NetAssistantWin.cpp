#include "NetAssistantWin.h"
#include "ui_NetAssistantWin.h"
#include "../../Common/StyleData.h"
#include <QDebug>
#include "ClientSocket.h"
#include "Src/ComHandleModel/ReflexObject.h"
#include <QPainter>
#include <QBitmap>
#include <Windows.h>
#include <windowsx.h>
#include <QStyleOption>
#include "Src/DataHandleModel/DataOperate.h"

NetAssistantWin::NetAssistantWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::NetAssistantWin)
{
    ui->setupUi(this);
}

NetAssistantWin::~NetAssistantWin()
{
    delete ui;
}

void NetAssistantWin::InitClass()
{
    InitPushButton();
    InitlineEdit();
    InitLabel();
    InitComboBox();

    this->setMinimumWidth(350);
    this->setMinimumHeight(380);

    this->setStyleSheet("background-color:SkyBlue;");
    //设置无边框
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->layout()->setContentsMargins(20, 20, 20, 20);
}

void NetAssistantWin::InitPushButton()
{
    PushButtonList.clear();
    PushButtonList<<ui->ConnectBtn<<ui->CloseWinBtn<<ui->WriteIPAndPortBtn;
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

void NetAssistantWin::InitlineEdit()
{
    QFont font = QFont("Microsoft YaHei",10,QFont::Bold,false);
    ui->lineEdit->setFont(font);
    ui->lineEdit_2->setFont(font);

    ui->lineEdit->setStyleSheet("QLineEdit{border:3px solid blue;border-radius: 5px;}");
    ui->lineEdit_2->setStyleSheet("QLineEdit{border:3px solid blue;border-radius: 5px;}");

    //    ui->lineEdit->setEnabled(false);
    //    ui->lineEdit_2->setEnabled(false);
}

void NetAssistantWin::InitLabel()
{
    QFont font = QFont("Microsoft YaHei",10,QFont::Bold,false);
    ui->label->setFont(font);
    ui->label_2->setFont(font);
    ui->label_3->setFont(font);
    ui->label_4->setFont(font);
    ui->label_5->setFont(font);
}

void NetAssistantWin::InitComboBox()
{
    ui->comboBox->setStyleSheet("border: 2px solid blue;border-radius: 10px;");
    ui->comboBox->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
    //ui->comboBox->lineEdit()->setAlignment(Qt::AlignCenter);
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexChangedSlots(int)));
}

void NetAssistantWin::ConnectOrDisConnect(bool isConnect)
{
    if(isConnect==true){//连接 只要设置好IP Port就可自动连接
        QString serverIPAddress=ui->lineEdit->text();
        QString serverPort=ui->lineEdit_2->text();
        ClientSocket *pClientSocket = dynamic_cast<ClientSocket*>(ReflexObject::Instance()->GetObjectIns("ClientSocket"));
        pClientSocket->SetServeIPAdressAndServePort(serverIPAddress,serverPort);
        ui->comboBox->setEnabled(false);
        ui->lineEdit->setEnabled(false);
        ui->lineEdit_2->setEnabled(false);
    }
    else{//断开
        ClientSocket *pClientSocket = dynamic_cast<ClientSocket*>(ReflexObject::Instance()->GetObjectIns("ClientSocket"));
        pClientSocket->DisConnectToServerFunc();
        ui->comboBox->setEnabled(true);
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
    }
}

void NetAssistantWin::ModifyConnectStatus(QString connectStatus, short status)
{
    ui->label_5->setText(QString("连接状态: %1").arg(connectStatus));
    if(status==0)
        ui->label_5->setStyleSheet("color:red;");
    else if (status==1)
        ui->label_5->setStyleSheet("color:MistyRose;");
    else
        ui->label_5->setStyleSheet("color:SpringGreen;");
}

void NetAssistantWin::UpDateServerIPAddressList(QList<QString> iPAddressList)
{
    ui->comboBox->clear();
    ui->comboBox->addItem("Any");
    ui->comboBox->addItem("LocalHost");

   QString serverIP= DataOperate::Instance()->GetServerIP();
   int selectCurIndex=0;

    int iPAddressListSize=iPAddressList.size();
    for (int index=0;index<iPAddressListSize;index++) {
        ui->comboBox->addItem(iPAddressList.at(index));
        if(serverIP==iPAddressList.at(index)){
            selectCurIndex=index+2;
        }
    }
    ui->comboBox->setCurrentIndex(selectCurIndex);
    if(ui->comboBox->count()>0)
        ui->lineEdit->setText(ui->comboBox->currentText());
}

void NetAssistantWin::UpDateServerPort(QString port)
{
    ui->lineEdit_2->setText(port);
}

void NetAssistantWin::PushButtonSlots()
{
    QPushButton*PushButton = qobject_cast<QPushButton*>(sender());
    qDebug()<<PushButton->objectName()<<"  "<<PushButton->text();

    if(PushButton->objectName()=="ConnectBtn"){
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
    else if(PushButton->objectName()=="CloseWinBtn") {
        this->hide();
    }
    else if (PushButton->objectName()=="WriteIPAndPortBtn") {
        QString serverIp=ui->lineEdit->text();
        QString serverPort=ui->lineEdit_2->text();

        DataOperate::Instance()->SetServerIP(serverIp);
        DataOperate::Instance()->SetServerPort(serverPort);
    }
}

void NetAssistantWin::currentIndexChangedSlots(int index)
{
    QComboBox*ComboBox = qobject_cast<QComboBox*>(sender());
    qDebug()<<ComboBox->objectName()<<"  "<<ComboBox->currentText();

    if(index>=0)
        ui->lineEdit->setText(ui->comboBox->currentText());
}

void NetAssistantWin::paintEvent(QPaintEvent *paint)
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

bool NetAssistantWin::nativeEvent(const QByteArray &eventType, void *message, long *result)
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
