#include "Mymap.h"

//
//MyMap构造函数
//参数分别为是否选择双人模式、计时器、加时道具、困难模式，以及该窗口的父窗口指针（默认为空）
//
MyMap::MyMap(bool mode, bool timer, bool prop, bool _boxNum, QWidget *parent)
    :QWidget(parent), isMode(mode), isTimer(timer), isProp(prop)
{
    if (true == _boxNum)
        boxNum = 12;
     //窗口设置及地图初始化
    setWindowTitle(tr("生肖连连看"));
    resize(994, 750);
    srand((int)time(0));
    mapInit();
     //设置计时器
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, QOverload<>::of(&MyMap::update));
    updateTimer -> start(10);
    if (isTimer){
        countdownTimer = new QTimer(this);
        connect(countdownTimer, &QTimer::timeout, this, &MyMap::countDown);
        countdownTimer -> start(1000);
    }
     //地图界面重新开始、暂停、洗牌、读档、存档按钮设置
    restartBtn = new QPushButton("新游戏", this);
    restartBtn -> resize(75, 30);
    restartBtn -> move(300, 630);
    restartBtn -> setFocusPolicy(Qt::NoFocus);
    connect(restartBtn, &QPushButton::clicked, this, &MyMap::restart);
    pauseBtn = new QPushButton("暂停", this);
    pauseBtn -> resize(65, 30);
    pauseBtn -> move(385, 630);
    pauseBtn -> setFocusPolicy(Qt::NoFocus);
    connect(pauseBtn, &QPushButton::clicked, this, &MyMap::pause);
    shuffleBtn = new QPushButton("洗牌", this);
    shuffleBtn -> resize(65, 30);
    shuffleBtn -> move(460, 630);
    shuffleBtn -> setFocusPolicy(Qt::NoFocus);
    connect(shuffleBtn, &QPushButton::clicked, this, &MyMap::shuffle);
    fileBtn = new QPushButton("存档", this);
    fileBtn -> resize(65, 30);
    fileBtn -> move(535, 630);
    fileBtn -> setFocusPolicy(Qt::NoFocus);
    connect(fileBtn, &QPushButton::clicked, this, &MyMap::writeFile);
    readFilrBtn = new QPushButton("读档", this);
    readFilrBtn -> resize(65, 30);
    readFilrBtn -> move(610, 630);
    readFilrBtn -> setFocusPolicy(Qt::NoFocus);
    connect(readFilrBtn, &QPushButton::clicked, this, &MyMap::readFile);
    if(!isTimer)
        pauseBtn -> setDisabled(true);
}

//
//MyMap析构函数
//
MyMap::~MyMap()
{

}

//
//单元测试时调用，生成一个不随机的地图
//无返回值
//
void MyMap::initForTest()
{
    //生成占地图1/4的矩阵，四角标记为0，其余为1
    for (int i = 0; i <= m+1; ++i)
        for (int j = 0; j <= n+1; ++j) {
            map[i][j] = 0;
        }

    for (int i = 1; i <= m / 2; ++i)
        for (int j = 1; j <= n / 2; ++j) {
            map[i][j] = 1;
        }
     map[1][1] = 0;
     map[1][n / 2] = 0;
     map[m / 2][1] = 0;
     map[m / 2][n / 2] = 0;
     map[2][3] = 2;
}

//
//地图初始化：人物生成、图案随机生成
//无返回值
//
void MyMap::mapInit()
{
    seconds = 200;
    isPause = false;
    role1 = new MyRole(0, 0, m, n, this);
    map[0][0] = -1;
    if(isMode){     //双人模式下生成角色2，置于地图右下角
        role2 = new MyRole(m + 1, n + 1, m, n, this);
        map[m + 1][n + 1] = -2;
    }
     //地图数组初始化
    for (int i = 0; i <= m+1; ++i)
        for (int j = 0; j <= n+1; ++j) {
            map[i][j] = 0;
        }

    int num[boxNum + 1];
    num[0] = 0;
    for (int i = 1; i <= boxNum; i++)
        num[i] = m * n / boxNum;    //保证每种图案都成对出现
    if(isProp){     //选择道具模式，留出道具位置
        num[(std::rand()) % boxNum + 1] -= 2;
        map[(std::rand()) % m + 1][(std::rand()) % n + 1] = -3;
        int i = (std::rand()) % m + 1, j = (std::rand()) % n + 1;
        if(map[i][j] != -3)
            map[i][j] = -3;
        else    //两次随机到同一个位置
            map[(i + 5) % m + 1][(j + 7) % n + 1] = -3;
    }
     // 随机填入一个类型的box,哈希表线性探测保证每个box为偶数个
    int boxVal;
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j) {
            if(map[i][j] == -3)
                continue;
            boxVal = ((i * j + std::rand())) % boxNum + 1;//值为1~boxNnum
            while(num[boxVal] == 0) {
                boxVal = (boxVal + 1) % (boxNum + 1);
            }
            map[i][j] = boxVal;
            num[boxVal] --;
        }
}

//
//画图事件，计时器每10ms触发一次
//参数为QPaintEvent指针，意为触发绘图事件
//无返回值
//
void MyMap::paintEvent(QPaintEvent *)
{
     //设置painter
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setWindow(-2, -2, 994, 750);
    //if(role1 -> isActivated && map[role1 -> activatedLocation[0]][role1 -> activatedLocation[1]] <= 0)
        //role1 -> isActivated = false;
     //角色1激活图案时画出红色方框
    if(role1 -> isActivated){
        painter.setPen(QPen(Qt::red, 3));
        int x = 55 * (role1 -> activatedLocation[1]), y = 55 * (role1 -> activatedLocation[0]);
        painter.drawLine(x, y, x + 50, y);
        painter.drawLine(x + 50, y, x + 50, y + 50);
        painter.drawLine(x, y + 50, x + 50, y + 50);
        painter.drawLine(x, y + 50, x, y);
    }
     //双人模式下地图更新（涉及位置重叠）及激活处理
    if (isMode){
        //if(role2 -> isActivated && map[role2 -> activatedLocation[0]][role2 -> activatedLocation[1]] <= 0)
            //role2 -> isActivated = false;
        if((role1 -> lastLocation[0] != role2 -> myLocation[0]) || (role1 -> lastLocation[1] != role2 -> myLocation[1]))
            map[role1 -> lastLocation[0]][role1 -> lastLocation[1]] = 0;
        map[role1 -> myLocation[0]][role1 -> myLocation[1]] = -1;
        if(map[role2 -> lastLocation[0]][role2 -> lastLocation[1]] != -1)
            map[role2 -> lastLocation[0]][role2 -> lastLocation[1]] = 0;
        map[role2 -> myLocation[0]][role2 -> myLocation[1]] = -2;
         //角色2激活
        if(role2 -> isActivated){
            painter.setPen(QPen(Qt::blue, 3));
            int x = 55 * (role2 -> activatedLocation[1]), y = 55 * (role2 -> activatedLocation[0]);
            painter.drawLine(x, y, x + 50, y);
            painter.drawLine(x + 50, y, x + 50, y + 50);
            painter.drawLine(x, y + 50, x + 50, y + 50);
            painter.drawLine(x, y + 50, x, y);
        }
    } else{
        map[role1 -> lastLocation[0]][role1 -> lastLocation[1]] = 0;
        map[role1 -> myLocation[0]][role1 -> myLocation[1]] = -1;
    }

    drawMap(&painter);   //画游戏地图
    drawOthers(&painter);   //画地图下方游戏状态栏
    if((role1 -> isRemoved) || ((isMode) && (role2 -> isRemoved)))
        drawLines(&painter);    //逻辑判定成功后画出示意折线
}

//
//画游戏主地图
//参数为painter指针，画图事件函数中调用，用于绘制界面
//无返回值
//
void MyMap::drawMap(QPainter *painter)
{
    QPixmap pix[15];
    pix[0].load("D:/galaxy/SEP/qt/my pro/my_qLink/1.jpg");
    pix[1].load("D:/galaxy/SEP/qt/my pro/my_qLink/2.jpg");
    pix[2].load("D:/galaxy/SEP/qt/my pro/my_qLink/3.jpg");
    pix[3].load("D:/galaxy/SEP/qt/my pro/my_qLink/4.jpg");
    pix[4].load("D:/galaxy/SEP/qt/my pro/my_qLink/5.jpg");
    pix[5].load("D:/galaxy/SEP/qt/my pro/my_qLink/6.jpg");
    pix[6].load("D:/galaxy/SEP/qt/my pro/my_qLink/7.jpg");
    pix[7].load("D:/galaxy/SEP/qt/my pro/my_qLink/8.jpg");
    pix[8].load("D:/galaxy/SEP/qt/my pro/my_qLink/9.jpg");
    pix[9].load("D:/galaxy/SEP/qt/my pro/my_qLink/10.jpg");
    pix[10].load("D:/galaxy/SEP/qt/my pro/my_qLink/11.jpg");
    pix[11].load("D:/galaxy/SEP/qt/my pro/my_qLink/12.jpg");
    pix[12].load("D:/galaxy/SEP/qt/my pro/my_qLink/role1");
    pix[13].load("D:/galaxy/SEP/qt/my pro/my_qLink/role2");
    pix[14].load("D:/galaxy/SEP/qt/my pro/my_qLink/timer");

    bool flag = true;
    for (int i = 0; i <= m + 1; i++)
        for (int j = 0; j <= n + 1; j++){
            if (0 == map[i][j])
                continue;
            if (-1 == map[i][j]){
                painter -> drawPixmap(55 * j, 55 * i, 50, 50, pix[12]);
                continue;
            }
            if (-2 == map[i][j]){
                painter -> drawPixmap(55 * j, 55 * i, 50, 50, pix[13]);
                continue;
            }
            if (-3 == map[i][j]){
                painter -> drawPixmap(55 * j, 55 * i, 50, 50, pix[14]);
                continue;
            }
            flag = false;
            painter -> drawPixmap(55 * j, 55 * i, 50, 50, pix[map[i][j] - 1]);
        }
     //画出游戏地图外边框
    painter -> setPen(QPen(Qt::black, 5));
    painter -> drawLine(0, 0, 0, 605);
    painter -> drawLine(0, 605, 990, 605);
    painter -> drawLine(990, 0, 990, 605);
    painter -> drawLine(990, 0, 0, 0);
     //没有可消除图案填入或计时结束，游戏结束
    if((flag) || (seconds == 0))
        drawEndWidget(painter);
}

//
//画地图下方游戏状态栏
//参数为painter指针，画图事件函数中调用，用于绘制界面
//无返回值
//
void MyMap::drawOthers(QPainter *painter) const
{
    QPixmap pix[2];
    pix[0].load("D:/galaxy/SEP/qt/my pro/my_qLink/role1");
    pix[1].load("D:/galaxy/SEP/qt/my pro/my_qLink/role2");
     //左侧角色头像、分数、消除状态提示
    QFont font;
    font.setPointSize(10);
    painter -> setFont(font);
    painter -> drawPixmap(20, 630, 100, 100, pix[0]);
    painter -> drawText(140, 660, QString::fromStdString("玩家1得分:" + std::to_string(role1 -> point)));
    if((role1 -> isRemoved) && !(role1 -> isActivated))
        painter -> drawText(140, 700, "消除");
    if(!(role1 -> isRemoved) && !(role1 -> isActivated))
        painter -> drawText(140, 700, "未消除");
     //右侧鼠标事件提示或角色头像、分数、消除状态提示
    if(isMode){
        painter -> drawPixmap(874, 630, 100, 100, pix[1]);
        painter -> drawText(704, 650, QString::fromStdString("玩家2得分:" + std::to_string(role2 -> point)));
        if((role2 -> isRemoved) && !(role2 -> isActivated))
            painter -> drawText(704, 700, "消除");
        if(!(role2 -> isRemoved) && !(role2 -> isActivated))
            painter -> drawText(704, 700, "未消除");
    } else{
        painter -> drawText(704, 660, QString::fromStdString("Flash:鼠标单击可触发角色移动"));
    }
     //倒计时状态
    if(isTimer){
        font.setPointSize(20);
        painter -> setFont(font);
        painter -> drawText(330, 710, QString::fromStdString("剩余时间:" + std::to_string(seconds) + "秒"));
    }
}

//
//倒计时函数，每1s被触发一次
//无返回值
//
void MyMap::countDown()
{
    if(seconds == 0)
        return;
    seconds--;
}

//
//发生按键事件时调用
//参数为按键事件指针
//无返回值
//
void MyMap::keyPressEvent(QKeyEvent *event)
{
    if(isPause)
        return;

    //双人模式
    if((event -> key() == Qt::Key_W) && isMode)
        moveUp(role1);
    if((event -> key() == Qt::Key_A) && isMode)
        moveLeft(role1);
    if((event -> key() == Qt::Key_S) && isMode)
        moveDown(role1);
    if((event -> key() == Qt::Key_D) && isMode)
        moveRight(role1);
    if((event -> key() == Qt::Key_Up) && isMode)
        moveUp(role2);
    if((event -> key() == Qt::Key_Down) && isMode)
        moveDown(role2);
    if((event -> key() == Qt::Key_Left) && isMode)
        moveLeft(role2);
    if((event -> key() == Qt::Key_Right) && isMode)
        moveRight(role2);

    //单人模式，两组方位按键都可使用
    if((event -> key() == Qt::Key_W) && !isMode)
        moveUp(role1);
    if((event -> key() == Qt::Key_A) && !isMode)
        moveLeft(role1);
    if((event -> key() == Qt::Key_S) && !isMode)
        moveDown(role1);
    if((event -> key() == Qt::Key_D) && !isMode)
        moveRight(role1);
    if((event -> key() == Qt::Key_Up) && !isMode)
        moveUp(role1);
    if((event -> key() == Qt::Key_Down) && !isMode)
        moveDown(role1);
    if((event -> key() == Qt::Key_Left) && !isMode)
        moveLeft(role1);
    if((event -> key() == Qt::Key_Right) && !isMode)
        moveRight(role1);

}

//
//角色向上移动操作
//参数为激活该操作的角色指针
//无返回值
//
void MyMap::moveUp(MyRole *role)
{
    if(isProp){     //判断是否为道具
        if(map[role -> myLocation[0] - 1][role -> myLocation[1]] == -3){
            seconds++;
            map[role -> myLocation[0] - 1][role -> myLocation[1]] = 0;
            return;
        }
    }
    if((role -> myLocation[0] == 0) || (map[role -> myLocation[0] - 1][role -> myLocation[1]] <= 0))
        role -> moveUp();    //普通移动，调用角色类移动函数
    else {
        if((role -> isActivated) && ((role -> activatedLocation[0] != role -> myLocation[0] - 1) || (role -> activatedLocation[1] != role -> myLocation[1]))) {
             //第二次激活图案，进入逻辑判断
            bool flag = isLegal(role -> activatedLocation[0], role -> activatedLocation[1], role -> myLocation[0] - 1, role -> myLocation[1]);
            if (flag) {     //逻辑判定true
                role -> point += POINTS;
                role -> isActivated = false;
                role -> isRemoved = true;
                map[role -> activatedLocation[0]][role -> activatedLocation[1]] = 0;
                map[role -> myLocation[0] - 1][role -> myLocation[1]] = 0;
            } else {
                role -> isActivated = false;
                role -> isRemoved = false;
            }
        } else {     //激活图案
            role -> isActivated = true;
            role -> activatedLocation[0] = role -> myLocation[0] - 1;
            role -> activatedLocation[1] = role -> myLocation[1];
        }
    }
}

//
//角色向下移动操作
//参数为激活该操作的角色指针
//无返回值
//
void MyMap::moveDown(MyRole *role)
{
    if(isProp){
        if(map[role -> myLocation[0] + 1][role -> myLocation[1]] == -3){
            seconds++;
            map[role -> myLocation[0] + 1][role -> myLocation[1]] = 0;
            return;
        }
    }
    if((role -> myLocation[0] == m + 1) || (map[role -> myLocation[0] + 1][role -> myLocation[1]] <= 0))
        role -> moveDown();
    else {
        if((role -> isActivated) && ((role -> activatedLocation[0] != role -> myLocation[0] + 1) || (role -> activatedLocation[1] != role -> myLocation[1]))) {
            bool flag = isLegal(role -> activatedLocation[0], role -> activatedLocation[1], role -> myLocation[0] + 1, role -> myLocation[1]);
            if (flag) {
                role -> point += POINTS;
                role -> isActivated = false;
                role -> isRemoved = true;
                map[role -> activatedLocation[0]][role -> activatedLocation[1]] = 0;
                map[role -> myLocation[0] + 1][role -> myLocation[1]] = 0;
            } else {
                role -> isActivated = false;
                role -> isRemoved = false;
            }
        } else {
            role -> isActivated = true;
            role -> activatedLocation[0] = role -> myLocation[0] + 1;
            role -> activatedLocation[1] = role -> myLocation[1];
        }
    }
}

//
//角色向左移动操作
//参数为激活该操作的角色指针
//无返回值
//
void MyMap::moveLeft(MyRole *role)
{
    if(isProp){
        if(map[role -> myLocation[0]][role -> myLocation[1] - 1] == -3){
            seconds++;
            map[role -> myLocation[0]][role -> myLocation[1] - 1] = 0;
            return;
        }
    }
    if((role -> myLocation[1] == 0) || (map[role -> myLocation[0]][role -> myLocation[1] - 1] <= 0))
        role -> moveLeft();
    else {
        if((role -> isActivated) && ((role -> activatedLocation[0] != role -> myLocation[0]) || (role -> activatedLocation[1] != role -> myLocation[1] - 1))) {
            bool flag = isLegal(role -> activatedLocation[0], role -> activatedLocation[1], role -> myLocation[0], role -> myLocation[1] - 1);
            if (flag) {
                role -> point += POINTS;
                role -> isActivated = false;
                role -> isRemoved = true;
                map[role -> activatedLocation[0]][role -> activatedLocation[1]] = 0;
                map[role -> myLocation[0]][role -> myLocation[1] - 1] = 0;
            } else{
                role -> isActivated = false;
                role -> isRemoved = false;
            }
        } else {
            role -> isActivated = true;
            role -> activatedLocation[0] = role -> myLocation[0];
            role -> activatedLocation[1] = role -> myLocation[1] - 1;
        }
    }
}

//
//角色向右移动操作
//参数为激活该操作的角色指针
//无返回值
//
void MyMap::moveRight(MyRole *role)
{
    if(isProp){
        if(map[role -> myLocation[0]][role -> myLocation[1] + 1] == -3){
            seconds++;
            map[role -> myLocation[0]][role -> myLocation[1] + 1] = 0;
            return;
        }
    }
    if((role -> myLocation[1] == n + 1) || (map[role -> myLocation[0]][role -> myLocation[1] + 1] <= 0))
        role -> moveRight();
    else {
        if((role -> isActivated) && ((role -> activatedLocation[0] != role -> myLocation[0]) || (role -> activatedLocation[1] != role -> myLocation[1] + 1))) {
            bool flag = isLegal(role -> activatedLocation[0], role -> activatedLocation[1], role -> myLocation[0], role -> myLocation[1] + 1);
            if (flag) {
                role -> point += POINTS;
                role -> isActivated = false;
                role -> isRemoved = true;
                map[role -> activatedLocation[0]][role -> activatedLocation[1]] = 0;
                map[role -> myLocation[0]][role -> myLocation[1] + 1] = 0;
            } else {
                role -> isActivated = false;
                role -> isRemoved = false;
            }
        } else {
            role -> isActivated = true;
            role -> activatedLocation[0] = role -> myLocation[0];
            role -> activatedLocation[1] = role -> myLocation[1] + 1;
        }
    }
}

//
//逻辑判定函数
//m1,n1与m2,n2分别是先后激活的图案在地图数组中的行数和列数
//两个图案能在两次转弯内连接返回true，反之返回false
//
bool MyMap::isLegal(int m1, int n1, int m2, int n2)
{
    pixX1 = m1;
    pixX2 = m2;
    pixY1 = n1;
    pixY2 = n2;
    if(map[m1][n1] != map[m2][n2])
        return false;
     //判断是否能直线连接
    bool flag;
    if((m1 == m2) || (n1 == n2)) {
        flag = isStraightLine(m1, n1, m2, n2);
        if(flag){
            isLinesDrawn = false;
            return flag;
        }
    }
     //判断是否能一次转弯连接，不能则判断两次转弯
    flag = isOneTurn(m1, n1, m2, n2);
    if(flag){
        isLinesDrawn = false;
        return flag;
    }
    else
        flag = isTwoTurn(m1, n1, m2, n2);
    if(flag)
        isLinesDrawn = false;
    return flag;
}

//
//判断是否为直线连接
//m1,n1与m2,n2分别是先后激活的图案在地图数组中的行数和列数
//两个图案能直线连接返回true，反之返回false
//
bool MyMap::isStraightLine(int m1, int n1, int m2, int n2)
{
    if(m1 == m2) {      //在同一行
        if(n1 < n2)
            for(int i = n1 + 1; i < n2; i++) {
                if(map[m1][i] > 0)
                    return false;
            }
        else
            for(int i = n2 + 1; i < n1; i++)
                if(map[m1][i] > 0)
                    return false;
    } else {    //在同一列
        if(m1 < m2)
            for(int i = m1 + 1; i < m2; i++) {
                if(map[i][n1] > 0)
                    return false;
            }
        else
            for(int i = m2 + 1; i < m1; i++)
                if(map[i][n1] > 0)
                    return false;
    }
    turnX1 = m1;
    turnX2 = m2;
    turnY1 = n1;
    turnY2 = n2;
    return true;
}

//
//判断是否为一次转弯连接
//m1,n1与m2,n2分别是先后激活的图案在地图数组中的行数和列数
//两个图案能经过一次转弯连接返回true，反之返回false
//
bool MyMap::isOneTurn(int m1, int n1, int m2, int n2)
{
    if((m1 == m2) || (n1 == n2))
        return false;
     //转弯在左上角
    bool flag = true;
    for(int i = qMin(m1, m2); i < qMax(m1, m2); i++)
        if(map[i][qMin(n1, n2)] > 0)
            flag = false;
    for(int i = qMin(n1, n2); i < qMax(n1, n2); i++)
        if(map[qMin(m1, m2)][i] > 0)
            flag = false;
    if(flag){
        turnX1 = turnX2 = qMin(m1, m2);
        turnY1 = turnY2 = qMin(n1, n2);
        return flag;
    }
     //转弯在右上角
    flag = true;
    for(int i = qMin(m1, m2); i < qMax(m1, m2); i++)
        if(map[i][qMax(n1, n2)] > 0)
            flag = false;
    for(int i = qMin(n1, n2) + 1; i <= qMax(n1, n2); i++)
        if(map[qMin(m1, m2)][i] > 0)
            flag = false;
    if(flag){
        turnX1 = turnX2 = qMin(m1, m2);
        turnY1 = turnY2 = qMax(n1, n2);
        return flag;
    }
     //转弯在左下角
    flag = true;
    for(int i = qMin(m1, m2) + 1; i <= qMax(m1, m2); i++)
        if(map[i][qMin(n1, n2)] > 0)
            flag = false;
    for(int i = qMin(n1, n2); i < qMax(n1, n2); i++)
        if(map[qMax(m1, m2)][i] > 0)
            flag = false;
    if(flag){
        turnX1 = turnX2 = qMax(m1, m2);
        turnY1 = turnY2 = qMin(n1, n2);
        return flag;
    }
     //转弯在右下角
    flag = true;
    for(int i = qMin(m1, m2) + 1; i <= qMax(m1, m2); i++)
        if(map[i][qMax(n1, n2)] > 0)
            flag = false;
    for(int i = qMin(n1, n2) + 1; i <= qMax(n1, n2); i++)
        if(map[qMax(m1, m2)][i] > 0)
            flag = false;
    if(flag){
        turnX1 = turnX2 = qMax(m1, m2);
        turnY1 = turnY2 = qMax(n1, n2);
        return flag;
    }
    return false;
}

//
//判断是否为两次转弯连接
//m1,n1与m2,n2分别是先后激活的图案在地图数组中的行数和列数
//两个图案经过两次转弯连接返回true，反之返回false
//
bool MyMap::isTwoTurn(int m1, int n1, int m2, int n2)
{
    bool flag = true;
     //从第一个图案向左找
    flag = findLeft(m1, n1, m2, n2);
    if(flag)
        return flag;
     //从第一个图案向右找
    flag = findRight(m1, n1, m2, n2);
    if(flag)
        return flag;
     //从第一个图案向上找
    flag = findUp(m1, n1, m2, n2);
    if(flag)
        return flag;
     //从第一个图案向下找
    flag = findDown(m1, n1, m2, n2);
    if(flag)
        return flag;

    return flag;
}

//
//从第一个图案向左找，判断是否为两次转弯连接
//m1,n1与m2,n2分别是先后激活的图案在地图数组中的行数和列数
//判断能由第一个图案向左出发两次转弯达到第二个图案返回true，反之返回false
//
bool MyMap::findLeft(int m1, int n1, int m2, int n2)
{
    if(m1 == m2)
        return false;

    int i, j, k;
    //从第一个点左侧开始找，遇到图片跳出循环
    for(i = n1 - 1; i >= 0; i--){
        if(map[m1][i] > 0)
            break;
        //进入第i列寻找，从下到上
        for(j = (m1 > m2 ? m1 : m2); (j >= m1) || (j >= m2); j--){
            if(map[j][i] > 0)
                break;        
            //第三段水平线
            for(k = (i < n2 ? i : n2) + 1; (k != n2) && (k != i); k++){
                if(map[m2][k] > 0)
                    break;
            }
        }
        if(((k == n2) || (k == i)) && (j < m1) && (j < m2)){
            turnX1 = m1;
            turnY1 = i;
            turnX2 = m2;
            turnY2 = i;
            return true;
        }
    }

    return false;
}

//
//从第一个图案向右找，判断是否为两次转弯连接
//m1,n1与m2,n2分别是先后激活的图案在地图数组中的行数和列数
//判断能由第一个图案向右出发两次转弯达到第二个图案返回true，反之返回false
//
bool MyMap::findRight(int m1, int n1, int m2, int n2)
{
    if(m1 == m2)
        return false;

    int i, j, k;
    //从第一个点向右找
    for(i = n1 + 1; i < n + 2; i++){
        if(map[m1][i] > 0)
            break;
        //进入第i列，从下往上找
        for(j = (m1 > m2 ? m1 : m2); (j >= m1) || (j >= m2); j--){
            if(map[j][i] > 0)
                break;
            //第三段
            for(k = (i < n2 ? i : n2) + 1; (k != n2) && (k != i); k++){
                if(map[m2][k] > 0)
                    break;
            }
        }
        if(((k == n2) || (k == i)) && (j < m1) && (j < m2)){
            turnX1 = m1;
            turnY1 = i;
            turnX2 = m2;
            turnY2 = i;
            return true;
        }
    }

    return false;
}

//
//从第一个图案向上找，判断是否为两次转弯连接
//m1,n1与m2,n2分别是先后激活的图案在地图数组中的行数和列数
//判断能由第一个图案向上出发两次转弯达到第二个图案返回true，反之返回false
//
bool MyMap::findUp(int m1, int n1, int m2, int n2)
{
    if(n1 == n2)
        return false;

    int i, j, k;
    //从第一个点向上找
    for(i = m1 - 1; i >= 0; i--){
        if(map[i][n1] > 0)
            break;
        //进入第i行
        for(j = (n1 > n2 ? n1 : n2); (j >= n1) || (j >= n2); j--){
            if(map[i][j] > 0)
                break;
            //第三段
            for(k = (i < m2 ? i : m2) + 1; (k != m2) && (k != i); k++){
                if(map[k][n2] > 0)
                    break;
            }            
        }
        if(((k == m2) || (k == i)) && (j < n1) && (j < n2)){
            turnX1 = i;
            turnY1 = n1;
            turnX2 = i;
            turnY2 = n2;
            return true;
        }
    }

    return false;
}

//
//从第一个图案向下找，判断是否为两次转弯连接
//m1,n1与m2,n2分别是先后激活的图案在地图数组中的行数和列数
//判断能由第一个图案向下出发两次转弯达到第二个图案返回true，反之返回false
//
bool MyMap::findDown(int m1, int n1, int m2, int n2)
{
    if(n1 == n2)
        return false;

    int i, j, k;
    //从第一个点向下找
    for(i = m1 + 1; i < m + 2; i++){
        if(map[i][n1] > 0)
            break;
        //进入第i行
        for(j = (n1 > n2 ? n1 : n2); (j >= n1) || (j >= n2); j--){
            if(map[i][j] > 0)
                break;
            //第三段
            for(k = (i < m2 ? i : m2) + 1; (k != m2) && (k != i); k++){
                if(map[k][n2] > 0)
                    break;
            }
        }
        if(((k == m2) || (k == i)) && (j < n1) && (j < n2)){
            turnX1 = i;
            turnY1 = n1;
            turnX2 = i;
            turnY2 = n2;
            return true;
        }
    }

    return false;
}

//
//绘制游戏结束时界面
//参数为painter指针，由绘图事件函数调用，用于绘制界面
//无返回值
//
void MyMap::drawEndWidget(QPainter *painter)
{
    isPause = true;     //结束后操作停止
    static const QPoint endWidget[4] = {
        QPoint(332, 150),
        QPoint(662, 150),
        QPoint(662, 400),
        QPoint(332, 400)
    };
    painter -> setBrush(Qt::gray);
    painter -> drawConvexPolygon(endWidget, 4);
    QFont font;
    font.setPointSize(20);
    painter -> setFont(font);
    if(!isMode){    //单人模式
        if(seconds == 0)
            painter -> drawText(400, 280, QString::fromStdString("游戏时间到"));
        else
            painter -> drawText(400, 280, QString::fromStdString("游戏获胜！"));
    } else{     //双人模式游戏结束判定胜负
        if((role1 -> point) > (role2 -> point))
            painter -> drawText(400, 280, QString::fromStdString("玩家1获胜！"));
        if((role1 -> point) < (role2 -> point))
            painter -> drawText(400, 280, QString::fromStdString("玩家2获胜！"));
        if((role1 -> point) == (role2 -> point))
            painter -> drawText(400, 280, QString::fromStdString("双方平局！"));
    }

}

//
//绘制逻辑判定正确后的连接图案的折线
//参数为painter指针，由绘图事件函数调用，用于绘制界面
//无返回值
//
void MyMap::drawLines(QPainter *painter)
{
     //避免重复绘制
    if(isLinesDrawn == true)
        return;

    if(role1 -> isRemoved){
        painter -> setPen(QPen(Qt::green, 3));
        painter -> drawLine(55 * pixY1 + 25, 55 * pixX1 + 25, 55 * turnY1 + 25, 55 * turnX1 + 25);
        painter -> drawLine(55 * turnY2 + 25, 55 * turnX2 + 25, 55 * turnY1 + 25, 55 * turnX1 + 25);
        painter -> drawLine(55 * pixY2 + 25, 55 * pixX2 + 25, 55 * turnY2 + 25, 55 * turnX2 + 25);
        isLinesDrawn = true;
        return;
    }
    if(isMode && role2 -> isRemoved){
        painter -> setPen(QPen(Qt::green, 3));
        painter -> drawLine(55 * pixY1 + 25, 55 * pixX1 + 25, 55 * turnY1 + 25, 55 * turnX1 + 25);
        painter -> drawLine(55 * turnY2 + 25, 55 * turnX2 + 25, 55 * turnY1 + 25, 55 * turnX1 + 25);
        painter -> drawLine(55 * pixY2 + 25, 55 * pixX2 + 25, 55 * turnY2 + 25, 55 * turnX2 + 25);
    }
    isLinesDrawn = true;
}

//
//鼠标点击事件，单人模式下实现鼠标控制人物移动
//参数为鼠标点击事件指针，鼠标事件发生时触发
//无返回值
//
void MyMap::mousePressEvent(QMouseEvent *event)
{
    if(isPause)
        return;

    int m, n;
    m = (event -> y() - 2) / 55;
    n = (event -> x() - 2) / 55;
    if(!isMode){    //单人模式下可用
      if(map[m][n] == 0){    //只能移动到空白位置
          role1 -> lastLocation[0] = role1 -> myLocation[0];
          role1 -> lastLocation[1] = role1 -> myLocation[1];
          role1 -> myLocation[0] = m;
          role1 -> myLocation[1] = n;
          map[m][n] = -1;
          map[role1 -> lastLocation[0]][role1 -> lastLocation[1]] = 0;
      }
    }
}

//
//实现洗牌功能，打乱界面剩余图案
//无参数，无返回值
//
void MyMap::shuffle()
{
    int num[boxNum];    //记录各个图案剩余个数
    for(int i = 1; i <= m; i++)
        for(int j = 1; j <= n; j++){
            if(map[i][j] > 0)
                num[map[i][j] - 1]++;
        }

    int boxVal;
    for(int i = 1; i <= m; i++)
        for(int j = 1; j <= n; j++){
            if(map[i][j] <= 0)
                continue;
            boxVal = ((i * j + std::rand())) % boxNum;//值为0 ~ boxNnum - 1
            while(num[boxVal] == 0) {
                boxVal = (boxVal + 1) % boxNum;
            }
            map[i][j] = boxVal + 1;
            num[boxVal] --;
        }
}

//
//实现重新开始功能，生成新地图，刷新游戏状态
//无参数，无返回值
//
void MyMap::restart()
{
    delete role1;
    if(isMode)
        delete role2;
    mapInit();
}

//
//实现暂停，继续功能
//无参数，无返回值
//
void MyMap::pause()
{
    if(isPause){     //当前状态为暂停，点击后继续游戏
        shuffleBtn -> setDisabled(false);
        restartBtn -> setDisabled(false);
        pauseBtn -> setText("暂停");
        countdownTimer -> start(1000);
        updateTimer -> start(10);
        isPause = false;
    }
    else{    //当前状态为游戏进行中，点击后暂停游戏
        shuffleBtn -> setDisabled(true);
        restartBtn -> setDisabled(true);
        pauseBtn -> setText("继续");
        countdownTimer -> stop();
        updateTimer -> stop();
        isPause = true;
    }
}

//
//实现存档功能
//无参数，无返回值
//
void MyMap::writeFile()
{
    if(!isPause)
        pause();
    QString fileName = QFileDialog::getSaveFileName(this, "Save");
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Failed to save file: " + file.errorString());
        return;
    }
    QTextStream stream(&file);
    stream << "qlink" << endl;
    stream << "isMode" << endl;
    stream << int(isMode) << endl;
    stream << "isTimer" << endl;
    stream << int(isTimer) << endl;
    stream << "isProp" << endl;
    stream << int(isProp) << endl;
    stream << "isBoxNum" << endl;
    if(boxNum == 12)
        stream << 1 << endl;
    else
        stream << 0 << endl;
    stream << "m = " << endl;
    stream << m << endl;
    stream << "n = " << endl;
    stream << n << endl;
    for(int i = 0; i < m + 2; i++)
        for(int j = 0; j < n + 2; j++)
            stream << map[i][j] << endl;
    stream << "role1" << endl;
    stream << "ponit" << endl;
    stream << role1 -> point << endl;
    stream << "location0" << endl;
    stream << role1 -> myLocation[0] << endl;
    stream << "location1" << endl;
    stream << role1 -> myLocation[1] << endl;
    stream << "seconds" << endl;
    stream << seconds << endl;
    if(isMode){
        stream << "role2" << endl;
        stream << "ponit" << endl;
        stream << role2 -> point << endl;
        stream << "location0" << endl;
        stream << role2 -> myLocation[0] << endl;
        stream << "location1" << endl;
        stream << role2 -> myLocation[1] << endl;
    }
    file.close();
}

//
//实现读档功能
//无参数，无返回值
//
void MyMap::readFile()
{
    if(!isPause)
        pause();
    QString fileName = QFileDialog::getOpenFileName(this, "Open");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Failed to open file: " + file.errorString());
        return;
    }
    QTextStream stream(&file);
    QString text = stream.readAll();
    QStringList list = text.split("\n");
     //检查文件头是否合法
    if(list[0] != "qlink"){
        QMessageBox::warning(this, "Warning", "Open the wrong file!");
        return;
    }
    if (list[2] == "1")
        isMode = true;
    else
        isMode = false;
    if (list[4] == "1")
        isTimer = true;
    else
        isTimer = false;
    if (list[6] == "1")
        isProp = true;
    else
        isProp = false;
    if (list[8] == "1")
        boxNum = 12;
    else
        boxNum = 6;

    loadFile(list);
    file.close();
}

//
//载入文档中存储的游戏状态
//参数为文档内容分割出的list引用，传递文档中每行信息
//无返回值
//
void MyMap::loadFile(const QStringList &list)
{
    delete role1;
    role1 = new MyRole(list[17 + (m + 2) * (n + 2)].toInt(), list[19 + (m + 2) * (n + 2)].toInt(), m, n, this);
    role1 -> point = list[15 + (m + 2) * (n + 2)].toInt();
    if(isMode){
        if(role2)
            delete role2;
        role2 = new MyRole(list[26 + (m + 2) * (n + 2)].toInt(), list[28 + (m + 2) * (n + 2)].toInt(), m, n, this);
        role2 -> point = list[24 + (m + 2) * (n + 2)].toInt();
    }

    if(isTimer)
        seconds = list[21 + (m + 2) * (n + 2)].toInt();

    for(int i = 0; i < m + 2; i++)
        for(int j = 0; j < n + 2; j++)
            map[i][j] = list[12 + i * (n + 2) + j + 1].toInt();

    if(!isPause)
        pause();
}
