#ifndef SERVERMAINWIN_H
#define SERVERMAINWIN_H

//#include <QWidget>
#include "../../Common/WidgetBase.h"
#include "../../Common/TransferDataModel.h"

class QTableWidget;
class QPushButton;

QT_BEGIN_NAMESPACE
namespace Ui { class ServerMainWin; }
QT_END_NAMESPACE

class ServerMainWin : public WidgetBase
{
    Q_OBJECT

public:
    ServerMainWin(QWidget *parent = nullptr);
    ~ServerMainWin();

private:
    Ui::ServerMainWin *ui;

public:
    virtual void InitClass() override;

public:
    void InitGroupBox();
    void InitTableWidget(QTableWidget *tableWidget);
    void InitPushButton();
    void InitlineEdit();
    void InitLabel();
    void InitComboBox();
    void InitTextEdit();


    void UpDateClientUserTableWidgetList();
    void UpDateClientUserTableWidgetList(QTableWidget *tableWidget);

    void UpDateServerIPAddressList(QList<QString> iPAddressList);
    void UpDateServerPort(QString port);

    void AppendTextEditLogText(QString logInfo);
    void AppendTextEditLogText(TransferDataModel transferDataModel);

    void ConnectOrDisConnect(bool isConnect);

    void FileSend();

public:
    QList<QPushButton*>PushButtonList;
    bool IsConnect=false;

signals:
    void UpDateTextEditLogText(TransferDataModel transferDataModel);
    void UpDateClientUserList();

public slots:
    void PushButtonSlots();
    void currentIndexChangedSlots(int index);
    void UpDateTextEditLogTextSlots(TransferDataModel transferDataModel);
    void UpDateClientUserListSlots();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *paint) override;
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
};
#endif // SERVERMAINWIN_H
