#include "MyLabel.h"

MyLabel::MyLabel(QWidget* parent):
    QLabel(parent)//初始化父类
{
    //设置最大最小尺寸，不受布局影响
    setMaximumSize(QSize(50, 50));
    setMinimumSize(QSize(50, 50));
    setScaledContents(true);//缩放图片，保证全部显示
    QPixmap pixmap(":/avatar.png");
    setPixmap(pixmap);
}

void MyLabel::leave()
{
    m_have = false;
    m_label = -1;
    QPixmap pixmap(":/avatar.png");
    setPixmap(pixmap);
}
