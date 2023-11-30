#ifndef INTERFACETESTWIN_H
#define INTERFACETESTWIN_H

//#include <QWidget>
#include "../../Common/WidgetBase.h"
#include "../../Common/TransferDataModel.h"
#include <mutex>

class FileReceiveWin;
class FileSendWin;

namespace Ui {
class InterfaceTestWin;
}

class InterfaceTestWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit InterfaceTestWin(QWidget *parent = nullptr);
    ~InterfaceTestWin();

private:
    Ui::InterfaceTestWin *ui;

public:
    virtual void InitClass() override;

public:
//    void InitPushButton();
//    void ReadySendFile();
    void HandelOperate(TransferDataModel &transferDataModel);
    void FileReceiveWinHandle(QString sendUserID,QString targetUserID,int curFileTransferBindID,int fileByteArrSize);
    void FileSendWinHandle(QString sendUserID,QString targetUserID,int curFileTransferBindID,int fileByteArrSize);


public:
//    QList<QPushButton*>PushButtonList;
    QList<FileReceiveWin*>FileReceiveWinList;
    QList<FileSendWin*>FileSendWinList;

    std::mutex FileReceiveWinListMutex;
    std::mutex FileSendWinListMutex;

signals:
    void HandelOperateSignal(TransferDataModel &transferDataModel);
    void FilePossessHandleSignal(int curNum);

public slots:
    void PushButtonSlots();
    void HandelOperateSolts(TransferDataModel &transferDataModel);
};

#endif // INTERFACETESTWIN_H
