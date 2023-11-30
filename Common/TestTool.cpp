#include "TestTool.h"
#include <windows.h>
#include <Tlhelp32.h>
#include <QDebug>
#include <thread>

TestTool::TestTool()
{

}

TestTool::~TestTool()
{

}

void TestTool::GetCurProcessThreadsNumber()
{
    std::thread t([=](){
        while(true){
            PROCESSENTRY32 ProEntry = {0};
            HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
            ProEntry.dwSize = sizeof(PROCESSENTRY32);
            Process32First(hSnap, &ProEntry);
            do{
                //wprintf(_T("%s %d %d \r\n"),ProEntry.szExeFile,ProEntry.th32ProcessID,ProEntry.cntThreads);
                QString szExeFile=QString::fromWCharArray(ProEntry.szExeFile);
                QString th32ProcessID=QString::number(ProEntry.th32ProcessID);
                QString cntThreads=QString::number(ProEntry.cntThreads);
                if(szExeFile=="ServerDemo.exe"){
                    qDebug()<<"szExeFile: "<<szExeFile <<"   th32ProcessID: "<<th32ProcessID << "   cntThreads:"<<cntThreads;
                }
                // 这里可以设置获取独立进程的线程数。
            } while (Process32Next(hSnap,&ProEntry));
            CloseHandle(hSnap);
            Sleep(1);
        }
    });
    t.detach();
}

QString TestTool::CalculateFileSize(int fileSize)
{
    QString fileSizeStr;
    if (fileSize < 1024) {
        fileSizeStr =QString("%1 B").arg(fileSize);
    } else if (fileSize < 1024*1024) {
        fileSizeStr =QString("%1 KB").arg((double)fileSize/(double)1024);
    } else if (fileSize < 1024*1024*1024) {
        fileSizeStr =QString("%1 MB").arg((double)fileSize/(double)(1024*1024));
    }else {
        fileSizeStr =QString("%1 GB").arg((double)fileSize/(double)(1024*1024*1024));
    }

    return fileSizeStr;
}

QString TestTool::CalculateTransferSpeed(float speedByte)
{
    QString speedStr="";
    if (speedByte < 1024) {
        speedStr=QString("%1 bytes/sec").arg(speedByte);
    } else if (speedByte < 1024*1024) {
        speedStr=QString("%1 kB/s").arg(speedByte/(float)1024);
    } else if (speedByte < 1024*1024*1024) {
        speedStr=QString("%1 MB/s").arg(speedByte/(float)(1024*1024));
    }else {
        speedStr=QString("%1 GB/s").arg(speedByte/(float)(1024*1024*1024));
    }
    return speedStr;
}
