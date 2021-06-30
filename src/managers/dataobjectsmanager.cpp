/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Implementation of the DataObjectsManager class
 */

#include <QTreeView>
#include <QSettings>
#include <QHBoxLayout>
#include <QToolBar>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QShortcut>
#include <QFileDialog>
#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include "dataobjectsmanager.h"
#include "central/uiconstants.h"
#include "core/scalardataobject.h"
#include "core/vectordataobject.h"
#include "core/matrixdataobject.h"
#include "core/surfacedataobject.h"
#include "core/utilities.h"
#include "models/table/basetablemodel.h"
#include "models/table/matrixtablemodel.h"
#include "models/table/surfacetablemodel.h"
#include "models/hierarchy/dataobjectshierarchymodel.h"
#include "doublespinboxitemdelegate.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::CDockAreaWidget;
using namespace QRS::Core;
using namespace QRS::Managers;
using namespace QRS::HierarchyModels;
using namespace QRS::TableModels;

const static QSize skToolBarIconSize = QSize(22, 22);

void setToolBarShortcutHints(QToolBar* pToolBar);
QIcon getDataObjectIcon(AbstractDataObject::ObjectType type);

DataObjectsManager::DataObjectsManager(DataObjects&& dataObjects, HierarchyTree&& hierarchyDataObjects,
                                       QString& lastPath, QSettings& settings, QWidget* parent)
    : AbstractProjectManager(lastPath, settings, kDataObjects, "DataObjectsManager", parent)
    , mDataObjects(std::move(dataObjects))
    , mHierarchyDataObjects(std::move(hierarchyDataObjects))
{
    setWindowTitle("Data Objects Manager[*]");
    setGeometry(0, 0, 700, 700);
    setWindowModified(false);
    createContent();
    restoreSettings();
    mpTreeDataObjectsModel->updateContent();
}

DataObjectsManager::~DataObjectsManager()
{
    delete mpItemDelegate;
    for (auto iter = mDataObjects.begin(); iter != mDataObjects.end(); ++iter)
        delete iter->second;
    mDataObjects.clear();
}

//! Create all the widgets
void DataObjectsManager::createContent()
{
    // Main layout
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
    pMainLayout->addWidget(mpDockManager);
    // Tables
    mpDockManager->addDockWidget(ads::LeftDockWidgetArea, createDataTableWidget());
    // Objects
    mpDockManager->addDockWidget(ads::RightDockWidgetArea, createHierarchyWidget());
    // Buttons
    pMainLayout->addLayout(createDialogControls());
}

//! Create a tabbed widget to interact with data tables
CDockWidget* DataObjectsManager::createDataTableWidget()
{
    CDockWidget* pDockWidget = new CDockWidget("Data Table");
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    mpDataTable = new QTreeView();
    mpDataTable->sortByColumn(-1, Qt::AscendingOrder);
    mpDataTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mpDataTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    mpDataTable->setHeaderHidden(false);
    pDockWidget->setWidget(mpDataTable);
    // Editor of table values
    mpItemDelegate = new DoubleSpinBoxItemDelegate();
    mpDataTable->setItemDelegate(mpItemDelegate);
    // Models
    mpBaseTableModel = new BaseTableModel(mpDataTable);
    mpMatrixTableModel = new MatrixTableModel(mpDataTable);
    mpSurfaceTableModel = new SurfaceTableModel(mpDataTable);
    // ToolBar
    QToolBar* pToolBar = pDockWidget->createDefaultToolBar();
    pDockWidget->setToolBarIconSize(skToolBarIconSize, CDockWidget::StateDocked);
    QAction* pAction;
    // Expanding actions
    pAction = pToolBar->addAction(QIcon(":/icons/arrows-expand.svg"), tr("Expand"), mpDataTable, &QTreeView::expandAll);
    pAction->setShortcut(Qt::Key_E);
    pAction = pToolBar->addAction(QIcon(":/icons/arrows-collapse.svg"), tr("Collapse"), mpDataTable, &QTreeView::collapseAll);
    pAction->setShortcut(Qt::Key_C);
    pToolBar->addSeparator();
    // Rows actions
    pAction = pToolBar->addAction(QIcon(":/icons/table-row-add.svg"), tr("Add Row"), this, &DataObjectsManager::insertItemAfterSelected);
    pAction->setShortcut(Qt::Key_A);
    pAction = pToolBar->addAction(QIcon(":/icons/table-row-delete.svg"), tr("Remove Row"), this, &DataObjectsManager::removeSelectedItem);
    pAction->setShortcut(Qt::Key_D);
    pToolBar->addSeparator();
    // Columns actions
    pAction = pToolBar->addAction(QIcon(":/icons/table-column-add.svg"), tr("Add Column"),
                                  this, &DataObjectsManager::insertLeadingItemAfterSelected);
    pAction->setShortcut(QKeySequence("Ctrl+A"));
    pAction = pToolBar->addAction(QIcon(":/icons/table-column-delete.svg"), tr("Remove Column"),
                                  this, &DataObjectsManager::removeSelectedLeadingItem);
    pAction->setShortcut(QKeySequence("Ctrl+D"));
    // Import action
    pToolBar->addSeparator();
    pToolBar->addAction(QIcon(":/icons/link-import.svg"), tr("Import"), this, &DataObjectsManager::importDataObjects);
    setToolBarShortcutHints(pToolBar);
    return pDockWidget;
}

//! Create an object to represent a hierarchy of data objects
CDockWidget* DataObjectsManager::createHierarchyWidget()
{
    QSize const kIconSize = QSize(16, 16);
    CDockWidget* pDockWidget = new CDockWidget("Objects");
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    // Tree widget
    mpTreeDataObjects = new QTreeView();
    mpTreeDataObjects->setIconSize(kIconSize);
    mpTreeDataObjects->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mpTreeDataObjects->setSelectionBehavior(QAbstractItemView::SelectItems);
    mpTreeDataObjects->setEditTriggers(QAbstractItemView::DoubleClicked);
    mpTreeDataObjects->setHeaderHidden(true);
    mpTreeDataObjects->setAcceptDrops(true);
    mpTreeDataObjects->setDragEnabled(true);
    pDockWidget->setWidget(mpTreeDataObjects);
    // Hierarchy model
    mpTreeDataObjectsModel = new DataObjectsHierarchyModel(mDataObjects, mHierarchyDataObjects, mpTreeDataObjects);
    mpTreeDataObjects->setModel(mpTreeDataObjectsModel);
    connect(mpTreeDataObjectsModel, &DataObjectsHierarchyModel::dataModified,
            this, &DataObjectsManager::setWindowModified);
    connect(mpTreeDataObjectsModel, &DataObjectsHierarchyModel::selected,
            this, &DataObjectsManager::representDataObject);
    connect(mpTreeDataObjects->selectionModel(), &QItemSelectionModel::selectionChanged,
            mpTreeDataObjectsModel, &DataObjectsHierarchyModel::retrieveSelectedItem);
    connect(mpTreeDataObjectsModel, &DataObjectsHierarchyModel::selectionCleared,
            this, &DataObjectsManager::clearDataObjectRepresentation);
    // ToolBar
    QToolBar* pToolBar = pDockWidget->createDefaultToolBar();
    pDockWidget->setToolBarIconSize(skToolBarIconSize, CDockWidget::StateDocked);
    // Actions
    QAction* pAction;
    pAction = pToolBar->addAction(QIcon(":/icons/letter-s.svg"), tr("Scalar"), this, &DataObjectsManager::addScalar);
    pAction->setShortcut(QKeySequence("Ctrl+1"));
    pAction = pToolBar->addAction(QIcon(":/icons/letter-v.svg"), tr("Vector"), this, &DataObjectsManager::addVector);
    pAction->setShortcut(QKeySequence("Ctrl+2"));
    pAction = pToolBar->addAction(QIcon(":/icons/letter-m.svg"), tr("Matrix"), this, &DataObjectsManager::addMatrix);
    pAction->setShortcut(QKeySequence("Ctrl+3"));
    pAction = pToolBar->addAction(QIcon(":/icons/letter-xy.svg"), tr("Surface"), this, &DataObjectsManager::addSurface);
    pAction->setShortcut(QKeySequence("Ctrl+4"));
    pToolBar->addSeparator();
    pAction = pToolBar->addAction(QIcon(":/icons/delete.svg"), tr("Remove"),
                                  mpTreeDataObjectsModel, &DataObjectsHierarchyModel::removeSelectedItems);
    pAction->setShortcut(Qt::Key_R);
    setToolBarShortcutHints(pToolBar);
    return pDockWidget;
}

//! Create dialog controls
QLayout* DataObjectsManager::createDialogControls()
{
    QHBoxLayout* pLayout = new QHBoxLayout();
    pLayout->setContentsMargins(0, 0, 3, 5);
    QPushButton* pAcceptButton = new QPushButton(QIcon(":/icons/edit-ok.svg"), tr("Apply"));
    connect(pAcceptButton, &QPushButton::clicked, this, &DataObjectsManager::apply);
    pLayout->addStretch();
    pLayout->addWidget(pAcceptButton);
    return pLayout;
}

//! Apply all the changes made by user
void DataObjectsManager::apply()
{
    emit dataObjectsModified(mDataObjects, mHierarchyDataObjects);
    setWindowModified(false);
    qInfo() << tr("Data objects were modified through the manager");
}

//! Add a scalar object
AbstractDataObject* DataObjectsManager::addScalar()
{
    static QString const kScalarName = "Scalar ";
    QString name = kScalarName + QString::number(ScalarDataObject::numberInstances() + 1);
    AbstractDataObject* pObject = new ScalarDataObject(name);
    emplaceDataObject(pObject);
    return pObject;
}

//! Add a vector object
AbstractDataObject* DataObjectsManager::addVector()
{
    static QString const kVectorName = "Vector ";
    QString name = kVectorName + QString::number(VectorDataObject::numberInstances() + 1);
    AbstractDataObject* pObject = new VectorDataObject(name);
    emplaceDataObject(pObject);
    return pObject;
}

//! Add a matrix object
AbstractDataObject* DataObjectsManager::addMatrix()
{
    static QString const kMatrixName = "Matrix ";
    QString name = kMatrixName + QString::number(MatrixDataObject::numberInstances() + 1);
    AbstractDataObject* pObject = new MatrixDataObject(name);
    emplaceDataObject(pObject);
    return pObject;
}

//! Add a surface object
AbstractDataObject* DataObjectsManager::addSurface()
{
    static QString const kSurfaceName = "Surface ";
    QString name = kSurfaceName + QString::number(SurfaceDataObject::numberInstances() + 1);
    AbstractDataObject* pObject = new SurfaceDataObject(name);
    emplaceDataObject(pObject);
    return pObject;
}

//! Select a data object by row index
void DataObjectsManager::selectDataObject(int iRow)
{
    mpTreeDataObjectsModel->selectItem(iRow);
}

//! Represent a selected data object according to its type
void DataObjectsManager::representDataObject(DataIDType id)
{
    clearDataObjectRepresentation();
    if (!mDataObjects.contains(id))
        return;
    AbstractDataObject* pObject = mDataObjects[id];
    switch (pObject->type())
    {
    case AbstractDataObject::ObjectType::kScalar:
    case AbstractDataObject::ObjectType::kVector:
        mpDataTable->setSortingEnabled(false);
        mpBaseTableModel->setDataObject(pObject);
        mpDataTable->setSortingEnabled(true);
        mpDataTable->setModel(mpBaseTableModel);
        mpTableModelInterface = mpBaseTableModel;
        break;
    case AbstractDataObject::ObjectType::kMatrix:
        mpDataTable->setSortingEnabled(false);
        mpMatrixTableModel->setDataObject(pObject);
        mpDataTable->setSortingEnabled(true);
        mpDataTable->setModel(mpMatrixTableModel);
        mpTableModelInterface = mpMatrixTableModel;
        mpDataTable->expandAll();
        break;
    case AbstractDataObject::ObjectType::kSurface:
        mpDataTable->setSortingEnabled(false);
        mpSurfaceTableModel->setDataObject((SurfaceDataObject*)pObject);
        mpDataTable->setModel(mpSurfaceTableModel);
        mpTableModelInterface = mpSurfaceTableModel;
        break;
    }
}

//! Clear a visual data of a data object
void DataObjectsManager::clearDataObjectRepresentation()
{
    mpTableModelInterface = nullptr;
    mpBaseTableModel->setDataObject(nullptr);
    mpMatrixTableModel->setDataObject(nullptr);
    mpSurfaceTableModel->setDataObject(nullptr);
}

//! Insert a new array into the data object
void DataObjectsManager::insertItemAfterSelected()
{
    if (isDataTableModifiable())
    {
        mpTableModelInterface->insertItemAfterSelected(mpDataTable->selectionModel());
        setWindowModified(true);
    }
}

//! Insert a new leading item into the data object
void DataObjectsManager::insertLeadingItemAfterSelected()
{
    if (isDataTableModifiable())
    {
        mpTableModelInterface->insertLeadingItemAfterSelected(mpDataTable->selectionModel());
        setWindowModified(true);
    }
}

//! Remove a selected item
void DataObjectsManager::removeSelectedItem()
{
    if (isDataTableModifiable())
    {
        mpTableModelInterface->removeSelectedItem(mpDataTable->selectionModel());
        setWindowModified(true);
    }
}

//! Remove a selected leading item
void DataObjectsManager::removeSelectedLeadingItem()
{
    if (isDataTableModifiable())
    {
        mpTableModelInterface->removeSelectedLeadingItem(mpDataTable->selectionModel());
        setWindowModified(true);
    }
}

//! Import data objects from a file
void DataObjectsManager::importDataObjects()
{
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Select one or more files to import",
                                                      mLastPath,
                                                      "Data files (*.prn)");
    if (files.isEmpty())
        return;
    for (auto& filePath : files)
    {
        QFileInfo info(filePath);
        importDataObject(info.path(), info.fileName());
    }
    mLastPath = QFileInfo(files[0]).path();
}

//! Helper function to insert data objects into the manager
void DataObjectsManager::emplaceDataObject(AbstractDataObject* pDataObject)
{
    DataIDType id = pDataObject->id();
    mDataObjects.emplace(id, pDataObject);
    mHierarchyDataObjects.appendNode(new HierarchyNode(HierarchyNode::NodeType::kObject, id));
    mpTreeDataObjectsModel->updateContent();
    setWindowModified(true);
}

//! Import a data object from a file
void DataObjectsManager::importDataObject(QString const& path, QString const& fileName)
{
    auto [type, pFile] = Utilities::File::getDataObjectFile(path, fileName);
    if (pFile == nullptr)
        return;
    QTextStream stream(pFile.data());
    quint32 numDataObjects;
    stream.readLine();
    stream >> numDataObjects;
    stream.readLine();
    for (quint32 iDataObject = 0; iDataObject != numDataObjects; ++iDataObject)
    {
        AbstractDataObject* pDataObject = nullptr;
        switch (type)
        {
        case AbstractDataObject::ObjectType::kScalar:
            pDataObject = addScalar();
            break;
        case AbstractDataObject::ObjectType::kVector:
            pDataObject = addVector();
            break;
        case AbstractDataObject::ObjectType::kMatrix:
            pDataObject = addMatrix();
            break;
        case AbstractDataObject::ObjectType::kSurface:
            pDataObject = addSurface();
            break;
        }
        if (pDataObject)
            pDataObject->import(stream);
    }
    pFile->close();
}

//! Helper function to check if it is possible to interact with data object content
bool DataObjectsManager::isDataTableModifiable()
{
    return mpTableModelInterface;
}
