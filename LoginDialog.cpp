#include "LoginDialog.h"
#include "ui_LoginDialog.h"
#include "SubjectInfo.h"
#include "MainWindow.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    
    connect(ui->pushButton_go, &QPushButton::clicked, [=](){
        //保存全局数据
        SubjectInfo::getInstance()->setSubject(ui->lineEdit_sub->text());
        SubjectInfo::getInstance()->setId(ui->lineEdit_id->text());
        SubjectInfo::getInstance()->setNum(ui->lineEdit_num->text().toUInt());
        //切换界面
        MainWindow* w = new MainWindow;
        w->show();
        close();
    });
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
