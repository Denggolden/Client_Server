#ifndef TESTTOOL_H
#define TESTTOOL_H

#include <QObject>

class TestTool
{
public:
    TestTool();
    ~TestTool();

public:
    void GetCurProcessThreadsNumber();

    static QString CalculateFileSize(int fileSize);
    static QString CalculateTransferSpeed(float speedByte);
};

#endif // TESTTOOL_H
