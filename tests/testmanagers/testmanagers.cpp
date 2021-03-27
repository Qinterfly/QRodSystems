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

//! Test how the data objects manager handles with data
void TestManagers::testDataObjectsManager()
{
    mpDataObjectsManager->show();
    // Adding a scalar object
    mpDataObjectsManager->addScalar();
    auto dataObjects = mpDataObjectsManager->getDataObjects();
    ScalarDataObject* pScalar = (ScalarDataObject*) dataObjects.begin()->second;
    // Inserting itmes into the object
    int nStep = 10;
    double startValue = 0.1;
    double endValue = 1.5;
    double step = (endValue - startValue) / (double) (nStep - 1);
    for (int i = 0; i != nStep; ++i)
    {
        auto& t = pScalar->addItem(startValue + i * step);
        t[0][0] = startValue / endValue * (i + 1);
    }
    pScalar->addItem(endValue); // Already existed key
    // Selecting it
    mpDataObjectsManager->selectDataObject(0);
    QTest::qWait(10000);
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
