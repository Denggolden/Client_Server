#ifndef INTERFACETESTWIN_H
#define INTERFACETESTWIN_H

//#include <QWidget>
#include "../../Common/WidgetBase.h"
#include "../../Common/TransferDataModel.h"

class QPushButton;

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
    void InitPushButton();

    void ReadySendFile();
    void HandelOperate(TransferDataModel &transferDataModel);


public:
    QList<QPushButton*>PushButtonList;


signals:
    void HandelOperateSignal(TransferDataModel &transferDataModel);

public slots:
    void PushButtonSlots();
    void HandelOperateSolts(TransferDataModel &transferDataModel);
};

#endif // INTERFACETESTWIN_H
