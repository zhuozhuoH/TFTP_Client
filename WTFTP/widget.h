#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#if _MSC_VER >= 1600 // MSVC2015>1899, MSVC VER=l4 . 0
#pragma execution_character_set("utf-8")
#endif

#include <QWidget>
#include <QPushButton>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QHostAddress>
#include <QProcessEnvironment>
//最小化相关
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>


#include "pre_defination.h"
#include "util.h"
#include "mythread.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();


private:
    Ui::Widget* ui;
    //自定义窗口通信参数
    int mbox_return;

    //messagebox* mbox;
    mythread thread;


    void start();

    EXTEND extended = { 0 };

    unsigned short blkSize = TFTP_DEFAULT_BLOCK_SIZE;

    int serverPort, mode = MODE_OCTET;
    SOCKET socketFd = INVALID_SOCKET;
    SOCKADDR_IN serverAddr = { 0 };
    int serverAddr_len = sizeof(serverAddr);

    const char *serverFile, *clientFile, *serverIP;
    std::string remoteFileStd, localFileStd, serverIPStd, remoteFileGB, localFileGB;
    QByteArray remoteFileByteArray, localFileByteArray;

    mythread * myThread = new mythread();

    int checkArgs(int op);

    void setModuleEnabled(bool isEnabled);

    QTimer *timer = new QTimer();

signals:
    void btnchicked(int num);

    void sendReadFileArgs(SOCKET sock, SOCKADDR_IN servAddr, const char* reFile, const char* loFile, int mode, EXTEND ext, int oper);


private slots:

    void on_btnLocalfile_clicked();
    void socket_finish();
    void on_btnClear_clicked();
    void download_start();
    void upload_start();
    void log_save(int code,QString message);
    void on_btnSave_clicked();

    void BarMaxSetValue(int max);
    void BarValSetValue(int value);
    void TSizeSetValue(QString tsize);

    // 扩展选项是否开启
    void tsize_check(int state);
    void blksize_check(int state);
    void timeoutsize_check(int state);

    void blksSetValue(int value);
    void timeoutSetValue(int value);
    void modeSetValue(int index); 





protected:
    void closeEvent(QCloseEvent *event);

};
#endif // WIDGET_H
