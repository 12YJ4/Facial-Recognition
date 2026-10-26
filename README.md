\# 智能人脸识别考试签到系统 (Exam Face Recognition System)



基于 Qt 和 OpenCV 开发的桌面端智能考场管理系统，通过摄像头实时检测人脸，实现考生的自动签到、签退与座位分配，并将数据持久化保存。



\# 技术栈

开发语言：C++11

1.界面框架： Qt 5

2.视觉库： OpenCV 3.4.5 (含 opencv\_contrib 扩展模块)

3.数据库：SQLite 3 (通过 Qt SQL 模块连接)

4.版本控制： Git / GitHub



\# 核心功能

1.考场初始：录入考试科目、代号及人数，动态生成对应数量的座位。

2.人脸检测与录入：摄像头实时采集画面，使用 Haar 级联分类器定位人脸，并利用 LBPH 算法训练人脸模型。

3.自动排座：考生签到时，系统自动从空闲座位中随机分配，并显示人脸快照。

4.签退与记录：识别考生人脸后自动记录离场时间，清空座位。

5.数据管理与导出：支持查看当前考场所有签到记录，并一键导出为 txt 文件。



\# 运行说明

1.确保本地已安装 Qt5 及OpenCV 3.4.5。

2.克隆本仓库到本地：

&#x20;  ```bash

&#x20;  git clone https://github.com/12YJ4/Facial-Recognition.git

3.使用 Qt Creator 打开 FinalPro.pro 文件（由于 OpenCV 是本地绝对路径配置，克隆本项目后请修改 FinalPro.pro 中的 INCLUDEPATH 和 LIBS 路径）

4.配置 OpenCV 路径：找到 FinalPro.pro 文件，将以下两行修改为你自己电脑上的 OpenCV 安装路径：

<img width="1488" height="300" alt="image" src="https://github.com/user-attachments/assets/cdc4ca29-d16f-479b-9d71-2e6b96e63650" />

5.编译运行即可（初次运行会在当前目录自动生成 student.db 数据库）



