#pragma execution_character_set("utf-8")
#include "mythread.h"

mythread::mythread(QObject *parent) : QThread(parent) {}
void mythread::run() {
    // 展示任务信息
    if (operation == OP_READ_REQ)
    {
        emit showMessage(NO_ERROR, QString("--------------------- READ START ---------------------"));
    }
    if (operation == OP_WRITE_REQ)
    {
        emit showMessage(NO_ERROR, QString("--------------------- WRITE START --------------------"));
    }
    emit showMessage(NO_ERROR, QString("LOCAL FILE  : %1").arg(QString::fromLocal8Bit(clientFile)));
    emit showMessage(NO_ERROR, QString("REMOTE FILE : %1").arg(QString::fromLocal8Bit(serverFile)));
    char tempIP[16] = "0";
    InetNtopA(AF_INET, &serverAddr.sin_addr, tempIP, 16);
    emit showMessage(NO_ERROR, QString("SERVER IP   : %1").arg(tempIP));
    emit showMessage(NO_ERROR, QString("SERVER PORT : %1").arg(ntohs(serverAddr.sin_port)));

    // 检查参数是否正确
    if (MessageChecking() != TFTP_NO_ERROR)
    {
        processStop(false);
        return;
    }

    // 打开客户端文件
    if (OpenClientFile() != TFTP_NO_ERROR)
    {
        processStop(false);
        return;
    }

    // 发送请求报文
    if (operation == OP_WRITE_REQ)
    {
        tSize = get_filesize(clientFile);
    }
    if ((request_len = sendReqest()) < TFTP_NO_ERROR)
    {
        processStop(false); return;
    }

    // 设置进度条和tsize
    emit sendProcessBarMaximum(operation == OP_WRITE_REQ ? (tSize <= 0 ? 0 : tSize) : 0);
    emit sendTSize(operation == OP_WRITE_REQ ? QString::number(tSize <= 0 ? 0 : tSize) : "Unknown");

    // Execute the task. 执行任务
    startTime = clock();
    if (operation == OP_READ_REQ) { DownloadFile(); }
    if (operation == OP_WRITE_REQ) { UploadFile(); }
    quit();
}




// 得到相应的服务器IP地址以及端口号
// 上传和下载的文件名
// 选择的上传和下载模式
// 是否添加扩展选项
void mythread::getClientInfo(SOCKET sock, SOCKADDR_IN serverAddress, const char* remoFile, const char* locaFile, int mode, EXTEND ext, int oper) {
    this->serverAddr = serverAddress;
    this->serverFile = remoFile;
    this->clientFile = locaFile;
    this->mode = mode;
    this->extended = ext;
    this->operation = oper;

    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    // 设置默认的扩展选项
    tSize = 0;
    blkSize = TFTP_DEFAULT_BLOCK_SIZE;
    timeOut = TFTP_DEFAULT_TIMEOUT_SEC;
    tvTimeOut = { timeOut, 0 };
    // 设置进度条初始状态
    lastBytesRecv = lastBytesSend = bytesRecv = bytesSend = 0;
    totalRetransmitCount = fileSize = 0;
    percent = 0;

    this->socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

// 检查当前的参数
int mythread::MessageChecking() {
    // 检查报文长度
    size_t MessageChecking = FULLSTRLEN(serverFile) + FULLSTRLEN(mode == MODE_NETASCII ? "netascii" : "octet");
    // 判断是否有扩展选项
    if (extended.Extended_flag)
    {
        // 若为上传则获取文件大小
        int temp_size = (operation == OP_WRITE_REQ ? get_filesize(clientFile) : 0);
        // 三个扩展选项
        MessageChecking += extended.tsize_flag ? FULLSTRLEN("tsize") + FULLNUMLEN(0) : FULLNUMLEN(temp_size <= 0 ? 0 : temp_size);
        MessageChecking += extended.blksize_flag ? FULLSTRLEN("blksize") + FULLNUMLEN(extended.blksize) : 0;
        MessageChecking += extended.timeout_flag ? FULLSTRLEN("timeout") + FULLNUMLEN(extended.timeout) : 0;
    }
    // 判断是否超过最大长度
    if (MessageChecking > REQ_SIZE)
    {
        emit showMessage(ERROR_TOO_LONG_REQUEST, QString("Warning: Too long request : <%1>.").arg(MessageChecking + OP_SIZE));
        return ERROR_TOO_LONG_REQUEST;
    }

    // 检查是否符合两种模式
    if (mode != MODE_NETASCII && mode != MODE_OCTET)
    {
        emit showMessage(ERROR_UNDEFINED_MODE, QString("Warning: Undefined mode : <%1>.").arg(mode));
        return ERROR_UNDEFINED_MODE;
    }
    emit showMessage(TFTP_NO_ERROR, "Warning : Valid.");
    return TFTP_NO_ERROR;
}

// 使用不同方式打开客户端文件
int mythread::OpenClientFile() {
    if (fopen_s(&fp, clientFile, operation == OP_READ_REQ ? "wb" : "rb") == 0)
    {
        emit showMessage(TFTP_NO_ERROR, QString("Open file : Success : <%1>.").arg(QString::fromLocal8Bit(clientFile)));
        return TFTP_NO_ERROR;
    }
    else
    {
        emit showMessage(ERROR_OPENFILE_FAIL, QString("Open file : Fail : <%1>.").arg(QString::fromLocal8Bit(clientFile)));
        return ERROR_OPENFILE_FAIL;
    }
}

// 组装发送请求报文
int mythread::sendReqest() {
    // 组装请求报文
    int req_Len = 0;
    reqPkt.operation = htons(operation);
    append_Message(reqPkt.request_message, REQ_SIZE, &req_Len, "%s", serverFile);
    append_Message(reqPkt.request_message, REQ_SIZE, &req_Len, mode == MODE_NETASCII ? "netascii" : "octet");
    if (extended.Extended_flag)
    {
        if (extended.tsize_flag)
        {
            append_Message(reqPkt.request_message, REQ_SIZE, &req_Len, "%s%c%d", "tsize", 0, tSize <= 0 ? 0 : tSize);
        }
        if (extended.blksize_flag)
        {
            append_Message(reqPkt.request_message, REQ_SIZE, &req_Len, "%s%c%d", "blksize", 0, extended.blksize);
        }
        if (extended.timeout_flag)
        {
            append_Message(reqPkt.request_message, REQ_SIZE, &req_Len, "%s%c%d", "timeout", 0, extended.timeout);
        }
    }

    // 发送报文
    int res = sendPkt((char*)&reqPkt, OP_SIZE + req_Len);
    if (res < 0)
    {
        emit showMessage(ERROR_SEND_REQ_FAIL, QString("REQUEST : Send request packet fail with code : <%1>.").arg((WSAGetLastError())));
        return ERROR_SEND_REQ_FAIL;
    }
    else if (res < OP_SIZE + req_Len)
    {
        emit showMessage(TFTP_NO_ERROR, QString("REQUEST : Send request packet partically with length : <%1>.").arg(res));
    }
    else
    {
        emit showMessage(TFTP_NO_ERROR, QString("REQUEST : Send request packet success with length : <%1>.").arg(res));
        if (extended.Extended_flag)
        {
            if (extended.tsize_flag)
            {
                emit showMessage(TFTP_NO_ERROR, QString("Option tsize : <%1>.").arg(tSize <= 0 ? 0 : tSize));
            }
            if (extended.blksize_flag)
            {
                emit showMessage(TFTP_NO_ERROR, QString("Option blksize : <%1>.").arg(extended.blksize));
            }
            if (extended.timeout_flag)
            {
                emit showMessage(TFTP_NO_ERROR, QString("Option timeout : <%1>.").arg(extended.timeout));
            }
        }
    }
    return req_Len;
}

// 下载文件
int mythread::DownloadFile() {
    bool no_extend = false;
    int rcvdSize;
    int retransmitCount = 0;
    int ret;
    int waitForDataRet = 0;
    curBlock = extended.Extended_flag ? 0 : 1;

    // 接收OACK报文
    if (extended.Extended_flag)
    {
        while (retransmitCount <= TFTP_MAX_RETRANSMIT)
        {
            int waitForOACKRet = waitForOACK(timeOut * 1000, rcvdSize);

            if (waitForOACKRet == TFTP_OACK_PKT)
            { // OACK
                emit showMessage(TFTP_NO_ERROR, "OACK : Support extended options.");
                getOptionFromOAckPkt(rcvdSize);
                if (sendAck(curBlock++) < 0)
                {
                    processStop(false);
                    return ERROR_SEND_ACK_FAIL;
                }
                break;
            }
            else if (waitForOACKRet == TFTP_DATA_PKT)
            { // 不支持扩展选项
                emit showMessage(TFTP_NOT_SUPPORT_EXT, QString("DATA <1> : Not support extended."));
                curBlock = 1;
                no_extend = true;
                break;
            }
            else if (waitForOACKRet == TFTP_ERROR_PKT)
            { // ERROR报文
                if (rcvdSize < OP_SIZE + ERRCODE_SIZE)
                {
                    emit showMessage(ERROR_WRONG_PKT, "ERRO : Wrong error packet.");
                } else
                {
                    emit showMessage(ntohs(rcvdPkt.error_code), QString(rcvdPkt.error_message));
                }
                processStop(false);
                return ntohs(rcvdPkt.error_code);
            }
            else
            { // 其他报文
                retransmitCount++;
                totalRetransmitCount++;
                emit showMessage(ERROR_RETRANSMIT_EQU, "Retransmit request packet.");
                if (sendPkt((char*)&reqPkt, OP_SIZE + request_len) < 0)
                {
                    emit showMessage(ERROR_SEND_REQ_FAIL, "REQUEST : Send request packet fail.");
                    processStop(false);
                    return ERROR_SEND_REQ_FAIL;
                }
            }
        }

        if (retransmitCount > TFTP_MAX_RETRANSMIT)
        {
            emit showMessage(ERROR_RETRANSMIT_TOO_MUCH, "Retransmission times too much.");
            processStop(false);
            return ERROR_RETRANSMIT_TOO_MUCH;
        }
    }

    // 写入本地文件
    retransmitCount = 0;
    while (retransmitCount <= TFTP_MAX_RETRANSMIT)
    {
        if (no_extend && curBlock == 1)
        {
            waitForDataRet = TFTP_DATA_PKT;
        }
        else
        {
            waitForDataRet = waitForSpecificPkt(curBlock, timeOut * 1000, rcvdSize, OP_DATA);
        }

        if (waitForDataRet == TFTP_DATA_PKT)
        {  // DATA
            ret = (int)fwrite(rcvdPkt.data, 1, rcvdSize - OP_SIZE - BLOCK_SIZE, fp);
            if (ret < (size_t)rcvdSize - OP_SIZE - BLOCK_SIZE)
            {
                emit showMessage(ERROR_FWRITE_FAIL, "FWRITE : Fail.");
                processStop(false);
                return ERROR_FWRITE_FAIL;
            }
            // 发送ACK
            if (sendAck(curBlock) < 0)
            {
                processStop(false);
                return ERROR_SEND_ACK_FAIL;
            }
            // 更新统计数据
            curBlock++;
            retransmitCount = 0;
            fileSize += rcvdSize - OP_SIZE - BLOCK_SIZE;
            // 检测是否结束
            if (rcvdSize == blkSize + OP_SIZE + BLOCK_SIZE)
            {
                if (extended.tsize_flag && (double)fileSize / tSize > percent)
                {
                    percent = (double)fileSize / tSize + 0.01;
                    emit sendProcessBarValue(fileSize);
                }
                continue;
            }
            else
            {
                processStop(true);
                break;
            }
        }
        else if (waitForDataRet == TFTP_ERROR_PKT)
        {  // ERROR报文
            if (rcvdSize < OP_SIZE + ERRCODE_SIZE)
            {
                emit showMessage(ERROR_WRONG_PKT, "ERRO : Wrong error packet.");
            }
            else
            {
                emit showMessage(ntohs(rcvdPkt.error_code), QString(rcvdPkt.error_message));
            }
            processStop(false);
            return ERROR_WRONG_PKT;
        }
        else
        {  // 其他报文
            retransmitCount++;
            totalRetransmitCount++;
            if (!extended.Extended_flag && curBlock == 1)
            {
                emit showMessage(ERROR_RETRANSMIT_EQU, "Retransmit request packet.");
                if (sendPkt((char*)&reqPkt, OP_SIZE + request_len) < 0)
                {
                    emit showMessage(ERROR_SEND_REQ_FAIL, "REQUEST : Send request packet fail.");
                    processStop(false);
                    return ERROR_SEND_REQ_FAIL;
                }
            }
            else
            {
                emit showMessage(ERROR_RETRANSMIT_ACK, QString("Retransmit ACK packet <%1>.").arg(curBlock - 1));
                if (sendAck(curBlock - 1) < 0)
                {
                    processStop(false);
                    return ERROR_SEND_ACK_FAIL;
                }
            }
        }
    }

    if (retransmitCount > TFTP_MAX_RETRANSMIT)
    {
        emit showMessage(ERROR_RETRANSMIT_TOO_MUCH, "Retransmission times too much.");
        processStop(false);
        return ERROR_RETRANSMIT_TOO_MUCH;
    }

    if (mode == MODE_NETASCII)
    {
        decodeNetascii(clientFile, PLATFORM_WINDOWS);
    }

    return TFTP_NO_ERROR;
}

// 上传文件
int mythread::UploadFile() {
    bool haveGetFirstAck = false;
    bool isFinished = false;
    int rcvdSize, dataSize = 0, retransmitCount = 0, waitForDataRet = 0;
    curBlock = 0;

    // OACK报文
    if (extended.Extended_flag)
    {
        while (retransmitCount <= TFTP_MAX_RETRANSMIT)
        {
            int waitForOACKRet = waitForOACK(timeOut * 1000, rcvdSize);
            if (waitForOACKRet == TFTP_OACK_PKT)
            {
                emit showMessage(TFTP_NO_ERROR, "OACK : Support extended options.");
                getOptionFromOAckPkt(rcvdSize);
                haveGetFirstAck = true;
                break;
            }
            else if (waitForOACKRet == TFTP_ACK_PKT)
            {
                emit showMessage(TFTP_NOT_SUPPORT_EXT, QString("ACK  <0> : Not support extended options. Use default."));
                haveGetFirstAck = true;
                break;
            }
            else if (waitForOACKRet == TFTP_ERROR_PKT)
            {
                if (rcvdSize < OP_SIZE + ERRCODE_SIZE)
                {
                    emit showMessage(ERROR_WRONG_PKT, "ERRO : Wrong error packet.");
                }
                else
                {
                    emit showMessage(ntohs(rcvdPkt.error_code), QString(rcvdPkt.error_message));
                }
                processStop(false);
                return ntohs(rcvdPkt.error_code);
            }
            else {  // 其他报文
                retransmitCount++;
                totalRetransmitCount++;
                emit showMessage(ERROR_RETRANSMIT_EQU, "Retransmit request packet.");
                if (sendPkt((char*)&reqPkt, OP_SIZE + request_len) < 0)
                {
                    emit showMessage(ERROR_SEND_REQ_FAIL, "REQUEST : Send request packet fail.");
                    processStop(false);
                    return ERROR_SEND_REQ_FAIL;
                }
            }
        }

        if (retransmitCount > TFTP_MAX_RETRANSMIT)
        {
            emit showMessage(ERROR_RETRANSMIT_TOO_MUCH, "Retransmission times too much.");
            processStop(false);
            return ERROR_RETRANSMIT_TOO_MUCH;
        }
    }

    // 发送DATA报文
    retransmitCount = 0;
    while (retransmitCount <= TFTP_MAX_RETRANSMIT)
    {
        if (curBlock == 0 && haveGetFirstAck)
        {
            waitForDataRet = TFTP_ACK_PKT;
        }
        else
        {
            waitForDataRet = waitForSpecificPkt(curBlock, timeOut * 1000, rcvdSize, OP_ACK);
        }

        if (waitForDataRet == TFTP_ACK_PKT)
        {  // ACK报文
            if (!isFinished)
            {
                // 本地文件
                dataSize = (int)fread(dataPkt.data, 1, blkSize, fp);
                if (dataSize < blkSize) { isFinished = true; }
                // 发送DATA
                dataPkt.block = htons(++curBlock);
                if (sendPkt((char*)&dataPkt, dataSize + OP_SIZE + BLOCK_SIZE) < 0)
                {
                    emit showMessage(ERROR_SEND_DATA_FAIL, QString("DATA <%1> : Send DATA packet fail.").arg(curBlock - 1));
                    processStop(false);
                    return ERROR_SEND_DATA_FAIL;
                }
                // 更新统计数据和进度条
                retransmitCount = 0;
                fileSize += dataSize;
                if ((double)fileSize / tSize > percent)
                {
                    percent = (double)fileSize / tSize + 0.01;
                    emit sendProcessBarValue(fileSize);
                }
           }
           else
           {
               processStop(true);
               break;
           }
        }
        else if (waitForDataRet == TFTP_ERROR_PKT)
        {  //ERROR报文
            if (rcvdSize < OP_SIZE + ERRCODE_SIZE)
            {
                emit showMessage(ERROR_WRONG_PKT, "ERRO : Wrong error packet.");
            }
            else
            {
                emit showMessage(ntohs(rcvdPkt.error_code), QString(rcvdPkt.error_message));
            }
            processStop(false);
            return ntohs(rcvdPkt.error_code);
        }
        else
        {  // 其他报文
            retransmitCount++;
            totalRetransmitCount++;
            if (!extended.Extended_flag && curBlock == 0)
            {
                emit showMessage(ERROR_RETRANSMIT_EQU, "Retransmit request packet.");
                if (sendPkt((char*)&reqPkt, OP_SIZE + request_len) < 0)
                {
                    emit showMessage(ERROR_SEND_REQ_FAIL, "REQUEST : Send request packet fail.");
                    processStop(false);
                    return ERROR_SEND_REQ_FAIL;
                }
            }
            else
            {
                emit showMessage(ERROR_RETRANSMIT_ACK, QString("Retransmit DATA packet <%1>.").arg(curBlock));
                if (sendPkt((char*)&dataPkt, dataSize + OP_SIZE + BLOCK_SIZE) < 0)
                {
                    emit showMessage(ERROR_SEND_DATA_FAIL, QString("DATA <%1> : Send DATA packet fail.").arg(curBlock));
                    processStop(false);
                    return ERROR_SEND_DATA_FAIL;
                }
            }
        }
    }

    if (retransmitCount > TFTP_MAX_RETRANSMIT)
    {
        emit showMessage(ERROR_RETRANSMIT_TOO_MUCH, "Retransmission times too much.");
        processStop(false);
        return TFTP_MAX_RETRANSMIT;
    }

    return TFTP_NO_ERROR;
}

// 发送ACK报文
int mythread::sendAck(WORD block) {
    ackPkt.block = htons(block);
    int ret = sendPkt((char*)&ackPkt, sizeof(PKG_ACK));
    if (ret < 0) {
        emit showMessage(ERROR_SEND_ACK_FAIL, QString("ACK  <%1> : Send ACK packet fail.").arg(block));
    }
    return ret;
}

// 发送报文
int mythread::sendPkt(const char* buffer, int length) {
    int ret = sendto(socketFd, buffer, length, 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (ret > 0)
    {
        bytesSend += ret;
    }
    return ret;
}
// 接收报文
int mythread::recvPkt(char* buf, int len) {
    int ret = recvfrom(socketFd, buf, len, 0, (SOCKADDR*)&serverAddr, &serverAddr_len);
    if (ret > 0) { bytesRecv += ret; }
    return ret;
}

// 从OACK报文获得扩展
int mythread::getOptionFromOAckPkt(int rcvdSize) {
    int count = 0;
    char opt[ERROR_SIZE] = { 0 }, val[ERROR_SIZE] = { 0 };
    for (int offset = 0; offset < rcvdSize - 2;) {
        get_Message(opt, ERROR_SIZE, rcvdPkt.operation_message, &offset);
        get_Message(val, ERROR_SIZE, rcvdPkt.operation_message, &offset);
        if (extended.tsize_flag && strcmp(opt, "tsize") == 0)
        {
            emit showMessage(TFTP_NO_ERROR, QString("Option tsize : <%1>.").arg(val));
            emit sendTSize(QString::number(tSize = atoi(val)));
            count++;
        }
        if (extended.blksize_flag && strcmp(opt, "blksize") == 0)
        {
            emit showMessage(TFTP_NO_ERROR, QString("Option blksize : <%1>.").arg(val));
            blkSize = atoi(val);
            count++;
        }
        if (extended.timeout_flag && strcmp(opt, "timeout") == 0)
        {
            emit showMessage(TFTP_NO_ERROR, QString("Option timeout : <%1>.").arg(val));
            tvTimeOut.tv_sec = timeOut = atoi(val) + 1;
            count++;
        }
    }
    if (extended.tsize_flag) { emit sendProcessBarMaximum(tSize); }
    return count;
}

// 接收报文
int mythread:: waitForPkt(int timeOutMs, int &rcvdSize) {
    FD_SET readFds;
    TIMEVAL tv = {timeOutMs / 1000, timeOutMs % 1000 * 1000};
    FD_ZERO(&readFds);
    FD_SET(socketFd, &readFds);
    int selectRet = select(socketFd + 1, &readFds, NULL, NULL, &tv);

    if (selectRet == SOCKET_ERROR)
    {
        return ERROR_SOCKET_ERROR;
    }
    else if (selectRet == 0)
    {
        return TFTP_ERROR_TIMEOUT;
    }
    else
    {
        rcvdSize = recvPkt((char*)&rcvdPkt, sizeof(PKG_DATA_ERROR_OACK));
        if (rcvdSize == SOCKET_ERROR)
        {
            emit showMessage(ERROR_SOCKET_ERROR, "RECV : SOCKET_ERROR.");
            return ERROR_SOCKET_ERROR;
        }
        else if (rcvdSize == 0)
        {
            emit showMessage(ERROR_CONNECT_CLOSE, "RECV : The connection has been closed.");
            return ERROR_CONNECT_CLOSE;
        }
    }
    if (ntohs(rcvdPkt.operation) == OP_DATA || ntohs(rcvdPkt.operation) == OP_ACK || ntohs(rcvdPkt.operation) == OP_ERROR)
    {
        if (rcvdSize < OP_SIZE + MODE_SIZE)
        {
            return TFTP_ERROR_WRONG_PKT;
        }
    }
    else if (ntohs(rcvdPkt.operation) == OP_OACK)
    {
        if (rcvdSize < OP_SIZE) { return TFTP_ERROR_WRONG_PKT; }
    }
    return TFTP_NO_ERROR;
}

// 接收OACK
int mythread::waitForOACK(int timeOutMs, int &rcvdSize) {
    int restTime = timeOutMs, waitForPktRet, startTime;

    do {
        startTime = clock();
        waitForPktRet = waitForPkt(restTime, rcvdSize);
        restTime -= (clock() - startTime);
    } while (waitForPktRet == TFTP_NO_ERROR && (operation == OP_READ_REQ ? (ntohs(rcvdPkt.operation) == OP_DATA && ntohs(rcvdPkt.block) != 1) :
                                                (ntohs(rcvdPkt.operation) == OP_ACK && ntohs(rcvdPkt.block) != 0)));
    if (restTime <= 0)
    {
        emit showMessage(TFTP_ERROR_TIMEOUT, QString("OACK : Timeout."));
    }
    return waitForPktRet != TFTP_NO_ERROR ? waitForPktRet : ntohs(rcvdPkt.operation);
}

// 接收指定报文
int mythread::waitForSpecificPkt(WORD block, int timeOutMs, int &rcvdSize, int pktType) {
    int restTime = timeOutMs, waitForPktRet, startTime;

    do {
        startTime = clock();
        waitForPktRet = waitForPkt(restTime, rcvdSize);
        restTime -= (clock() - startTime);
    } while (waitForPktRet == TFTP_NO_ERROR && (ntohs(rcvdPkt.operation) != pktType || ntohs(rcvdPkt.block) != block));
    if (restTime <= 0)
    {
        emit showMessage(TFTP_ERROR_TIMEOUT, (pktType == TFTP_ACK_PKT ? "ACK " : "DATA") + QString(" <%1> : Timeout.").arg(block));
    }
    return waitForPktRet != TFTP_NO_ERROR ? waitForPktRet : ntohs(rcvdPkt.operation);
}

// 结束进程同时显示信息
void mythread::processStop(bool flag) {
    emit sendProcessBarMaximum(1);
    emit sendProcessBarValue(flag ? 1 : 0);
    if (flag)
    {
        endTime = clock();
        double timeSec = (double)(endTime - startTime) / CLOCKS_PER_SEC;
        if (operation == OP_READ_REQ)
        {
            emit showMessage(TFTP_NO_ERROR, QString("Read Success  : <%1> ==> <%2>").arg(QString::fromLocal8Bit(serverFile), QString::fromLocal8Bit(clientFile)));
        }
        else
        {
            emit showMessage(TFTP_NO_ERROR, QString("WRITE Success : <%1> ==> <%2>").arg(QString::fromLocal8Bit(clientFile), QString::fromLocal8Bit(serverFile)));
        }
        emit showMessage(TFTP_NO_ERROR, QString("Block      : <%1 blks | %2 B/blk> ").arg(curBlock - (operation == OP_READ_REQ ? 1 : 0)).arg(blkSize));
        emit showMessage(TFTP_NO_ERROR, QString("Time       : <%1 s> ").arg(timeSec));
        emit showMessage(TFTP_NO_ERROR, QString("File Size  : <%1 kB> ").arg((double)fileSize / 1024));
        emit showMessage(TFTP_NO_ERROR, QString("Down       : <%1 kB> ").arg((double)bytesRecv / 1024));
        emit showMessage(TFTP_NO_ERROR, QString("Down Speed : <%1 kB/s> ").arg(((double)bytesRecv / 1024 / timeSec)));
        emit showMessage(TFTP_NO_ERROR, QString("Up         : <%1 kB> ").arg((double)bytesSend / 1024));
        emit showMessage(TFTP_NO_ERROR, QString("Up Speed   : <%1 kB/s> ").arg(((double)bytesSend / 1024 / timeSec)));
        emit showMessage(TFTP_NO_ERROR, QString("Retransmit : <%1 times> ").arg(totalRetransmitCount));
        emit showMessage(TFTP_NO_ERROR, QString("---------------------- END SUCC ----------------------"));
    }
    else
    {
        emit showMessage(TFTP_NO_ERROR, QString("---------------------- END FAIL ----------------------"));
    }
    if (fp) { fclose(fp); }
}

// 更新实时吞吐量
void mythread::recvTimeOutSignal()
{
    emit sendUploadSpeed(QString::number(((double)bytesSend - lastBytesSend) / TFTP_REFRESH_INTERVAL * 1000 / 1024));
    emit sendDownloadSpeed(QString::number(((double)bytesRecv - lastBytesRecv) / TFTP_REFRESH_INTERVAL * 1000 / 1024));
    lastBytesRecv = bytesRecv;
    lastBytesSend = bytesSend;
}


















