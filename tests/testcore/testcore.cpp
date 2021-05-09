/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
 * \brief Testing of the core functionality
 */

#include <QtTest/QTest>

#include "array.h"
#include "project.h"
#include "scalardataobject.h"
#include "hierarchytree.h"

using namespace QRS;

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
    const uint numObjects = 5;
    mpProject = new Project("test");
    for (uint i = 0; i != numObjects; ++i)
    {
        mpProject->addDataObject(kScalar);
        mpProject->addDataObject(kVector);
        mpProject->addDataObject(kMatrix);
        mpProject->addDataObject(kSurface);
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
    QCOMPARE(matrix.cols(), uint(0));
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
}

//! Try creating a hierarchy of data objects
void TestCore::createHierarchyTree()
{
    HierarchyTree hierarchy;
    HierarchyNode& rootNode = hierarchy.root();
    // 0 - level
    HierarchyNode* pFolderNode1 = new HierarchyNode(HierarchyNode::NodeType::kDirectory, "Folder 1");
    HierarchyNode* pFolderNode2 = new HierarchyNode(HierarchyNode::NodeType::kDirectory, "Folder 2");
    HierarchyNode* pFolderNode3 = new HierarchyNode(HierarchyNode::NodeType::kDirectory, "Folder 3");
    // 1 - level
    pFolderNode2->appendChild(new HierarchyNode(HierarchyNode::NodeType::kObject, "Object 1"));
    HierarchyNode* pFolderNode4 = new HierarchyNode(HierarchyNode::NodeType::kDirectory, "Folder 4");
    // 2 - level
    pFolderNode4->appendChild(new HierarchyNode(HierarchyNode::NodeType::kObject, "Object 2"));
    // Appending all the leafs
    pFolderNode2->appendChild(pFolderNode4);
    rootNode.appendChild(pFolderNode1);
    rootNode.appendChild(pFolderNode2);
    rootNode.appendChild(pFolderNode3);
    QCOMPARE(hierarchy.size(), 7);
    // Make a duplicate
    HierarchyTree duplicateHierarchy = hierarchy.clone();
    QCOMPARE(duplicateHierarchy.size(), 7);
    // Print the hierarchy structure
    qDebug().noquote() << hierarchy;
    hierarchy.removeNode(HierarchyNode::NodeType::kDirectory, "Folder 1");
    hierarchy.removeNode(HierarchyNode::NodeType::kDirectory, "Folder 2");
    hierarchy.changeNodeValue(HierarchyNode::NodeType::kDirectory, "Folder 3", "Folder 1");
    qDebug().noquote() << hierarchy;
    qDebug().noquote() << duplicateHierarchy;
}

//! Cleanup
void TestCore::cleanupTestCase()
{
    delete mpProject;
}


QTEST_APPLESS_MAIN(TestCore)

#include "testcore.moc"
