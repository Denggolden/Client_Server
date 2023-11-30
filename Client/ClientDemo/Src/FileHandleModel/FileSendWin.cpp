#pragma execution_character_set("utf-8")
#include "FileSendWin.h"
#include "ui_FileSendWin.h"
#include <QProgressBar>
#include <QTimer>
#include "FileSendSocketThread.h"
#include <QThread>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QElapsedTimer>
#include "Src/DataHandleModel/DataOperate.h"
#include "../../Common/TestTool.h"

FileSendWin::FileSendWin(QString sendUserID,QString targetUserID,int curFileTransferBindID,QString filePath,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileTransferWin),SendUserID(sendUserID),TargetUserID(targetUserID),CurFileTransferBindID(curFileTransferBindID),FilePath(filePath)
{
    ui->setupUi(this);

    InitLabel();
    InitProgressBar();
    InitFileSendSocketThread();

    InitTimer();
}

FileSendWin::~FileSendWin()
{
    delete ui;
}

void FileSendWin::InitLabel()
{
    QFont font = QFont("Microsoft YaHei",8,QFont::Bold,false);
    ui->label->setFont(font);
    ui->label_2->setFont(font);
    ui->label_4->setFont(font);
    ui->label_5->setFont(font);

    ui->label_4->setStyleSheet("color:DodgerBlue;");
    ui->label_5->setStyleSheet("color:DodgerBlue;");

    //ui->label_3;//图标
}

void FileSendWin::InitProgressBar()
{
    // 获取文件信息
    QFileInfo fileInfo(FilePath);

    FileName=fileInfo.fileName();
    MaxNum=fileInfo.size();

    FileAllSize=TestTool::CalculateFileSize(MaxNum);

    QFileIconProvider iconProvider;
    QIcon icon = iconProvider.icon(fileInfo);
    QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(24, 24)));
    //当然，您也可以简单地选择第一个/最后一个可用尺寸:
//    QPixmap pixmap = icon.pixmap(icon.availableSizes().first());
//    QPixmap pixmap = icon.pixmap(icon.availableSizes().last());

    //使用 QIcon::availableSizes() 查询大小:
//    QList<QSize> sizes = icon.availableSizes();
//    int maximum = sizes[0].width();
//    for (int i=1; i < sizes.size(); ++i)
//        maximum = qMax(maximum, sizes[i].width());
//    QPixmap pixmap = icon.pixmap(QSize(maximum, maximum));

    ui->label->setText(QString("正在发送至：%1\n%2").arg(TargetUserID).arg(FileName));
    ui->label_2->setText(QString("总大小：%1 已接收：%2 字节").arg(FileAllSize).arg(0));
    ui->label_3->setPixmap(pixmap);

    pProgressBar = new QProgressBar();
    pProgressBar->setOrientation(Qt::Horizontal);  // 水平方向
    pProgressBar->setMinimum(0);  // 最小值
    pProgressBar->setMaximum(MaxNum);  // 最大值
    //pProgressBar->setValue(0);  // 当前进度
    //pProgressBar->setFormat(QString("登录中..."));
    //    double dProgress = (pProgressBar->value() - pProgressBar->minimum()) * 100.0
    //                    / (pProgressBar->maximum() - pProgressBar->minimum());
    //    pProgressBar->setFormat(QString("当前进度为：%1%").arg(QString::number(dProgress, 'f', 1)));
    //    pProgressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式

    //    pProgressBar->setStyleSheet("QProgressBar::chunk{border-radius:5px;background:qlineargradient(spread:pad,x1:0,y1:0,x2:1,y2:0,stop:0 #01FAFF,stop:1  #26B4FF);}"
    //                                "QProgressBar#progressBar{height:22px;text-align:center;font-size:10px;color:white;border-radius:5px;background: #1D5573 ;}");
    //100 149 237
    pProgressBar->setStyleSheet("QProgressBar{font:normal bold 10px Microsoft YaHei;color:white;border-radius:8px;text-align:center;border:1px solid;background-color:rgb(100,149,237);border-color:rgb(255, 255, 255);}"
                                "QProgressBar:chunk{border-radius:8px;background-color:rgb(0, 255, 255);}");

    QHBoxLayout *pHBoxLayoutFrame2 = new QHBoxLayout();//水平布局
    pHBoxLayoutFrame2->addWidget(pProgressBar);
    ui->frame_2->setLayout(pHBoxLayoutFrame2);
    ui->frame_2->layout()->setContentsMargins(0, 0, 0, 0);//控件居中显示 消除四周边距
}

void FileSendWin::InitTimer()
{
    pTimer = new QTimer();
    pTimer->setInterval(500);//0.5s的延时
    connect(pTimer,SIGNAL(timeout()),this,SLOT(OnTimeOut()));
    //    pTimer->stop();//停止定时器
    pTimer->start();//启动定时器

    pElapsedTimer=new QElapsedTimer();
    pElapsedTimer->start();
}

void FileSendWin::InitFileSendSocketThread()
{
    QString serveIPAdress = DataOperate::Instance()->GetServerIP();
    QString servePort =DataOperate::Instance()->GetServerPort();
    //QString path="C:/Users/admin/Desktop/chat/C6000上位机 V2023.8.7自测报告（初版后续再更新）.docx";
    FileSendSocketThread *pFileSendSocketThread=new FileSendSocketThread(serveIPAdress,servePort,SendUserID,TargetUserID,CurFileTransferBindID,FilePath);
    QThread *pThread = new QThread;
    pFileSendSocketThread->moveToThread(pThread);
    pThread->start();//启动事件循环  用以处理信号对应的槽函数

    //connect(thread,SIGNAL(started()),task,SLOT(doWork()));//非必要
    //从Qt4.4开始，qthreads-no-longer-abstract，run 默认调用 QThread::exec(),即启动消息队列
    connect(pFileSendSocketThread,SIGNAL(workFinishedSignals()),pThread,SLOT(quit()));//回收标志
    //automatically delete thread and task object when work is done:
    connect(pThread,SIGNAL(finished()),pFileSendSocketThread,SLOT(deleteLater()));//回收 ClientSocketThread
    connect(pThread,SIGNAL(finished()),pThread,SLOT(deleteLater()));//回收 QThread This signal can be connected to QObject::deleteLater()，to free objects in that thread.

    connect(pFileSendSocketThread,&FileSendSocketThread::SetProgressBarCurNum, this,&FileSendWin::SetProgressBarCurNumSlots);

    TransferDataModel transferDataModel;
    transferDataModel.SetFileTransferBindInfo(FileSendTransferBind,SendUserID,TargetUserID,CurFileTransferBindID,SendUserID,FilePath);
    pFileSendSocketThread->SendData(transferDataModel);
    QThread::msleep(100);
    pFileSendSocketThread->SendFileData();
}

void FileSendWin::OnTimeOut()
{
    CurNumMutex.lock();
    pProgressBar->setValue(CurNum);  // 当前进度

    float speed=(float)CurNum/ElapsedTimer;
    qDebug()<<"ElapsedTimer: "<<ElapsedTimer;
    QString speedStr=TestTool::CalculateTransferSpeed(speed);
    QString curFileSizeStr=TestTool::CalculateFileSize(CurNum);
    ui->label_2->setText(QString("总大小：%1 已发送：%2  速度：%3").arg(FileAllSize).arg(curFileSizeStr).arg(speedStr));

    double percentage=(double)CurNum/(double)MaxNum*100;
    pProgressBar->setFormat(QString("百分比：%1 %").arg(QString::number(percentage,'f', 2)));
    CurNumMutex.unlock();

    if(MaxNum==CurNum){
        pTimer->stop();
        delete pTimer;
        delete pElapsedTimer;
        pTimer=nullptr;
        pElapsedTimer=nullptr;
        qDebug()<<"pTimer=nullptr;";
    }
}

void FileSendWin::SetProgressBarCurNumSlots(int curNum)
{
    CurNumMutex.lock();
    //ElapsedTimer=(double)pElapsedTimer->nsecsElapsed()/(double)1000000;
    ElapsedTimer=(double)pElapsedTimer->elapsed()/(double)1000;//毫秒 转化成秒
    CurNum=curNum;
    CurNumMutex.unlock();
}
