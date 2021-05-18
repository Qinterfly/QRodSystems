/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the ProjectHierarchyModel class
 */

#include <QTreeView>
#include "projecthierarchymodel.h"

using namespace QRS::HierarchyModels;
using namespace QRS::Core;

ProjectHierarchyModel::ProjectHierarchyModel(Project& project, QTreeView* pView)
    : AbstractHierarchyModel("central/projectHierarchy", pView)
    , mDataObjects(project.mDataObjects)
    , mHierarchyDataObjects(project.mHierarchyDataObjects)
{

}

//! Update all the content
void ProjectHierarchyModel::updateContent()
{

}

//! Clear all the items
void ProjectHierarchyModel::clearContent()
{

}
