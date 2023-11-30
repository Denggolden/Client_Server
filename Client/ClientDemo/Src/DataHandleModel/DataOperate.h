#pragma execution_character_set("UTF-8")
#ifndef DATAOPERATE_H
#define DATAOPERATE_H

#include <QObject>
#include "../../Common/DataModel.h"
#include "../../Common/TransferDataModel.h"

class DataOperate
{

public:
    DataOperate(DataOperate&)=delete;
    DataOperate& operator=(const DataOperate&)=delete;
    static DataOperate* Instance();


public:
    bool JudgeTransferDataModelIsLegal(TransferDataModel &transferDataModel);

    void LoadSaveData();

    void LoadIniSetting();

    QString GetIniFileNode(QString groupName,QString key,QString defaultValue);
    void WriteIniFile(QString groupName,QString key,QString value);

    void SetServerIP(QString serverIP);
    void SetServerPort(QString serverPort);

    QString GetServerIP();
    QString GetServerPort();

private:
    DataOperate();
    static DataOperate Ins;
    QString ServerIP="";
    QString ServerPort="";
};

#endif // DATAOPERATE_H
