/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the ProjectHierarchyModel class
 */

#include <QTreeView>
#include "projecthierarchymodel.h"
#include "managers/dataobjectshierarchyitem.h"

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
    DataObjectsHierarchyItem* pHierachyDataObjects = new DataObjectsHierarchyItem(dataObjects, hierarchyDataObjects, "Data Objects");
    // Insert all the base items linked to each category
    pRootItem->appendRow(pHierachyDataObjects);
    // Create a font for base items
    QFont baseFont = pRootItem->font();
    baseFont.setUnderline(true);
    // Expand all the categories
    QTreeView* pView = (QTreeView*)parent();
    int nRows = rowCount();
    for (int i = 0; i != nRows; ++i)
    {
        QStandardItem* pBaseItem = item(i);
        pBaseItem->setFont(baseFont);
        pView->expand(pBaseItem->index());
    }
}

//! Clear all the items
void ProjectHierarchyModel::clearContent()
{
    removeRows(0, rowCount());
}
