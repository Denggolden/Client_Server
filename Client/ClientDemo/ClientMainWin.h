#ifndef CLIENTMAINWIN_H
#define CLIENTMAINWIN_H

//#include <QWidget>
#include "../../Common/WidgetBase.h"

class QToolButton;
class UserInfoWin;
class InterfaceTestWin;

QT_BEGIN_NAMESPACE
namespace Ui { class ClientMainWin; }
QT_END_NAMESPACE

class ClientMainWin : public WidgetBase
{
    Q_OBJECT

public:
    ClientMainWin(QWidget *parent = nullptr);
    ~ClientMainWin();

private:
    Ui::ClientMainWin *ui;

public:
    virtual void InitClass() override;

public:
    void InitFrame();
    void InitToolButton();
    void InitStackedWidget();



public:
    QList<QToolButton*>ToolButtonList;

public:
    UserInfoWin *pUserInfoWin=nullptr;
    InterfaceTestWin *pInterfaceTestWin=nullptr;


public slots:
    void ToolButtonSlots();
};
#endif // CLIENTMAINWIN_H
