#include "LoginDialog.h"
#include "DataBase.h"
#include "SubjectInfo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    LoginDialog ld;
    ld.show();
    
    int r = a.exec();
    //释放一些资源
    DataBase::getInstance()->close();
    DataBase::releaseInstance();
    SubjectInfo::releaseInstance();
    return r;
}
