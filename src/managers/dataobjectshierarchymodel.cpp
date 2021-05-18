/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the DataObjectsHierarchyModel class
 */

#include <QTreeView>
#include <QMimeData>
#include "dataobjectshierarchymodel.h"
#include "dataobjectshierarchyitem.h"
#include "core/abstractdataobject.h"
#include "core/hierarchytree.h"

using namespace QRS::HierarchyModels;
using namespace QRS::Core;

DataObjectsHierarchyModel::DataObjectsHierarchyModel(mapDataObjects& dataObjects, HierarchyTree& hierarchyDataObjects, QTreeView* pView)
    : AbstractHierarchyModel("dataobjectsmanager/hierarchy", pView)
    , mDataObjects(dataObjects)
    , mHierarchyDataObjects(hierarchyDataObjects)
{
    connect(this, &QStandardItemModel::itemChanged, this, &DataObjectsHierarchyModel::renameDataObject);
    DataObjectsHierarchyModel::updateContent();
}

//! Update all the content
void DataObjectsHierarchyModel::updateContent()
{
    clearContent();
    if (isEmpty())
        return;
    QStandardItem* pRootItem = invisibleRootItem();
    AbstractHierarchyItem* pHierarchyItem = new DataObjectsHierarchyItem(mDataObjects, mHierarchyDataObjects);
    int nRows = pHierarchyItem->rowCount();
    for (int i = 0; i != nRows; ++i)
        pRootItem->appendRow(pHierarchyItem->takeChild(i));
    delete pHierarchyItem;
}

//! Clear all
void DataObjectsHierarchyModel::clearContent()
{
    removeRows(0, rowCount());
    emit selectionCleared();
}

//! Check if there are data objcects to represent
inline bool DataObjectsHierarchyModel::isEmpty() const
{
    return mDataObjects.size() == 0 || mHierarchyDataObjects.size() <= 1;
}

//! Rename a data object after editing
void DataObjectsHierarchyModel::renameDataObject(QStandardItem* pStandardItem)
{
    DataObjectsHierarchyItem* pItem = (DataObjectsHierarchyItem*)pStandardItem;
    QString newName = pItem->data(Qt::DisplayRole).toString();
    if (pItem->mpDataObject)
        pItem->mpDataObject->setName(newName);
    else if (pItem->mpNode->type() == HierarchyNode::NodeType::kDirectory)
        pItem->mpNode->value() = newName;
    emit dataModified(true);
}

//! Retrieve a selected data object
void DataObjectsHierarchyModel::retrieveSelectedDataObject()
{
    QTreeView* pView = (QTreeView*)parent();
    if (pView->selectionModel()->selection().isEmpty())
    {
        emit selectionCleared();
        return;
    }
    QModelIndexList indices = pView->selectionModel()->selectedIndexes();
    if (indices.count() > 1)
    {
        emit selectionCleared();
        return;
    }
    DataObjectsHierarchyItem* pItem = (DataObjectsHierarchyItem*)itemFromIndex(indices[0]);
    AbstractDataObject* pDataObject = pItem->mpDataObject;
    if (pDataObject)
        emit dataObjectSelected(pDataObject->id());
    else
        emit selectionCleared();
}

//! Select an item by row index
void DataObjectsHierarchyModel::selectItem(int iRow)
{
    if (iRow > invisibleRootItem()->rowCount())
        return;
    DataObjectsHierarchyItem* pItem = (DataObjectsHierarchyItem*)invisibleRootItem()->child(iRow);
    QModelIndex const& selectionIndex = pItem->index();
    QTreeView* pView = (QTreeView*)parent();
    pView->selectionModel()->select(selectionIndex, QItemSelectionModel::SelectionFlag::SelectCurrent);
    AbstractDataObject const* pDataObject = pItem->mpDataObject;
    if (pDataObject)
        emit dataObjectSelected(pDataObject->id());
    else
        emit selectionCleared();
}

//! Remove data objects under selection
void DataObjectsHierarchyModel::removeSelectedItems()
{
    QTreeView* pView = (QTreeView*)parent();
    if (pView->selectionModel()->selection().isEmpty())
        return;
    QModelIndexList indices = pView->selectionModel()->selectedIndexes();
    for (QModelIndex const& index : indices)
    {
        DataObjectsHierarchyItem* pItem = (DataObjectsHierarchyItem*)itemFromIndex(index);
        AbstractDataObject* pDataObject = pItem->mpDataObject;
        if (pDataObject)
            mDataObjects.erase(pDataObject->id());
        mHierarchyDataObjects.removeNode(pItem->mpNode);
    }
    updateContent();
    emit dataModified(true);
    emit selectionCleared();
}
