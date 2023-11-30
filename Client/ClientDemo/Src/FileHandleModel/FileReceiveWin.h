#ifndef FILERECEIVEWIN_H
#define FILERECEIVEWIN_H

#include <QWidget>
#include <mutex>

class QProgressBar;
class QElapsedTimer;

namespace Ui {
class FileReceiveWin;
}

class FileReceiveWin : public QWidget
{
    Q_OBJECT

public:
    explicit FileReceiveWin(QString sendUserID,QString targetUserID,int curFileTransferBindID,QString filePath,qint64 allFileSize,QWidget *parent = nullptr);
    ~FileReceiveWin();

private:
    Ui::FileReceiveWin *ui;


public:
    void InitLabel();
    void InitProgressBar();
    void InitTimer();
    void InitFileSendSocketThread();

public:
    QTimer *pTimer=nullptr;
    QProgressBar *pProgressBar;
    QString FileName="";
    int MaxNum=0;
    int CurNum=0;

    std::mutex CurNumMutex;

    QString SendUserID="";
    QString TargetUserID="";
    int CurFileTransferBindID=0;
    QString FilePath="";
    qint64 AllFileSize=0;

    QString FileAllSize="";

    QElapsedTimer *pElapsedTimer=nullptr;
    float ElapsedTimer=0.0;

public slots:
    void OnTimeOut();
//    void SetFileNameSlots(QString fileName);
//    void SetProgressBarMaxNumSlots(int maxNum);
    void SetProgressBarCurNumSlots(int curNum);
};

#endif // FILERECEIVEWIN_H
