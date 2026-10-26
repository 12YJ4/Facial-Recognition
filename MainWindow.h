#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv.hpp>
#include <QTimer>
#include "MyLabel.h"
#include <vector>
#include <face.hpp>

using namespace cv;
using namespace std;
using namespace cv::face;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    void showFrame();//显示摄像头图像
    void initLabel();//初始化头像
    void findFace();//查找人脸
    void initRecognizer();//初始化人脸识别器
    void trainFace(int id);//训练人脸
    void randSeet(int label);//随机一个座位 参数人脸的label
    int insertStudent();//将学生信息插入数据库，返回学生的id，用来机器学习
    int checkFace();//识别人脸，返回人脸标签
    void leaveSeet(int label);//离开座位
private slots:
    void onTimerTimeout();//定时器函数
    void on_pushButton_enter_clicked();//进入考场按钮
    void on_pushButton_exit_clicked();//离开考场
    void on_pushButton_watch_clicked();//查看考试信息
    void on_pushButton_export_clicked();//导出考试信息
private:
    Ui::MainWindow *ui;
    VideoCapture m_vc;
    Mat m_frame;//存放一帧图像
    QTimer m_timer;
    CascadeClassifier m_cc;//级联分类器
    vector<Rect> m_face;//找到的人脸
    vector<MyLabel*> m_labels;//保存MyLabel的指针
    Ptr<FaceRecognizer> m_recognizer;//人脸识别器
};
#endif // MAINWINDOW_H
