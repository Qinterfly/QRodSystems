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
#include "core/abstractcrosssectionrodcomponent.h"
#include "aliasmanagers.h"

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
    Core::AbstractRodComponent* addCrossSection(Core::AbstractCrossSectionRodComponent::SectionType sectionType);

private:
    // Content
    void createContent();
    QLayout* createDialogControls();
    void retrieveDataObjects();
    void retrieveRodComponents();
    ads::CDockWidget* createHierarchyWidget();
    ads::CDockWidget* createComponentsDockWidget();
    // Helpers
    void emplaceRodComponent(Core::AbstractRodComponent* pRodComponent);
    // Selection
    void representRodComponent(Core::DataIDType id);
    void clearRodComponentRepresentation();
    // Toolbars
    QToolBar* createMainToolBar();
    QWidget* makeGeometryToolBar();
    QWidget* makeCrossSectionsToolBar();
    QWidget* makeBoundaryConditionsToolBar();
    QWidget* makeLoadingToolBar();
    QWidget* makeMaterialToolBar();
    QWidget* makeModificationToolBar();

private:
    // Widgets
    ads::CDockWidget* mpComponentDockWidget;
    QTreeView* mpTreeRodComponents;
    // Data objects
    ScalarDataObjects mScalarDataObjects;
    VectorDataObjects mVectorDataObjects;
    MatrixDataObjects mMatrixDataObjects;
    SurfaceDataObjects mSurfaceDataObjects;
    // Rod components data
    Core::RodComponents mRodComponents;
    Core::HierarchyTree mHierarchyRodComponents;
    // Models
    HierarchyModels::RodComponentsHierarchyModel* mpTreeRodComponentsModel;
};

}

}

#endif // RODCOMPONENTSMANAGER_H
