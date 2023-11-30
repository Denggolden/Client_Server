#ifndef FILESENDWIN_H
#define FILESENDWIN_H

#include <QWidget>
#include <mutex>

class QProgressBar;
class QElapsedTimer;

namespace Ui {
class FileTransferWin;
}

class FileSendWin : public QWidget
{
    Q_OBJECT

public:
    explicit FileSendWin(QString sendUserID,QString targetUserID,int curFileTransferBindID,QString filePath,QWidget *parent = nullptr);
    ~FileSendWin();

private:
    Ui::FileTransferWin *ui;

public:
    void InitLabel();
    void InitProgressBar();
    void InitTimer();

    QString GetSendUserID();
    QString GetTargetUserID();
    int GetCurFileTransferBindID();

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

    QString FileAllSize="";

    QElapsedTimer *pElapsedTimer=nullptr;
    float ElapsedTimer=0.0;

public slots:
    void OnTimeOut();
//    void SetFileNameSlots(QString fileName);
//    void SetProgressBarMaxNumSlots(int maxNum);
    void SetProgressBarCurNumSlots(int curNum);
};

#endif // FILESENDWIN_H
