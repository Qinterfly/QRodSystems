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

#include "dataobjectsmanager.h"
#include "ui_dataobjectsmanager.h"
#include "../central/uiconstants.h"
#include "../core/project.h"
#include "../core/scalardataobject.h"
#include "../core/vectordataobject.h"
#include "../core/matrixdataobject.h"
#include "../core/surfacedataobject.h"
#include "../core/utilities.h"
#include "basetablemodel.h"
#include "matrixtablemodel.h"
#include "surfacetablemodel.h"
#include "doublespinboxitemdelegate.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::CDockAreaWidget;
using namespace QRS;

const static QSize kIconSize = QSize(22, 22);
const QString skDataObjectsWindow = "DataObjectsManager";

void setToolBarShortcutHints(QToolBar* pToolBar);
QIcon getDataObjectIcon(DataObjectType type);

DataObjectsManager::DataObjectsManager(QRS::Project& project, QSettings& settings, QString& lastPath, QWidget* parent)
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
    delete mpUi;
    for (auto iter = mDataObjects.begin(); iter != mDataObjects.end(); ++iter)
        delete iter->second;
    mDataObjects.clear();
}

//! Save settings and delete handling widgets before closing the window
void DataObjectsManager::closeEvent(QCloseEvent* event)
{
    event->ignore();
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
        mpDockManager->deleteLater();
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
    mpDockManager->setStyleSheet("");
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
    mpDataTable->setSelectionBehavior(QAbstractItemView::SelectItems);
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
    CDockWidget* pDockWidget = new CDockWidget("Objects");
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    mpListDataObjects = new QListWidget();
    mpListDataObjects->setIconSize(QSize(11, 11));
    mpListDataObjects->setSelectionMode(QAbstractItemView::SingleSelection);
    mpListDataObjects->setSelectionBehavior(QAbstractItemView::SelectItems);
    mpListDataObjects->setEditTriggers(QAbstractItemView::DoubleClicked);
    connect(mpListDataObjects, &QListWidget::itemChanged, this, &DataObjectsManager::renameDataObject);
    connect(mpListDataObjects, &QListWidget::currentItemChanged, this, &DataObjectsManager::representSelectedDataObject);
    pDockWidget->setWidget(mpListDataObjects);
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
    for (auto const& mapItem : mDataObjects)
        addListDataObjects(mapItem.second);
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

//! Select a data object from the list
void DataObjectsManager::selectDataObject(int index)
{
    if (index > mpListDataObjects->count())
        return;
    mpListDataObjects->setCurrentRow(index);
}

//! Represent a selected data object according to its type
void DataObjectsManager::representSelectedDataObject()
{
    mpInterfaceTableModel = nullptr;
    int iRowSelected = mpListDataObjects->currentRow();
    if (iRowSelected < 0 || iRowSelected > mpListDataObjects->count())
        return;
    DataIDType id = mpListDataObjects->item(iRowSelected)->data(Qt::UserRole).toUInt();
    AbstractDataObject* pObject = mDataObjects[id];
    mpBaseTableModel->setDataObject(nullptr);
    mpMatrixTableModel->setDataObject(nullptr);
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
    if (mpListDataObjects->currentRow() < 0)
        return;
    QListWidgetItem* item = mpListDataObjects->currentItem();
    DataIDType id = item->data(Qt::UserRole).toUInt();
    mpListDataObjects->removeItemWidget(item);
    mDataObjects.erase(id);
    mHierarchyDataObjects.removeNode(HierarchyNode::NodeType::kObject, id);
    delete item;
    if (!mpListDataObjects->count())
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
void DataObjectsManager::emplaceDataObject(AbstractDataObject* dataObject)
{
    DataIDType id = dataObject->id();
    mDataObjects.emplace(id, dataObject);
    mHierarchyDataObjects.appendNode(new HierarchyNode(HierarchyNode::NodeType::kObject, id));
    addListDataObjects(dataObject);
    setWindowModified(true);
}

//! Add a data object to the list
void DataObjectsManager::addListDataObjects(AbstractDataObject* dataObject)
{
    QIcon icon = getDataObjectIcon(dataObject->type());
    QListWidgetItem* item = new QListWidgetItem(icon, dataObject->name());
    item->setData(Qt::UserRole, dataObject->id());
    item->setFlags (item->flags () | Qt::ItemIsEditable);
    mpListDataObjects->addItem(item);
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
    return mpListDataObjects->currentRow() >= 0 && mpInterfaceTableModel;
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

//! Helper function to assign appropriate data object icon
QIcon getDataObjectIcon(DataObjectType type)
{
    switch(type)
    {
    case kScalar:
        return QIcon(":/icons/letter-s.svg");
    case kVector:
        return QIcon(":/icons/letter-v.svg");
    case kMatrix:
        return QIcon(":/icons/letter-m.svg");
    case kSurface:
        return QIcon(":/icons/letter-xy.svg");
    default:
        return QIcon();
    }
}
