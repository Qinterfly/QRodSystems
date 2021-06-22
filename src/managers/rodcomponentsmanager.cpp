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
#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include "rodcomponentsmanager.h"
#include "core/project.h"
#include "core/vectordataobject.h"
#include "core/matrixdataobject.h"
#include "core/geometryrodcomponent.h"
#include "core/usercrosssectionrodcomponent.h"
#include "managers/geometryrodcomponentwidget.h"
#include "models/hierarchy/rodcomponentshierarchymodel.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::CDockAreaWidget;
using namespace QRS::Core;
using namespace QRS::Managers;
using namespace QRS::HierarchyModels;

RodComponentsManager::RodComponentsManager(Project& project, QString& lastPath, QSettings& settings, QWidget* parent)
    : AbstractProjectManager(project, lastPath, settings, kRodComponents, "RodComponentsManager", parent)
{
    setWindowTitle("Rod Components Manager[*]");
    setGeometry(0, 0, 700, 700);
    setWindowModified(false);
    createContent();
    restoreSettings();
    retrieveDataObjects();
    retrieveRodComponents();
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
    // Main layout
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
    pMainLayout->addWidget(mpDockManager);
    // Components dock widget
    mpComponentDockWidget = createComponentsDockWidget();
    mpDockManager->addDockWidget(ads::LeftDockWidgetArea, mpComponentDockWidget);
    // Hierarchy of components
    mpDockManager->addDockWidget(ads::RightDockWidgetArea, createHierarchyWidget());
    // Buttons
    pMainLayout->addLayout(createDialogControls());
}

//! Create a widget to show a hierarchy of rod components
CDockWidget* RodComponentsManager::createHierarchyWidget()
{
    QSize const kIconSize = QSize(16, 16);
    QSize const kToolBarIconSize = QSize(27, 27);
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
    mpTreeRodComponentsModel = new RodComponentsHierarchyModel(mRodComponents, mHierarchyRodComponents, mpTreeRodComponents);
    mpTreeRodComponents->setModel(mpTreeRodComponentsModel);
    connect(mpTreeRodComponentsModel, &RodComponentsHierarchyModel::dataModified,
            this, &RodComponentsManager::setWindowModified);
    connect(mpTreeRodComponentsModel, &RodComponentsHierarchyModel::selected,
            this, &RodComponentsManager::representRodComponent);
    connect(mpTreeRodComponents->selectionModel(), &QItemSelectionModel::selectionChanged,
            mpTreeRodComponentsModel, &RodComponentsHierarchyModel::retrieveSelectedItem);
    connect(mpTreeRodComponentsModel, &RodComponentsHierarchyModel::selectionCleared,
            this, &RodComponentsManager::clearRodComponentRepresentation);
    // ToolBar
    QToolBar* pToolBar = pDockWidget->createDefaultToolBar();
    pDockWidget->setToolBarIconSize(kToolBarIconSize, CDockWidget::StateDocked);
    // Actions
    QAction* pAction;
    pAction = pToolBar->addAction(QIcon(":/icons/axis.svg"), tr("Geometry"), this, &RodComponentsManager::addGeometry);
    pAction->setShortcut(QKeySequence("Ctrl+1"));
    pAction = pToolBar->addAction(QIcon(":/icons/tubes.svg"), tr("Cross section"));
    pAction->setShortcut(QKeySequence("Ctrl+2"));
    pAction = pToolBar->addAction(QIcon(":/icons/clamp.svg"), tr("Boundary condition"));
    pAction->setShortcut(QKeySequence("Ctrl+3"));
    pAction = pToolBar->addAction(QIcon(":/icons/force.svg"), tr("Force"));
    pAction->setShortcut(QKeySequence("Ctrl+4"));
    pAction = pToolBar->addAction(QIcon(":/icons/material.svg"), tr("Material"));
    pAction->setShortcut(QKeySequence("Ctrl+5"));
    pToolBar->addSeparator();
    pAction = pToolBar->addAction(QIcon(":/icons/delete.svg"), tr("Remove"),
                                  mpTreeRodComponentsModel, &RodComponentsHierarchyModel::removeSelectedItems);
    pAction->setShortcut(Qt::Key_R);
    setToolBarShortcutHints(pToolBar);
    return pDockWidget;
}

//! Create a dock widget to contain constructors of rod components
ads::CDockWidget* RodComponentsManager::createComponentsDockWidget()
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

//! Apply all the changes made by user
void RodComponentsManager::apply()
{
    mProject.setRodComponents(mRodComponents, mHierarchyRodComponents);
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

//! Retrieve data objects according to their type
void RodComponentsManager::retrieveDataObjects()
{
    DataObjects const& dataObjects = mProject.getDataObjects();
    for (auto const& iter : dataObjects)
    {
        AbstractDataObject const* pDataObject = iter.second;
        DataIDType id = pDataObject->id();
        switch (pDataObject->type())
        {
        case AbstractDataObject::ObjectType::kScalar:
            mScalarDataObjects.emplace(id, (ScalarDataObject const*)pDataObject);
            break;
        case AbstractDataObject::ObjectType::kVector:
            mVectorDataObjects.emplace(id, (VectorDataObject const*)pDataObject);
            break;
        case AbstractDataObject::ObjectType::kMatrix:
            mMatrixDataObjects.emplace(id, (MatrixDataObject const*)pDataObject);
            break;
        case AbstractDataObject::ObjectType::kSurface:
            mSurfaceDataObjects.emplace(id, (SurfaceDataObject const*)pDataObject);
            break;
        }
    }
}

//! Make a copy of existed rod components
void RodComponentsManager::retrieveRodComponents()
{
    mRodComponents = mProject.cloneRodComponents();
    mHierarchyRodComponents = mProject.cloneHierarchyRodComponents();
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
        mpComponentDockWidget->setWidget(new GeometryRodComponentWidget(*pGeometry, mVectorDataObjects, mMatrixDataObjects, mpComponentDockWidget));
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

