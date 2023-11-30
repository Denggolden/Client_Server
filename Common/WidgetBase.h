#ifndef WIDGETBASE_H
#define WIDGETBASE_H

#include <QWidget>

namespace Ui {
class WidgetBase;
}

class WidgetBase : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetBase(QWidget *parent = nullptr);
    virtual ~WidgetBase();

//private:
//    Ui::WidgetBase *ui;

public:
    virtual void InitClass();
};

#endif // WIDGETBASE_H
