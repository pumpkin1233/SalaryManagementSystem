#include "register1.h"
#include "ui_register1.h"
#include "stdafx.h"


register1::register1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::register1)
{
    ui->setupUi(this);
    setWindowTitle("注册");
}

register1::~register1()
{
    delete ui;
}

//注册
void register1::on_pushButton_register_clicked()
{
    QString account = ui->lineEdit_account->text();
    QString pwd = ui->lineEdit_pwd->text();
    QString confirm_pwd = ui->lineEdit_confirm_pwd->text();

    if(ui->lineEdit_account->text().trimmed().isEmpty())
    {
        QMessageBox::information(this,
                              "消息","请输入账户!",
                              QMessageBox::Ok,0,0);
        return;
    }
    else if (ui->lineEdit_pwd->text().trimmed().isEmpty()) {
        QMessageBox::information(this,
                              "消息" , "请输入密码!",
                              QMessageBox::Ok, 0, 0);
        return;
    }
    else if (ui->lineEdit_confirm_pwd->text().trimmed().isEmpty()) {
        QMessageBox::information(this,
                              "消息" , "请确认密码!",
                              QMessageBox::Ok, 0, 0);
        return;
    }

    if(pwd != confirm_pwd)
    {
        QMessageBox::critical(this,
                              "错误" , "两次密码输入不一致,请重新输入!",
                              QMessageBox::Ok, 0, 0);
    }
    else
    {
        qDebug() << account<<"  "<<pwd;

        QSqlQuery query;
        query.prepare("insert into salary.manager_table (账号,密码,姓名,性别) values(:account,:pwd,:name,:sex)");
        query.bindValue(":account",account);
        query.bindValue(":pwd",pwd);
        query.bindValue(":name","管理员");
        query.bindValue(":sex","男");
        int ok = query.exec();
        qDebug()<<ok;
        if(!ok)
        {
            QMessageBox::critical(this,
                                  "错误" , "注册账户已存在!",
                                  QMessageBox::Ok, 0, 0);
            ui->lineEdit_account->text().clear();
            ui->lineEdit_pwd->text().clear();
            ui->lineEdit_confirm_pwd->text();
            return;
        }
        else
        {
            QMessageBox::information(this,
                                  "消息" , "注册成功！",
                                  QMessageBox::Ok, 0, 0);
            ui->lineEdit_account->text().clear();
            ui->lineEdit_pwd->text().clear();
            ui->lineEdit_confirm_pwd->text();
            return;
        }

    }


}

void register1::on_pushButton_back_clicked()
{
    this->close();
}
