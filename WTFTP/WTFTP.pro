QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets \
    network

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mythread.cpp \
    util.cpp \
    util.cpp \
    widget.cpp

HEADERS += \
    mythread.h \
    pre_defination.h \
    util.h \
    widget.h

FORMS += \
    widget.ui



# 添加库文件
LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64" \
        -lws2_32


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc \


RC_ICONS = logo_wz.ico
