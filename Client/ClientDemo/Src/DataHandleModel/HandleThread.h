#ifndef HANDLETHREAD_H
#define HANDLETHREAD_H

//#include <QObject>
#include "../../Common/ObjectBase.h"
#include <mutex>
#include <QQueue>
#include "../../Common/TransferDataModel.h"

class HandleThread : public ObjectBase
{
    Q_OBJECT
public:
    explicit HandleThread(QObject *parent = nullptr);
    ~HandleThread();

public:
    virtual void InitClass() override;


public:
    void ReceiveMsg(TransferDataModel transferDataModel);

    void HandleMsgLoop();
    void HandleMsg(TransferDataModel &transferDataModel);


public:
    QQueue<TransferDataModel> MsgInfoQueue;
    std::mutex MsgQueueMutex;

signals:

};

#endif // HANDLETHREAD_H
