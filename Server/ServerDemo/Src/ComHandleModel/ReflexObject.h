#ifndef REFLEXOBJECT_H
#define REFLEXOBJECT_H

//#include <QObject>
//#include <QWidget>
#include "../../Common/ObjectBase.h"
#include "../../Common/WidgetBase.h"

#include <QMap>

class HandleThread;
class ServerSocket;
class ServerMainWin;
class InterfaceTestWin;
class FileManageWin;

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
    HandleThread *pHandleThread=nullptr;
    ServerSocket *pServerSocket=nullptr;

    //QWidget
    ServerMainWin *pServerMainWin=nullptr;
    InterfaceTestWin *pInterfaceTestWin=nullptr;
    FileManageWin *pFileManageWin=nullptr;

private:
    ReflexObject();
    static ReflexObject Ins;
    QMap<QString, QObject*> ObjectMap;
};

#endif // REFLEXOBJECT_H
