#ifndef STUDENTINFO_H
#define STUDENTINFO_H

#include <QString>

//student表中的一条记录（数据类型的结构体）
struct StudentInfo
{
    int     id;         //主键，自增长
    QString examNumber; //考号
    QString enterTime;  //入场时间
    QString leaveTime;  //离场时间（未离场时为空字符串）

    //只要在类的内部实现函数，就是内联函数，隐式内联。
    int getId() const { return id; }
    void setId(int id) { this->id = id; }

    QString getExamNumber() const { return examNumber; }
    void setExamNumber(const QString &examNumber) { this->examNumber = examNumber; }

    QString getEnterTime() const { return enterTime; }
    void setEnterTime(const QString &enterTime) { this->enterTime = enterTime; }

    QString getLeaveTime() const { return leaveTime; }
    void setLeaveTime(const QString &leaveTime) { this->leaveTime = leaveTime; }

    //id小于0表示这条数据无效（没有写入数据库）
    bool isValid() const { return id > 0; }

    //默认构造，id为-1表示无效数据
    StudentInfo()
        : id(-1)
    {
    }

    StudentInfo(int id, const QString &examNumber, const QString &enterTime, const QString &leaveTime)
        : id(id)
        , examNumber(examNumber)
        , enterTime(enterTime)
        , leaveTime(leaveTime)
    {
    }
};

#endif // STUDENTINFO_H
