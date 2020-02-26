#ifndef REGISTER1_H
#define REGISTER1_H

#include <QMainWindow>
#include "stdafx.h"


namespace Ui {
class register1;
}

class register1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit register1(QWidget *parent = nullptr);
    ~register1();

private slots:
    void on_pushButton_register_clicked();

    void on_pushButton_back_clicked();
    //void on_pushButton_Login_clicked();


private:
    Ui::register1 *ui;
    QSqlTableModel *model;
    QPushButton *pushButton_register;
    QPushButton *pushButton_back;
};

#endif // REGISTER1_H
