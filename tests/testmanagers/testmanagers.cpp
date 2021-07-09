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
#include "core/usersectionrodcomponent.h"
#include "core/materialrodcomponent.h"
#include "core/loadrodcomponent.h"
#include "core/constraintrodcomponent.h"
#include "managers/managersfactory.h"
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
    ManagersFactory* mpManagersFactory;
    QString mLastPath;
};

//! Init
void TestManagers::initTestCase()
{
    mpProject = new Project("Test");
    mpSettings = new QSettings("Settings.ini", QSettings::IniFormat);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    qApp->setStyle("Fusion");
    qApp->setStyleSheet(File::loadFileContent(":/styles/modern.qss"));
    QFontDatabase::addApplicationFont(":/fonts/SourceSansPro-Regular.ttf");
    uint fontSize = 12;
#ifdef Q_OS_WIN
    fontSize = 10;
#endif
    qApp->setFont(QFont("Source Sans Pro", fontSize));
    mpManagersFactory = new ManagersFactory(*mpProject, mLastPath, *mpSettings, nullptr);
}

//! Test how the data objects manager handles with data
void TestManagers::testDataObjectsManager()
{
    // Creating a manager
    mpManagersFactory->createManager(AbstractManager::ManagerType::kDataObjects);
    DataObjectsManager* pManager = (DataObjectsManager*)mpManagersFactory->manager(AbstractManager::ManagerType::kDataObjects);
    // Creating data objects of different types
    ScalarDataObject* pScalar = (ScalarDataObject*)pManager->addScalar();
    VectorDataObject* pVector = (VectorDataObject*)pManager->addVector();
    MatrixDataObject* pMatrix = (MatrixDataObject*)pManager->addMatrix();
    SurfaceDataObject* pSurface = (SurfaceDataObject*)pManager->addSurface();
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
    pManager->selectDataObject(3);
    pManager->apply();
    mpManagersFactory->deleteManager(AbstractManager::ManagerType::kDataObjects);
}

//! Test how to create components of a rod
void TestManagers::testRodComponentsManager()
{
    // Creating sample objects
    ScalarDataObject* pScalar = (ScalarDataObject*)mpProject->addDataObject(AbstractDataObject::kScalar);
    VectorDataObject* pVector = (VectorDataObject*)mpProject->addDataObject(AbstractDataObject::kVector);
    MatrixDataObject* pMatrix = (MatrixDataObject*)mpProject->addDataObject(AbstractDataObject::kMatrix);
    // Creating a manager
    mpManagersFactory->createManager(AbstractManager::ManagerType::kRodComponents);
    RodComponentsManager* pManager = (RodComponentsManager*)mpManagersFactory->manager(AbstractManager::ManagerType::kRodComponents);
    // Adding a geometrical component
    GeometryRodComponent* pGeometry = (GeometryRodComponent*)pManager->addGeometry();
    pGeometry->setRadiusVector(pVector);
    pGeometry->setRotationMatrix(pMatrix);
    QVERIFY(pGeometry->isDataComplete());
    // Adding a user-defined cross section
    UserSectionRodComponent* pSection;
    pSection = (UserSectionRodComponent*)pManager->addSection(AbstractSectionRodComponent::kUserDefined);
    pSection->setArea(pScalar);
    QVERIFY(pSection->isDataComplete());
    // Adding a material rod component
    MaterialRodComponent* pMaterial = (MaterialRodComponent*)pManager->addMaterial();
    pMaterial->setElasticModulus(pScalar);
    pMaterial->setShearModulus(pScalar);
    pMaterial->setDensity(pScalar);
    pMaterial->setPoissonsRatio(pScalar);
    QVERIFY(pMaterial->isDataComplete());
    // Adding a rod load
    LoadRodComponent* pLoad = (LoadRodComponent*)pManager->addLoad();
    pLoad->setType(LoadRodComponent::LoadType::kPointForce);
    pLoad->setDirectionVector(pVector);
    QVERIFY(pLoad->isDataComplete());
    // Adding a rod constraint
    ConstraintRodComponent* pConstraint = (ConstraintRodComponent*)pManager->addConstraint();
    pConstraint->setConstraint(ConstraintRodComponent::kDisplacementX, ConstraintRodComponent::kLocal);
    pConstraint->setConstraint(ConstraintRodComponent::kDisplacementY, ConstraintRodComponent::kGlobal);
    QVERIFY(pConstraint->isDataComplete());
    // Selecting
    pManager->selectRodComponent(4);
    pManager->apply();
}

//! Cleanup
void TestManagers::cleanupTestCase()
{
    qApp->exec();
    delete mpProject;
    delete mpSettings;
    delete mpManagersFactory;
}

QTEST_MAIN(TestManagers)

#include "testmanagers.moc"
