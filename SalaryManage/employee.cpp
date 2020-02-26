#include "employee.h"
#include "ui_employee.h"
#include "stdafx.h"

employee::employee(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::employee)
{
    ui->setupUi(this);
    setWindowTitle("工资管理系统");
    model = new QSqlTableModel(ui->tableView);
    model->setTable("employee_table");

}

employee::~employee()
{
    delete ui;
}

//初始化
void employee::init()
{
    ui->add_id->clear();
    ui->add_sex->clear();
    ui->add_fine->clear();
    ui->add_name->clear();
    ui->add_reward->clear();
    ui->add_subsidy->clear();
    ui->add_fineMoney->clear();
    ui->add_baseSalary->clear();
    ui->add_department->clear();
    ui->add_subsidyMoney->clear();
    ui->delete_Id->clear();
    ui->search_name->clear();
    ui->search_salary1->clear();
    ui->search_salary2->clear();
    ui->statistic_salary1->clear();
    ui->statistic_salary2->clear();
    ui->statistic_department->clear();
}

//显示所有记录
void employee::on_pushButton_showRecords_clicked()
{
    model->setTable("employee_table");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    ui->tableView->setModel(model);
}

//添加记录
void employee::on_pushButton_add_clicked()
{
    people p;
    p.name = ui->add_name->text();
    p.sex = ui->add_sex->text();
    p.id = ui->add_id->text().toInt();
    p.department = ui->add_department->text();
    p.subsidy = ui->add_subsidy->text();
    p.subsidyMoney = ui->add_subsidyMoney->text().toFloat();
    p.fine = ui->add_fine->text();
    p.fineMoney = ui->add_fineMoney->text().toFloat();
    p.baseSalary = ui->add_baseSalary->text().toFloat();
    p.reward = ui->add_reward->text().toFloat();

    QSqlQuery query;
    query.prepare("insert into salary.employee_table (职工号,职工姓名,职工性别,部门名称,补贴项目,补贴金额,扣款项目,扣款金额,基本工资) values (:add_id,:add_name,:add_sex,:add_department,:add_subsidy,:add_subsidyMoney,:add_fine,:add_fineMoney,:add_baseSalary)");

    //判断输入姓名内容非空，且格式正确
    if(ui->add_name->text().trimmed().isEmpty())
    {
        QMessageBox::information(this,
                              "消息","请输入姓名",
                              QMessageBox::Ok,0,0);
        return;
    }
    else
    {
        QRegExp r;
        r.setPattern("^[A-Za-z ]+$");
        if(!r.exactMatch(ui->add_name->text()))
        {
            QMessageBox::critical(this,"错误","姓名格式有误，请重新输入！");
        }
        else
        {
                query.bindValue(":add_name",p.name);
        }
    }

    if(ui->add_id->text().trimmed().isEmpty())
    {
        QMessageBox::critical(this,
                              "消息","请输入职工号",
                              QMessageBox::Ok,0,0);
        return;
    }
    else
    {
        QRegExp r;
        r.setPattern("^[0-9 ]+$");
        if(!r.exactMatch(ui->add_id->text()))
        {
            QMessageBox::critical(this,"错误","职工号格式有误，请重新输入！");
        }
        else
        {
            query.bindValue(":add_id",p.id);
        }
    }

    if (ui->add_department->text().trimmed().isEmpty())
    {
        QMessageBox::information(this,
                              "消息","请输入所属部门",
                              QMessageBox::Ok,0,0);
        return;
    }
    else
    {
        query.bindValue(":add_department",p.department);
    }

    query.bindValue(":add_sex",p.sex);
    query.bindValue(":add_subsidy",p.subsidy);
    query.bindValue(":add_subsidyMoney",p.subsidyMoney);
    query.bindValue(":add_fine",p.fine);
    query.bindValue(":add_fineMoney",p.fineMoney);
    query.bindValue(":add_baseSalary",p.baseSalary);
    int flag = query.exec();

    if(flag)
    {
        //插入对应数据salary_table
        QSqlQuery query2;
        FLOAT Money = p.subsidyMoney-p.fineMoney;
        p.salaryAll = p.baseSalary+p.reward+Money;
        query2.prepare("insert into salary.salary_table values(:add_salary_id,:add_salary_reward,:add_salary_all)");
        query2.bindValue(":add_salary_id",p.id);
        query2.bindValue(":add_salary_reward",p.reward);
        query2.bindValue(":add_salary_all",p.salaryAll);
        query2.exec();
        QMessageBox::information(this,
                              "消息","成功插入一条记录!",
                              QMessageBox::Ok,0,0);
        init();
        model->setTable("employee_table");

    }
    else {
        QMessageBox::critical(this,
                              "错误","插入失败！",
                              QMessageBox::Ok,0,0);
    }
    model->setTable("employee_table");
    model->select();

}

//删除记录
void employee::on_pushButton_delete_clicked()
{
    int curRow = -1;
    curRow = ui->tableView->currentIndex().row();//获取选中的行
    qDebug()<<"curRow:"<<curRow<<endl;
    int delete_Id = ui->delete_Id->text().toInt();
    if(curRow != 0 && curRow != -1)
    {
        model->removeRow(curRow);//删除该行
        //model_merge->removeRow(curRow);
        int ok = QMessageBox::warning(this,tr("删除当前行！"),tr("你确定删除当前行吗？"),QMessageBox::Yes,QMessageBox::No);
        if(ok == QMessageBox::No)
        {
            model->revertAll();//如果不删除，则撤销
        }
        else
        {
            model->submitAll();//否则提交，在数据库中删除该行
        }
        model->setTable("employee_table");
        model->select();
    }
    else if(delete_Id != NULL)
    {
        QRegExp r;
        r.setPattern("^[0-9 ]+$");
        if(!r.exactMatch(ui->delete_Id->text()))
        {
            QMessageBox::critical(this,"错误","职工号格式有误，请重新输入！");
            init();
        }
        QSqlTableModel *model2 = new QSqlTableModel();
        model2->setTable("employee_table");
        QString s=QString("职工号='%1'").arg(delete_Id);
        model2->setFilter(s);
        model2->select();
        if(model2->rowCount() == 1)
        {
            //model2->removeRows(0,model2->rowCount());
            int ok = QMessageBox::warning(this,tr("删除当前行！"),tr("你确定删除当前行吗？"),QMessageBox::Yes,QMessageBox::No);
            if(ok == QMessageBox::No)
            {
                //model2->revertAll();//如果不删除，则撤销
                init();
                return;
            }
            else
            {
                model2->removeRows(0,model2->rowCount());
                model2->submitAll();//否则提交，在数据库中删除该行
                model->setTable("employee_table");
                model->select();
                ui->tableView->setModel(model);
                QMessageBox::information(this,
                                      "消息","删除成功!",
                                      QMessageBox::Ok,0,0);
            }

            init();
        }
        else
        {
            QMessageBox::critical(this,
                                  "错误","删除失败!",
                                  QMessageBox::Ok,0,0);
            init();
        }
    }
    else
    {
        QMessageBox::information(this,
                              "消息","请输入职工号或选取需要删除的记录!",
                              QMessageBox::Ok,0,0);
    }


}

//修改记录
void employee::on_pushButton_modify_clicked()
{
    model->database().transaction();

    if(model->submitAll())
    {
        model->database().commit();
        QMessageBox::information(this,"消息","修改成功!");
    }
    else
    {
        model->database().rollback();
        QMessageBox::warning(this,tr("tableModel"),tr("数据库错误: %1").arg(model->lastError().text()));

    }
}

//显示记录完整的职工信息
void employee::on_pushButton_showAll_clicked()
{
    //先清空显示
    //ui->tableView->clearSpans();
    model_merge = new QStandardItemModel(ui->tableView);

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(QString("select * from salary.employee_table"));
    QSqlQueryModel *model_2 = new QSqlQueryModel;
    model_2->setQuery(QString("select * from salary.salary_table"));

    /*将model中数据输入到model_merge中*/
    for(int i=0;i<model->rowCount();i++){
        for(int j=0;j<model->columnCount();j++){
            QModelIndex index = model->index(i,j);
            model_merge->setItem(i,j,new QStandardItem(model->data(index).toString()));
        }
    }

    /*将model_2中的数据与model_merge进行匹配*/
    QVector<int> nullRow_2; // 用于记录空值
    for(int i=0;i<model_merge->rowCount();i++)
    {
        QModelIndex index_m = model_merge->index(i,0);
        for(int j=0;j<model_2->rowCount();j++)
        {
            QModelIndex index_2 = model_2->index(j,0);

            //qDebug()<<"index_m"<<model_merge->data(index_m).toString()<<"index_2"<<model_2->data(index_2).toString();
            // 若两组数据键值相等，则将model_2数据扩展到model_merge已有列之后
            if(model_merge->data(index_m).toString()==model_2->data(index_2).toString())
            {
                for(int k=1;k<3;k++)
                {
                    QModelIndex index_m2 = model_2->index(j,k);

                    model_merge->setItem(i,k+8,new QStandardItem(model_2->data(index_m2).toString()));
                    //qDebug()<<model_merge->data(index_m2).toString();
                }
                break;
            }

            // 如果遍历完依然没有匹配上则存入nullRow_2
            if(j==(model_2->rowCount()-1))
            {
                nullRow_2.append(i);
            }
        }
    }

    // 删除nullRow_2中存放的无法匹配的数据
    for(int i=nullRow_2.size();i>0;i--){
        model_merge->removeRow(nullRow_2[i-1]);
    }

    //添加视图自定义标题
    model_merge->setHeaderData(0,Qt::Horizontal,tr("职工号"));
    model_merge->setHeaderData(1,Qt::Horizontal,tr("职工姓名"));
    model_merge->setHeaderData(2,Qt::Horizontal,tr("职工性别"));
    model_merge->setHeaderData(3,Qt::Horizontal,tr("部门名称"));
    model_merge->setHeaderData(4,Qt::Horizontal,tr("补贴项目"));
    model_merge->setHeaderData(5,Qt::Horizontal,tr("补贴金额"));
    model_merge->setHeaderData(6,Qt::Horizontal,tr("扣款项目"));
    model_merge->setHeaderData(7,Qt::Horizontal,tr("扣款金额"));
    model_merge->setHeaderData(8,Qt::Horizontal,tr("基本工资"));
    model_merge->setHeaderData(9,Qt::Horizontal,tr("奖励工资"));
    model_merge->setHeaderData(10,Qt::Horizontal,tr("总工资"));

    ui->tableView->setModel(model_merge);

}

//按职工号升序
void employee::on_sortByIdAscButton_clicked()
{
    model->setSort(0,Qt::AscendingOrder);
    model->select();
}

//按职工号降序
void employee::on_sortByIdDscButton_clicked()
{
    model->setSort(0,Qt::DescendingOrder);
    model->select();
}

//按姓名字母升序
void employee::on_sortByNameAscButton_clicked()
{
    model->setSort(1,Qt::AscendingOrder);
    model->select();
}

//按姓名字母降序
void employee::on_sortByNameDscButton_clicked()
{
    model->setSort(1,Qt::DescendingOrder);
    model->select();
}

//按人名查询
void employee::on_searchByNameButton_clicked()
{
    QSqlTableModel *model_s = new QSqlTableModel(ui->tableView);
    model_s->setTable("employee_table");
    QString search_name = ui->search_name->text();

    QRegExp r;
    r.setPattern("^[A-Za-z ]+$");
    if(search_name == "")
    {
        QMessageBox::information(this,"消息","姓名为空! 请输入姓名!");
    }
    else
    {
        if(!r.exactMatch(search_name))
        {
            QMessageBox::critical(this,"错误","姓名格式有误，请重新输入！");
        }
        else
        {
            QString s = QString("职工姓名 = '%1'").arg(search_name);
            model_s->setFilter(s);
            model_s->select();
            ui->tableView->setModel(model_s);
            init();
        }
        if(model_s->rowCount()==0)
        {
             QMessageBox::critical(this, "消息", "查无此人！");
        }
    }
}

//按工资范围查询
void employee::on_searchBySalaryButton_clicked()
{
    model->setTable("employee_table");
    model->select();
    QString salary1 = ui->search_salary1->text();
    QString salary2 = ui->search_salary2->text();

    if(salary1.trimmed().isEmpty())
    {
        QMessageBox::information(this,
                              "消息","请输入工资下限",
                              QMessageBox::Ok,0,0);
        return;
    }
    else if(salary2.trimmed().isEmpty())
    {
        QMessageBox::information(this,
                              "消息","请输入工资上限",
                              QMessageBox::Ok,0,0);
        return;
    }

    QSqlQuery query;
    QString sql = "select * from employee_table where 基本工资 between '"+salary1+"' and '"+salary2+"'";
    query.exec(sql);
    int count = query.size();
    if(count)
    {
        QSqlQueryModel *model_q = new QSqlQueryModel(ui->tableView);
        model_q->setQuery(sql);
        ui->tableView->setModel(model_q);
    }
    else
    {
        QMessageBox::information(this,
                              "消息","此工资范围无记录，请重新输入！",
                              QMessageBox::Ok,0,0);
        init();
        return;
    }

}

//按工资范围统计员工数量
void employee::on_statisticBySalaryButton_clicked()
{
    model->setTable("employee_table");
    QString salary1 = ui->statistic_salary1->text();
    QString salary2 = ui->statistic_salary2->text();

    if(salary1.trimmed().isEmpty())
    {
        QMessageBox::critical(this,
                              "错误","请输入工资下限",
                              QMessageBox::Ok,0,0);
        return;
    }
    else if(salary2.trimmed().isEmpty())
    {
        QMessageBox::critical(this,
                              "错误","请输入工资上限",
                              QMessageBox::Ok,0,0);
        return;
    }

    QSqlQuery query;
    QString sql = "select * from employee_table where 基本工资 between '"+salary1+"' and '"+salary2+"'";
    query.exec(sql);
    int count = query. size();
    qDebug() << "count:" <<count<<endl;
    if(count)
    {
        QSqlQueryModel *model_q = new QSqlQueryModel(ui->tableView);
        model_q->setQuery(sql);
        ui->tableView->setModel(model_q);
        QString s = QString("此区间工资人数有%1人。").arg(count);
        ui->label_statistic_salary->setText(s);
    }
    else
    {
        QMessageBox::information(this,
                              "消息","此工资范围无记录，请重新输入！",
                              QMessageBox::Ok,0,0);
        init();
        return;
    }
}

//按部门统计部门工资总数
void employee::on_statisticByDepartmentButton_clicked()
{
    model->setTable("employee_table");
    QString department = ui->statistic_department->text();
    if(department.trimmed().isEmpty())
    {
        QMessageBox::critical(this,
                              "错误","请输入部门名称！",
                              QMessageBox::Ok,0,0);
        return;
    }

    QSqlQuery query;
    QString sql = "select * from employee_table where 部门名称 = '"+department+"'";
    query.exec(sql);
    int count = query.size();
    QSqlQuery query1;
    query1 = query;
    int salary;
    int salaryAll = 0;
    while(query1.next())
    {
        salary = query1.value("基本工资").toInt();
        salaryAll = salaryAll + salary;
    }
    if(count)
    {
        QSqlQueryModel *model_s = new QSqlQueryModel(ui->tableView);
        model_s->setQuery(sql);
        ui->tableView->setModel(model_s);
        QString s = QString(" 此部门工资总和为%1元。").arg(salaryAll);
        ui->label_statistic_department->setText(s);
    }
    else
    {
        QMessageBox::information(this,
                              "消息","没有此部门，请重新输入！",
                              QMessageBox::Ok,0,0);
        init();
        return;
    }
}

//退出
void employee::on_actionquit_triggered()
{
    this->close();
}

//about
void employee::on_actionhelp_triggered()
{
    QMessageBox message(QMessageBox::NoIcon,"新年快乐!",nullptr,QMessageBox::Ok);
    message.setIconPixmap(QPixmap(":/new/resources/HappyNewYear.jpg"));
    message.exec();
}

//新建文件
void employee::on_actionnew_triggered()
{
    QMessageBox::information(this,
                          "消息","此功能正在添加中!",
                          QMessageBox::Ok,0,0);
    return;
}

//打开文件
void employee::on_actionopen_triggered()
{
    QMessageBox::information(this,
                          "消息","此功能正在添加中!",
                          QMessageBox::Ok,0,0);
    return;
}

//保存
void employee::on_actionsave_triggered()
{
    QMessageBox::information(this,
                          "消息","此功能正在添加中!",
                          QMessageBox::Ok,0,0);
    return;
}

//另存为
void employee::on_actionsaveAs_triggered()
{
    QMessageBox::information(this,
                          "消息","此功能正在添加中!",
                          QMessageBox::Ok,0,0);
    return;
}

//导入
void employee::on_actionimport_triggered()
{
    QMessageBox::information(this,
                          "消息","此功能正在添加中!",
                          QMessageBox::Ok,0,0);
    return;
}

//导出
void employee::on_actionexport_triggered()
{
    QMessageBox::information(this,
                          "消息","此功能正在添加中!",
                          QMessageBox::Ok,0,0);
    return;
}

//打印
void employee::on_actionprint_triggered()
{
    QMessageBox::information(this,
                          "消息","此功能正在添加中!",
                          QMessageBox::Ok,0,0);
    return;
}
