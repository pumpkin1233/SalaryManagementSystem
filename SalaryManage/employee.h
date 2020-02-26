#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QMainWindow>
#include "stdafx.h"

namespace Ui {
class employee;
}

class QSqlTableModel;

class employee : public QMainWindow
{
    Q_OBJECT

public:
    explicit employee(QWidget *parent = nullptr);   //employee构造函数
    ~employee();                                    //employee构造函数析构函数

private slots:
    void on_pushButton_add_clicked();               //添加记录
    void on_pushButton_delete_clicked();            //删除记录
    void on_pushButton_modify_clicked();            //修改记录
    void on_pushButton_showRecords_clicked();       //显示所有记录
    void on_pushButton_showAll_clicked();
    void init();                                    //初始化

    void on_sortByIdAscButton_clicked();            //按职工号升序
    void on_sortByIdDscButton_clicked();            //按职工号降序
    void on_sortByNameAscButton_clicked();          //按姓名字母升序
    void on_sortByNameDscButton_clicked();          //按姓名字母降序

    void on_searchByNameButton_clicked();           //按人名查询
    void on_searchBySalaryButton_clicked();         //按工资范围查询

    void on_statisticBySalaryButton_clicked();      //按工资范围统计员工数量
    void on_statisticByDepartmentButton_clicked();  //按部门统计部门工资总数

    void on_actionquit_triggered();                 //退出
    void on_actionhelp_triggered();                 //帮助
    void on_actionnew_triggered();                  //新建文件
    void on_actionopen_triggered();                 //打开文件
    void on_actionsave_triggered();                 //保存文件
    void on_actionsaveAs_triggered();               //另存为
    void on_actionimport_triggered();               //导入
    void on_actionexport_triggered();               //导出
    void on_actionprint_triggered();                //打印

private:
    Ui::employee *ui;
    QPushButton *pushButton_add;                    //添加记录按钮，下同按钮控件
    QPushButton *pushButton_delete;
    QPushButton *pushButton_modify;
    QPushButton *pushButton_showAll;
    QPushButton *sortByIdAscButton;
    QPushButton *sortByIdDscButton;
    QPushButton *sortByNameAscButton;
    QPushButton *sortByNameDscButton;
    QPushButton *searchByNameButton;
    QPushButton *searchBySalaryButton;
    QPushButton *statisticBySalaryButton;
    QPushButton *statisticByDepartmentButton;

    QSqlTableModel *model;
    QStandardItemModel *model_merge;


};

//职工信息结构体
struct people{
    int id;                 //职工号
    QString name;           //姓名
    QString sex;            //性别
    QString department;     //所属部门
    QString subsidy;        //补贴项目
    FLOAT subsidyMoney;     //补贴金额
    QString fine;           //扣款项目
    FLOAT fineMoney;        //扣款金额
    FLOAT baseSalary;       //基本工资
    FLOAT reward;           //奖励工资
    FLOAT salaryAll;        //总工资
};
#endif  // EMPLOYEE_H
