#pragma execution_character_set("UTF-8")
#include "DataOperate.h"
#include <QDataStream>
#include <QSettings>

DataOperate DataOperate::Ins;

DataOperate::DataOperate()
{

}

DataOperate *DataOperate::Instance()
{
    return &Ins;
}

bool DataOperate::JudgeTransferDataModelIsLegal(TransferDataModel &transferDataModel)
{
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out <<transferDataModel;
    int byteArraySize= byteArray.size();

    if(byteArraySize>=64512)
        return false;
    return true;
}

void DataOperate::LoadSaveData()
{
    LoadIniSetting();
}

void DataOperate::LoadIniSetting()
{
    QString fileName = QCoreApplication::applicationDirPath() + "/ClientSetting.ini";
    QFile file(fileName);
    if(file.exists()){
        //创建QSettings对象并指定ini文件路径并将格式设置为ini
        //QSettings setting(fileName, QSettings::IniFormat);
        ServerIP=GetIniFileNode("ServerGroup","ServerIP","192.168.1.0");
        ServerPort=GetIniFileNode("ServerGroup","ServerPort","8520");
    }
    else{//不存在则写入初始值

        WriteIniFile("ServerGroup","ServerIP","192.168.1.0");
        WriteIniFile("ServerGroup","ServerPort","8520");
    }
}

QString DataOperate::GetIniFileNode(QString groupName, QString key, QString defaultValue)
{
    QString fileName = QCoreApplication::applicationDirPath() + "/ClientSetting.ini";
    QString value=defaultValue;
    //创建QSettings对象并指定ini文件路径并将格式设置为ini
    QSettings setting(fileName, QSettings::IniFormat);
    setting.setIniCodec("utf-8");// 解决乱码
    //添加第二个分组以及其键值对（创建分组的第二种方法）
    QString Key=groupName+"/"+key;
    if (setting.contains(Key)==true)
        value= setting.value(Key).toString();
    else {
        WriteIniFile(groupName,key,defaultValue);
    }
    return value;
}

void DataOperate::WriteIniFile(QString groupName, QString key, QString value)
{
    QString fileName = QCoreApplication::applicationDirPath() + "/ClientSetting.ini";
    //创建QSettings对象并指定ini文件路径并将格式设置为ini
    QSettings setting(fileName, QSettings::IniFormat);
    setting.setIniCodec("utf-8");// 解决乱码
    //添加第二个分组以及其键值对（创建分组的第二种方法）
    QString Key=groupName+"/"+key;
    setting.setValue(Key, value);
}

void DataOperate::SetServerIP(QString serverIP)
{
    ServerIP=serverIP;
    WriteIniFile("ServerGroup","ServerIP",serverIP);
}

void DataOperate::SetServerPort(QString serverPort)
{
    ServerPort=serverPort;
    WriteIniFile("ServerGroup","ServerPort",serverPort);
}

QString DataOperate::GetServerIP()
{
    return ServerIP;
}

QString DataOperate::GetServerPort()
{
    return ServerPort;
}
