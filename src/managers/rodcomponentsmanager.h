/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the RodComponentsManager class
 */

#ifndef RODCOMPONENTSMANAGER_H
#define RODCOMPONENTSMANAGER_H

#include "managers/abstractprojectmanager.h"
#include "core/aliasdataset.h"
#include "core/hierarchytree.h"

QT_BEGIN_NAMESPACE
class QTreeView;
QT_END_NAMESPACE

namespace ads
{
class CDockWidget;
}

namespace QRS
{

namespace Core
{
class AbstractRodComponent;
}

namespace HierarchyModels
{
class RodComponentsHierarchyModel;
}

namespace Managers
{

//! Manager to create rod components, such as a geometry, cross section and force
class RodComponentsManager : public AbstractProjectManager
{
    Q_OBJECT

public:
    RodComponentsManager(Core::Project& project, QString& lastPath, QSettings& settings, QWidget* parent = nullptr);
    ~RodComponentsManager();
    void selectRodComponent(int iRow);

public slots:
    void apply() override;
    Core::AbstractRodComponent* addGeometry();

private:
    // Content
    void createContent();
    QLayout* createDialogControls();
    void retrieveRodComponents();
    ads::CDockWidget* createHierarchyWidget();
    ads::CDockWidget* createComponentsDockWidget();
    // Helpers
    void emplaceRodComponent(Core::AbstractRodComponent* pComponent);
    // Selection
    void representRodComponent(Core::DataIDType id);
    void clearRodComponentRepresentation();

private:
    // Widgets
    ads::CDockWidget* mpComponentDockWidget;
    QTreeView* mpTreeRodComponents;
    // Data
    Core::HierarchyTree mHierarchyRodComponents;
    Core::RodComponents mRodComponents;
    Core::DataObjects const& mDataObjects;
    // Models
    HierarchyModels::RodComponentsHierarchyModel* mpTreeRodComponentsModel;
};

}

}

#endif // RODCOMPONENTSMANAGER_H
