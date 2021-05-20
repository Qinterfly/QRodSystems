/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the ProjectHierarchyModel class
 */

#ifndef PROJECTHIERARCHYMODEL_H
#define PROJECTHIERARCHYMODEL_H

#include "models/abstracthierarchymodel.h"
#include "core/datatypes.h"
#include "core/project.h"

namespace QRS::HierarchyModels
{

//! Project hierarchy representative
class ProjectHierarchyModel : public AbstractHierarchyModel
{
public:
    ProjectHierarchyModel(QTreeView* pView = nullptr);
    void updateContent() override;
    void clearContent() override;
    void setProject(Core::Project* pProject);

private:
    Core::Project* mpProject = nullptr;
};

}

#endif // PROJECTHIERARCHYMODEL_H
