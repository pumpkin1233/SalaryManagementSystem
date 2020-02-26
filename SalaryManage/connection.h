#ifndef CONNECTION_H
#define CONNECTION_H

#include<QMessageBox>
#include<QSqlDatabase>
#include<QSqlQuery>
#include "stdafx.h"

//连接MySQL数据库
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");        //设置主机名
    db.setDatabaseName("salary");       //设置你的数据库名
    db.setUserName("root");             //设置账户
    db.setPassword("12345Snn");         //这里输入你的密码
    bool ok = db.open();                // 打开数据库
    if (!ok) {
        QMessageBox::critical(0, QObject::tr("无法打开数据库"),
    "无法创建数据库连接！ ", QMessageBox::Cancel);
        return false;
    }
    return true;
}

#endif // CONNECTION_H
