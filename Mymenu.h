#pragma once

#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QCheckBox>
#include <QAction>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include "Mymap.h"

class MyMenu:public QMainWindow
{
    Q_OBJECT
public:
    MyMenu(QWidget *parent = nullptr);
    ~MyMenu();

protected:
    void showHelpInfo() const;
    void readFile();

public slots:
    void startBtnClicked();

private:
    MyMap *map;
    QPushButton *startBtn;
    QPushButton *fileBtn;
    QPushButton *helpBtn;
    QCheckBox *modeCheck;
    QCheckBox *boxNumCheck;
    QCheckBox *timerCheck;
    QCheckBox *propCheck;
    QLabel *helpInfo;

};
