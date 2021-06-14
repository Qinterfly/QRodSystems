/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the RodComponentsManager class
 */

#include <QVBoxLayout>
#include <QPushButton>
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
    // Widget holder to present components of different types
    mpComponentDockWidget = new CDockWidget("Component Constructor");
    mpComponentDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    mpDockManager->addDockWidget(ads::LeftDockWidgetArea, mpComponentDockWidget);
    // TODO : Hierarchy of components
    // Buttons
    pMainLayout->addLayout(createDialogControls());
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
