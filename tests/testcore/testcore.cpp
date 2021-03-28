#include <QtTest/QTest>

#include "array.h"

using namespace QRS;

class TestCore : public QObject
{
    Q_OBJECT

private slots:
    void testArray();
    void testModifyArray();
};

//! Test how an array is created
void TestCore::testArray()
{
    Array<double> matrix(2, 2);
    matrix[0][0] = 1.0;
    matrix[0][1] = 2.0;
    matrix[1][0] = 3.0;
    matrix[1][1] = 4.0;
    matrix.resize(3, 3);
    matrix.resize(5, 5);
    qDebug() << matrix;
    matrix.removeColumn(1);
    qDebug() << matrix;
    matrix.resize(1, 1);
    QCOMPARE(matrix[0][0], 1);
    matrix.resize(0, 1);
    QCOMPARE(matrix.cols(), 0);
}

//! Test how an array object can be modified
void TestCore::testModifyArray()
{
    std::map<double, Array<double>> map;
    map.emplace(0.0, Array<double>(1, 1));
    map.emplace(1.0, Array<double>(2, 2));
    Array<double>& t = map.at(1.0);
    t[0][0] = 10;
    t[1][1] = 15;
    QCOMPARE(map[1.0][0][0], 10);
    QCOMPARE(map[1.0][1][1], 15);
}


QTEST_APPLESS_MAIN(TestCore)

#include "testcore.moc"
