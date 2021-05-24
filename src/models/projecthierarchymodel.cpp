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
    connect(this, &ProjectHierarchyModel::itemChanged, this, &ProjectHierarchyModel::renameItem);
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
    pHierachyDataObjects->setFlags(Qt::ItemIsEnabled);
    // Insert all the base items linked to each category
    pRootItem->appendRow(pHierachyDataObjects);
}

//! Clear all the items
void ProjectHierarchyModel::clearContent()
{
    removeRows(0, rowCount());
}

//! Check if an item selection is correct and if it is not -- correct it
void ProjectHierarchyModel::validateItemSelection()
{
    {
        QTreeView* pView = (QTreeView*) parent();
        QItemSelectionModel* selectionModel = pView->selectionModel();
        QSignalBlocker const signalBlocker(selectionModel);
        QModelIndexList indices = selectionModel->selectedIndexes();
        if (indices.isEmpty())
            return;
        AbstractHierarchyItem* pItem = (AbstractHierarchyItem*)itemFromIndex(indices[0]);
        int mainType = pItem->type();
        int numSelection = indices.size();
        // Add the first item by default
        QVector<AbstractHierarchyItem*> validatedItems;
        validatedItems.push_back(pItem);
        // Deselect the items which have types different to the main one
        for (int i = 1; i != numSelection; ++i)
        {
            pItem = (AbstractHierarchyItem*)itemFromIndex(indices[i]);
            if (pItem->type() == mainType)
                validatedItems.push_back(pItem);
            else
                selectionModel->select(pItem->index(), QItemSelectionModel::SelectionFlag::Deselect);
        }
        emit selectionValidated(validatedItems);
    }
}

//! Rename an item
void ProjectHierarchyModel::renameItem(QStandardItem* pStandardItem)
{
    switch (pStandardItem->type())
    {
    case kDataObjects:
    {
        DataObjectsHierarchyItem* pItem = (DataObjectsHierarchyItem*)pStandardItem;
        QString newName = pItem->data(Qt::DisplayRole).toString();
        if (pItem->mpDataObject)
            pItem->mpDataObject->setName(newName);
        else if (pItem->mpNode->type() == HierarchyNode::NodeType::kDirectory)
            pItem->mpNode->value() = newName;
        emit dataModified(true);
        emit selectionValidated({(AbstractHierarchyItem*)pItem});
        break;
    }
    }
}

