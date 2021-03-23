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
#include "DockManager.h"
#include "DockWidget.h"

#include "dataobjectsmanager.h"
#include "ui_dataobjectsmanager.h"
#include "../core/project.h"
#include "../core/scalardataobject.h"
#include "scalartablemodel.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::CDockAreaWidget;
using namespace QRS;

const static QSize kIconSize = QSize(20, 20);

DataObjectsManager::DataObjectsManager(QRS::Project& project, QSettings& settings, QWidget* parent)
    : QDialog(parent)
    , mpUi(new Ui::DataObjectsManager)
    , mProject(project)
    , mSettings(settings)
{
    mpUi->setupUi(this);
    createContent();
    restoreSettings();
    retrieveDataObjects();
}

DataObjectsManager::~DataObjectsManager()
{
    delete mpUi;
}

//! Save settings and delete handling widgets before closing the window
void DataObjectsManager::closeEvent(QCloseEvent* event)
{
    saveSettings();
    mpDockManager->deleteLater();
    QDialog::closeEvent(event);
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
    mpDockDataTable = new CDockWidget("Data Table");
    mpDockDataTable->setFeature(CDockWidget::DockWidgetClosable, false);
    mpDataTable = new QTreeView();
    mpDockDataTable->setWidget(mpDataTable);
    // ToolBar
    QToolBar* pToolBar = mpDockDataTable->createDefaultToolBar();
    pToolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    mpDockDataTable->setToolBarIconSize(kIconSize, CDockWidget::StateDocked);
    pToolBar->addAction(QIcon(":/icons/plus.svg"), tr("Add"));
    pToolBar->addAction(QIcon(":/icons/minus.svg"), tr("Remove"));
    return mpDockDataTable;
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
    mpListObjects->setEditTriggers(QAbstractItemView::EditKeyPressed);
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
    pToolBar->addAction(QIcon(":/icons/delete.svg"), tr("Remove"));
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
}

//! Restore settings from a file
void DataObjectsManager::restoreSettings()
{
    restoreGeometry(mSettings.value("dataObjectsManager/Geometry").toByteArray());
    mpDockManager->restoreState(mSettings.value("dataObjectsManager/DockingState").toByteArray());
}

//! Apply all the changes made by a user
void DataObjectsManager::apply()
{
    // TODO
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
    QString name = kScalarName + QString::number(ScalarDataObject::numberScalars() + 1);
    ScalarDataObject* scalar = new ScalarDataObject(name);
    mDataObjects.emplace(scalar->id(), scalar);
    QListWidgetItem* item = new QListWidgetItem(icon, name);
    item->setData(Qt::UserRole, scalar->id());
    mpListObjects->addItem(item);
}

//! Add a vector object
void DataObjectsManager::addVector()
{
    // TODO
}

//! Add a matrix object
void DataObjectsManager::addMatrix()
{
    // TODO
}

//! Add a surface object
void DataObjectsManager::addSurface()
{
    // TODO
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
    uint iRowSelected = mpListObjects->currentRow();
    DataIDType id = mpListObjects->item(iRowSelected)->data(Qt::UserRole).toUInt();
    AbstractDataObject* pObject = mDataObjects[id];
    switch (pObject->type())
    {
    case kScalar:
        ScalarTableModel* model = new ScalarTableModel(static_cast<ScalarDataObject*>(pObject), mpDockDataTable);
        mpDataTable->setModel(model);
        mpDataTable->expandAll();
        break;
    }
}

