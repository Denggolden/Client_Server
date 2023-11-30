#include "ClientMainWin.h"
#include "ui_ClientMainWin.h"
#include <QDebug>
#include "UserInfoWin.h"
#include "Src/ComHandleModel/ReflexObject.h"
#include "Src/TestHandleModel/InterfaceTestWin.h"

ClientMainWin::ClientMainWin(QWidget *parent)
    : WidgetBase(parent)
    , ui(new Ui::ClientMainWin)
{
    ui->setupUi(this);
}

ClientMainWin::~ClientMainWin()
{
    delete ui;
}

void ClientMainWin::InitClass()
{
    InitFrame();
    InitStackedWidget();
    InitToolButton();
}

void ClientMainWin::InitFrame()
{
    ui->frame->setStyleSheet("#frame{border:3px solid rgb(85, 170, 255);border-radius: 10px;}");

    ui->frame->layout()->setContentsMargins(0, 0, 0, 0);//控件居中显示 消除四周边距
}

void ClientMainWin::InitToolButton()
{
    int Size=5;
    int IconSize=10;
    ToolButtonList.clear();
    ToolButtonList<<ui->ChatTBtn<<ui->ContactsTbtn<<ui->PendingTBtn<<ui->UserInfoTBtn;
    int ToolButtonListSize=ToolButtonList.size();
    for (int index=0;index<ToolButtonListSize ;index++ ) {
        //ToolButtonList.at(index)->setText(ToolBtnList.at(index).BtnText);
        //ToolButtonList.at(index)->setObjectName(ToolBtnList.at(index).BtnObjectName);
        ToolButtonList.at(index)->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));
        ToolButtonList.at(index)->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        //New->setIcon(QIcon(":/Image/openFile.png"));            //添加图标
        ToolButtonList.at(index)->setIcon(QIcon(QString(":/Image/%1.png")
                               .arg(index+1)));            //添加图标
        //toolBtn->setFixedSize(Size,Size);                      //调图标大小（不是setIconSize)
        ToolButtonList.at(index)->setMinimumSize(Size*2.8,Size*2.8);
        ToolButtonList.at(index)->setIconSize(QSize(IconSize,IconSize));
        //绑定槽函数
        connect(ToolButtonList.at(index),SIGNAL(clicked()),this,SLOT(ToolButtonSlots()));
    }
}

void ClientMainWin::InitStackedWidget()
{
    pInterfaceTestWin = dynamic_cast<InterfaceTestWin*>(ReflexObject::Instance()->GetObjectIns("InterfaceTestWin"));
    pUserInfoWin = dynamic_cast<UserInfoWin*>(ReflexObject::Instance()->GetObjectIns("UserInfoWin"));

    ui->stackedWidget->addWidget(pInterfaceTestWin);
    ui->stackedWidget->addWidget(pUserInfoWin);

    ui->stackedWidget->setCurrentWidget(pInterfaceTestWin);

    ui->stackedWidget->setStyleSheet("background-color:rgb(255,255,255);");//更改背景颜色
}

void ClientMainWin::ToolButtonSlots()
{
    QToolButton*ToolButton = qobject_cast<QToolButton*>(sender());
    qDebug()<<ToolButton->objectName()<<"  "<<ToolButton->text();

    if(ToolButton->objectName()=="ChatTBtn"){//聊天

    }
    else if(ToolButton->objectName()=="ContactsTbtn"){//联系人

    }
    else if(ToolButton->objectName()=="PendingTBtn"){//待定
        ui->stackedWidget->setCurrentWidget(pInterfaceTestWin);
    }
    else if(ToolButton->objectName()=="UserInfoTBtn"){//用户个人信息
        ui->stackedWidget->setCurrentWidget(pUserInfoWin);
    }
}

