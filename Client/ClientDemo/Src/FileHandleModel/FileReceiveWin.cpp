#pragma execution_character_set("utf-8")
#include "FileReceiveWin.h"
#include "ui_FileReceiveWin.h"

#include <QProgressBar>
#include <QTimer>
#include "FileReceiveSocketThread.h"
#include <QThread>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QElapsedTimer>
#include "Src/DataHandleModel/DataOperate.h"
#include "../../Common/TestTool.h"

FileReceiveWin::FileReceiveWin(QString sendUserID,QString targetUserID,int curFileTransferBindID,QString filePath,qint64 allFileSize,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileReceiveWin),SendUserID(sendUserID),TargetUserID(targetUserID),CurFileTransferBindID(curFileTransferBindID),FilePath(filePath),AllFileSize(allFileSize)
{
    ui->setupUi(this);

    InitLabel();
    InitProgressBar();
    InitFileSendSocketThread();

    InitTimer();

//    this->layout()->setContentsMargins(0, 0, 0, 0);//控件居中显示 消除四周边距
}

FileReceiveWin::~FileReceiveWin()
{
    delete ui;
}

void FileReceiveWin::InitLabel()
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

void FileReceiveWin::InitProgressBar()
{
    // 获取文件信息
    QFileInfo fileInfo(FilePath);

    FileName=fileInfo.fileName();//创建个临时文件获取图标
    MaxNum=AllFileSize;

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

    ui->label->setText(QString("正在接收来自：%1\n%2").arg(SendUserID).arg(FileName));
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

void FileReceiveWin::InitTimer()
{
    pTimer = new QTimer();
    pTimer->setInterval(500);//0.5s的延时
    connect(pTimer,SIGNAL(timeout()),this,SLOT(OnTimeOut()));
    //    pTimer->stop();//停止定时器
    pTimer->start();//启动定时器

    pElapsedTimer=new QElapsedTimer();
    pElapsedTimer->start();
}

void FileReceiveWin::InitFileSendSocketThread()
{
    QString serveIPAdress = DataOperate::Instance()->GetServerIP();
    QString servePort =DataOperate::Instance()->GetServerPort();
    //FileReceiveSocketThread *pFileReceiveSocketThread=new FileReceiveSocketThread("192.168.0.87","8520",SendUserID,TargetUserID,FilePath);
    FileReceiveSocketThread *pFileReceiveSocketThread=new FileReceiveSocketThread(serveIPAdress,servePort,SendUserID,TargetUserID,FileName);//需不需要FileName看情况
    QThread *pThread = new QThread;
    pFileReceiveSocketThread->moveToThread(pThread);
    pThread->start();//启动事件循环  用以处理信号对应的槽函数

    //connect(thread,SIGNAL(started()),task,SLOT(doWork()));//非必要
    //从Qt4.4开始，qthreads-no-longer-abstract，run 默认调用 QThread::exec(),即启动消息队列
    connect(pFileReceiveSocketThread,SIGNAL(workFinishedSignals()),pThread,SLOT(quit()));//回收标志
    //automatically delete thread and task object when work is done:
    connect(pThread,SIGNAL(finished()),pFileReceiveSocketThread,SLOT(deleteLater()));//回收 ClientSocketThread
    connect(pThread,SIGNAL(finished()),pThread,SLOT(deleteLater()));//回收 QThread This signal can be connected to QObject::deleteLater()，to free objects in that thread.
    connect(pFileReceiveSocketThread,&FileReceiveSocketThread::SetProgressBarCurNum, this,&FileReceiveWin::SetProgressBarCurNumSlots);

    TransferDataModel transferDataModel;
    transferDataModel.SetFileTransferBindInfo(FileRecTransferBind,SendUserID,TargetUserID,CurFileTransferBindID,TargetUserID,FilePath);
    pFileReceiveSocketThread->SendData(transferDataModel);
    QThread::msleep(100);
}

void FileReceiveWin::OnTimeOut()
{
    CurNumMutex.lock();
    pProgressBar->setValue(CurNum);  // 当前进度

    float speed=(float)CurNum/ElapsedTimer;
    qDebug()<<"ElapsedTimer: "<<ElapsedTimer;
    QString speedStr=TestTool::CalculateTransferSpeed(speed);
    QString curFileSizeStr=TestTool::CalculateFileSize(CurNum);
    ui->label_2->setText(QString("总大小：%1 已接收：%2  速度：%3").arg(FileAllSize).arg(curFileSizeStr).arg(speedStr));

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

void FileReceiveWin::SetProgressBarCurNumSlots(int curNum)
{
    CurNumMutex.lock();
    //ElapsedTimer=(double)pElapsedTimer->nsecsElapsed()/(double)1000000;
    ElapsedTimer=(double)pElapsedTimer->elapsed()/(double)1000;//毫秒 转化成秒
    CurNum=curNum;
    CurNumMutex.unlock();
}
