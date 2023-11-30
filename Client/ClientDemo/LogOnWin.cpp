#pragma execution_character_set("UTF-8")
#include "LogOnWin.h"
#include "ui_LogOnWin.h"
#include "../../Common/StyleData.h"
#include <QDebug>
#include <QStyleOption>
#include <QBitmap>
#include <QPainter>
#include <Windows.h>
#include <windowsx.h>
#include <QAction>
#include <QMessageBox>
#include <QProgressBar>
#include <thread>
#include "Src/NetHandleModel/NetAssistantWin.h"
#include "Src/ComHandleModel/ReflexObject.h"
#include "Src/NetHandleModel/ClientSocket.h"
#include "../../Common/TransferDataModel.h"
#include "ClientMainWin.h"
#include "UserInfoWin.h"

#pragma comment(lib,"user32")

LogOnWin::LogOnWin(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::LogOnWin)
{
    ui->setupUi(this);
}

LogOnWin::~LogOnWin()
{
    delete ui;
}

void LogOnWin::InitClass()
{
    ProgressBarStop=false;

    InitFrame();
    InitPushButton();
    InitlineEdit();
    InitLable();
    InitCheckBox();
    InitProgressBar();

    this->setMinimumWidth(500);
    this->setMinimumHeight(400);

    this->setStyleSheet("background-color:CornflowerBlue;");
    //设置无边框
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->layout()->setContentsMargins(0, 0, 0, 0);//控件居中显示 四周边距为0
}

void LogOnWin::InitFrame()
{
    //ui->frame->setStyleSheet("#frame{border:1px solid white;border-radius: 10px;background-color: rgba(0,191,255,0.1);}");
    ui->frame->setStyleSheet("#frame{border:1px solid white;border-radius: 10px;}");

    //ui->frame->layout()->setSpacing(0);//设置内边距
    ui->frame->layout()->setContentsMargins(10, 10, 10, 10);

    ui->frame_5->hide();
}

void LogOnWin::InitPushButton()
{
    PushButtonList.clear();
    PushButtonList<<ui->CloseWinBtn<<ui->LogOnBtn<<ui->RegisterBtn<<ui->ViewNetworkBtn;
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

void LogOnWin::InitlineEdit()
{
    QFont font = QFont("Microsoft YaHei",10,QFont::Bold,false);
    ui->lineEdit->setFont(font);
    ui->lineEdit_2->setFont(font);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);

    ui->lineEdit->setStyleSheet("QLineEdit{border:1px solid white;border-radius: 3px;color:black}");
    ui->lineEdit_2->setStyleSheet("QLineEdit{border:1px solid white;border-radius: 3px;color:black}");

    ui->lineEdit->setMinimumHeight(40);
    ui->lineEdit_2->setMinimumHeight(40);

    ui->lineEdit->setAlignment(Qt::AlignCenter);
    ui->lineEdit_2->setAlignment(Qt::AlignCenter);

    ui->lineEdit->setPlaceholderText("请输入用户名");
    ui->lineEdit_2->setPlaceholderText("请输入密码");

    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);

    //一、新建action
    QAction* userAction = new QAction(ui->lineEdit);
    QAction* passwordAction = new QAction(ui->lineEdit_2);
    //二、给action添加icon
    userAction->setIcon(QIcon(":/Image/user.png"));
    passwordAction->setIcon(QIcon(":/Image/password.png"));
    //三、给空间添加action
    ui->lineEdit->addAction(userAction,QLineEdit::LeadingPosition);
    ui->lineEdit_2->addAction(passwordAction,QLineEdit::LeadingPosition);
}

void LogOnWin::InitLable()
{
    //    QList<QLabel*> LableList = this->findChildren<QLabel*>();
    //    foreach (QLabel *Lable, LableList) {
    //        if (Lable != nullptr){
    //            //Lable->setStyleSheet("border: 2px solid blue");
    //            Lable->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));        //设置字体
    //            //Lable->setAlignment(Qt::AlignCenter);
    //        }
    //    }
    ui->label->setFont(QFont("Microsoft YaHei", 16, QFont::Bold));
    ui->label->setStyleSheet("QLabel{background-color: transparent;color:white}");
    ui->label->setAlignment(Qt::AlignCenter);

    ui->label_2->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));
    ui->label_2->setStyleSheet("QLabel{background-color: transparent;color:white}");
    ui->label_2->setAlignment(Qt::AlignCenter);

    ui->label_3->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));
    ui->label_3->setStyleSheet("QLabel{background-color: transparent;color:white}");
    ui->label_3->setAlignment(Qt::AlignCenter);
}

void LogOnWin::InitCheckBox()
{
    ui->checkBox->setStyleSheet(CheckBoxQss);
    ui->checkBox->setCheckState(Qt::Unchecked);   			//复选框初始状态
    ui->checkBox->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));
}

void LogOnWin::InitProgressBar()
{
    pProgressBar = new QProgressBar();
    //pProgressBar->setGeometry(100,140,120,20);
    pProgressBar->setOrientation(Qt::Horizontal);  // 水平方向
    pProgressBar->setMinimum(0);  // 最小值
    pProgressBar->setMaximum(100);  // 最大值
    pProgressBar->setValue(0);  // 当前进度
    pProgressBar->setFormat(QString("登录中..."));
    //    double dProgress = (pProgressBar->value() - pProgressBar->minimum()) * 100.0
    //                    / (pProgressBar->maximum() - pProgressBar->minimum());
    //    pProgressBar->setFormat(QString("当前进度为：%1%").arg(QString::number(dProgress, 'f', 1)));
    //    pProgressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式

    //    pProgressBar->setStyleSheet("QProgressBar::chunk{border-radius:5px;background:qlineargradient(spread:pad,x1:0,y1:0,x2:1,y2:0,stop:0 #01FAFF,stop:1  #26B4FF);}"
    //                                "QProgressBar#progressBar{height:22px;text-align:center;font-size:10px;color:white;border-radius:5px;background: #1D5573 ;}");
    //100 149 237
    pProgressBar->setStyleSheet("QProgressBar{font:normal bold 14px Microsoft YaHei;color:white;border-radius:8px;text-align:center;border:1px solid;background-color:rgb(100,149,237);border-color:rgb(255, 255, 255);}"
                                "QProgressBar:chunk{border-radius:8px;background-color:rgb(0, 255, 255);}");

    QHBoxLayout *pHBoxLayoutFrame5 = new QHBoxLayout();//水平布局
    pHBoxLayoutFrame5->addWidget(pProgressBar);
    ui->frame_5->setLayout(pHBoxLayoutFrame5);
    ui->frame_5->layout()->setContentsMargins(0, 0, 0, 0);//控件居中显示 消除四周边距

    connect(this,SIGNAL(ProgressBarSignal()),this,SLOT(ProgressBarSlots()));
}

void LogOnWin::RegisterOrLogin(QString infoMsg)
{
    if(ui->lineEdit->text().isEmpty()==true||ui->lineEdit_2->text().isEmpty()==true){
        if(ui->lineEdit->text().isEmpty()==true)
            ui->lineEdit->setStyleSheet("QLineEdit{border:1px solid white;border-radius: 3px;color:red}");
        if(ui->lineEdit_2->text().isEmpty()==true)
            ui->lineEdit_2->setStyleSheet("QLineEdit{border:1px solid white;border-radius: 3px;color:red}");
        return;
    }

    //这里把账号密码提交上去即可
    TransferDataModel transferDataModel;
    ClientSocket *pClientSocket = dynamic_cast<ClientSocket*>(ReflexObject::Instance()->GetObjectIns("ClientSocket"));
    if(infoMsg=="注册中"){
        QString userName=ui->lineEdit->text();
        QString userID=ui->lineEdit->text();
        QString userPassWord=ui->lineEdit_2->text();
        QFile file("D:/Users/DJL/Desktop/chat-9.16/Client/PreImg/头像.png");
        if (!file.open(QIODevice::ReadOnly)){
        }
        QByteArray userHeadImgByte = file.readAll();
        file.close();
        transferDataModel.SetRegisterInfo(Register,userName,userID,userPassWord,userHeadImgByte);
    }else if (infoMsg=="登录中") {
        QString userID=ui->lineEdit->text();
        QString userPassWord=ui->lineEdit_2->text();
        transferDataModel.SetLoginInfo(Login,userID,userPassWord);
    }
    pClientSocket->SendData(transferDataModel);

    //ui->CancelBtn->setEnabled(false);
    //ui->LogOnBtn->setEnabled(false);
    ui->frame_5->show();
    IsServerRetMsgFlag=false;//重置状态
    ProgressBarStop=false;//重置状态
    pProgressBar->setFormat(QString("%1...").arg(infoMsg));
    std::thread t([=](){
        while (true) {
            ProgressBarStopMutex.lock();
            if(ProgressBarStop==true)
                break;
            ProgressBarStopMutex.unlock();
            emit ProgressBarSignal();
            if(pProgressBar->value()<=90){
                Sleep(30);
            }
            else {
                while (true) {//等待服务器返回消息
                    IsServerRetMsgFlagMutex.lock();
                    if(IsServerRetMsgFlag==true)
                        break;
                    IsServerRetMsgFlagMutex.unlock();
                }
                IsServerRetMsgFlagMutex.unlock();
                Sleep(100);
            }
        }
        ProgressBarStopMutex.unlock();
    });
    t.detach();
    IsServerRetMsgFlag=false;//重置状态
}

void LogOnWin::SetIsServerRetMsgFlag(TransferDataModel transferDataModel)
{
    IsServerRetMsgFlagMutex.lock();
    IsServerRetMsgFlag=true;
    TransferDataModelG=transferDataModel;
    IsServerRetMsgFlagMutex.unlock();

    //qDebug()<<"isServerRetMsgFlag: "<<isServerRetMsgFlag <<"   retMsg: "<<retMsg;
}

void LogOnWin::RegisterOrLoginRetResult()
{
    CommandType commandType=TransferDataModelG.commandType;
    bool isSeverRetSuccess=TransferDataModelG.IsSeverRetSuccess;
    QString textMsg=TransferDataModelG.TextMsg;

    if(commandType==RegisterRet){/*注册不需要这样*/}

    if(commandType==LoginRet&&isSeverRetSuccess==true){//登录成功
        QString userName=TransferDataModelG.UserName;
        QString userID=TransferDataModelG.UserID;
        QString userPassWord=TransferDataModelG.UserPassword;
        QByteArray userHeadImgByte=TransferDataModelG.FileByteArr;
        qDebug()<<"登录成功";
        ui->frame_5->hide();
        LogOnWin *pLogOnWin = dynamic_cast<LogOnWin*>(ReflexObject::Instance()->GetObjectIns("LogOnWin"));
        pLogOnWin->hide();

        ClientMainWin *pClientMainWin = dynamic_cast<ClientMainWin*>(ReflexObject::Instance()->GetObjectIns("ClientMainWin"));
        pClientMainWin->show();

        UserInfoWin *pUserInfoWin = dynamic_cast<UserInfoWin*>(ReflexObject::Instance()->GetObjectIns("UserInfoWin"));
        pUserInfoWin->SetUserInfo(userName,userID,userPassWord,userHeadImgByte);
    }
    if(commandType==LoginRet&&isSeverRetSuccess==false){//登录失败
        qDebug()<<"登录失败";
    }

    pProgressBar->setFormat(textMsg);
}

void LogOnWin::PushButtonSlots()
{
    QPushButton*PushButton = qobject_cast<QPushButton*>(sender());
    qDebug()<<PushButton->objectName()<<"  "<<PushButton->text();
    if(PushButton->objectName()=="RegisterBtn"){
        RegisterOrLogin("注册中");
    }
    else if (PushButton->objectName()=="LogOnBtn") {
        RegisterOrLogin("登录中");
    }
    else if (PushButton->objectName()=="CloseWinBtn") {
        qApp->exit();
    }
    else if (PushButton->objectName()=="ViewNetworkBtn") {
        NetAssistantWin *pNetAssistantWin= dynamic_cast<NetAssistantWin*>(ReflexObject::Instance()->GetObjectIns("NetAssistantWin"));
        pNetAssistantWin->show();
    }
}

void LogOnWin::ProgressBarSlots()
{
    int value=0;
    if(pProgressBar->value()==100){
        value=0;
    }
    else {
        value=pProgressBar->value()+1;
    }
    pProgressBar->setValue(value);

    if(pProgressBar->value()==100){
        ProgressBarStopMutex.lock();
        ProgressBarStop=true;
        ProgressBarStopMutex.unlock();

        RegisterOrLoginRetResult();
    }
}

void LogOnWin::showEvent(QShowEvent *event)
{
    ui->LogOnBtn->setFocus();
    QWidget::showEvent(event);
}

bool LogOnWin::eventFilter(QObject *object, QEvent *event)
{
    if (object->objectName() == "lineEdit"){
        if (event->type() == QEvent::FocusIn)
            ui->lineEdit->setStyleSheet("QLineEdit{border:1px solid white;border-radius: 3px;color:black}");
    }
    if (object->objectName() == "lineEdit_2"){
        if (event->type() == QEvent::FocusIn)
            ui->lineEdit_2->setStyleSheet("QLineEdit{border:1px solid white;border-radius: 3px;color:black}");
    }
    return QWidget::eventFilter(object, event);
}

void LogOnWin::paintEvent(QPaintEvent *paint)
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

bool LogOnWin::nativeEvent(const QByteArray &eventType, void *message, long *result)
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
