#ifndef FILEMANAGEWIN_H
#define FILEMANAGEWIN_H

//#include <QWidget>
#include "../../Common/WidgetBase.h"
#include "../../Common/TransferDataModel.h"

class QPushButton;

namespace Ui {
class FileManageWin;
}

class FileManageWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit FileManageWin(QWidget *parent = nullptr);
    ~FileManageWin();

private:
    Ui::FileManageWin *ui;

public:
    virtual void InitClass() override;

public:
    void InitTabWidget();
    void InitPushButton();
    void InitGroupBox();
    void InitListWidget();

    void HandelOperate(TransferDataModel &transferDataModel);

    void CreateFileSendWinItem(TransferDataModel &transferDataModel);
    void CreateFileReceiveWinItem(TransferDataModel &transferDataModel);

public:
    QList<QPushButton*>PushButtonList;


signals:
    void HandelOperateSignal(TransferDataModel &transferDataModel);

public slots:
    void TabWidgetcurrentChanged(int index);
    void PushButtonSlots();
    void HandelOperateSolts(TransferDataModel &transferDataModel);

protected :
    virtual void closeEvent(QCloseEvent *event) override;
};

#endif // FILEMANAGEWIN_H
