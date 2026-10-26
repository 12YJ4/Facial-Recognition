#include "DataBase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDateTime>
#include <QVariant>
#include <QDebug>

#define TABLE_STUDENT "student"//表名

DataBase* DataBase::instance = nullptr;

DataBase* DataBase::getInstance()
{
    if(instance == nullptr)
    {
        instance = new DataBase;
    }
    return instance;
}

void DataBase::releaseInstance()
{
    if(instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

DataBase::DataBase()
{
}

//打开数据库，成功打开后自动建表
bool DataBase::open(const QString &dbName)
{
    if(isOpen())//已经打开，直接返回
        return true;

    //使用默认连接，避免重复添加连接
    if(QSqlDatabase::contains())
    {
        m_db = QSqlDatabase::database();
    }
    else
    {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
    }

    m_db.setDatabaseName(dbName);
    if(!m_db.open())
    {
        qDebug() << "打开数据库失败:" << m_db.lastError().text();
        return false;
    }

    return createTable();
}

void DataBase::close()
{
    if(m_db.isValid() && m_db.isOpen())
    {
        m_db.close();
    }
}

bool DataBase::isOpen() const
{
    return m_db.isValid() && m_db.isOpen();
}

//创建student表：id(整数，主键，自增长)、考号、入场时间、离场时间
bool DataBase::createTable()
{
    QSqlQuery query(m_db);
    QString sql = QString("CREATE TABLE IF NOT EXISTS %1 ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                          "examNumber TEXT NOT NULL, "
                          "enterTime TEXT, "
                          "leaveTime TEXT)").arg(TABLE_STUDENT);

    if(!query.exec(sql))
    {
        qDebug() << "创建表失败:" << query.lastError().text();
        return false;
    }
    return true;
}

//当前系统时间，格式：yyyy-MM-dd HH:mm:ss
QString DataBase::currentTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
}

//插入一条信息，只传考号，入场时间自动使用当前系统时间
int DataBase::insertStudent(const QString &examNumber)
{
    if(!isOpen())
    {
        qDebug() << "数据库未打开，无法插入数据";
        return -1;
    }

    QSqlQuery query(m_db);
    query.prepare(QString("INSERT INTO %1 (examNumber, enterTime, leaveTime) "
                          "VALUES (:examNumber, :enterTime, :leaveTime)").arg(TABLE_STUDENT));
    query.bindValue(":examNumber", examNumber);
    query.bindValue(":enterTime", currentTime());//入场时间取当前系统时间
    query.bindValue(":leaveTime", QString());//离场时间暂时为空

    if(!query.exec())
    {
        qDebug() << "插入数据失败:" << query.lastError().text();
        return -1;
    }

    return query.lastInsertId().toInt();//返回新插入记录的id
}

//更新一条数据，传id更新离场时间，不传时间则使用当前系统时间
bool DataBase::updateLeaveTime(int id, const QString &leaveTime)
{
    if(!isOpen())
    {
        qDebug() << "数据库未打开，无法更新数据";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(QString("UPDATE %1 SET leaveTime = :leaveTime WHERE id = :id").arg(TABLE_STUDENT));
    query.bindValue(":leaveTime", leaveTime.isEmpty() ? currentTime() : leaveTime);
    query.bindValue(":id", id);

    if(!query.exec())
    {
        qDebug() << "更新数据失败:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

//查询所有数据
QList<StudentInfo> DataBase::queryAll()
{
    QList<StudentInfo> list;
    if(!isOpen())
    {
        qDebug() << "数据库未打开，无法查询数据";
        return list;
    }

    QSqlQuery query(m_db);
    QString sql = QString("SELECT id, examNumber, enterTime, leaveTime FROM %1").arg(TABLE_STUDENT);
    if(!query.exec(sql))
    {
        qDebug() << "查询数据失败:" << query.lastError().text();
        return list;
    }

    while(query.next())
    {
        StudentInfo info;
        info.setId(query.value("id").toInt());
        info.setExamNumber(query.value("examNumber").toString());
        info.setEnterTime(query.value("enterTime").toString());
        info.setLeaveTime(query.value("leaveTime").toString());
        list.append(info);
    }
    return list;
}

//查询最后一次插入数据的id，没有数据返回-1
int DataBase::queryLastId()
{
    if(!isOpen())
    {
        qDebug() << "数据库未打开，无法查询数据";
        return -1;
    }

    QSqlQuery query(m_db);
    QString sql = QString("SELECT MAX(id) FROM %1").arg(TABLE_STUDENT);
    if(!query.exec(sql))
    {
        qDebug() << "查询最后id失败:" << query.lastError().text();
        return -1;
    }

    if(query.next())
    {
        QVariant v = query.value(0);
        if(v.isNull())//表中没有数据
            return -1;
        return v.toInt();
    }
    return -1;
}
