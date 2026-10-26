#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QList>
#include <QString>
#include "StudentInfo.h"

//sqlite数据库操作类（单例）
class DataBase
{
public:
    static DataBase* getInstance();//静态函数创建对象
    static void releaseInstance();//释放对象

    //打开数据库，dbName为数据库文件名，默认在当前程序运行目录下
    bool open(const QString &dbName = "student.db");
    //关闭数据库
    void close();
    //数据库是否已经打开
    bool isOpen() const;

    //插入一条信息：只传考号，自动以当前系统时间作为入场时间
    //返回新插入记录的id，失败返回-1
    int insertStudent(const QString &examNumber);

    //更新一条数据：传id，更新离场时间（不传时间则使用当前系统时间）
    bool updateLeaveTime(int id, const QString &leaveTime = QString());

    //查询所有数据
    QList<StudentInfo> queryAll();

    //查询最后一次插入数据的id，没有数据返回-1
    int queryLastId();

private:
    DataBase();
    DataBase(const DataBase& other){
        Q_UNUSED(other)
    }
    DataBase& operator=(const DataBase& other){
        Q_UNUSED(other)
        return *this;
    }
    ~DataBase(){}

    //创建student表
    bool createTable();
    //得到当前系统时间字符串，格式：yyyy-MM-dd HH:mm:ss
    static QString currentTime();

    static DataBase* instance;

private:
    QSqlDatabase m_db;//数据库连接
};

#endif // DATABASE_H
