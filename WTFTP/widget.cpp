#include "widget.h"
#include "ui_widget.h"

#pragma execution_character_set("utf-8")


Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("TFTP");
    this->resize(QSize(1200, 735));
    // 初始化UI组件
    ui->progressBar->setRange(0,1);
    ui->progressBar->setValue(0);
    ui->modecomboBox->addItem("OCTET");
    ui->modecomboBox->addItem("NETASCII");


#ifdef WIN32
    // 初始化WinSock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) { QMessageBox::warning(this, "Init WinSock Fail.", "Init socket fail."); exit(-1); }
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) { WSACleanup(); exit(-1); }
#endif

    // 选择本地文件
    connect(ui->btnLocalfile,SIGNAL(clicked()), this, SLOT(on_btnLocalfile_clicked()));
    // 下载文件
    connect(ui->btnDownload, SIGNAL(clicked()), this, SLOT(download_start()));
    // 上传文件
    connect(ui->btnUpload, SIGNAL(clicked()), this, SLOT(upload_start()));
    // 记录log和保存log
    connect(myThread, SIGNAL(showMessage(int,QString)), this, SLOT(log_save(int,QString)));
    connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(on_btnSave_clicked()));
    // 清空按钮
    connect(ui->btnClear, SIGNAL(clicked()), this,SLOT(on_btnClear_clicked()));

    connect(this, &Widget::sendReadFileArgs, myThread, &mythread::getClientInfo);
    connect(myThread, SIGNAL(finished()), this, SLOT(socket_finish()));

    // 设置UI组件动作
    connect(myThread, SIGNAL(sendProcessBarMaximum(int)), this, SLOT(BarMaxSetValue(int)));
    connect(myThread, SIGNAL(sendProcessBarValue(int)), this, SLOT(BarValSetValue(int)));
    connect(myThread, SIGNAL(sendTSize(QString)), this, SLOT(TSizeSetValue(QString)));
    connect(ui->tsizecheckBox, SIGNAL(stateChanged(int)), this, SLOT(tsize_check(int)));
    connect(ui->blksizecheckBox, SIGNAL(stateChanged(int)), this, SLOT(blksize_check(int)));
    connect(ui->timeoutcheckBox, SIGNAL(stateChanged(int)), this, SLOT(timeoutsize_check(int)));
    connect(ui->blksizespinBox, SIGNAL(valueChanged(int)), this, SLOT(blksSetValue(int)));
    connect(ui->timeoutspinBox, SIGNAL(valueChanged(int)), this, SLOT(timeoutSetValue(int)));
    connect(ui->modecomboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modeSetValue(int)));

    ui->tsizecheckBox->setCheckState(Qt::Checked);
    ui->blksizecheckBox->setCheckState(Qt::Checked);
    extended.blksize = TFTP_DEFAULT_BLOCK_SIZE;
    extended.timeout = TFTP_DEFAULT_TIMEOUT_SEC;

    // 实时吞吐量
    connect(timer, &QTimer::timeout, myThread, &mythread::recvTimeOutSignal);
    connect(myThread, &mythread::sendUploadSpeed, this, [=](QString speed) { ui->upload_value->setText(speed); });
    connect(myThread, &mythread::sendDownloadSpeed, this, [=](QString speed) { ui->download_value->setText(speed); });
}


// 中止整个程序
void Widget::closeEvent(QCloseEvent *event)
{
    if (thread.isRunning())
    {
        thread.terminate();
        thread.wait();
    }
    event->accept();
}

Widget::~Widget() {
    if(socketFd != INVALID_SOCKET)
    {
        closesocket(socketFd);
    }
#ifdef WIN32
    WSACleanup();
#endif
    delete ui;
}

// 检查输入框中的参数是否正确
int Widget::checkArgs(int op) {
    if (ui->textEditServerIP->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "Server IP Error.", "Server IP missing. Please input the Server IP.");
        return ERROR_INVALID_ARG;
    }

    if (ui->textEditServerPort->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "Server Port Error.", "Server port missing. Please input the Server port.");
        return ERROR_INVALID_ARG;
    }

    if (ui->textLocalfile->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "Local File Error.", "Local file missing. Please input the local file.");
        return ERROR_INVALID_ARG;
    }

    if (ui->textRemotefile->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "Remote File Error.", "Remote file missing. Please input the Remote file.");
        return ERROR_INVALID_ARG;
    }

    QHostAddress *ip = new QHostAddress(ui->textEditServerIP->toPlainText());
    if (ip->isNull()) {
        QMessageBox::critical(this, "Server IP Error.", "Wrong Server IP. Please input the Server IP again.");
        return ERROR_INVALID_ARG;
    }
    serverIPStd =ui->textEditServerIP->toPlainText().toStdString();
    serverIP = serverIPStd.c_str();

    QTextCodec *code = QTextCodec::codecForName("GB2312");

    localFileStd = ui->textLocalfile->toPlainText().toStdString();
    clientFile = localFileStd.c_str();
    localFileByteArray = code->fromUnicode(clientFile);
    localFileGB = localFileByteArray.data();
    clientFile = localFileGB.c_str();
    if (op == OP_READ_REQ && _access(clientFile, 0) != -1) {
        QMessageBox::StandardButton res = QMessageBox::question(this, "Local File Warning.", "File exist. Do you want to overlay it?");
        if (res == QMessageBox::No) { return ERROR_INVALID_ARG; }
    }

    remoteFileStd = ui->textRemotefile->toPlainText().toStdString();
    serverFile = remoteFileStd.c_str();
    remoteFileByteArray = code->fromUnicode(serverFile);
    remoteFileGB = remoteFileByteArray.data();
    serverFile = remoteFileGB.c_str();

    extended.Extended_flag = extended.tsize_flag || extended.blksize_flag || extended.timeout_flag;
    return TFTP_NO_ERROR;
}


// 选择文件
void Widget::on_btnLocalfile_clicked(){
    QString Path = QProcessEnvironment::systemEnvironment().value("USERPROFILE") + "\\desktop";
    QString File_Path = QFileDialog::getSaveFileName(this, "请选择本地文件", Path);
    if(!File_Path.isEmpty())
    {
        ui->textLocalfile->setText(File_Path);
    }
};

// 关闭套接字
void Widget::socket_finish(){
    setModuleEnabled(true);
    timer->stop();
    if(socketFd != INVALID_SOCKET)
    {
        closesocket(socketFd);
    }
}
// 设置各种参数值
void Widget::BarMaxSetValue(int max)
{
    ui->progressBar->setMaximum(max);
};
void Widget::BarValSetValue(int value)
{
    ui->progressBar->setValue(value);
};
void Widget::TSizeSetValue(QString tsize){
    ui->tsizelabel->setText(tsize);
}

void Widget::tsize_check(int state){
    extended.tsize_flag = state == Qt::Checked ? true : false;
}
void Widget::blksize_check(int state){
    extended.blksize_flag = state == Qt::Checked ? true : false;
}
void Widget::timeoutsize_check(int state){
    extended.timeout_flag = state == Qt::Checked ? true : false;
}

void Widget::blksSetValue(int value){
    ui->blksizespinBox->setValue(value);
}

void Widget::timeoutSetValue(int value){
    ui->timeoutspinBox->setValue(value);
}

void Widget::modeSetValue(int index){
    mode = index == 0 ? MODE_OCTET : MODE_NETASCII;
}

// 设置图形化界面的组件初始值
void Widget::setModuleEnabled(bool flag) {
    ui->btnUpload->setEnabled(flag);
    ui->btnDownload->setEnabled(flag);
    ui->btnBraek->setEnabled(!flag);
    ui->tsizecheckBox->setEnabled(flag);
    ui->blksizecheckBox->setEnabled(flag);
    ui->timeoutcheckBox->setEnabled(flag);
    ui->blksizespinBox->setEnabled(flag);
    ui->timeoutspinBox->setEnabled(flag);
    // 设置上传、下载速度的初始值
    if (flag) {
        ui->upload_value->setText("0.00");
        ui->download_value->setText("0.00");
    }
    return;
}

// 启动下载
void Widget::download_start(){
    if (checkArgs(OP_READ_REQ) == TFTP_NO_ERROR)
    {
        timer->start(TFTP_REFRESH_INTERVAL);
        setModuleEnabled(false);
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(SERV_PORT);
        InetPtonA(AF_INET, serverIP, (void*)&serverAddr.sin_addr);
        socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (socketFd == INVALID_SOCKET) { QMessageBox::warning(this, "Socket Failed.", "socket failed."); exit(-1); }
        emit sendReadFileArgs(socketFd, serverAddr, serverFile, clientFile, mode, extended, OP_READ_REQ);
        myThread->start();
    }
}

// 启动上传
void Widget::upload_start(){
    if (checkArgs(OP_WRITE_REQ) == TFTP_NO_ERROR)
    {
        timer->start(TFTP_REFRESH_INTERVAL);
        setModuleEnabled(false);
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(SERV_PORT);
        InetPtonA(AF_INET, serverIP, (void*)&serverAddr.sin_addr);
        if (mode == MODE_NETASCII)
        {    // 编码成Netascii
            encodeNetascii(clientFile);
            localFileStd += ".netascii";
            clientFile = localFileStd.c_str();
            QTextCodec *code = QTextCodec::codecForName("GB2312");
            localFileByteArray = code->fromUnicode(clientFile);
            localFileGB = localFileByteArray.data();
            clientFile = localFileGB.c_str();
        }
        socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (socketFd == INVALID_SOCKET) { QMessageBox::warning(this, "Socket Fail.", "socket fail."); exit(-1); }
        emit sendReadFileArgs(socketFd, serverAddr, serverFile, clientFile, mode, extended, OP_WRITE_REQ);
        myThread->start();
    }

}

// 清空
void Widget::on_btnClear_clicked(){
    ui->treeWidget->clear();
}

// 显示信息并保存日志
void Widget::log_save(int code, QString message){
    QString fullMessage = QDateTime::currentDateTime().toString("[MM/dd hh:mm:ss.zzz]");
    fullMessage += (code == TFTP_NO_ERROR ? " OK " : " ER ") + message;
    QTreeWidgetItem* info = new QTreeWidgetItem();
    info->setData(0, 0, fullMessage);
    ui->treeWidget->addTopLevelItem(info);

    QString logFileName = "TFTP_log_" + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss") + ".txt";
    QFile file("./" + logFileName);
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream out(&file);
    out << fullMessage << '\n';
    file.close();
};

// 日志成功保存
void Widget::on_btnSave_clicked(){
    QMessageBox::information(this, "Save Log Success.", "Save log sucess in process directory.");
};



