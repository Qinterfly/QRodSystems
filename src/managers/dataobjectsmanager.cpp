/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
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
#include <QMessageBox>
#include <QShortcut>
#include <QFileDialog>
#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include "dataobjectsmanager.h"
#include "ui_dataobjectsmanager.h"
#include "central/uiconstants.h"
#include "core/project.h"
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

const static QSize kToolBarIconSize = QSize(22, 22);
const QString skDataObjectsWindow = "DataObjectsManager";

void setToolBarShortcutHints(QToolBar* pToolBar);
QIcon getDataObjectIcon(DataObjectType type);

DataObjectsManager::DataObjectsManager(Project& project, QSettings& settings, QString& lastPath, QWidget* parent)
    : QDialog(parent)
    , mpUi(new Ui::DataObjectsManager)
    , mProject(project)
    , mSettings(settings)
    , mLastPath(lastPath)
{
    mpUi->setupUi(this);
    setWindowModified(false);
    createContent();
    restoreSettings();
    retrieveDataObjects();
}

DataObjectsManager::~DataObjectsManager()
{
    delete mpItemDelegate;
    delete mpUi;
    for (auto iter = mDataObjects.begin(); iter != mDataObjects.end(); ++iter)
        delete iter->second;
    mDataObjects.clear();
}

//! Save settings and delete handling widgets before closing the window
void DataObjectsManager::closeEvent(QCloseEvent* pEvent)
{
    pEvent->ignore();
    bool isClosed = false;
    if (isWindowModified())
    {
        auto dialogResult = QMessageBox::question(this
                            , tr("Close confirmation")
                            , tr("Data objects manager contains unsaved changes. Would you like to close it anyway?")
                            , QMessageBox::Yes | QMessageBox::No);
        isClosed = QMessageBox::Yes == dialogResult;
    }
    else
    {
        isClosed = true;
    }
    if (isClosed)
    {
        saveSettings();
        emit closed();
        pEvent->accept();
    }
}

//! Create all the widgets
void DataObjectsManager::createContent()
{
    // Dock manager
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
    mpDockManager = new CDockManager();
    mpDockManager->setStyleSheet("");
    pMainLayout->addWidget(mpDockManager);
    // Tables
    mpDockManager->addDockWidget(ads::LeftDockWidgetArea, createDataTableWidget());
    // Objects
    mpDockManager->addDockWidget(ads::RightDockWidgetArea, createDataObjectsWidget());
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
    pToolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    pDockWidget->setToolBarIconSize(kToolBarIconSize, CDockWidget::StateDocked);
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
    pAction = pToolBar->addAction(QIcon(":/icons/table-column-add.svg"), tr("Add Column"), this, &DataObjectsManager::insertLeadingItemAfterSelected);
    pAction->setShortcut(QKeySequence("Ctrl+A"));
    pAction = pToolBar->addAction(QIcon(":/icons/table-column-delete.svg"), tr("Remove Column"), this, &DataObjectsManager::removeSelectedLeadingItem);
    pAction->setShortcut(QKeySequence("Ctrl+D"));
    // Import action
    pToolBar->addSeparator();
    pToolBar->addAction(QIcon(":/icons/link-import.svg"), tr("Import"), this, &DataObjectsManager::importDataObjects);
    setToolBarShortcutHints(pToolBar);
    return pDockWidget;
}

//! Create an object to present all data objects
CDockWidget* DataObjectsManager::createDataObjectsWidget()
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
    connect(mpTreeDataObjectsModel, &DataObjectsHierarchyModel::dataModified, this, &DataObjectsManager::setWindowModified);
    connect(mpTreeDataObjectsModel, &DataObjectsHierarchyModel::dataObjectSelected, this, &DataObjectsManager::representDataObject);
    connect(mpTreeDataObjects->selectionModel(), &QItemSelectionModel::selectionChanged, mpTreeDataObjectsModel, &DataObjectsHierarchyModel::retrieveSelectedDataObject);
    connect(mpTreeDataObjectsModel, &DataObjectsHierarchyModel::selectionCleared, this, &DataObjectsManager::clearDataObjectRepresentation);
    // ToolBar
    QToolBar* pToolBar = pDockWidget->createDefaultToolBar();
    pToolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    pDockWidget->setToolBarIconSize(kToolBarIconSize, CDockWidget::StateDocked);
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
    pAction = pToolBar->addAction(QIcon(":/icons/delete.svg"), tr("Remove"), mpTreeDataObjectsModel, &DataObjectsHierarchyModel::removeSelectedItems);
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

//! Save settings to a file
void DataObjectsManager::saveSettings()
{
    mSettings.beginGroup(skDataObjectsWindow);
    mSettings.setValue(UiConstants::Settings::skGeometry, saveGeometry());
    mSettings.setValue(UiConstants::Settings::skDockingState, mpDockManager->saveState());
    mSettings.endGroup();
}

//! Restore settings from a file
void DataObjectsManager::restoreSettings()
{
    mSettings.beginGroup(skDataObjectsWindow);
    restoreGeometry(mSettings.value(UiConstants::Settings::skGeometry).toByteArray());
    mpDockManager->restoreState(mSettings.value(UiConstants::Settings::skDockingState).toByteArray());
    mSettings.endGroup();
}

//! Apply all the changes made by user
void DataObjectsManager::apply()
{
    mProject.setDataObjects(mDataObjects, mHierarchyDataObjects);
    setWindowModified(false);
    qInfo() << tr("Data objects were modified through the data object manager");
}

//! Make a copy of existed data objects
void DataObjectsManager::retrieveDataObjects()
{
    mDataObjects = mProject.cloneDataObjects();
    mHierarchyDataObjects = mProject.cloneHierarchyDataObjects();
    mpTreeDataObjectsModel->updateContent();
}

//! Add a scalar object
DataIDType DataObjectsManager::addScalar()
{
    static QString const kScalarName = "Scalar ";
    QString name = kScalarName + QString::number(ScalarDataObject::numberInstances() + 1);
    AbstractDataObject* pObject = new ScalarDataObject(name);
    emplaceDataObject(pObject);
    return pObject->id();
}

//! Add a vector object
DataIDType DataObjectsManager::addVector()
{
    static QString const kVectorName = "Vector ";
    QString name = kVectorName + QString::number(VectorDataObject::numberInstances() + 1);
    AbstractDataObject* pObject = new VectorDataObject(name);
    emplaceDataObject(pObject);
    return pObject->id();
}

//! Add a matrix object
DataIDType DataObjectsManager::addMatrix()
{
    static QString const kMatrixName = "Matrix ";
    QString name = kMatrixName + QString::number(MatrixDataObject::numberInstances() + 1);
    AbstractDataObject* pObject = new MatrixDataObject(name);
    emplaceDataObject(pObject);
    return pObject->id();
}

//! Add a surface object
DataIDType DataObjectsManager::addSurface()
{
    static QString const kSurfaceName = "Surface ";
    QString name = kSurfaceName + QString::number(SurfaceDataObject::numberInstances() + 1);
    AbstractDataObject* pObject = new SurfaceDataObject(name);
    emplaceDataObject(pObject);
    return pObject->id();
}

//! Select a data object by row index
void DataObjectsManager::selectDataObject(int iRow)
{
    mpTreeDataObjectsModel->selectItem(iRow);
}

//! Clear a visual data of a data object
void DataObjectsManager::clearDataObjectRepresentation()
{
    mpTableModelInterface = nullptr;
    mpBaseTableModel->setDataObject(nullptr);
    mpMatrixTableModel->setDataObject(nullptr);
    mpSurfaceTableModel->setDataObject(nullptr);
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
    case kScalar:
    case kVector:
        mpDataTable->setSortingEnabled(false);
        mpBaseTableModel->setDataObject(pObject);
        mpDataTable->setSortingEnabled(true);
        mpDataTable->setModel(mpBaseTableModel);
        mpTableModelInterface = mpBaseTableModel;
        break;
    case kMatrix:
        mpDataTable->setSortingEnabled(false);
        mpMatrixTableModel->setDataObject(pObject);
        mpDataTable->setSortingEnabled(true);
        mpDataTable->setModel(mpMatrixTableModel);
        mpTableModelInterface = mpMatrixTableModel;
        mpDataTable->expandAll();
        break;
    case kSurface:
        mpDataTable->setSortingEnabled(false);
        mpSurfaceTableModel->setDataObject((SurfaceDataObject*)pObject);
        mpDataTable->setModel(mpSurfaceTableModel);
        mpTableModelInterface = mpSurfaceTableModel;
        break;
    }
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
    DataIDType id;
    for (quint32 iDataObject = 0; iDataObject != numDataObjects; ++iDataObject)
    {
        switch (type)
        {
        case kScalar:
            id = addScalar();
            break;
        case kVector:
            id = addVector();
            break;
        case kMatrix:
            id = addMatrix();
            break;
        case kSurface:
            id = addSurface();
            break;
        }
        AbstractDataObject* pDataObject = mDataObjects[id];
        pDataObject->import(stream);
    }
    pFile->close();
}

//! Helper function to check if it is possible to interact with data object content
bool DataObjectsManager::isDataTableModifiable()
{
    return mpTableModelInterface;
}

//! Helper function to add a shortcut hint to all actions which a toolbar contains
void setToolBarShortcutHints(QToolBar* pToolBar)
{
    QList<QAction*> listActions = pToolBar->actions();
    QString shortCut;
    for (auto& action : listActions)
    {
        shortCut = action->shortcut().toString();
        if (shortCut.isEmpty())
            action->setText(QString(action->text()));
        else
            action->setText(QString(action->text() + " (%1)").arg(shortCut));
    }
}
