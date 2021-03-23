#include <QtTest>
#include <QCoreApplication>

#include <QtWidgets>

#include "core/project.h"
#include "core/abstractdataobject.h"
#include "core/scalardataobject.h"

#include "managers/dataobjectsmanager.h"

using namespace QRS;

//! Test managers while creating data objects and modifying them
class TestManagers : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testDataObjectsManager();
    void cleanupTestCase();

private:
    Project* mpProject;
    QSettings* mpSettings;
    DataObjectsManager* mpDataObjectsManager;
};

//![0]
void TestManagers::initTestCase()
{
    mpProject = new Project("Test");
    mpSettings = new QSettings("Settings.ini", QSettings::IniFormat);
    mpDataObjectsManager = new DataObjectsManager(*mpProject, *mpSettings);
}

//![1]
void TestManagers::testDataObjectsManager()
{
    mpDataObjectsManager->show();
    mpDataObjectsManager->addScalar();
    mpDataObjectsManager->selectDataObject(0);
    QTest::qWait(5000);
}

//![2]
void TestManagers::cleanupTestCase()
{
    delete mpProject;
    delete mpSettings;
    delete mpDataObjectsManager;
}

QTEST_MAIN(TestManagers)

#include "testmanagers.moc"
