#pragma once
#ifndef MYTHREAD_H
#define MYTHREAD_H

#if _MSC_VER >= 1600 // MSVC2015>1899, MSVC VER=l4 . 0
#pragma execution_character_set("utf-8")
#endif

#include <QThread>
#include <QTimer>
#include <QTextCodec>
#include <iostream>

#include "pre_defination.h"
#include "util.h"

using namespace std;



class mythread : public QThread
{
    Q_OBJECT

public:
    explicit mythread(QObject *parent = nullptr);

private:
    FILE* fp = NULL;

    // 需要用到的Packet
    PKG_DATA_ERROR_OACK rcvdPkt = { 0 }, dataPkt = { htons(OP_DATA), { 0 } };
    PKG_ACK ackPkt = { htons(OP_ACK), 0 };
    PKG_REQUEST reqPkt = {  0 , { 0 } };

    // 任务参数
    int operation;
    SOCKET socketFd = INVALID_SOCKET;
    SOCKADDR_IN serverAddr = { 0 };
    const char *serverFile = NULL, *clientFile = NULL;
    int serverAddr_len = sizeof(serverAddr), mode = 0;
    EXTEND extended = { 0 };

    // 扩展参数
    int tSize;
    TIMEVAL tvTimeOut;
    unsigned short blkSize, timeOut;

    // 统计信息
    int request_len;
    double percent = 0;
    int bytesRecv, bytesSend, lastBytesRecv, lastBytesSend;
    int totalRetransmitCount;
    int fileSize;
    WORD curBlock;
    clock_t startTime, endTime;

    int MessageChecking();
    int OpenClientFile();
    int sendReqest();

    int DownloadFile();
    int UploadFile();

    int sendAck(WORD block);
    int sendPkt(const char* buf, int len);
    int recvPkt(char* buf, int len);
    int getOptionFromOAckPkt(int rcvdSize);
    int waitForPkt(int timeOutMs, int &rcvdSize);
    int waitForOACK(int timeOutMs, int &rcvdSize);
    int waitForSpecificPkt(WORD block, int timeOutMs, int &rcvdSize, int pktType);

    void processStop(bool isSuccess);

    QTimer *timer;

signals:
    void showMessage(int code, QString message);
    void sendProcessBarMaximum(int max);
    void sendProcessBarValue(int val);
    void sendTSize(QString tsize);
    void sendUploadSpeed(QString speed);
    void sendDownloadSpeed(QString speed);

protected:
    void run() override;

public slots:
    void getClientInfo(SOCKET sock, SOCKADDR_IN serAddress, const char* remoFile, const char* locaFile, int mode, EXTEND ext, int oper);
    void recvTimeOutSignal();
};




#endif // MYTHREAD_H
