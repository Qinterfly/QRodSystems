/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the DataObjectsHierarchyModel class
 */

#include <QTreeView>
#include <QMimeData>
#include <QCoreApplication>
#include "dataobjectshierarchymodel.h"
#include "dataobjectshierarchyitem.h"
#include "core/abstractdataobject.h"
#include "core/hierarchytree.h"

using namespace QRS;

static uint sNumFolders = 0;
static const QString skBaseFolderName = "Group ";

DataObjectsHierarchyModel::DataObjectsHierarchyModel(mapDataObjects& dataObjects, QRS::HierarchyTree& hierarchyDataObjects, QTreeView* pView)
    : AbstractHierarchyModel("dataobjectsmanager/hierarchy", pView)
    , mDataObjects(dataObjects)
    , mHierarchyDataObjects(hierarchyDataObjects)
{
    connect(this, &QStandardItemModel::itemChanged, this, &DataObjectsHierarchyModel::renameDataObject);
    updateContent();
}

//! Update all the content
void DataObjectsHierarchyModel::updateContent()
{
    clearContent();
    if (isEmpty())
        return;
    QStandardItem* pRootItem = invisibleRootItem();
    HierarchyItem* pHierarchyItem = new DataObjectsHierarchyItem(mDataObjects, mHierarchyDataObjects);
    int nRows = pHierarchyItem->rowCount();
    for (int i = 0; i != nRows; ++i)
        pRootItem->appendRow(pHierarchyItem->takeChild(i));
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

//! Process the drop action
bool DataObjectsHierarchyModel::dropMimeData(QMimeData const* pMimeData, Qt::DropAction action, int row, int /*column*/, const QModelIndex& indexParent)
{
    if (!pMimeData->hasFormat(kMimeType))
        return false;
    if (action == Qt::IgnoreAction)
        return true;
    QByteArray encodedData = pMimeData->data(kMimeType);
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    int numItems;
    stream >> numItems;
    bool isDroppedOnView = row != -1;
    if (numItems <= 0)
        return false;
    bool isProcessed = false;
    if (isDroppedOnView)
        isProcessed = processDropBetweenItems(stream, numItems, indexParent, row);
    else if (indexParent.isValid())
        isProcessed = processDropOnItem(stream, numItems, indexParent);
    if (isProcessed)
    {
        updateContent();
        emit dataModified(true);
    }
    return false;
}

//! Merge several items into one entity
bool DataObjectsHierarchyModel::processDropOnItem(QDataStream& stream, int& numItems, QModelIndex const& indexParent)
{
    // Read the first item
    DataObjectsHierarchyItem dropItem;
    dropItem.read(stream);
    --numItems;
    HierarchyNode* pDropNode = dropItem.mpNode;
    DataObjectsHierarchyItem* pParentItem = (DataObjectsHierarchyItem*)itemFromIndex(indexParent);
    HierarchyNode* pParentNode = pParentItem->mpNode;
    HierarchyNode* pResNode = mHierarchyDataObjects.groupNodes(pParentNode, pDropNode);
    if (!pResNode)
        return false;
    /*
     When an item is dropped on another item, we create a folder first and then insert both items into it.
     Otherwise, the item is inserted into the existing folder.
    */
    if (pParentNode != pResNode && pResNode->type() == HierarchyNode::NodeType::kDirectory)
    {
        ++sNumFolders;
        QVariant varFolder = skBaseFolderName + QString::number(sNumFolders);
        pResNode->value() = varFolder;
    }
    // Insert other items into the created folder
    while (numItems > 0)
    {
        dropItem.read(stream);
        pDropNode = dropItem.mpNode;
        mHierarchyDataObjects.groupNodes(pResNode, pDropNode);
        --numItems;
    }
    return true;
}

//! Change the order of items
bool DataObjectsHierarchyModel::processDropBetweenItems(QDataStream& stream, int& numItems, QModelIndex const& indexParent, int row)
{
    // Process the first item differently to recognize the pattern
    DataObjectsHierarchyItem dropItem;
    dropItem.read(stream);
    --numItems;
    HierarchyNode* pDropNode = dropItem.mpNode;
    QStandardItem* pParentItem = nullptr;
    // Retrieve the parental item
    if (indexParent.isValid())
        pParentItem = itemFromIndex(indexParent);
    else
        pParentItem = invisibleRootItem();
    int numRows = pParentItem->rowCount();
    if (numRows < 1)
        return false;
    bool isSetAfter = row != 0;
    int iCurrentRow = isSetAfter ? row - 1 : 0;
    DataObjectsHierarchyItem* pCurrentItem = (DataObjectsHierarchyItem*)pParentItem->child(iCurrentRow);
    HierarchyNode* pCurrentNode = pCurrentItem->mpNode;
    if (isSetAfter)
        mHierarchyDataObjects.setAfter(pCurrentNode, pDropNode);
    else
        mHierarchyDataObjects.setBefore(pCurrentNode, pDropNode);
    pCurrentNode = pDropNode;
    // Set the rest of items after the last one
    while (numItems > 0)
    {
        dropItem.read(stream);
        pDropNode = dropItem.mpNode;
        mHierarchyDataObjects.setAfter(pCurrentNode, pDropNode);
        pCurrentNode = pDropNode;
        --numItems;
    }
    return true;
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
