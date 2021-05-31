/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the RodComponentsManager class
 */

#ifndef RODCOMPONENTSMANAGER_H
#define RODCOMPONENTSMANAGER_H

#include "managers/abstractprojectmanager.h"
#include "core/hierarchytree.h"

namespace ads
{
class CDockWidget;
}

namespace QRS
{

namespace Managers
{

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
    void createContent();
    QLayout* createDialogControls();

private:
    ads::CDockWidget* mpComponentDockWidget;
    Core::HierarchyTree mHierarchyRodComponents;
};

}

}

#endif // RODCOMPONENTSMANAGER_H
