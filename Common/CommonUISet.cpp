#include "CommonUISet.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QTabWidget>
#include <QTabBar>

CommonUISet::CommonUISet()
{

}

void CommonUISet::InitTableWidgetSet(QTableWidget *tableWidget, QStringList header, int ratio[], int hHeaderHeight, int hHeaderMinWidth,bool hHeaderVisible)
{
    int ColumnCount=header.size();
    tableWidget->setColumnCount(ColumnCount);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//选择整行
    tableWidget->verticalHeader()->setVisible(false);  //隐藏垂直表头
    tableWidget->horizontalHeader()->setVisible(hHeaderVisible);  //隐藏水平表头
    tableWidget->setAlternatingRowColors(true); // 隔行变色
    //设置选中行的背景色，必须要显示设置，即代码设置或者在ui文件的控件中设置，用系统默认的是无法代码选中高亮某行
    tableWidget->setStyleSheet("selection-background-color:rgb(0,191,255)");
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//平均分配列宽
    tableWidget->horizontalHeader()->setFixedHeight(hHeaderHeight);//设置表头高度
    tableWidget->horizontalHeader()->setMinimumWidth(hHeaderMinWidth);//设置表格最小宽度 这句是关键

    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView{background-color:rgb(30,144,255);font:bold 8pt 'Microsoft YaHei';color: white;}"
                                                   "QHeaderView::section{background-color:rgb(30,144,255);font:bold 8pt 'Microsoft YaHei';color: white;}");  //行表头

    int totalWidth=tableWidget->horizontalHeader()->width();
    int allRatio=0;
    for (int index=0;index<ColumnCount;index++) {
        allRatio+=ratio[index];
    }
    for (int col=0;col<ColumnCount;col++) {
        tableWidget->setColumnWidth(col,totalWidth*ratio[col]/allRatio);
    }

    //设置表头
    tableWidget->setHorizontalHeaderLabels(header);

    //    for (int col=0;col<12 ;col++ ) {
    //        tableWidget->horizontalHeader()->setSectionResizeMode(col, QHeaderView::ResizeToContents);
    //    }

}

void CommonUISet::TableWidgetDynamicSet(QTableWidget *tableWidget, int ratio[], int columnCount)
{
    int totalWidth=tableWidget->horizontalHeader()->width();
    int allRatio=0;
    for (int index=0;index<columnCount;index++) {
        allRatio+=ratio[index];
    }
    for (int col=0;col<columnCount;col++) {
        tableWidget->setColumnWidth(col,totalWidth*ratio[col]/allRatio);
    }
}

void CommonUISet::InitTabWidgetTabSet(QTabWidget *tabWidget, QList<QString> tabNameList,int curPageIndex)
{
    int tabNameListSize=tabNameList.size();
    for(int index=0;index<tabNameListSize;index++){
        QString tabName= tabNameList.at(index);
        int tabNameSize=tabName.size();
        QString tabNameStr="";
        for(int i=0;i<tabNameSize;i++){
            tabNameStr.append(tabName.at(i));
            if(i!=(tabNameSize-1))
                tabNameStr.append("\n");
        }

        QLabel *labelTab=new QLabel(tabNameStr);
        //labelTab->setStyleSheet("QLabel {background-color: transparent;}");
        labelTab->setAlignment(Qt::AlignCenter);
        tabWidget->tabBar()->setTabButton(index,QTabBar::ButtonPosition::RightSide,labelTab);
        tabWidget->tabBar()->tabButton(index,QTabBar::ButtonPosition::RightSide)->setFixedSize(45,85);
    }
    TabWidgetTabDynamicSet(tabWidget,curPageIndex);
}

void CommonUISet::TabWidgetTabDynamicSet(QTabWidget *tabWidget, int curPageIndex)
{
    int tabWidgetPageCount=tabWidget->count();
    for(int i=0;i<tabWidgetPageCount;i++){
        if(i==curPageIndex)
            tabWidget->tabBar()->tabButton(i,QTabBar::ButtonPosition::RightSide)->setStyleSheet("background-color:rgba(0,0,0,0);color:DodgerBlue;font: bold 10pt 'Microsoft YaHei';");
        else
            tabWidget->tabBar()->tabButton(i,QTabBar::ButtonPosition::RightSide)->setStyleSheet("background-color:rgba(0,0,0,0);color:White;font: bold 10pt 'Microsoft YaHei';");
    }
}
