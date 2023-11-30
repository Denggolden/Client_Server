#include "ReflexObject.h"
#include <QDebug>
#include "ClientMainWin.h"
#include "LogOnWin.h"
#include "Src/NetHandleModel/ClientSocket.h"
#include "Src/NetHandleModel/NetAssistantWin.h"
#include "Src/DataHandleModel/HandleThread.h"
#include "UserInfoWin.h"
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
    pClientMainWin=new ClientMainWin();
    pClientMainWin->setObjectName("ClientMainWin");
    Ins.AddObjectIns(pClientMainWin);

    pUserInfoWin=new UserInfoWin();
    pUserInfoWin->setObjectName("UserInfoWin");
    Ins.AddObjectIns(pUserInfoWin);

    pFileManageWin=new FileManageWin();
    pFileManageWin->setObjectName("FileManageWin");
    Ins.AddObjectIns(pFileManageWin);

    pLogOnWin=new LogOnWin();
    pLogOnWin->setObjectName("LogOnWin");
    Ins.AddObjectIns(pLogOnWin);

    pNetAssistantWin=new NetAssistantWin();
    pNetAssistantWin->setObjectName("NetAssistantWin");
    Ins.AddObjectIns(pNetAssistantWin);

    pInterfaceTestWin=new InterfaceTestWin();
    pInterfaceTestWin->setObjectName("InterfaceTestWin");
    Ins.AddObjectIns(pInterfaceTestWin);

    //QObject
    pClientSocket=new ClientSocket();
    pClientSocket->setObjectName("ClientSocket");
    Ins.AddObjectIns(pClientSocket);

//    pClientSocket=new ClientSocket();
//    pClientSocket->setObjectName("ClientSocket1");
//    Ins.AddObjectIns(pClientSocket);

    pHandleThread=new HandleThread();
    pHandleThread->setObjectName("HandleThread");
    Ins.AddObjectIns(pHandleThread);

    InitObjectClass();
}

void ReflexObject::InitObjectClass()
{
    //https://blog.csdn.net/stephan14/article/details/48883325#:~:text=%E7%BB%8F%E8%BF%87%E6%88%91%E6%9F%A5%E9%98%85%E7%BD%91%E4%B8%8A%E7%9A%84%E8%B5%84%E6%96%99%EF%BC%8C%E5%8F%91%E7%8E%B0%E5%A6%82%E6%9E%9C%E4%BD%A0%E5%85%81%E8%AE%B8%E7%B1%BB%E6%9C%89%E8%99%9A%E5%87%BD%E6%95%B0%E7%9A%84%E8%AF%9D%EF%BC%8C%E5%8F%AF%E4%BB%A5%E9%80%9A%E8%BF%87%E5%9C%A8%E5%9F%BA%E7%B1%BB%E4%B8%AD%E6%B7%BB%E5%8A%A0%E8%99%9A%E5%87%BD%E6%95%B0%E7%B1%BB%E5%AE%9E%E7%8E%B0%E5%88%A4%E6%96%AD%E6%8C%87%E9%92%88%E6%89%80%E6%8C%87%E5%90%91%E7%9A%84%E7%9C%9F%E6%AD%A3%E5%AD%90%E7%B1%BB%EF%BC%9B%E5%A6%82%E6%9E%9C%E7%B1%BB%E4%B8%AD%E4%B8%8D%E5%85%81%E8%AE%B8%E5%87%BA%E7%8E%B0%E8%99%9A%E5%87%BD%E6%95%B0%EF%BC%8C%E9%82%A3%E4%B9%88%E4%BD%A0%E5%8F%AA%E8%83%BD%E5%9C%A8%E6%AF%8F%E4%B8%AA%E7%B1%BB%E4%B8%AD%E5%AE%9E%E7%8E%B0%E7%B1%BB%E4%BC%BC%E4%BA%8EgetType%20%28%29%E7%9A%84%E5%87%BD%E6%95%B0%EF%BC%8C%E8%AE%A9%E5%AE%83%E8%BF%94%E5%9B%9E%E8%83%BD%E8%A1%A8%E7%A4%BA%E6%89%80%E6%9C%89%E7%B1%BB%E7%B1%BB%E5%9E%8B%E7%9A%84%E6%9E%9A%E4%B8%BE%E7%B1%BB%E5%9E%8B%EF%BC%88%E8%BF%99%E9%87%8C%E4%BB%8B%E7%BB%8D%E4%B8%80%E4%B8%AAstackoverflow%E4%B8%8A%E7%9A%84%E5%9B%9E%E7%AD%94%20%E7%82%B9%E5%87%BB%E6%89%93%E5%BC%80%E9%93%BE%E6%8E%A5%20%EF%BC%89%E3%80%82%20%E5%BD%93%E7%84%B6%EF%BC%8C%E4%B8%80%E8%88%AC%E4%B8%8D%E4%BC%9A%E5%87%BA%E7%8E%B0%E5%90%8E%E8%80%85%E7%9A%84%E6%83%85%E5%86%B5%EF%BC%8C%E6%88%91%E8%BF%98%E6%98%AF%E5%BB%BA%E8%AE%AE%E5%86%8D%E7%94%A8%E5%89%8D%E8%80%85%E7%9A%84%E6%96%B9%E6%B3%95%EF%BC%8C%E6%9C%80%E7%AE%80%E5%8D%95%E7%9A%84%E6%96%B9%E5%BC%8F%E5%B0%B1%E6%98%AF%E5%B0%86%E6%9E%90%E6%9E%84%E5%87%BD%E6%95%B0%E8%AE%BE%E7%BD%AE%E6%88%90%E8%99%9A%E5%87%BD%E6%95%B0%E3%80%82%20C%2B%2B%20%E4%B8%AD%EF%BC%8C%E5%8F%AF%E4%BB%A5%E4%BD%BF%E7%94%A8%20%E5%9F%BA%E7%B1%BB,%E6%9D%A5%E8%B0%83%E7%94%A8%E5%AF%B9%E5%BA%94%E7%9A%84%E9%87%8D%E8%BD%BD%E5%87%BD%E6%95%B0%EF%BC%8C%E8%BF%99%E5%B0%B1%E5%AE%9E%E7%8E%B0%E4%BA%86%E5%A4%9A%E6%80%81%E6%80%A7%E3%80%82%20%E4%B8%BE%E4%B8%AA%E4%BE%8B%E5%AD%90%EF%BC%8C%E5%81%87%E8%AE%BE%E6%9C%89%E4%B8%80%E4%B8%AA%20%E5%AD%90%E7%B1%BB%20%E5%9F%BA%E7%B1%BB%E6%8C%87%E9%92%88%E6%8C%87%E5%90%91%E5%AD%90%E7%B1%BB%20%E7%9A%84%20sound%20%28%29%20%E5%87%BD%E6%95%B0%E3%80%82

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
