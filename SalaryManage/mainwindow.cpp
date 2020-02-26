#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdafx.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("登录");
}

MainWindow::~MainWindow()
{
    delete ui;
}

//点击登录按钮
void MainWindow::on_pushButton_Login_clicked()
{
    if(ui->LineEdit_accout->text().trimmed().isEmpty())
    {
        QMessageBox::critical(this,
                              "错误","请输入账户",
                              QMessageBox::Ok,0,0);
        return;
    }
    else if (ui->LineEdit_password->text().trimmed().isEmpty()) {
        QMessageBox::critical(this,
                              "错误" , "请输入密码",
                              QMessageBox::Ok, 0, 0);
        return;
    }

    QString account = ui->LineEdit_accout->text();
    QString password = ui->LineEdit_password->text();

    QSqlQuery query;
    query.exec("select * from manager_table");
    QSqlRecord rec = query.record();

    int flag = 0;
    while(query.next())
    {
        rec = query.record();
        QString dbaccount = query.value("账号").toString();
        QString dbpassword = query.value("密码").toString();

        if(dbaccount == account && dbpassword == password)
        {
            flag = 1;
        }
    }
    if(flag)
    {
        //QMessageBox::information(this,"正确","恭喜成功登录",QMessageBox::Ok,0,0);
        this->hide();
        employ->show();
    }
    else
    {
        QMessageBox::critical(this,"错误","账户和密码错误！",QMessageBox::Ok,0,0);
    }
}

//点击注册按钮，跳转注册界面
void MainWindow::on_pushButton_Register_clicked()
{
    // this->hide();
    register11->show();
}
