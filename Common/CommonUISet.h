#ifndef COMMONUISET_H
#define COMMONUISET_H

#include <QObject>

class QTableWidget;
class QTabWidget;

class CommonUISet
{
public:
    CommonUISet();
    void InitTableWidgetSet(QTableWidget *tableWidget,QStringList header,int ratio[],int hHeaderHeight,int hHeaderMinWidth,bool hHeaderVisible);
    void TableWidgetDynamicSet(QTableWidget *tableWidget,int ratio[],int columnCount);


    void InitTabWidgetTabSet(QTabWidget *tabWidget,QList<QString> tabNameList,int curPageIndex);
    void TabWidgetTabDynamicSet(QTabWidget *tabWidget,int curPageIndex);
};

#endif // COMMONUISET_H
