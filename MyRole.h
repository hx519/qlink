#pragma once

#include <QObject>
#include <QtWidgets>
#include <QKeyEvent>

class MyRole:public QWidget
{
    Q_OBJECT

    friend class MyMap;

public:
    MyRole(int x, int y, int h, int w, QWidget *parent = nullptr);
    void setControlKeys(bool isRoleTow);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

private:
    int lastLocation[2];
    int myLocation[2];
    int isActivated = false;
    int activatedLocation[2];
    int point = 0;
    int height;
    int width;
    bool isRemoved = false;

};

