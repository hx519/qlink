#include "Mymenu.h"
#include <iostream>

//
//构造函数，生成菜单界面
//参数为父窗口指针
//
MyMenu::MyMenu(QWidget *parent)
    :QMainWindow(parent)
{
    //窗口设置
    setWindowTitle(tr("生肖连连看"));
    setWindowIcon(QIcon("D:/galaxy/SEP/qt/my pro/my_qLink/theme"));
    resize(500,600);

    //设置复选框
    modeCheck = new QCheckBox(tr("双人模式"), this);
    timerCheck = new QCheckBox(tr("计时器"), this);
    propCheck = new QCheckBox(tr("加时道具"), this);
    boxNumCheck = new QCheckBox(tr("困难模式"), this);
    modeCheck -> move(150, 330);
    timerCheck -> move(150, 280);
    propCheck -> move(150, 230);
    boxNumCheck -> move(150, 180);

    //设置按钮
    fileBtn = new QPushButton(QIcon("D:/galaxy/SEP/qt/my pro/my_qLink/file.jpg"), "读档", this);
    helpBtn = new QPushButton(QIcon("D:/galaxy/SEP/qt/my pro/my_qLink/help.jpg"), "帮助", this);
    startBtn = new QPushButton("开始", this);
    fileBtn -> resize(130, 50);
    helpBtn -> resize(130, 50);
    startBtn -> resize(200, 120);
    fileBtn -> move(100, 100);
    helpBtn -> move(270, 100);
    startBtn -> move(150, 400);
    connect(startBtn, &QPushButton::clicked, this, &MyMenu::startBtnClicked);
    connect(helpBtn, &QPushButton::clicked, this, &MyMenu::showHelpInfo);
    connect(fileBtn, &QPushButton::clicked, this, &MyMenu::readFile);
}

//
//析构函数
//
MyMenu::~MyMenu()
{

}

//
//显示游戏帮助弹窗
//无参数，无返回值
//
void MyMenu::showHelpInfo() const
{
    //新建窗口标签显示游戏规则
    QWidget *helpInfo = new QWidget;
    QLabel *label = new QLabel(helpInfo);

    //设置窗口格式
    helpInfo -> resize(716, 188);
    helpInfo -> setWindowTitle("游戏帮助");

    //帮助文本信息
    label -> setText("1.主界面勾选模式后点击“开始”进入游戏地图\n2.单人模式下“上、下、左、右”控制角色移动，双人模式下“W,S,A,D”控制2号角色\n3.角色移动到目标方块后继续向前激活该目标，移动到边界继续前进从另一侧穿出\n4.单人模式下鼠标点击空白处可触发角色移动\n5.两次激活相同图案且能在两次转弯内连接即可消除");

    helpInfo -> show();
}

//
//游戏开始函数，读取游戏模式选择，触发游戏界面
//无参数，无返回值
//
void MyMenu::startBtnClicked()
{
    map = new MyMap(modeCheck -> isChecked(), timerCheck -> isChecked(), propCheck -> isChecked(), boxNumCheck -> isChecked());
    map -> show();
    setVisible(false);
}

//
//实现读档功能
//无参数，无返回值
//
void MyMenu::readFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Failed to open file: " + file.errorString());
        return;
    }
    QTextStream stream(&file);
    QString text = stream.readAll();
    QStringList list = text.split("\n");

    if(list[0] != "1"){
        QMessageBox::warning(this, "Warning", "Open the wrong file!");
        return;
    }
    bool isMode = false;
    bool isProp = false;
    bool isTimer = false;
    bool isBoxNum = false;
    if (list[2] == "1")
        isMode = true;
    if (list[4] == "1")
        isTimer = true;
    if (list[6] == "1")
        isProp = true;
    if (list[8] == "1")
        isBoxNum = true;

    map = new MyMap(isMode, isTimer, isProp, isBoxNum);
    map -> loadFile(list);
    map -> show();
    file.close();
    setVisible(false);
}
