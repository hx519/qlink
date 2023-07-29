#include "MyRole.h"

//
//构造函数
//参数为角色初始位置行数m，列数n，地图总行数h，地图总列数w，父窗口指针
//
MyRole::MyRole(int m, int n, int h, int w, QWidget *parent)
    :QWidget(parent)
{
    myLocation[0] = m;
    myLocation[1] = n;
    lastLocation[0] = m;
    lastLocation[1] = n;
    height = h;
    width = w;
}

//
//向上移动函数，改变角色坐标
//无参数，无返回值
//
void MyRole::moveUp()
{
    lastLocation[0] = myLocation[0];
    lastLocation[1] = myLocation[1];
    if(myLocation[0] == 0)
        myLocation[0] = height + 1;
    else
        myLocation[0] -= 1;
}

//
//向下移动函数，改变角色坐标
//无参数，无返回值
//
void MyRole::moveDown()
{
    lastLocation[0] = myLocation[0];
    lastLocation[1] = myLocation[1];
    if(myLocation[0] == (height + 1))
        myLocation[0] = 0;
    else
        myLocation[0] += 1;
}

//
//向左移动函数，改变角色坐标
//无参数，无返回值
//
void MyRole::moveLeft()
{
    lastLocation[0] = myLocation[0];
    lastLocation[1] = myLocation[1];
    if(myLocation[1] == 0)
        myLocation[1] = width + 1;
    else
        myLocation[1] -= 1;
}

//
//向右移动函数，改变角色坐标
//无参数，无返回值
//
void MyRole::moveRight()
{
    lastLocation[0] = myLocation[0];
    lastLocation[1] = myLocation[1];
    if(myLocation[1] == (width + 1))
        myLocation[1] = 0;
    else
        myLocation[1] += 1;
}
