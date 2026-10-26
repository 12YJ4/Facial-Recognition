#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SubjectInfo.h"
#include <QFile>
#include <QMessageBox>
#include <QRandomGenerator>
#include "DataBase.h"
#include <QDebug>
#include "InfoDialog.h"

#define MODEL_PATH "face.xml"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    //定时器
    connect(&m_timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
    m_timer.start(50);
    
    //摄像头
    m_vc.open(0);
    
    initLabel();
    
    m_cc.load("haarcascade_frontalface_alt2.xml");//加载级联分类器模型
    initRecognizer();//初始化人脸识别器
    
    DataBase::getInstance()->open();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showFrame()
{
    Mat rgb;
    cvtColor(m_frame, rgb, COLOR_BGR2RGB);
    
    QImage img(rgb.data, rgb.cols, rgb.rows, rgb.cols*rgb.channels(), QImage::Format_RGB888);
    ui->label_frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::initLabel()
{
    int num = SubjectInfo::getInstance()->getNum();//获得考生人数
    const int lineNum = 8;
    //根据人数创建MyLabel对象，依次添加到网格布局中
    for(int i = 0;i < num;i++)
    {
        MyLabel* ml = new MyLabel(this);
        int y = i/lineNum;
        int x = i%lineNum;
        ui->gridLayout->addWidget(ml,y, x);
        m_labels.push_back(ml);//保存指针
    }
}

void MainWindow::findFace()
{
    Mat gray;
    cvtColor(m_frame, gray, COLOR_BGR2GRAY);
    
    m_cc.detectMultiScale(gray, m_face, 1.1, 3, 0, Size(150, 150), Size(200, 200));
    
    for(Rect r : m_face)
    {
        rectangle(m_frame, r, Scalar(0, 0, 255));
    }
}

void MainWindow::initRecognizer()
{
    QFile file(MODEL_PATH);
    if(file.exists())//模型存在就加载
    {
        m_recognizer = FaceRecognizer::load<LBPHFaceRecognizer>(MODEL_PATH);
    }
    else//模型不存在就创建空模型
    {
        m_recognizer = LBPHFaceRecognizer::create();
    }
    m_recognizer->setThreshold(100);//设置阈值
}

void MainWindow::trainFace(int id)
{
    if(m_face.size() != 1)
    {
        QMessageBox::warning(this, "警告", "要保证摄像头中只有一个人脸");
        return ;
    }
    Mat face = m_frame(m_face[0]);
    cvtColor(face, face, COLOR_BGR2GRAY);//转灰度图
    cv::resize(face, face, Size(80, 80));//统一尺寸
    vector<Mat> studyFace;
    vector<int> studyLabel;
    studyFace.push_back(face);
    studyLabel.push_back(id);
    m_recognizer->update(studyFace, studyLabel);//机器学习
    m_recognizer->save(MODEL_PATH);//保存模型
}

void MainWindow::randSeet(int label)
{
    int seet;
    do
    {
        //随机得到座位下标
        seet = QRandomGenerator::global()->bounded(0, m_labels.size());
    }while(m_labels[seet]->getHave());//如果位置有人，重新随机
    
    m_labels[seet]->setHave(true);//将作为设置为有人
    //在作为显示人脸
    if(m_face.size() != 1)
        return;
    Mat face = m_frame(m_face[0]);
    Mat rgb;
    cvtColor(face, rgb, COLOR_BGR2RGB);
    QImage img(rgb.data, rgb.cols, rgb.rows, rgb.cols*rgb.channels(), 
               QImage::Format_RGB888);
    m_labels[seet]->setPixmap(QPixmap::fromImage(img));
    m_labels[seet]->setLabel(label);//保存学生人脸的label
}

int MainWindow::insertStudent()
{
    QString examNum = ui->lineEdit_id->text();//获得学生考号
    int id = DataBase::getInstance()->insertStudent(examNum);//学生信息插入数据库，返回id
    return id;
}

int MainWindow::checkFace()
{
    if(m_face.size() != 1 || m_recognizer.empty())
    {
        QMessageBox::warning(this, "警告", "要保证摄像头中只有一个人脸");
        return -1;
    }
    Mat face = m_frame(m_face[0]);//截取人脸
    cvtColor(face, face, COLOR_BGR2GRAY);//转灰度图
    cv::resize(face, face, Size(80, 80));//调整图片大小
    
    int label;//识别结果
    double con;//识别准确度
    m_recognizer->predict(face, label, con);
    return label;
}

void MainWindow::leaveSeet(int label)
{
    
    for(MyLabel* ml : m_labels)
    {
        //查找学生所在的座位
        if(ml->getLabel() == label)
        {
            ml->leave();
            return;
        }
    }
}

void MainWindow::onTimerTimeout()
{
    m_vc>>m_frame;
    flip(m_frame, m_frame, 1);
    findFace();//找脸
    showFrame();//显示图像
}

void MainWindow::on_pushButton_enter_clicked()
{
    //1  保存数据库
    int id = insertStudent();
    //2  学习人脸
    trainFace(id);
    //3  随机座位(确认是否有人) 并
    randSeet(id);
}

void MainWindow::on_pushButton_exit_clicked()
{
    //1 识别人脸
    int label = checkFace();
    //2 更新数据库时间
    DataBase::getInstance()->updateLeaveTime(label);
    //3 去掉头像
    leaveSeet(label);
}

void MainWindow::on_pushButton_watch_clicked()
{
    InfoDialog* id = new InfoDialog(this);
    id->exec();//模态显示界面
}

void MainWindow::on_pushButton_export_clicked()
{
    //读取全部数据，写入文件
    QList<StudentInfo> datas = DataBase::getInstance()->queryAll();
    //创建文件对象并打开文件
    QFile file("export_data.txt");
    file.open(QFile::WriteOnly);//写权限打开文件
    for(StudentInfo si : datas)
    {
        QString saveData = QString("id:%1 考号:%2 进场时间:%3 出场时间:%4\n")
                .arg(si.id).arg(si.examNumber).arg(si.enterTime).arg(si.leaveTime);
        
        file.write(saveData.toUtf8());
    }
    file.close();
}

