/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
 * \brief Testing of the central window
 */

#include <QtTest>
#include <QCoreApplication>

#include "ui_mainwindow.h"
#include "mainwindow.h"

//! Test the central window
class TestCentral : public QObject
{
    Q_OBJECT

public:
    TestCentral() = default;
    ~TestCentral() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testProject();

private:
    MainWindow* mWindow;
};

//! Initialization
void TestCentral::initTestCase()
{
    mWindow = new MainWindow();
    mWindow->openProject("../../../../examples/simple.qrs");
}

//! Check a loaded project representation
void TestCentral::testProject()
{
    mWindow->show();
    QTest::qWait(1000);
}

//! Cleanup
void TestCentral::cleanupTestCase()
{
    delete mWindow;
}

QTEST_MAIN(TestCentral)

#include "testcentral.moc"
