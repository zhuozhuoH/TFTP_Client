#pragma once
#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QWidget>

namespace Ui {
    class messagebox;
}

class messagebox : public QWidget
{
    Q_OBJECT

public:
    explicit messagebox(QWidget* parent = nullptr);
    ~messagebox();

private slots:
    void on_btn_1_clicked();

    void on_btn_2_clicked();

signals:
    void btnchicked(bool flag);

private:
    Ui::messagebox* ui;
    void initUi();
};

#endif // MESSAGEBOX_H
