/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the RodComponentsManager class
 */

#ifndef RODCOMPONENTSMANAGER_H
#define RODCOMPONENTSMANAGER_H

#include "managers/abstractmanager.h"
#include "core/aliasdataset.h"
#include "core/hierarchytree.h"
#include "core/abstractsectionrodcomponent.h"

QT_BEGIN_NAMESPACE
class QTreeView;
QT_END_NAMESPACE

namespace ads
{
class CDockWidget;
}

namespace QRS
{

namespace HierarchyModels
{
class DataObjectsHierarchyModel;
class RodComponentsHierarchyModel;
}

namespace Managers
{

//! Manager to create rod components, such as a geometry, cross section and force
class RodComponentsManager : public AbstractManager
{
    Q_OBJECT

public:
    RodComponentsManager(Core::DataObjects& dataObjects, Core::HierarchyTree& hieararchyDataObjects,
                         Core::RodComponents&& rodComponents, Core::HierarchyTree&& hierarchyRodComponents,
                         QString& lastPath, QSettings& settings, QWidget* parent = nullptr);
    ~RodComponentsManager();
    void selectRodComponent(int iRow);
    void updateDataObjects();

signals:
    void applied(Core::RodComponents const& rodComponents, Core::HierarchyTree const& hierarchyRodComponents);
    void editDataObjectRequested(Core::DataIDType id);

public slots:
    void apply() override;
    Core::AbstractRodComponent* addGeometry();
    Core::AbstractRodComponent* addSection(Core::AbstractSectionRodComponent::SectionType sectionType);
    Core::AbstractRodComponent* addMaterial();
    Core::AbstractRodComponent* addLoad();
    Core::AbstractRodComponent* addConstraint();
    Core::AbstractRodComponent* addMechanical();
    void resolveRodComponentsReferences();

private:
    // Content
    void createContent();
    QLayout* createDialogControls();
    ads::CDockWidget* createHierarchyRodComponentsWidget();
    ads::CDockWidget* createConstructorDockWidget();
    ads::CDockWidget* createHierarchyDataObjectsWidget();
    // Helpers
    void emplaceRodComponent(Core::AbstractRodComponent* pRodComponent);
    // Selection
    void representRodComponent(Core::DataIDType id);
    void clearRodComponentRepresentation();
    // Toolbars
    QToolBar* createMainToolBar();
    QWidget* makeGeometryToolBar();
    QWidget* makeSectionsToolBar();
    QWidget* makeBoundaryConditionsToolBar();
    QWidget* makeLoadingToolBar();
    QWidget* makeMaterialToolBar();
    QWidget* makeMechanicalToolBar();
    QWidget* makeModificationToolBar();

private:
    // Widgets
    ads::CDockWidget* mpComponentDockWidget;
    QTreeView* mpTreeRodComponents;
    // Data objects
    Core::DataObjects& mDataObjects;
    Core::HierarchyTree& mHierarchyDataObjects;
    // Rod components data
    Core::RodComponents mRodComponents;
    Core::HierarchyTree mHierarchyRodComponents;
    // Models
    HierarchyModels::DataObjectsHierarchyModel* mpTreeDataObjectsModel;
    HierarchyModels::RodComponentsHierarchyModel* mpTreeRodComponentsModel;
};

}

}

#endif // RODCOMPONENTSMANAGER_H
