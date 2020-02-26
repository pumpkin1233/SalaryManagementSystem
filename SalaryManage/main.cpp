#include "mainwindow.h"
#include <QApplication>
#include "stdafx.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  //定义并创建应用程序

    //创建数据库连接
    if (!createConnection())
    {
        cout<<"error!"<<endl;
    }
    else
    {
        cout<<"ok!"<<endl;
    }

    MainWindow w;    //定义并创建窗口
    w.show();        //显示窗口

    return a.exec(); //应用程序运行
}
