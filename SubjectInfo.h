#ifndef SUBJECTINFO_H
#define SUBJECTINFO_H

#include <QString>

class SubjectInfo
{
public:
    //只要在类的内部实现函数，就是内联函数，隐式内联。
    // 科目
    QString getSubject() const { return m_subject; }
    void setSubject(const QString &subject) { m_subject = subject; }
    
    // 考试编号
    QString getId() const { return m_id; }
    void setId(const QString &id) { m_id = id; }
    
    // 考试人数
    int getNum() const { return m_num; }
    void setNum(int num) { m_num = num; }
    
    static SubjectInfo* getInstance();//静态函数创建对象
    static void releaseInstance();//释放对象
private:
    SubjectInfo();
    SubjectInfo(const SubjectInfo& other){
        Q_UNUSED(other)
    }
    ~SubjectInfo(){}
    static SubjectInfo* instance;
    
private:
    QString m_subject;//科目
    QString m_id;//考试编号
    int m_num;//考试人数
};

#endif // SUBJECTINFO_H
