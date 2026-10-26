#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget* parent = nullptr);
    
    void setHave(bool h){m_have = h;}
    bool getHave()const{return m_have;}
    
    void setLabel(int label){m_label = label;}
    int getLabel()const{return m_label;}
    
    void leave();
private:
    bool m_have = false;//是否有人
    int m_label = -1;//保存座位学生信息
};

#endif // MYLABEL_H
