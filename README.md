# 智能人脸识别考试签到系统 (Exam Face Recognition System)

基于 Qt 和 OpenCV 开发的桌面端智能考场管理系统，通过摄像头实时检测人脸，实现考生的自动签到、签退与座位分配，并将数据持久化保存。

## 🛠 技术栈

* **开发语言：** C++11
* **界面框架：** Qt 5
* **视觉库：** OpenCV 3.4.5 (含 opencv_contrib 扩展模块)
* **数据库：** SQLite 3 (通过 Qt SQL 模块连接)
* **版本控制：** Git / GitHub

## ✨ 核心功能

1. **考场初始化：** 录入考试科目、代号及人数，动态生成对应数量的座位。
2. **人脸检测与录入：** 摄像头实时采集画面，使用 Haar 级联分类器定位人脸，并利用 LBPH 算法训练人脸模型。
3. **自动排座：** 考生签到时，系统自动从空闲座位中随机分配，并显示人脸快照。
4. **签退与记录：** 识别考生人脸后自动记录离场时间，清空座位。
5. **数据管理与导出：** 支持查看当前考场所有签到记录，并一键导出为 txt 文件。

## ⚙️ 运行说明（重要）

1. 确保本地已安装 **Qt 5** 及 **OpenCV 3.4.5**。
2. 克隆本仓库到本地： `git clone https://github.com/12YJ4/Facial-Recognition.git`
3. 使用 Qt Creator 打开 `FinalPro.pro` 文件。
4. **配置 OpenCV 路径：** 找到 `FinalPro.pro` 文件，将以下两行修改为你自己电脑上的 OpenCV 安装路径：
   ```qmake
   INCLUDEPATH += 你的OpenCV路径/include
   LIBS += 你的OpenCV路径/x64/mingw/lib/libopencv_*.a
