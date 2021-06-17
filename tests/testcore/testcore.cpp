/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
 * \brief Testing of the core functionality
 */

#include <QtTest/QTest>

#include "core/array.h"
#include "core/project.h"
#include "core/scalardataobject.h"
#include "core/vectordataobject.h"
#include "core/matrixdataobject.h"
#include "core/hierarchytree.h"
#include "core/geometryrodcomponent.h"

using namespace QRS::Core;

class TestCore : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void createArray();
    void modifyArray();
    void importDataObjects();
    void saveProject();
    void readProject();
    void createHierarchyTree();
    void reorganizeHierarchyTree();
    void createRodGeometry();
    void cleanupTestCase();

private:
    Project* mpProject;
    const QString mBasePath = "../../../../";
    const QString mExamplesPath = mBasePath + "examples";
    const QString mImportPath = mBasePath + "preparation/Mathcad/dwds/Input";
};

//! Initialize data
void TestCore::initTestCase()
{
    mpProject = new Project("test");
    const quint32 numSets = 5;
    VectorDataObject* pVector;
    MatrixDataObject* pMatrix;
    GeometryRodComponent* pGeometry;
    for (quint32 i = 0; i != numSets; ++i)
    {
        // Creating data objects
        mpProject->addDataObject(AbstractDataObject::ObjectType::kScalar);
        pVector = (VectorDataObject*)mpProject->addDataObject(AbstractDataObject::ObjectType::kVector);
        pMatrix = (MatrixDataObject*)mpProject->addDataObject(AbstractDataObject::ObjectType::kMatrix);
        mpProject->addDataObject(AbstractDataObject::ObjectType::kSurface);
        // Creating rod components
        pGeometry = (GeometryRodComponent*)mpProject->addRodComponent(AbstractRodComponent::ComponentType::kGeometry);
        pGeometry->setRadiusVector(pVector);
        pGeometry->setRotationMatrix(pMatrix);
    }
}

//! Test how an array is created
void TestCore::createArray()
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
    QCOMPARE(matrix.cols(), quint32(0));
}

//! Test how an array object can be modified
void TestCore::modifyArray()
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

//! Try importing data objects
void TestCore::importDataObjects()
{
    Project tempProject("imported");
    tempProject.importDataObjects(mImportPath, "w1.prn");
    tempProject.importDataObjects(mImportPath, "w3.prn");
    tempProject.importDataObjects(mImportPath, "w9.prn");
    tempProject.importDataObjects(mImportPath, "xy.prn");
    QVERIFY(tempProject.save(mExamplesPath, tempProject.name()));
}

//! Try saving a project
void TestCore::saveProject()
{
    QVERIFY(mpProject->save(mExamplesPath, mpProject->name()));
}

//! Try reading a project
void TestCore::readProject()
{
    Project tempProject(mExamplesPath, mpProject->name());
    QCOMPARE(mpProject->name(), tempProject.name());
    QCOMPARE(mpProject->numberDataObjects(), tempProject.numberDataObjects());
    QCOMPARE(mpProject->numberRodComponents(), tempProject.numberRodComponents());
}

//! Try creating a hierarchial tree
void TestCore::createHierarchyTree()
{
    HierarchyTree hierarchy;
    HierarchyNode* rootNode = hierarchy.root();
    // 0 - level
    HierarchyNode* pFolderNode1 = new HierarchyNode(HierarchyNode::NodeType::kDirectory, "Folder 1");
    HierarchyNode* pFolderNode2 = new HierarchyNode(HierarchyNode::NodeType::kDirectory, "Folder 2");
    HierarchyNode* pFolderNode3 = new HierarchyNode(HierarchyNode::NodeType::kDirectory, "Folder 3");
    HierarchyNode* pFolderNode4 = new HierarchyNode(HierarchyNode::NodeType::kDirectory, "Folder 4");
    // 1 - level
    pFolderNode2->appendChild(new HierarchyNode(HierarchyNode::NodeType::kObject, "Object 1"));
    HierarchyNode* pFolderNode5 = new HierarchyNode(HierarchyNode::NodeType::kDirectory, "Folder 5");
    pFolderNode4->appendChild(new HierarchyNode(HierarchyNode::NodeType::kObject, "Object 2"));
    // 2 - level
    pFolderNode5->appendChild(new HierarchyNode(HierarchyNode::NodeType::kObject, "Object 3"));
    // Appending all the leafs
    pFolderNode2->appendChild(pFolderNode5);
    rootNode->appendChild(pFolderNode1);
    rootNode->appendChild(pFolderNode2);
    rootNode->appendChild(pFolderNode3);
    rootNode->appendChild(pFolderNode4);
    QCOMPARE(hierarchy.size(), quint32(9));
    // Make a duplicate
    HierarchyTree duplicateHierarchy = hierarchy.clone();
    QCOMPARE(duplicateHierarchy.size(), hierarchy.size());
    // Print the hierarchy structure
    qDebug().noquote() << hierarchy;
    // Modify the structure
    hierarchy.removeNode(HierarchyNode::NodeType::kDirectory, "Folder 1");
    hierarchy.removeNode(HierarchyNode::NodeType::kDirectory, "Folder 2");
    hierarchy.changeNodeValue(HierarchyNode::NodeType::kDirectory, "Folder 3", "Folder 1");
    hierarchy.changeNodeValue(HierarchyNode::NodeType::kDirectory, "Folder 4", "Folder 2");
    pFolderNode3->groupNodes(pFolderNode4);
    qDebug().noquote() << hierarchy;
}

//! Try reorganizing a hierarchial tree
void TestCore::reorganizeHierarchyTree()
{
    const quint32 kNodes = 6;
    const quint32 iStartMerge = 2;
    HierarchyTree hierarchy;
    HierarchyNode* pRootNode = hierarchy.root();
    QVector<HierarchyNode*> nodes;
    for (int i = 0; i != kNodes; ++i)
    {
        HierarchyNode* pNode = new HierarchyNode(HierarchyNode::NodeType::kObject, QString("Node %1").arg(i + 1));
        pRootNode->appendChild(pNode);
        nodes.push_back(pNode);
    }
    HierarchyNode* pGroup = nodes[iStartMerge]->groupNodes(nodes[iStartMerge + 1]);
    for (int i = iStartMerge + 1; i != kNodes - 1; ++i)
        pGroup->groupNodes(nodes[i + 1]);
    QVERIFY(pGroup->setAfter(nodes[2]));
    QVERIFY(pGroup->setBefore(nodes[2]));
    QVERIFY(nodes[1]->setAfter(nodes[2]));
    QVERIFY(nodes[0]->setBefore(nodes[2]));
    hierarchy.removeNode(pGroup);
    qDebug().noquote() << hierarchy;
}

//! Try creating a geometrical configuration of a rod
void TestCore::createRodGeometry()
{
    VectorDataObject* pRadiusVector = new VectorDataObject("Radius");
    MatrixDataObject* pRotationMatrix = new MatrixDataObject("Rotation");
    GeometryRodComponent pGeometry("Geometry");
    pGeometry.setRadiusVector(pRadiusVector);
    pGeometry.setRotationMatrix(pRotationMatrix);
    QVERIFY(pGeometry.isDataComplete());
    delete pRadiusVector;
    QVERIFY(!pGeometry.isDataComplete());
    delete pRotationMatrix;
}

//! Cleanup
void TestCore::cleanupTestCase()
{
    delete mpProject;
}

QTEST_APPLESS_MAIN(TestCore)

#include "testcore.moc"
