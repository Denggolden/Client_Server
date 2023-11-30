#pragma execution_character_set("UTF-8")
#include "ServerMainWin.h"

#include <QApplication>

#include "Src/ComHandleModel/ReflexObject.h"
#include "ServerMainWin.h"
#include "../../Common/TestTool.h"
#include "Src/DataHandleModel/DataBase.h"
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ReflexObject::Instance()->InitObjectIns();

    ServerMainWin *pServerMainWin = dynamic_cast<ServerMainWin*>(ReflexObject::Instance()->GetObjectIns("ServerMainWin"));
    pServerMainWin->show();

//    QFile file("C:/Users/admin/Desktop/chat/pic.PNG");
//    if (!file.open(QIODevice::ReadOnly))
//    {
//    }
//    QByteArray inByteArray = file.readAll();
//    qDebug()<<"inByteArray.size(): "<<inByteArray.size();
//    DataBase dataBase;
//    dataBase.InsertUserTable("用户1","123","456",inByteArray);
//    dataBase.InsertUserTable("用户2","234","567",inByteArray);
//    dataBase.InsertUserTable("用户3","345","678",inByteArray);
    //dataBase.InsertTest();
    //dataBase.SelectTest();
    //dataBase.LoadUserTable();

    //    TestTool testTool;
    //    testTool.GetCurProcessThreadsNumber();
    //    ServerMainWin w;
    //    w.show();
    return a.exec();
}
