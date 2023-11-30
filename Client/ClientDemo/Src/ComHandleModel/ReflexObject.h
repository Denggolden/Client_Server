#pragma execution_character_set("UTF-8")
#ifndef REFLEXOBJECT_H
#define REFLEXOBJECT_H

//#include <QObject>
//#include <QWidget>
#include "../../Common/ObjectBase.h"
#include "../../Common/WidgetBase.h"

#include <QMap>

class UserInfoWin;

class ClientMainWin;
class LogOnWin;

class InterfaceTestWin;

class ClientSocket;
class NetAssistantWin;
class FileManageWin;

class HandleThread;

class ReflexObject
{
    //Q_OBJECT
public:
    void AddObjectIns(QObject* pObject);

    //void DeleteWinObjectIns(QString objectName);
    void DeleteObjectIns(QString objectName);

    QObject *GetObjectIns(QString objectName);

    void InitObjectIns();
    void InitObjectClass();

public:
    ReflexObject(ReflexObject&) = delete;
    ReflexObject& operator=(const ReflexObject&) = delete;
    static ReflexObject* Instance();


public:
    //QObject
    ClientSocket *pClientSocket=nullptr;
    HandleThread *pHandleThread=nullptr;

    //QWidget
    UserInfoWin *pUserInfoWin=nullptr;

    FileManageWin *pFileManageWin=nullptr;

    ClientMainWin *pClientMainWin=nullptr;
    InterfaceTestWin *pInterfaceTestWin=nullptr;
    LogOnWin *pLogOnWin=nullptr;
    NetAssistantWin *pNetAssistantWin=nullptr;

private:
    ReflexObject();
    static ReflexObject Ins;
    QMap<QString, QObject*> ObjectMap;
};

#endif // REFLEXOBJECT_H
