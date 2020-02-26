#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"register1.h"
#include <QMainWindow>
#include "stdafx.h"

namespace Ui {   //一个命名空间Ui，包含一个类MainWindow
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Register_clicked();

     void on_pushButton_Login_clicked();

private:
    Ui::MainWindow *ui;  //使用Ui::MainWindow定义的一个指针
    register1 *register11 = new register1();
    employee *employ = new employee();
    QSqlTableModel *model;
    QPushButton *pushButton_Login;
    QPushButton *pushButton_Register;

};

#endif // MAINWINDOW_H
