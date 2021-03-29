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
#include "DockManager.h"
#include "DockWidget.h"

#include "dataobjectsmanager.h"
#include "ui_dataobjectsmanager.h"
#include "../core/project.h"
#include "../core/scalardataobject.h"
#include "../core/vectordataobject.h"
#include "../core/matrixdataobject.h"
#include "../core/surfacedataobject.h"
#include "basetablemodel.h"
#include "matrixtablemodel.h"
#include "surfacetablemodel.h"
#include "doublespinboxitemdelegate.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::CDockAreaWidget;
using namespace QRS;

const static QSize kIconSize = QSize(22, 22);

DataObjectsManager::DataObjectsManager(QRS::Project& project, QSettings& settings, QWidget* parent)
    : QDialog(parent)
    , mpUi(new Ui::DataObjectsManager)
    , mProject(project)
    , mSettings(settings)
{
    mpUi->setupUi(this);
    setWindowModified(false);
    createContent();
    restoreSettings();
    retrieveDataObjects();
}

DataObjectsManager::~DataObjectsManager()
{
    delete mpUi;
    for (auto iter = mDataObjects.begin(); iter != mDataObjects.end(); ++iter)
        delete iter->second;
    mDataObjects.clear();
}

//! Save settings and delete handling widgets before closing the window
void DataObjectsManager::closeEvent(QCloseEvent* event)
{
    event->ignore();
    if (isWindowModified())
    {
        auto dialogResult = QMessageBox::question(this
                            , tr("Close confirmation")
                            , tr("Data objects manager contains unsaved changes. Would you like to close it anyway?")
                            , QMessageBox::Yes | QMessageBox::No);
        if (QMessageBox::Yes == dialogResult)
        {
            saveSettings();
            mpDockManager->deleteLater();
            event->accept();
        }
    }
    else
    {
        event->accept();
    }
}

//! Create all the widgets
void DataObjectsManager::createContent()
{
    // Dock manager
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
    mpDockManager = new CDockManager();
    pMainLayout->addWidget(mpDockManager);
    // Tables
    mpDockManager->addDockWidget(ads::LeftDockWidgetArea, createDataTableWidget());
    // Objects
    CDockAreaWidget* pArea = mpDockManager->addDockWidget(ads::RightDockWidgetArea, createDataObjectsWidget());
    // Code
    mpDockManager->addDockWidget(ads::BottomDockWidgetArea, createCodeWidget(), pArea);
    // Buttons
    pMainLayout->addLayout(createDialogControls());
}

//! Create a tabbed widget to interact with data tables
CDockWidget* DataObjectsManager::createDataTableWidget()
{
    CDockWidget* pDockWidget = new CDockWidget("Data Table");
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    mpDataTable = new QTreeView();
    mpDataTable->sortByColumn(0, Qt::SortOrder::AscendingOrder);
    mpDataTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mpDataTable->setHeaderHidden(false);
    pDockWidget->setWidget(mpDataTable);
    // Editor of table values
    DoubleSpinBoxItemDelegate* itemDelegate = new DoubleSpinBoxItemDelegate();
    mpDataTable->setItemDelegate(itemDelegate);
    // Models
    mpBaseTableModel = new BaseTableModel(pDockWidget);
    mpMatrixTableModel = new MatrixTableModel(pDockWidget);
    mpSurfaceTableModel = new SurfaceTableModel(pDockWidget);
    // ToolBar
    QToolBar* pToolBar = pDockWidget->createDefaultToolBar();
    pToolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    pDockWidget->setToolBarIconSize(kIconSize, CDockWidget::StateDocked);
    pToolBar->addAction(QIcon(":/icons/arrows-expand.svg"), tr("Expand"), mpDataTable, &QTreeView::expandAll);
    pToolBar->addAction(QIcon(":/icons/arrows-collapse.svg"), tr("Collapse"), mpDataTable, &QTreeView::collapseAll);
    pToolBar->addSeparator();
    pToolBar->addAction(QIcon(":/icons/table-row-add.svg"), tr("Add Roww"), this, &DataObjectsManager::insertItemAfterSelected);
    pToolBar->addAction(QIcon(":/icons/table-row-delete.svg"), tr("Remove Row"), this, &DataObjectsManager::removeSelectedItem);
    pToolBar->addSeparator();
    pToolBar->addAction(QIcon(":/icons/table-column-add.svg"), tr("Add Column"), this, &DataObjectsManager::insertLeadingItemAfterSelected);
    pToolBar->addAction(QIcon(":/icons/table-column-delete.svg"), tr("Remove Column"), this, &DataObjectsManager::removeSelectedLeadingItem);
    return pDockWidget;
}

//! Create an object to present all data objects
CDockWidget* DataObjectsManager::createDataObjectsWidget()
{
    CDockWidget* pDockWidget = new CDockWidget("Objects");
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    mpListObjects = new QListWidget();
    mpListObjects->setIconSize(QSize(11, 11));
    mpListObjects->setSelectionMode(QAbstractItemView::SingleSelection);
    mpListObjects->setSelectionBehavior(QAbstractItemView::SelectItems);
    mpListObjects->setEditTriggers(QAbstractItemView::DoubleClicked);
    connect(mpListObjects, &QListWidget::itemChanged, this, &DataObjectsManager::renameDataObject);
    connect(mpListObjects, &QListWidget::currentItemChanged, this, &DataObjectsManager::representSelectedDataObject);
    pDockWidget->setWidget(mpListObjects);
    // ToolBar
    QToolBar* pToolBar = pDockWidget->createDefaultToolBar();
    pToolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    pDockWidget->setToolBarIconSize(kIconSize, CDockWidget::StateDocked);
    // Actions
    pToolBar->addAction(QIcon(":/icons/letter-s.svg"), tr("Scalar"), this, &DataObjectsManager::addScalar);
    pToolBar->addAction(QIcon(":/icons/letter-v.svg"), tr("Vector"), this, &DataObjectsManager::addVector);
    pToolBar->addAction(QIcon(":/icons/letter-m.svg"), tr("Matrix"), this, &DataObjectsManager::addMatrix);
    pToolBar->addAction(QIcon(":/icons/letter-xy.svg"), tr("Surface"), this, &DataObjectsManager::addSurface);
    pToolBar->addSeparator();
    pToolBar->addAction(QIcon(":/icons/delete.svg"), tr("Remove"), this, &DataObjectsManager::removeSelectedDataObject);
    return pDockWidget;
}

//! Create a widget enables to code data objects
CDockWidget* DataObjectsManager::createCodeWidget()
{
    CDockWidget* pDockWidget = new CDockWidget(tr("Code"));
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    QTextEdit* pWidget = new QTextEdit();
    pDockWidget->setWidget(pWidget);
    // ToolBar
    QToolBar* pToolBar = pDockWidget->createDefaultToolBar();
    pToolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    pDockWidget->setToolBarIconSize(kIconSize, CDockWidget::StateDocked);
    pToolBar->addAction(QIcon(":/icons/debug-start.svg"), tr("Start"));
    pToolBar->addAction(QIcon(":/icons/debug-stop.svg"), tr("Stop"));
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
    mSettings.setValue("dataObjectsManager/Geometry", saveGeometry());
    mSettings.setValue("dataObjectsManager/DockingState", mpDockManager->saveState());
    mpDockManager->savePerspectives(mSettings);
}

//! Restore settings from a file
void DataObjectsManager::restoreSettings()
{
    restoreGeometry(mSettings.value("dataObjectsManager/Geometry").toByteArray());
    mpDockManager->restoreState(mSettings.value("dataObjectsManager/DockingState").toByteArray());
    mpDockManager->loadPerspectives(mSettings);
}

//! Apply all the changes made by user
void DataObjectsManager::apply()
{
    mProject.setDataObjects(mDataObjects);
    setWindowModified(false);
    qInfo() << tr("Data objects were modified through the data object manager");
}

//! Make a copy of existed data objects
void DataObjectsManager::retrieveDataObjects()
{
    mDataObjects = mProject.getDataObjects();
}

//! Add a scalar object
void DataObjectsManager::addScalar()
{
    static QString const kScalarName = "Scalar ";
    static QIcon icon = QIcon(":/icons/letter-s.svg");
    QString name = kScalarName + QString::number(ScalarDataObject::numberInstances() + 1);
    emplaceDataObject(new ScalarDataObject(name), icon, name);
}

//! Add a vector object
void DataObjectsManager::addVector()
{
    static QString const kVectorName = "Vector ";
    static QIcon icon = QIcon(":/icons/letter-v.svg");
    QString name = kVectorName + QString::number(VectorDataObject::numberInstances() + 1);
    emplaceDataObject(new VectorDataObject(name), icon, name);
}

//! Add a matrix object
void DataObjectsManager::addMatrix()
{
    static QString const kMatrixName = "Matrix ";
    static QIcon icon = QIcon(":/icons/letter-m.svg");
    QString name = kMatrixName + QString::number(MatrixDataObject::numberInstances() + 1);
    emplaceDataObject(new MatrixDataObject(name), icon, name);
}

//! Add a surface object
void DataObjectsManager::addSurface()
{
    static QString const kSurfaceName = "Surface ";
    static QIcon icon = QIcon(":/icons/letter-xy.svg");
    QString name = kSurfaceName + QString::number(SurfaceDataObject::numberInstances() + 1);
    emplaceDataObject(new SurfaceDataObject(name), icon, name);
}

//! Select a data object from the list
void DataObjectsManager::selectDataObject(int index)
{
    if (index > mpListObjects->count())
        return;
    mpListObjects->setCurrentRow(index);
}

//! Represent a selected data object according to its type
void DataObjectsManager::representSelectedDataObject()
{
    mpInterfaceTableModel = nullptr;
    int iRowSelected = mpListObjects->currentRow();
    if (iRowSelected < 0 || iRowSelected > mpListObjects->count())
        return;
    DataIDType id = mpListObjects->item(iRowSelected)->data(Qt::UserRole).toUInt();
    AbstractDataObject* pObject = mDataObjects[id];
    mpBaseTableModel->setDataObject(nullptr);
    mpMatrixTableModel->setDataObject(nullptr);
    mpDataTable->setHeaderHidden(false);
    switch (pObject->type())
    {
    case kScalar:
    case kVector:
        mpDataTable->setSortingEnabled(false);
        mpBaseTableModel->setDataObject(pObject);
        mpDataTable->setSortingEnabled(true);
        mpDataTable->setModel(mpBaseTableModel);
        mpInterfaceTableModel = mpBaseTableModel;
        break;
    case kMatrix:
        mpDataTable->setSortingEnabled(false);
        mpMatrixTableModel->setDataObject(pObject);
        mpDataTable->setSortingEnabled(true);
        mpDataTable->setModel(mpMatrixTableModel);
        mpInterfaceTableModel = mpMatrixTableModel;
        mpDataTable->expandAll();
        break;
    case kSurface:
        mpDataTable->setSortingEnabled(false);
        mpDataTable->setHeaderHidden(true);
        mpSurfaceTableModel->setDataObject((SurfaceDataObject*)pObject);
        mpDataTable->setModel(mpSurfaceTableModel);
        mpInterfaceTableModel = mpSurfaceTableModel;
        break;
    }
}

//! Insert a new array into the data object
void DataObjectsManager::insertItemAfterSelected()
{
    if (isDataTableModifiable())
    {
        mpInterfaceTableModel->insertItemAfterSelected(mpDataTable->selectionModel());
        setWindowModified(true);
    }
}

//! Insert a new leading item into the data object
void DataObjectsManager::insertLeadingItemAfterSelected()
{
    if (isDataTableModifiable())
    {
        mpInterfaceTableModel->insertLeadingItemAfterSelected(mpDataTable->selectionModel());
        setWindowModified(true);
    }
}

//! Remove a selected item
void DataObjectsManager::removeSelectedItem()
{
    if (isDataTableModifiable())
    {
        mpInterfaceTableModel->removeSelectedItem(mpDataTable->selectionModel());
        setWindowModified(true);
    }
}

//! Remove a selected leading item
void DataObjectsManager::removeSelectedLeadingItem()
{
    if (isDataTableModifiable())
    {
        mpInterfaceTableModel->removeSelectedLeadingItem(mpDataTable->selectionModel());
        setWindowModified(true);
    }
}

//! Remove a selected data object
void DataObjectsManager::removeSelectedDataObject()
{
    if (mpListObjects->currentRow() < 0)
        return;
    QListWidgetItem* item = mpListObjects->currentItem();
    DataIDType id = item->data(Qt::UserRole).toUInt();
    mpListObjects->removeItemWidget(item);
    mDataObjects.erase(id);
    delete item;
    if (!mpListObjects->count())
    {
        mpInterfaceTableModel = nullptr;
        mpDataTable->setModel(nullptr);
    }
    setWindowModified(true);
}

//! Rename a data object
void DataObjectsManager::renameDataObject(QListWidgetItem* item)
{
    DataIDType id = item->data(Qt::UserRole).toUInt();
    mDataObjects.at(id)->setName(item->text());
    setWindowModified(true);
}

//! Helper function to insert data objects into the manager
void DataObjectsManager::emplaceDataObject(AbstractDataObject* dataObject, QIcon const& icon, QString const& name)
{
    mDataObjects.emplace(dataObject->id(), dataObject);
    QListWidgetItem* item = new QListWidgetItem(icon, name);
    item->setData(Qt::UserRole, dataObject->id());
    item->setFlags (item->flags () | Qt::ItemIsEditable);
    mpListObjects->addItem(item);
    setWindowModified(true);
}

//! Helper function to check if it is possible to interact with data object content
bool DataObjectsManager::isDataTableModifiable()
{
    return mpListObjects->currentRow() >= 0 && mpInterfaceTableModel;
}

