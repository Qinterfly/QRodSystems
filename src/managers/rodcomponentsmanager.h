/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the RodComponentsManager class
 */

#ifndef RODCOMPONENTSMANAGER_H
#define RODCOMPONENTSMANAGER_H

#include "managers/abstractprojectmanager.h"
#include "core/datatypes.h"
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

namespace Managers
{

using mapRodComponents = std::unordered_map<Core::DataIDType, Core::AbstractRodComponent*>;

//! Manager to create rod components, such as a geometry, cross section and force
class RodComponentsManager : public AbstractProjectManager
{
    Q_OBJECT

public:
    RodComponentsManager(Core::Project& project, QString& lastPath, QSettings& settings, QWidget* parent = nullptr);
    ~RodComponentsManager();

public slots:
    void apply() override;

private:
    // Content
    void createContent();
    QLayout* createDialogControls();
    void retrieveRodComponents();
    ads::CDockWidget* createHierarchyWidget();
    ads::CDockWidget* createComponentsDockWidget();

private:
    // Widgets
    ads::CDockWidget* mpComponentDockWidget;
    QTreeView* mpTreeRodComponents;
    // Data
    Core::HierarchyTree mHierarchyRodComponents;
    mapRodComponents mRodComponents;
};

}

}

#endif // RODCOMPONENTSMANAGER_H
