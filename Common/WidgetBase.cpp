#include "WidgetBase.h"
//#include "ui_WidgetBase.h"
#include <QDebug>

WidgetBase::WidgetBase(QWidget *parent) :
    QWidget(parent)/*,
    ui(new Ui::WidgetBase)*/
{
    //ui->setupUi(this);
}

WidgetBase::~WidgetBase()
{
    //delete ui;
}

void WidgetBase::InitClass()
{
    qDebug()<<"WidgetBase::InitClass()";
}
