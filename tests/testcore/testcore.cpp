#include <QtTest/QTest>

#include "array.h"

using namespace QRS;

class TestCore : public QObject
{
    Q_OBJECT

private slots:
    void testArray();

};

//! Test the Array class
void TestCore::testArray()
{
    Array<double> matrix(2, 2);
    matrix[0][0] = 1.0;
    matrix[0][1] = 2.0;
    matrix[1][0] = 3.0;
    matrix[1][1] = 4.0;
    matrix.resize(3, 3);
    matrix.resize(5, 5);
    matrix.resize(1, 1);
    QCOMPARE(matrix[0][0], 1);
    matrix.resize(0, 1);
    QCOMPARE(matrix.cols(), 0);
}



QTEST_APPLESS_MAIN(TestCore)

#include "testcore.moc"
