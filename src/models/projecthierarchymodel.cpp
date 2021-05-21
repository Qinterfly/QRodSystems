/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the ProjectHierarchyModel class
 */

#include <QTreeView>
#include "projecthierarchymodel.h"
#include "dataobjectshierarchyitem.h"

using namespace QRS::HierarchyModels;
using namespace QRS::Core;

ProjectHierarchyModel::ProjectHierarchyModel(QTreeView* pView)
    : AbstractHierarchyModel("central/projectHierarchy", pView)
{

}

//! Set a project to represent
void ProjectHierarchyModel::setProject(Project* pProject)
{
    mpProject = pProject;
    connect(this, &AbstractHierarchyModel::dataModified, mpProject, &Project::setModified);
    updateContent();
}

//! Update all the content
void ProjectHierarchyModel::updateContent()
{
    clearContent();
    if (!mpProject)
        return;
    QStandardItem* pRootItem = invisibleRootItem();
    // Add a representative of data objects
    DataObjects& dataObjects = mpProject->mDataObjects;
    HierarchyTree& hierarchyDataObjects = mpProject->mHierarchyDataObjects;
    QString text = "Data Objects";
    QIcon icon(":/icons/database.svg");
    DataObjectsHierarchyItem* pHierachyDataObjects = new DataObjectsHierarchyItem(dataObjects, hierarchyDataObjects, text, icon);
    // Insert all the base items linked to each category
    pRootItem->appendRow(pHierachyDataObjects);
}

//! Clear all the items
void ProjectHierarchyModel::clearContent()
{
    removeRows(0, rowCount());
}
