/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Testing of managers used to create data objects
 */

#include <QtTest>
#include <QCoreApplication>
#include <QtWidgets>
#include "core/project.h"
#include "core/utilities.h"
#include "core/scalardataobject.h"
#include "core/vectordataobject.h"
#include "core/matrixdataobject.h"
#include "core/surfacedataobject.h"
#include "core/geometryrodcomponent.h"
#include "managers/dataobjectsmanager.h"
#include "managers/rodcomponentsmanager.h"

using namespace QRS::Managers;
using namespace QRS::Core;
using namespace QRS::Utilities;

//! Test managers while creating data objects and modifying them
class TestManagers : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testDataObjectsManager();
    void testRodComponentsManager();
    void cleanupTestCase();

private:
    Project* mpProject;
    QSettings* mpSettings;
    DataObjectsManager* mpDataObjectsManager;
    RodComponentsManager* mpRodComponentsManager;
    QString mLastPath;
};

//! Init
void TestManagers::initTestCase()
{
    mpProject = new Project("Test");
    mpSettings = new QSettings("Settings.ini", QSettings::IniFormat);
    qApp->setStyle("Fusion");
    qApp->setStyleSheet(File::loadFileContent(":/styles/modern.qss"));
    QFontDatabase::addApplicationFont(":/fonts/SourceSansPro-Regular.ttf");
    uint fontSize = 12;
#ifdef Q_OS_WIN
    fontSize = 10;
#endif
    qApp->setFont(QFont("Source Sans Pro", fontSize));
    mpDataObjectsManager = new DataObjectsManager(*mpProject, mLastPath, *mpSettings);
    mpRodComponentsManager = new RodComponentsManager(*mpProject, mLastPath, *mpSettings);
}

//! Test how the data objects manager handles with data
void TestManagers::testDataObjectsManager()
{
    // Creating data objects of different types
    ScalarDataObject* pScalar = (ScalarDataObject*)mpDataObjectsManager->addScalar();
    VectorDataObject* pVector = (VectorDataObject*)mpDataObjectsManager->addVector();
    MatrixDataObject* pMatrix = (MatrixDataObject*)mpDataObjectsManager->addMatrix();
    SurfaceDataObject* pSurface = (SurfaceDataObject*)mpDataObjectsManager->addSurface();
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
    mpDataObjectsManager->apply();
    QCOMPARE(mpProject->getDataObjects().size(), uint(4));
}

//! Test how to create components of a rod
void TestManagers::testRodComponentsManager()
{
    // Create sample objects
    VectorDataObject* pVector = (VectorDataObject*)mpProject->addDataObject(AbstractDataObject::kVector);
    MatrixDataObject* pMatrix = (MatrixDataObject*)mpProject->addDataObject(AbstractDataObject::kMatrix);
    // Add a geometrical component
    GeometryRodComponent* pGeometry = (GeometryRodComponent*)mpRodComponentsManager->addGeometry();
    pGeometry->setRadiusVector(pVector);
    pGeometry->setRotationMatrix(pMatrix);
    QVERIFY(pGeometry->isDataComplete());
    // User interaction
    mpRodComponentsManager->show();
    qApp->exec();
}

//! Cleanup
void TestManagers::cleanupTestCase()
{
    delete mpProject;
    delete mpSettings;
    delete mpDataObjectsManager;
    delete mpRodComponentsManager;
}

QTEST_MAIN(TestManagers)

#include "testmanagers.moc"
