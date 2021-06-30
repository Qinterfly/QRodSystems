/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the RodComponentsManager class
 */

#include <QVBoxLayout>
#include <QPushButton>
#include <QTreeView>
#include <QToolBar>
#include <QLabel>
#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include "rodcomponentsmanager.h"
#include "core/vectordataobject.h"
#include "core/matrixdataobject.h"
#include "core/geometryrodcomponent.h"
#include "core/usercrosssectionrodcomponent.h"
#include "managers/geometryrodcomponentwidget.h"
#include "models/hierarchy/dataobjectshierarchymodel.h"
#include "models/hierarchy/rodcomponentshierarchymodel.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::CDockAreaWidget;
using namespace QRS::Core;
using namespace QRS::Managers;
using namespace QRS::HierarchyModels;

QSize const skToolBarIconSize = QSize(27, 27);
QString const skDataObjectsMimeType = "rodcomponentsmanager/dataobjectshierarchy";

QWidget* addToolbarHeader(QToolBar* pToolBar, QString const& name);

RodComponentsManager::RodComponentsManager(DataObjects& dataObjects, HierarchyTree& hieararchyDataObjects,
                                           RodComponents&& rodComponents, HierarchyTree&& hierarchyRodComponents,
                                           QString& lastPath, QSettings& settings, QWidget* parent)
    : AbstractProjectManager(lastPath, settings, kRodComponents, "RodComponentsManager", parent)
    , mDataObjects(dataObjects)
    , mHierarchyDataObjects(hieararchyDataObjects)
    , mRodComponents(std::move(rodComponents))
    , mHierarchyRodComponents(std::move(hierarchyRodComponents))
{
    setWindowTitle("Rod Components Manager[*]");
    setGeometry(0, 0, 700, 700);
    setWindowModified(false);
    createContent();
    restoreSettings();
    mpTreeDataObjectsModel->updateContent();
    mpTreeRodComponentsModel->updateContent();
}

RodComponentsManager::~RodComponentsManager()
{
    for (auto iter = mRodComponents.begin(); iter != mRodComponents.end(); ++iter)
        delete iter->second;
    mRodComponents.clear();
}

//! Create all the widgets
void RodComponentsManager::createContent()
{
    // Components dock widget
    mpComponentDockWidget = createConstructorDockWidget();
    mpDockManager->addDockWidget(ads::LeftDockWidgetArea, mpComponentDockWidget);
    // Hierarchy of components
    CDockAreaWidget* pArea = mpDockManager->addDockWidget(ads::RightDockWidgetArea, createHierarchyRodComponentsWidget());
    // Hierarchy of data objects
    mpDockManager->addDockWidget(ads::BottomDockWidgetArea, createHierarchyDataObjectsWidget(), pArea);
    // Main layout
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
    pMainLayout->setSpacing(0);
    // Arrangement
    pMainLayout->addWidget(createMainToolBar());
    pMainLayout->addWidget(mpDockManager);
    pMainLayout->addLayout(createDialogControls());
}

//! Create a widget to show a hierarchy of rod components
CDockWidget* RodComponentsManager::createHierarchyRodComponentsWidget()
{
    QSize const kIconSize = QSize(16, 16);
    CDockWidget* pDockWidget = new CDockWidget("Components");
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    // Tree widget
    mpTreeRodComponents = new QTreeView();
    mpTreeRodComponents->setIconSize(kIconSize);
    mpTreeRodComponents->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mpTreeRodComponents->setSelectionBehavior(QAbstractItemView::SelectItems);
    mpTreeRodComponents->setEditTriggers(QAbstractItemView::DoubleClicked);
    mpTreeRodComponents->setHeaderHidden(true);
    mpTreeRodComponents->setAcceptDrops(true);
    mpTreeRodComponents->setDragEnabled(true);
    pDockWidget->setWidget(mpTreeRodComponents);
    // Hierarchy model
    mpTreeRodComponentsModel = new RodComponentsHierarchyModel(mRodComponents, mHierarchyRodComponents,
                                                               "rodcomponentsmanager/hierarchy", mpTreeRodComponents);
    mpTreeRodComponents->setModel(mpTreeRodComponentsModel);
    connect(mpTreeRodComponentsModel, &RodComponentsHierarchyModel::dataModified,
            this, &RodComponentsManager::setWindowModified);
    connect(mpTreeRodComponentsModel, &RodComponentsHierarchyModel::selected,
            this, &RodComponentsManager::representRodComponent);
    connect(mpTreeRodComponents->selectionModel(), &QItemSelectionModel::selectionChanged,
            mpTreeRodComponentsModel, &RodComponentsHierarchyModel::retrieveSelectedItem);
    connect(mpTreeRodComponentsModel, &RodComponentsHierarchyModel::selectionCleared,
            this, &RodComponentsManager::clearRodComponentRepresentation);
    return pDockWidget;
}

//! Create a widget to show a hierarchy of data objects
ads::CDockWidget* RodComponentsManager::createHierarchyDataObjectsWidget()
{
    CDockWidget* pDockWidget = new CDockWidget("Data Objects");
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    QTreeView* pTreeDataObjects = new QTreeView();
    pTreeDataObjects->setSelectionMode(QAbstractItemView::SingleSelection);
    pTreeDataObjects->setSelectionBehavior(QAbstractItemView::SelectItems);
    pTreeDataObjects->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pTreeDataObjects->setHeaderHidden(true);
    pTreeDataObjects->setAcceptDrops(false);
    pTreeDataObjects->setDragEnabled(true);
    pDockWidget->setWidget(pTreeDataObjects);
    // Hierarchy model
    mpTreeDataObjectsModel = new DataObjectsHierarchyModel(mDataObjects, mHierarchyDataObjects,
                                                           skDataObjectsMimeType, pTreeDataObjects);
    pTreeDataObjects->setModel(mpTreeDataObjectsModel);
    return pDockWidget;
}

//! Create a dock widget to contain constructors of rod components
ads::CDockWidget* RodComponentsManager::createConstructorDockWidget()
{
    CDockWidget* pDockWidget = new CDockWidget("Component Constructor");
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    return pDockWidget;
}

//! Create dialog controls
QLayout* RodComponentsManager::createDialogControls()
{
    QHBoxLayout* pLayout = new QHBoxLayout();
    pLayout->setContentsMargins(0, 0, 3, 5);
    QPushButton* pAcceptButton = new QPushButton(QIcon(":/icons/edit-ok.svg"), tr("Apply"));
    connect(pAcceptButton, &QPushButton::clicked, this, &RodComponentsManager::apply);
    pLayout->addStretch();
    pLayout->addWidget(pAcceptButton);
    return pLayout;
}

//! Select a rod component by row index
void RodComponentsManager::selectRodComponent(int iRow)
{
    mpTreeRodComponentsModel->selectItem(iRow);
}

//! Update the representation of data objects
void RodComponentsManager::updateDataObjects()
{
    mpTreeDataObjectsModel->updateContent();
}

//! Apply all the changes made by user
void RodComponentsManager::apply()
{
    emit rodComponentsModified(mRodComponents, mHierarchyRodComponents);
    setWindowModified(false);
    qInfo() << tr("Rod components were modified by means of the manager");
}

//! Add a geometrical component
AbstractRodComponent* RodComponentsManager::addGeometry()
{
    static QString const kBaseName = "Geometry ";
    QString name = kBaseName + QString::number(GeometryRodComponent::numberInstances() + 1);
    AbstractRodComponent* pRodComponent = new GeometryRodComponent(name);
    emplaceRodComponent(pRodComponent);
    return pRodComponent;
}

//! Add a cross section
AbstractRodComponent* RodComponentsManager::addCrossSection(AbstractCrossSectionRodComponent::SectionType sectionType)
{
    static QString const kBaseName = "Cross section ";
    QString name = kBaseName + QString::number(AbstractCrossSectionRodComponent::numberInstances() + 1);
    AbstractRodComponent* pRodComponent = nullptr;
    switch (sectionType)
    {
    case AbstractCrossSectionRodComponent::SectionType::kUserDefined:
        pRodComponent = new UserCrossSectionRodComponent(name);
        break;
    }
    emplaceRodComponent(pRodComponent);
    return pRodComponent;
}

//! Helper function to insert a rod component into the manager
void RodComponentsManager::emplaceRodComponent(AbstractRodComponent* pRodComponent)
{
    DataIDType id = pRodComponent->id();
    mRodComponents.emplace(id, pRodComponent);
    mHierarchyRodComponents.appendNode(new HierarchyNode(HierarchyNode::NodeType::kObject, id));
    mpTreeRodComponentsModel->updateContent();
    setWindowModified(true);
}

//! Represent a selected rod component according to its type
void RodComponentsManager::representRodComponent(Core::DataIDType id)
{
    clearRodComponentRepresentation();
    if (!mRodComponents.contains(id))
        return;
    AbstractRodComponent* pRodComponent = mRodComponents[id];
    switch (pRodComponent->componentType())
    {
    case AbstractRodComponent::ComponentType::kGeometry:
    {
        GeometryRodComponent* pGeometry = (GeometryRodComponent*)pRodComponent;
        GeometryRodComponentWidget* pGeometryWidget = new GeometryRodComponentWidget(*pGeometry, skDataObjectsMimeType, mpComponentDockWidget);
        connect(pGeometryWidget, &GeometryRodComponentWidget::modified, this, &RodComponentsManager::setWindowModified);
        mpComponentDockWidget->setWidget(pGeometryWidget);
        break;
    }
    case AbstractRodComponent::ComponentType::kCrossSection:
        // TODO
        break;
    }
}

//! Delete a widget to represent properties of a rod component
void RodComponentsManager::clearRodComponentRepresentation()
{
    QWidget* pWidget = mpComponentDockWidget->takeWidget();
    delete pWidget;
}

//! Create a menu to choose types of components to construct
QToolBar* RodComponentsManager::createMainToolBar()
{
    QToolBar* pMainToolBar = new QToolBar();
    // Geometry
    pMainToolBar->addWidget(makeGeometryToolBar());
    pMainToolBar->addSeparator();
    // Cross section
    pMainToolBar->addWidget(makeCrossSectionsToolBar());
    pMainToolBar->addSeparator();
    // Boundary condition
    pMainToolBar->addWidget(makeBoundaryConditionsToolBar());
    pMainToolBar->addSeparator();
    // Loading
    pMainToolBar->addWidget(makeLoadingToolBar());
    pMainToolBar->addSeparator();
    // Material
    pMainToolBar->addWidget(makeMaterialToolBar());
    pMainToolBar->addSeparator();
    // Modify
    pMainToolBar->addWidget(makeModificationToolBar());
    pMainToolBar->addSeparator();
    return pMainToolBar;
}

//! Create a toolbar to create geometrical components
QWidget* RodComponentsManager::makeGeometryToolBar()
{
    QToolBar* pToolBar = new QToolBar();
    pToolBar->addAction(QIcon(":/icons/axis.svg"), tr("Geometry"), this, &RodComponentsManager::addGeometry);
    pToolBar->setIconSize(skToolBarIconSize);
    return addToolbarHeader(pToolBar, "Geometry");
}

//! Create a toolbar to construct cross sections
QWidget* RodComponentsManager::makeCrossSectionsToolBar()
{
    QToolBar* pToolBar = new QToolBar();
    // User-defined
    pToolBar->addAction(QIcon(":/icons/abstract-shape.svg"), tr("User-defined"), this, [this]()
    {
        addCrossSection(AbstractCrossSectionRodComponent::kUserDefined);
    });
    pToolBar->addAction(QIcon(":/icons/rectangle.svg"), tr("Rectangular"));
    pToolBar->setIconSize(skToolBarIconSize);
    return addToolbarHeader(pToolBar, "Section");
}

//! Create a toolbar to construct boundary conditions
QWidget* RodComponentsManager::makeBoundaryConditionsToolBar()
{
    QToolBar* pToolBar = new QToolBar();
    pToolBar->addAction(QIcon(":/icons/clamp.svg"), tr("Clamp"));
    pToolBar->setIconSize(skToolBarIconSize);
    return addToolbarHeader(pToolBar, "Boundary");
}

//! Create a toolbar to construct loading
QWidget* RodComponentsManager::makeLoadingToolBar()
{
    QToolBar* pToolBar = new QToolBar();
    pToolBar->addAction(QIcon(":/icons/force.svg"), tr("Force"));
    pToolBar->setIconSize(skToolBarIconSize);
    return addToolbarHeader(pToolBar, "Loading");
}

//! Create a toolbar to construct materials
QWidget* RodComponentsManager::makeMaterialToolBar()
{
    QToolBar* pToolBar = new QToolBar();
    pToolBar->addAction(QIcon(":/icons/material.svg"), tr("Material"));
    pToolBar->setIconSize(skToolBarIconSize);
    return addToolbarHeader(pToolBar, "Material");
}

//! Create a toolbar to modify rod components
QWidget* RodComponentsManager::makeModificationToolBar()
{
    QToolBar* pToolBar = new QToolBar();
    QAction* pAction = pToolBar->addAction(QIcon(":/icons/delete.svg"), tr("Remove"),
                                           mpTreeRodComponentsModel, &RodComponentsHierarchyModel::removeSelectedItems);
    pAction->setShortcut(Qt::Key_R);
    pToolBar->setIconSize(skToolBarIconSize);
    setToolBarShortcutHints(pToolBar);
    return addToolbarHeader(pToolBar, "Modify");
}

//! Helper function to add the header to a toolbar
QWidget* addToolbarHeader(QToolBar* pToolBar, QString const& name)
{
    QWidget* pWidget = new QWidget();
    QGridLayout* pLayout = new QGridLayout();
    pLayout->setContentsMargins(3, 0, 3, 0);
    pLayout->setSpacing(0);
    // Separator
    QFrame* pLine = new QFrame();
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    // Arranging widgets
    pLayout->addWidget(new QLabel(name), 0, 0, Qt::AlignCenter);
    pLayout->addWidget(pLine, 1, 0);
    pLayout->addWidget(pToolBar, 2, 0, Qt::AlignCenter);
    pWidget->setLayout(pLayout);
    return pWidget;
}
