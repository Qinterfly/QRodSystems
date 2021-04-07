/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Testing of managers used to create data objects
 */

#include <QtTest>
#include <QCoreApplication>

#include <QtWidgets>

#include "core/project.h"
#include "core/abstractdataobject.h"
#include "core/scalardataobject.h"
#include "core/vectordataobject.h"
#include "core/matrixdataobject.h"
#include "core/surfacedataobject.h"

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
    QString mLastPath;
};

//! Init
void TestManagers::initTestCase()
{
    mpProject = new Project("Test");
    mpSettings = new QSettings("Settings.ini", QSettings::IniFormat);
    mpDataObjectsManager = new DataObjectsManager(*mpProject, *mpSettings, mLastPath);
}

//! Test how the data objects manager handles with data
void TestManagers::testDataObjectsManager()
{
    mpDataObjectsManager->show();
    // Creating data objects of different types
    mpDataObjectsManager->addScalar();
    mpDataObjectsManager->addVector();
    mpDataObjectsManager->addMatrix();
    mpDataObjectsManager->addSurface();
    auto dataObjects = mpDataObjectsManager->getDataObjects();
    ScalarDataObject* pScalar;
    VectorDataObject* pVector;
    MatrixDataObject* pMatrix;
    SurfaceDataObject* pSurface;
    for (auto& mapObj : dataObjects)
    {
        AbstractDataObject* obj = mapObj.second;
        switch (obj->type())
        {
        case (kScalar):
            pScalar = (ScalarDataObject*)obj;
            break;
        case (kVector):
            pVector = (VectorDataObject*)obj;
            break;
        case (kMatrix):
            pMatrix = (MatrixDataObject*)obj;
            break;
        case (kSurface):
            pSurface = (SurfaceDataObject*)obj;
            break;
        }
    }
    // Inserting itmes into the object
    int nStep = 10;
    double startValue = 0.1;
    double endValue = 1.5;
    double step = (endValue - startValue) / (double) (nStep - 1);
    double tempValue;
    for (int i = 0; i != nStep; ++i)
    {
        tempValue = startValue + i * step;
        // Scalar
        auto& t = pScalar->addItem(tempValue);
        t[0][0] = startValue / endValue * (i + 1);
        // Others
        pVector->addItem(tempValue);
        pMatrix->addItem(tempValue);
        pSurface->addItem(tempValue);
    }
    pScalar->addItem(endValue); // Already existed key
    // Selecting
    mpDataObjectsManager->selectDataObject(3);
    QTest::qWait(1000);
}

//! Cleanup
void TestManagers::cleanupTestCase()
{
    delete mpProject;
    delete mpSettings;
    delete mpDataObjectsManager;
}

QTEST_MAIN(TestManagers)

#include "testmanagers.moc"
