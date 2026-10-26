#include "InfoDialog.h"
#include "ui_InfoDialog.h"
#include "DataBase.h"

InfoDialog::InfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoDialog)
{
    ui->setupUi(this);
    
    QList<StudentInfo> datas = DataBase::getInstance()->queryAll();
    
    for(StudentInfo si : datas)
    {
        QString showData = QString("id:%1 考号:%2 入场时间:%3 出场时间:%4").arg(si.id)
                .arg(si.examNumber).arg(si.enterTime).arg(si.leaveTime);
        ui->listWidget->addItem(showData);
    }
}

InfoDialog::~InfoDialog()
{
    delete ui;
}
