#include "widget.h"

#include <QApplication>
#include <QFile>
#include <QFont>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Widget w;
    //加载样式表
    QFile file(":/qss/index.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        qApp->setStyleSheet(qss);
        file.close();
    }

    //全局字体
    w.show();
    return a.exec();
}
