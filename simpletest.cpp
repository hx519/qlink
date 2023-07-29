#include "simpletest.h"

//void SimpleTest::isLegal_data()
//{
//    QTest::addColumn<bool>("result");
//    QTest::addColumn<bool>("expected");

//    QTest::newRow("illegal") << false << false;
//    QTest::newRow("straightLine") << true << true;
//    QTest::newRow("turnOfCase1") << true << true;
//    QTest::newRow("turnOfCase2") << true << true;
//    QTest::newRow("turnOfCase3") << true << true;
//    QTest::newRow("turnOfCase4") << true << true;
//    QTest::newRow("findLeft") << true << true;
//    QTest::newRow("findRight") << true << true;
//    QTest::newRow("findUp") << true << true;
//    QTest::newRow("findDown") << true << true;
//}

void SimpleTest::isLegal()
{
    MyMap map;
    map.initForTest();

    QCOMPARE(map.isLegal(1, 3, 2, 3), false);//illegal(different pictures)
    QCOMPARE(map.isLegal(1, 3, 3, 1), false);//illegal
    QCOMPARE(map.isLegal(1, 4, 2, 4), true);//straightLine
    QCOMPARE(map.isLegal(2, 1, 1, 2), true);//turnOfCase1
    QCOMPARE(map.isLegal(1, 7, 2, 8), true);//turnOfCase2
    QCOMPARE(map.isLegal(3, 1, 4, 2), true);//turnOfCase3
    QCOMPARE(map.isLegal(4, 7, 3, 8), true);//turnOfCase4
    QCOMPARE(map.isLegal(1, 2, 3, 1), true);//findLeft
    QCOMPARE(map.isLegal(1, 7, 3, 8), true);//findRight
    QCOMPARE(map.isLegal(1, 3, 1, 6), true);//findUp
    QCOMPARE(map.isLegal(3, 1, 4, 5), true);//findDown
}

//QTEST_MAIN(SimpleTest)
