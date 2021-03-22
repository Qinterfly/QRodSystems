#include <QtTest/QTest>

#include "core/project.h"

class TestCore : public QObject
{
    Q_OBJECT

private slots:
    void testProject();

};


void TestCore::testProject()
{

}

QTEST_APPLESS_MAIN(TestCore)

#include "testcore.moc"
