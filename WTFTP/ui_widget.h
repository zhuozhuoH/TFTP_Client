/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout;
    QWidget *w_bg;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QTreeWidget *treeWidget;
    QVBoxLayout *verticalLayout;
    QLabel *title_label;
    QHBoxLayout *horizontalLayout_18;
    QPushButton *btnServerIP;
    QTextEdit *textEditServerIP;
    QHBoxLayout *horizontalLayout_14;
    QPushButton *btnServerPort;
    QTextEdit *textEditServerPort;
    QHBoxLayout *horizontalLayout_17;
    QPushButton *btnLocalfile;
    QTextEdit *textLocalfile;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnRemotefile;
    QTextEdit *textRemotefile;
    QHBoxLayout *horizontalLayout_4;
    QLabel *upload;
    QLabel *upload_value;
    QLabel *upload_danwei;
    QLabel *download;
    QLabel *download_value;
    QLabel *download_danwei;
    QProgressBar *progressBar;
    QGridLayout *gridLayout_4;
    QCheckBox *tsizecheckBox;
    QLabel *tsizelabel;
    QCheckBox *blksizecheckBox;
    QSpinBox *timeoutspinBox;
    QSpinBox *blksizespinBox;
    QCheckBox *timeoutcheckBox;
    QLabel *modelabel;
    QComboBox *modecomboBox;
    QHBoxLayout *horizontalLayout_19;
    QToolButton *btnUpload;
    QToolButton *btnDownload;
    QToolButton *btnBraek;
    QToolButton *btnSave;
    QToolButton *btnClear;
    QSpacerItem *verticalSpacer_6;
    QHBoxLayout *horizontalLayout_15;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_6;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1041, 602);
        Widget->setMinimumSize(QSize(0, 0));
        Widget->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(Widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        w_bg = new QWidget(Widget);
        w_bg->setObjectName(QString::fromUtf8("w_bg"));
        horizontalLayoutWidget_2 = new QWidget(w_bg);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(0, 16, 1021, 551));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        treeWidget = new QTreeWidget(horizontalLayoutWidget_2);
        QIcon icon;
        icon.addFile(QString::fromUtf8("icons/name.png"), QSize(), QIcon::Normal, QIcon::Off);
        QFont font;
        font.setFamily(QString::fromUtf8("Perpetua"));
        font.setPointSize(13);
        font.setBold(true);
        font.setItalic(true);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setFont(0, font);
        __qtreewidgetitem->setIcon(0, icon);
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setStyleSheet(QString::fromUtf8("QTreeWidget{\n"
"border: 1px solid grey ;  \n"
"border-radius:20px;\n"
"padding: 1px 12px 1px 12px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        horizontalLayout_2->addWidget(treeWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        title_label = new QLabel(horizontalLayoutWidget_2);
        title_label->setObjectName(QString::fromUtf8("title_label"));
        title_label->setStyleSheet(QString::fromUtf8("font-family: \"Arial\";"));
        title_label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(title_label);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        btnServerIP = new QPushButton(horizontalLayoutWidget_2);
        btnServerIP->setObjectName(QString::fromUtf8("btnServerIP"));
        btnServerIP->setMaximumSize(QSize(16777215, 35));
        btnServerIP->setCursor(QCursor(Qt::PointingHandCursor));
        btnServerIP->setStyleSheet(QString::fromUtf8(""));
        btnServerIP->setIconSize(QSize(20, 20));
        btnServerIP->setAutoDefault(false);
        btnServerIP->setFlat(false);

        horizontalLayout_18->addWidget(btnServerIP);

        textEditServerIP = new QTextEdit(horizontalLayoutWidget_2);
        textEditServerIP->setObjectName(QString::fromUtf8("textEditServerIP"));
        textEditServerIP->setMaximumSize(QSize(16777215, 35));
        textEditServerIP->setStyleSheet(QString::fromUtf8("QTextEdit\n"
"{\n"
"border: 1px solid grey ;  \n"
"border-radius:5px;\n"
"padding: 1px 1px 1px 1px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        horizontalLayout_18->addWidget(textEditServerIP);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        btnServerPort = new QPushButton(horizontalLayoutWidget_2);
        btnServerPort->setObjectName(QString::fromUtf8("btnServerPort"));
        btnServerPort->setMaximumSize(QSize(16777215, 35));
        btnServerPort->setCursor(QCursor(Qt::PointingHandCursor));
        btnServerPort->setStyleSheet(QString::fromUtf8(""));
        btnServerPort->setIconSize(QSize(20, 20));
        btnServerPort->setAutoDefault(false);
        btnServerPort->setFlat(false);

        horizontalLayout_14->addWidget(btnServerPort);

        textEditServerPort = new QTextEdit(horizontalLayoutWidget_2);
        textEditServerPort->setObjectName(QString::fromUtf8("textEditServerPort"));
        textEditServerPort->setMaximumSize(QSize(16777215, 35));
        textEditServerPort->setStyleSheet(QString::fromUtf8("QTextEdit\n"
"{\n"
"border: 1px solid grey ;  \n"
"border-radius:5px;\n"
"padding: 1px 1px 1px 1px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        horizontalLayout_14->addWidget(textEditServerPort);


        horizontalLayout_18->addLayout(horizontalLayout_14);


        verticalLayout->addLayout(horizontalLayout_18);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        btnLocalfile = new QPushButton(horizontalLayoutWidget_2);
        btnLocalfile->setObjectName(QString::fromUtf8("btnLocalfile"));
        btnLocalfile->setMaximumSize(QSize(16777215, 35));
        btnLocalfile->setCursor(QCursor(Qt::PointingHandCursor));
        btnLocalfile->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnLocalfile->setIcon(icon1);
        btnLocalfile->setIconSize(QSize(20, 20));
        btnLocalfile->setAutoDefault(false);
        btnLocalfile->setFlat(false);

        horizontalLayout_17->addWidget(btnLocalfile);

        textLocalfile = new QTextEdit(horizontalLayoutWidget_2);
        textLocalfile->setObjectName(QString::fromUtf8("textLocalfile"));
        textLocalfile->setMaximumSize(QSize(16777215, 35));
        textLocalfile->setStyleSheet(QString::fromUtf8("QTextEdit\n"
"{\n"
"border: 1px solid grey ;  \n"
"border-radius:5px;\n"
"padding: 1px 1px 1px 1px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        horizontalLayout_17->addWidget(textLocalfile);


        verticalLayout->addLayout(horizontalLayout_17);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnRemotefile = new QPushButton(horizontalLayoutWidget_2);
        btnRemotefile->setObjectName(QString::fromUtf8("btnRemotefile"));
        btnRemotefile->setMaximumSize(QSize(16777215, 35));
        btnRemotefile->setCursor(QCursor(Qt::PointingHandCursor));
        btnRemotefile->setStyleSheet(QString::fromUtf8(""));
        btnRemotefile->setIcon(icon1);
        btnRemotefile->setIconSize(QSize(20, 20));
        btnRemotefile->setAutoDefault(false);
        btnRemotefile->setFlat(false);

        horizontalLayout->addWidget(btnRemotefile);

        textRemotefile = new QTextEdit(horizontalLayoutWidget_2);
        textRemotefile->setObjectName(QString::fromUtf8("textRemotefile"));
        textRemotefile->setMaximumSize(QSize(16777215, 35));
        textRemotefile->setStyleSheet(QString::fromUtf8("QTextEdit\n"
"{\n"
"border: 1px solid grey ;  \n"
"border-radius:5px;\n"
"padding: 1px 1px 1px 1px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        horizontalLayout->addWidget(textRemotefile);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        upload = new QLabel(horizontalLayoutWidget_2);
        upload->setObjectName(QString::fromUtf8("upload"));

        horizontalLayout_4->addWidget(upload);

        upload_value = new QLabel(horizontalLayoutWidget_2);
        upload_value->setObjectName(QString::fromUtf8("upload_value"));

        horizontalLayout_4->addWidget(upload_value);

        upload_danwei = new QLabel(horizontalLayoutWidget_2);
        upload_danwei->setObjectName(QString::fromUtf8("upload_danwei"));

        horizontalLayout_4->addWidget(upload_danwei);

        download = new QLabel(horizontalLayoutWidget_2);
        download->setObjectName(QString::fromUtf8("download"));

        horizontalLayout_4->addWidget(download);

        download_value = new QLabel(horizontalLayoutWidget_2);
        download_value->setObjectName(QString::fromUtf8("download_value"));

        horizontalLayout_4->addWidget(download_value);

        download_danwei = new QLabel(horizontalLayoutWidget_2);
        download_danwei->setObjectName(QString::fromUtf8("download_danwei"));

        horizontalLayout_4->addWidget(download_danwei);

        progressBar = new QProgressBar(horizontalLayoutWidget_2);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);

        horizontalLayout_4->addWidget(progressBar);


        verticalLayout->addLayout(horizontalLayout_4);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        tsizecheckBox = new QCheckBox(horizontalLayoutWidget_2);
        tsizecheckBox->setObjectName(QString::fromUtf8("tsizecheckBox"));

        gridLayout_4->addWidget(tsizecheckBox, 0, 0, 1, 1);

        tsizelabel = new QLabel(horizontalLayoutWidget_2);
        tsizelabel->setObjectName(QString::fromUtf8("tsizelabel"));

        gridLayout_4->addWidget(tsizelabel, 1, 0, 1, 1);

        blksizecheckBox = new QCheckBox(horizontalLayoutWidget_2);
        blksizecheckBox->setObjectName(QString::fromUtf8("blksizecheckBox"));

        gridLayout_4->addWidget(blksizecheckBox, 0, 1, 1, 1);

        timeoutspinBox = new QSpinBox(horizontalLayoutWidget_2);
        timeoutspinBox->setObjectName(QString::fromUtf8("timeoutspinBox"));
        timeoutspinBox->setValue(2);

        gridLayout_4->addWidget(timeoutspinBox, 1, 2, 1, 1);

        blksizespinBox = new QSpinBox(horizontalLayoutWidget_2);
        blksizespinBox->setObjectName(QString::fromUtf8("blksizespinBox"));
        blksizespinBox->setMinimum(0);
        blksizespinBox->setMaximum(16384);
        blksizespinBox->setValue(512);

        gridLayout_4->addWidget(blksizespinBox, 1, 1, 1, 1);

        timeoutcheckBox = new QCheckBox(horizontalLayoutWidget_2);
        timeoutcheckBox->setObjectName(QString::fromUtf8("timeoutcheckBox"));

        gridLayout_4->addWidget(timeoutcheckBox, 0, 2, 1, 1);

        modelabel = new QLabel(horizontalLayoutWidget_2);
        modelabel->setObjectName(QString::fromUtf8("modelabel"));

        gridLayout_4->addWidget(modelabel, 0, 3, 1, 1);

        modecomboBox = new QComboBox(horizontalLayoutWidget_2);
        modecomboBox->setObjectName(QString::fromUtf8("modecomboBox"));

        gridLayout_4->addWidget(modecomboBox, 1, 3, 1, 1);


        verticalLayout->addLayout(gridLayout_4);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        btnUpload = new QToolButton(horizontalLayoutWidget_2);
        btnUpload->setObjectName(QString::fromUtf8("btnUpload"));
        btnUpload->setMinimumSize(QSize(70, 70));
        btnUpload->setCursor(QCursor(Qt::PointingHandCursor));
        btnUpload->setStyleSheet(QString::fromUtf8(""));
        btnUpload->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_19->addWidget(btnUpload);

        btnDownload = new QToolButton(horizontalLayoutWidget_2);
        btnDownload->setObjectName(QString::fromUtf8("btnDownload"));
        btnDownload->setMinimumSize(QSize(70, 70));
        btnDownload->setCursor(QCursor(Qt::PointingHandCursor));
        btnDownload->setStyleSheet(QString::fromUtf8(""));
        btnDownload->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_19->addWidget(btnDownload);

        btnBraek = new QToolButton(horizontalLayoutWidget_2);
        btnBraek->setObjectName(QString::fromUtf8("btnBraek"));
        btnBraek->setMinimumSize(QSize(70, 70));
        btnBraek->setCursor(QCursor(Qt::PointingHandCursor));
        btnBraek->setStyleSheet(QString::fromUtf8(""));
        btnBraek->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_19->addWidget(btnBraek);

        btnSave = new QToolButton(horizontalLayoutWidget_2);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setMinimumSize(QSize(70, 70));
        btnSave->setCursor(QCursor(Qt::PointingHandCursor));
        btnSave->setStyleSheet(QString::fromUtf8(""));
        btnSave->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_19->addWidget(btnSave);

        btnClear = new QToolButton(horizontalLayoutWidget_2);
        btnClear->setObjectName(QString::fromUtf8("btnClear"));
        btnClear->setMinimumSize(QSize(70, 70));
        btnClear->setCursor(QCursor(Qt::PointingHandCursor));
        btnClear->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout_19->addWidget(btnClear);


        verticalLayout->addLayout(horizontalLayout_19);

        verticalSpacer_6 = new QSpacerItem(223, 44, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_6);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_5);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_15);

        verticalLayout->setStretch(6, 1);

        horizontalLayout_2->addLayout(verticalLayout);


        gridLayout->addWidget(w_bg, 0, 0, 1, 1);


        retranslateUi(Widget);

        btnServerIP->setDefault(false);
        btnServerPort->setDefault(false);
        btnLocalfile->setDefault(false);
        btnRemotefile->setDefault(false);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("Widget", "Information", nullptr));
        title_label->setText(QCoreApplication::translate("Widget", "TFTP", nullptr));
        btnServerIP->setText(QCoreApplication::translate("Widget", "Server IP", nullptr));
        btnServerPort->setText(QCoreApplication::translate("Widget", "Server Port", nullptr));
        btnLocalfile->setText(QCoreApplication::translate("Widget", "local file", nullptr));
        btnRemotefile->setText(QCoreApplication::translate("Widget", "remote file", nullptr));
        upload->setText(QCoreApplication::translate("Widget", "\342\206\221:", nullptr));
        upload_value->setText(QCoreApplication::translate("Widget", "0.00", nullptr));
        upload_danwei->setText(QCoreApplication::translate("Widget", "kb/s", nullptr));
        download->setText(QCoreApplication::translate("Widget", "\342\206\223:", nullptr));
        download_value->setText(QCoreApplication::translate("Widget", "0.00", nullptr));
        download_danwei->setText(QCoreApplication::translate("Widget", "kb/s", nullptr));
        tsizecheckBox->setText(QCoreApplication::translate("Widget", "tsize", nullptr));
        tsizelabel->setText(QCoreApplication::translate("Widget", "Unknown", nullptr));
        blksizecheckBox->setText(QCoreApplication::translate("Widget", "blksize", nullptr));
        timeoutcheckBox->setText(QCoreApplication::translate("Widget", "timeout", nullptr));
        modelabel->setText(QCoreApplication::translate("Widget", "Mode", nullptr));
        btnUpload->setText(QCoreApplication::translate("Widget", "Upload", nullptr));
        btnDownload->setText(QCoreApplication::translate("Widget", "Download", nullptr));
        btnBraek->setText(QCoreApplication::translate("Widget", "break", nullptr));
        btnSave->setText(QCoreApplication::translate("Widget", "Save", nullptr));
        btnClear->setText(QCoreApplication::translate("Widget", "Clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
