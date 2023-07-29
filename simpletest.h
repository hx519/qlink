#ifndef SIMPLETEST_H
#define SIMPLETEST_H
#include <QtTest/QtTest>
#include "Mymap.h"

class SimpleTest:public QObject
{
    Q_OBJECT

private slots:
    //void isLegal_data();
    void isLegal();

};

#endif // SIMPLETEST_H
