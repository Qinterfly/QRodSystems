/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Testing of the central window
 */

#include <QtTest>
#include <QCoreApplication>

#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "utilities.h"

using namespace QRS::Utilities;
using namespace QRS::App;

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
    void testRun();

private:
    MainWindow* mWindow;
};

//! Initialization
void TestCentral::initTestCase()
{
    // App
    QCoreApplication::setOrganizationName(APP_AUTHOR);
    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    qInstallMessageHandler(throwMessage);
    qApp->setStyle("Fusion");
    qApp->setStyleSheet(File::loadFileContent(":/styles/modern.qss"));
    // Window
    mWindow = new MainWindow();
    mWindow->openProject("../../../../examples/base.qrs");
}

//! Test run
void TestCentral::testRun()
{
    mWindow->show();
    qApp->exec();
}

//! Cleanup
void TestCentral::cleanupTestCase()
{
    delete mWindow;
}

QTEST_MAIN(TestCentral)

#include "testcentral.moc"
