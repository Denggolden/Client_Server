#ifndef NETASSISTANTWIN_H
#define NETASSISTANTWIN_H

//#include <QWidget>
#include "../../Common/WidgetBase.h"


class QPushButton;

namespace Ui {
class NetAssistantWin;
}

class NetAssistantWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit NetAssistantWin(QWidget *parent = nullptr);
    ~NetAssistantWin();

private:
    Ui::NetAssistantWin *ui;

public:
    virtual void InitClass() override;

public:
    void InitPushButton();
    void InitlineEdit();
    void InitLabel();
    void InitComboBox();

    void ConnectOrDisConnect(bool isConnect);

    void ModifyConnectStatus(QString connectStatus,short status);

    void UpDateServerIPAddressList(QList<QString> iPAddressList);
    void UpDateServerPort(QString port);

public:
    QList<QPushButton*>PushButtonList;
    bool IsConnect=false;

public slots:
    void PushButtonSlots();
    void currentIndexChangedSlots(int index);

protected:
    void paintEvent(QPaintEvent *paint) override;
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
};

#endif // NETASSISTANTWIN_H
