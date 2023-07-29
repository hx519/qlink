#pragma once

#include <QtWidgets>
#include <QTime>
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QThread>
#include <QFile>
#include "MyRole.h"

#define MAX_MAP_SIZE 20
#define POINTS 10

class MyMap:public QWidget
{
    Q_OBJECT

friend class simpleTest;

public:
    MyMap(bool mode = false, bool timer = false, bool prop = false, bool _boxNum = false, QWidget *parent = nullptr);
    ~MyMap();
    void loadFile(const QStringList &list);
    void initForTest();
    bool isLegal(int m1, int n1, int m2, int n2);

protected:   
    void mapInit();
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void drawMap(QPainter *painter);
    void drawOthers(QPainter *painter) const;
    void drawEndWidget(QPainter *painter);
    void drawLines(QPainter *painter);
    void countDown();
    void shuffle();
    void restart();
    void pause();
    void writeFile();
    void readFile();

    void moveUp(MyRole *);
    void moveDown(MyRole *);
    void moveLeft(MyRole *);
    void moveRight(MyRole *);

    bool isStraightLine(int m1, int n1, int m2, int n2);
    bool isOneTurn(int m1, int n1, int m2, int n2);
    bool isTwoTurn(int m1, int n1, int m2, int n2);
    bool findLeft(int m1, int n1, int m2, int n2);
    bool findRight(int m1, int n1, int m2, int n2);
    bool findUp(int m1, int n1, int m2, int n2);
    bool findDown(int m1, int n1, int m2, int n2);

private:
    QTimer *updateTimer;
    QTimer *countdownTimer;
    MyRole *role1;
    MyRole *role2;
    QPushButton *restartBtn;
    QPushButton *pauseBtn;
    QPushButton *fileBtn;
    QPushButton *readFilrBtn;
    QPushButton *shuffleBtn;
    int seconds = 100;
    bool isMode;
    bool isTimer;
    bool isProp;
    bool isPause = false;
    bool isLinesDrawn = true;
    int turnX1 = 0;
    int turnX2 = 0;
    int turnY1 = 0;
    int turnY2 = 0;
    int pixX1;
    int pixX2;
    int pixY1;
    int pixY2;
    int m = 9, n = 16; //地图规格
    int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
    int boxNum = 6;//用户自定义图案种类，困难模式为12种，简单模式为6种
};
