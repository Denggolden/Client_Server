#include "HandleThread.h"
#include <thread>
#include "DataOperate.h"
#include "LogOnWin.h"
#include "Src/ComHandleModel/ReflexObject.h"
#include "Src/FileHandleModel/FileReceiveSocketThread.h"
#include <QThread>
#include "UserInfoWin.h"
#include "Src/TestHandleModel/InterfaceTestWin.h"
#include "Src/FileHandleModel/FileManageWin.h"

HandleThread::HandleThread(QObject *parent) : ObjectBase(parent)
{
}

HandleThread::~HandleThread()
{
}

void HandleThread::InitClass()
{
    MsgInfoQueue.clear();
    HandleMsgLoop();
}

void HandleThread::ReceiveMsg(TransferDataModel transferDataModel)
{
    std::thread t([=](){
        MsgQueueMutex.lock();
        MsgInfoQueue.enqueue(transferDataModel);
        MsgQueueMutex.unlock();
    });
    t.detach();
}

void HandleThread::HandleMsgLoop()
{
    std::thread t([=](){
        while(true){
            MsgQueueMutex.lock();
            while (!MsgInfoQueue.isEmpty()) {
                TransferDataModel msgInfo=MsgInfoQueue.dequeue();
                HandleMsg(msgInfo);
            }
            MsgQueueMutex.unlock();
        }
    });
    t.detach();
}

void HandleThread::HandleMsg(TransferDataModel &transferDataModel)
{
    CommandType commandType=transferDataModel.commandType;

    //对于客户端来说 目前只可能存在
    //1.谁发来的消息  方便对应到对应的条目下
    //2.个人信息修改成功/失败
    //3.添加好友同意或拒绝
    if(commandType==RegisterRet){
        LogOnWin *pLogOnWin = dynamic_cast<LogOnWin*>(ReflexObject::Instance()->GetObjectIns("LogOnWin"));
        pLogOnWin->SetIsServerRetMsgFlag(transferDataModel);
    }
    else if (commandType==LoginRet) {
        LogOnWin *pLogOnWin = dynamic_cast<LogOnWin*>(ReflexObject::Instance()->GetObjectIns("LogOnWin"));
        pLogOnWin->SetIsServerRetMsgFlag(transferDataModel);
    }
    else if (commandType==LogoutRet) {
        UserInfoWin *pUserInfoWin = dynamic_cast<UserInfoWin*>(ReflexObject::Instance()->GetObjectIns("UserInfoWin"));
        pUserInfoWin->LogoutUserRetFun(transferDataModel);
    }
    else if (commandType==ModifyUserInfoRet) {
        UserInfoWin *pUserInfoWin = dynamic_cast<UserInfoWin*>(ReflexObject::Instance()->GetObjectIns("UserInfoWin"));
        pUserInfoWin->ModifyUserRetInfoFun(transferDataModel);
    }
    //文件处理相关
    else if (commandType==SendFileReadyRet){
        //FileManageWin
//        InterfaceTestWin *pInterfaceTestWin = dynamic_cast<InterfaceTestWin*>(ReflexObject::Instance()->GetObjectIns("InterfaceTestWin"));
//        pInterfaceTestWin->HandelOperate(transferDataModel);
        FileManageWin *pFileManageWin = dynamic_cast<FileManageWin*>(ReflexObject::Instance()->GetObjectIns("FileManageWin"));
        pFileManageWin->HandelOperate(transferDataModel);
    }
    else if (commandType==ReceiveFileReady) {
//        InterfaceTestWin *pInterfaceTestWin = dynamic_cast<InterfaceTestWin*>(ReflexObject::Instance()->GetObjectIns("InterfaceTestWin"));
//        pInterfaceTestWin->HandelOperate(transferDataModel);
        FileManageWin *pFileManageWin = dynamic_cast<FileManageWin*>(ReflexObject::Instance()->GetObjectIns("FileManageWin"));
        pFileManageWin->HandelOperate(transferDataModel);
    }
//    else if (commandType==ReceiveFileReady) {
////        FileReceiveSocketThread *pFileReceiveSocketThread=new FileReceiveSocketThread("192.168.43.196","8520");//("192.168.43.196","8520",path);
////        QThread *pThread = new QThread;
////        pFileReceiveSocketThread->moveToThread(pThread);
////        pThread->start();//启动事件循环  用以处理信号对应的槽函数

////        //connect(thread,SIGNAL(started()),task,SLOT(doWork()));//非必要
////        //从Qt4.4开始，qthreads-no-longer-abstract，run 默认调用 QThread::exec(),即启动消息队列
////        connect(pFileReceiveSocketThread,SIGNAL(workFinishedSignals()),pThread,SLOT(quit()));//回收标志
////        //automatically delete thread and task object when work is done:
////        connect(pThread,SIGNAL(finished()),pFileReceiveSocketThread,SLOT(deleteLater()));//回收 ClientSocketThread
////        connect(pThread,SIGNAL(finished()),pThread,SLOT(deleteLater()));//回收 QThread This signal can be connected to QObject::deleteLater()，to free objects in that thread.

////        //告诉服务器开始接收了 准备工作已经做好了
////        TransferDataModel transferDataModel;
////        pFileReceiveSocketThread->StartReceiveFileData(transferDataModel);
//    }
}
