/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the ProjectHierarchyModel class
 */

#include <QTreeView>
#include "projecthierarchymodel.h"
#include "dataobjectshierarchyitem.h"
#include "rodcomponentshierarchyitem.h"

using namespace QRS::HierarchyModels;
using namespace QRS::Core;

ProjectHierarchyModel::ProjectHierarchyModel(QString const& mimeType, QTreeView* pView)
    : AbstractHierarchyModel(mimeType, pView)
{

}

//! Set a project to represent
void ProjectHierarchyModel::setProject(Project* pProject)
{
    mpProject = pProject;
    updateContent();
}

//! Update all the content
void ProjectHierarchyModel::updateContent()
{
    clearContent();
    if (!mpProject)
        return;
    QStandardItem* pRootItem = invisibleRootItem();
    pRootItem->setFlags(Qt::ItemIsEnabled);
    pRootItem->appendRow(retrieveDataObjectsItem());
    pRootItem->appendRow(retrieveRodComponentsItem());
}

//! Clear all the items
void ProjectHierarchyModel::clearContent()
{
    removeRows(0, rowCount());
}

//! Check if an item selection is correct and if it is not -- correct it
void ProjectHierarchyModel::validateItemSelection()
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

//! Retrieve a representative of data objects
DataObjectsHierarchyItem* ProjectHierarchyModel::retrieveDataObjectsItem()
{
    DataObjects& dataObjects = mpProject->mDataObjects;
    HierarchyTree& hierarchyDataObjects = mpProject->mHierarchyDataObjects;
    QString text = "Data Objects";
    QIcon icon(":/icons/database.svg");
    DataObjectsHierarchyItem* pItem = new DataObjectsHierarchyItem(dataObjects, hierarchyDataObjects, text, icon);
    pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled);
    return pItem;
}

//! Retrieve a representative of rod components
RodComponentsHierarchyItem* ProjectHierarchyModel::retrieveRodComponentsItem()
{
    RodComponents& rodComponents = mpProject->mRodComponents;
    HierarchyTree& hierarchyRodComponents = mpProject->mHierarchyRodComponents;
    QString text = "Rod Components";
    QIcon icon(":/icons/capacitor.svg");
    RodComponentsHierarchyItem* pItem = new RodComponentsHierarchyItem(rodComponents, hierarchyRodComponents, text, icon);
    pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled);
    return pItem;
}
