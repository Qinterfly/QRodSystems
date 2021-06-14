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
#include "managers/geometrycomponentwidget.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::CDockAreaWidget;
using namespace QRS::Managers;
using namespace QRS::Core;

RodComponentsManager::RodComponentsManager(Project& project, QString& lastPath, QSettings& settings, QWidget* parent)
    : AbstractProjectManager(project, lastPath, settings, kRodComponents, "RodComponentsManager", parent)
{
    setWindowTitle("Rod Components Manager[*]");
    setGeometry(0, 0, 700, 700);
    setWindowModified(false);
    createContent();
    restoreSettings();
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
    // ToolBar
    QToolBar* pToolBar = pDockWidget->createDefaultToolBar();
    pDockWidget->setToolBarIconSize(kToolBarIconSize, CDockWidget::StateDocked);
    // Actions
    QAction* pAction;
    pAction = pToolBar->addAction(QIcon(":/icons/axis.svg"), tr("Geometry"));
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
    pAction = pToolBar->addAction(QIcon(":/icons/delete.svg"), tr("Remove"));
    pAction->setShortcut(Qt::Key_R);
    // TODO: Hierarchy model
    // ...
    setToolBarShortcutHints(pToolBar);
    pDockWidget->setWidget(mpTreeRodComponents);
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

//! Apply all the changes made by user
void RodComponentsManager::apply()
{
    // TODO: Project data substitution
    setWindowModified(false);
    qInfo() << tr("Rod components were modified by means of the manager");
}

//! Make a copy of existed rod components
void RodComponentsManager::retrieveRodComponents()
{
    mRodComponents = mProject.cloneRodComponents();
    mHierarchyRodComponents = mProject.cloneHierarchyRodComponents();
    // TODO: Update content
}
