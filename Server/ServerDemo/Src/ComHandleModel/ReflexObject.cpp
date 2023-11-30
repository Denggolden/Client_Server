#include "ReflexObject.h"
#include <QDebug>
#include "ServerMainWin.h"
#include "Src/NetHandleModel/ServerSocket.h"
#include "Src/DataHandleModel/HandleThread.h"
#include "Src/TestHandleModel/InterfaceTestWin.h"
#include "Src/FileHandleModel/FileManageWin.h"

ReflexObject ReflexObject::Ins;

void ReflexObject::AddObjectIns(QObject *pObject)
{
    QString objectName=pObject->objectName();
    auto it = ObjectMap.find(objectName);
    if (it == ObjectMap.end()) {//找不到元素
        ObjectMap.insert(objectName, pObject);
        qDebug()<<"---"<<objectName<<" Create---"<<"  Address:"<<(void*)pObject;
    }
    else {// 找到元素
    }
}

void ReflexObject::DeleteObjectIns(QString objectName)
{
    if(ObjectMap.contains(objectName)){
        qDebug()<<"---"<<objectName<<" Delete---"<<"  Address:"<<(void*)ObjectMap.find(objectName).value();
        int DeleteSuccess= ObjectMap.remove(objectName);
        if(DeleteSuccess==1){
            qDebug()<<"---"<<objectName<<" Delete---";
        }
    }
}

//void ReflexObject::DeleteObjectIns(QString objectName)
//{

//}

QObject *ReflexObject::GetObjectIns(QString objectName)
{
    if(ObjectMap.contains(objectName)){
        qDebug()<<"---"<<objectName<<" find---"<<"  Address:"<<(void*)ObjectMap.find(objectName).value();
        return ObjectMap.find(objectName).value();
    }
    return nullptr;
}

void ReflexObject::InitObjectIns()
{

    //QWidget
    pServerMainWin=new ServerMainWin();
    pServerMainWin->setObjectName("ServerMainWin");
    Ins.AddObjectIns(pServerMainWin);

    pInterfaceTestWin=new InterfaceTestWin();
    pInterfaceTestWin->setObjectName("InterfaceTestWin");
    Ins.AddObjectIns(pInterfaceTestWin);

    pFileManageWin=new FileManageWin();
    pFileManageWin->setObjectName("FileManageWin");
    Ins.AddObjectIns(pFileManageWin);

    //QObject
    pHandleThread=new HandleThread();
    pHandleThread->setObjectName("HandleThread");
    Ins.AddObjectIns(pHandleThread);

    pServerSocket=new ServerSocket();
    pServerSocket->setObjectName("ServerSocket");
    Ins.AddObjectIns(pServerSocket);

    InitObjectClass();
}

void ReflexObject::InitObjectClass()
{
    QMap<QString, QObject*>::iterator it;
    for (it = ObjectMap.begin(); it != ObjectMap.end(); ++it){
        qDebug() << it.key() << ":" << it.value();
        if ((dynamic_cast<ObjectBase*>(it.value())) != nullptr) {
            //qDebug() << "此类为 ObjectBase";
            ObjectBase *pObjectBase = dynamic_cast<ObjectBase*>(it.value());
            pObjectBase->InitClass();
        }
        if ((dynamic_cast<WidgetBase*>(it.value())) != nullptr) {
            //qDebug() << "此类为 WidgetBase";
            WidgetBase *pWidgetBase = dynamic_cast<WidgetBase*>(it.value());
            pWidgetBase->InitClass();
        }
    }
}

ReflexObject::ReflexObject()
{

}

ReflexObject *ReflexObject::Instance()
{
    return &Ins;
}
