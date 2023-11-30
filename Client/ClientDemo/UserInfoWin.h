#ifndef USERINFOWIN_H
#define USERINFOWIN_H

//#include <QWidget>
#include "../../Common/WidgetBase.h"
#include "../../Common/TransferDataModel.h"

class QPushButton;
class QFile;

namespace Ui {
class UserInfoWin;
}

class UserInfoWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit UserInfoWin(QWidget *parent = nullptr);
    ~UserInfoWin();

private:
    Ui::UserInfoWin *ui;

public:
    virtual void InitClass() override;

public:
    void InitGroupBox();
    void InitPushButton();
    void InitlineEdit();
    void InitLable();

    void SetUserInfo(QString userName, QString userID, QString userPassword, QByteArray userHeadImgByte);

    void ChangeUserHeadImg();

    void ModifyUserInfoFun();
    void ModifyUserRetInfoFun(TransferDataModel &transferDataModel);

    void LogoutUser();
    void LogoutUserRetFun(TransferDataModel &transferDataModel);

    void writeFile(TransferDataModel &transferDataModel);
public:
    QList<QPushButton*>PushButtonList;

    QList<QFile*> FileList;

    QString UserName="";
    QString UserID="";
    QString UserPassword="";
    QByteArray UserHeadImgByte;
    QByteArray TempUserHeadImgByte;


signals:
    void ModifyUserRetInfoFunSignal(TransferDataModel &transferDataModel);
    void LogoutUserRetFunSignal(TransferDataModel &transferDataModel);

public slots:
    void PushButtonSlots();
    void ModifyUserRetInfoFunSlots(TransferDataModel &transferDataModel);
    void LogoutUserRetFunSignalSlots(TransferDataModel &transferDataModel);

protected:
    bool eventFilter(QObject * object, QEvent * event) override;
};

#endif // USERINFOWIN_H
