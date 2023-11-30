#include "ClientMainWin.h"

#include <QApplication>
#include "Src/ComHandleModel/ReflexObject.h"
#include "LogOnWin.h"
#include "ClientMainWin.h"
#include "../../Common/TestTool.h"
#include <QFile>
#include <QFileInfo>
#include "Src/DataHandleModel/DataOperate.h"

#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <QFileInfo>
#include <Windows.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //    TestTool testTool;
    //    testTool.GetCurProcessThreadsNumber();

//    QString FilePath="D:/Users/DJL/Desktop/chat-9.16/123.txt";
//    QFileInfo fileInfo(FilePath);
//    qDebug()<<"fileInfo.suffix(): "<< fileInfo.suffix();
//    qDebug()<<"fileInfo.fileName(): "<< fileInfo.fileName();
//    QString fileName = QCoreApplication::applicationDirPath() + "/ICONDir/IconFile." + fileInfo.suffix();

//    QString fileName = QCoreApplication::applicationDirPath() + "/ICONDir/IconFile." + fileInfo.suffix();

//    QFile file(fileName);
//    bool exist=true;
//    bool ok=false;

//    exist = file.exists();
//    if(exist==false){//如果文件不存在，则创建并打开该文件
//        ok = file.open(QIODevice::ReadWrite);
//    }
//    if(ok==true){
//        file.close();
//    }

    //https://www.cnblogs.com/findumars/p/7062281.html
//    //QT使用系统默认程序打开文件
//    QDesktopServices::openUrl(QUrl::fromLocalFile("D:/Users/DJL/Desktop/chat-9.16/dfg.csv"));
//    QDesktopServices::openUrl(QUrl::fromLocalFile("D:/Users/DJL/Desktop/chat-9.16/TW3GOTY132S.part01.rar"));

    //打开文件所在文件夹并选中
    QString path="D:/Users/DJL/Desktop/chat-9.16/TW3GOTY132S.part01.rar";
    QProcess process;
    process.startDetached(QString("explorer /select,%1").arg(path.replace("/", "\\")));//启动后分离，即非阻塞
    //proc.start("explorer /select," + pathStr.replace("/", "\\"));
    process.waitForFinished();

//    QString path="D:/Users/DJL/Desktop/chat-9.16/dfg.csv";
//    ShellExecuteW(NULL, L"open", L"explorer", QString("/select, \"%1\"").arg(path).toStdWString().c_str(), NULL, SW_SHOW);


    DataOperate::Instance();
    DataOperate::Instance()->LoadSaveData();

    ReflexObject::Instance()->InitObjectIns();

    LogOnWin *pLogOnWin = dynamic_cast<LogOnWin*>(ReflexObject::Instance()->GetObjectIns("LogOnWin"));
    pLogOnWin->show();

    //    ClientMainWin *pClientMainWin = dynamic_cast<ClientMainWin*>(ReflexObject::Instance()->GetWinObjectIns("ClientMainWin"));
    //    pClientMainWin->show();

    //    ClientMainWin w;
    //    w.show();
    return a.exec();
}
