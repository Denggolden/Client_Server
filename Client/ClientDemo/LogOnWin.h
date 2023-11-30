#ifndef LOGONWIN_H
#define LOGONWIN_H

//#include <QWidget>
#include "../../Common/WidgetBase.h"
#include <mutex>
#include "../../Common/TransferDataModel.h"

class QPushButton;
class QProgressBar;

namespace Ui {
class LogOnWin;
}

class LogOnWin : public WidgetBase
{
    Q_OBJECT

public:
    explicit LogOnWin(QWidget *parent = nullptr);
    ~LogOnWin();

private:
    Ui::LogOnWin *ui;

public:
    virtual void InitClass() override;

public:
    void InitFrame();
    void InitPushButton();
    void InitlineEdit();
    void InitLable();
    void InitCheckBox();
    void InitProgressBar();

    void RegisterOrLogin(QString infoMsg);

    void SetIsServerRetMsgFlag(TransferDataModel transferDataModel);
    void RegisterOrLoginRetResult();

public:
    QList<QPushButton*>PushButtonList;
    QProgressBar *pProgressBar;
    bool ProgressBarStop;
    bool IsServerRetMsgFlag=false;
    std::mutex IsServerRetMsgFlagMutex;
    std::mutex ProgressBarStopMutex;
    TransferDataModel TransferDataModelG;

signals:
    void ProgressBarSignal();

public slots:
    void PushButtonSlots();
    void ProgressBarSlots();

protected:
    void showEvent(QShowEvent *event) override;
    bool eventFilter(QObject * object, QEvent * event) override;
    void paintEvent(QPaintEvent *paint) override;
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
};

#endif // LOGONWIN_H
