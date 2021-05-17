/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the AbstractHierarchyModel class
 */

#include <QTreeView>
#include <QMimeData>
#include "abstracthierarchymodel.h"
#include "abstracthierarchyitem.h"
#include "core/hierarchynode.h"

using namespace QRS;

static uint sNumFolders = 0;
static const QString skBaseFolderName = "Group ";

AbstractHierarchyModel::AbstractHierarchyModel(QString const& mimeType, QTreeView* pView)
    : QStandardItemModel(pView)
    , kMimeType(mimeType)
{

}

AbstractHierarchyModel::~AbstractHierarchyModel()
{

}

//! Specify allowed drop actions
Qt::DropActions AbstractHierarchyModel::supportedDropActions() const
{
    return Qt::CopyAction;
}

//! Specify allowed drag actions
Qt::DropActions AbstractHierarchyModel::supportedDragActions() const
{
    return Qt::CopyAction;
}

//! Retrieve the mime types
QStringList AbstractHierarchyModel::mimeTypes() const
{
    return QStringList() << kMimeType;
}

//! Encode each item according to a given list of indicies
QMimeData* AbstractHierarchyModel::mimeData(const QModelIndexList& indicies) const
{
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    stream << indicies.count();
    for (QModelIndex const& index : indicies)
    {
        if (index.isValid())
        {
            AbstractHierarchyItem* pItem = (AbstractHierarchyItem*)itemFromIndex(index);
            pItem->writePointer(stream);
        }
    }
    QMimeData* pMimeData = new QMimeData();
    pMimeData->setData(kMimeType, encodedData);
    return pMimeData;
}

//! Process the drop action
bool AbstractHierarchyModel::dropMimeData(QMimeData const* pMimeData, Qt::DropAction action, int row, int /*column*/, const QModelIndex& indexParent)
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
bool AbstractHierarchyModel::processDropOnItem(QDataStream& stream, int& numItems, QModelIndex const& indexParent)
{
    // Read the first item
    AbstractHierarchyItem* pDropItem = AbstractHierarchyItem::readPointer(stream);
    --numItems;
    HierarchyNode* pDropNode = pDropItem->mpNode;
    AbstractHierarchyItem* pParentItem = (AbstractHierarchyItem*)itemFromIndex(indexParent);
    HierarchyNode* pParentNode = pParentItem->mpNode;
    HierarchyNode* pResNode = pParentNode->groupNodes(pDropNode);
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
        pDropItem = AbstractHierarchyItem::readPointer(stream);
        pDropNode = pDropItem->mpNode;
        pResNode->groupNodes(pDropNode);
        --numItems;
    }
    return true;
}

//! Change the order of items
bool AbstractHierarchyModel::processDropBetweenItems(QDataStream& stream, int& numItems, QModelIndex const& indexParent, int row)
{
    // Process the first item differently to recognize the pattern
    AbstractHierarchyItem* pDropItem = AbstractHierarchyItem::readPointer(stream);
    --numItems;
    HierarchyNode* pDropNode = pDropItem->mpNode;
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
    AbstractHierarchyItem* pCurrentItem = (AbstractHierarchyItem*)pParentItem->child(iCurrentRow);
    HierarchyNode* pCurrentNode = pCurrentItem->mpNode;
    bool isSuccess;
    if (isSetAfter)
        isSuccess = pCurrentNode->setAfter(pDropNode);
    else
        isSuccess = pCurrentNode->setBefore(pDropNode);
    if (!isSuccess)
        return false;
    pCurrentNode = pDropNode;
    // Set the rest of items after the last one
    while (numItems > 0)
    {
        pDropItem = AbstractHierarchyItem::readPointer(stream);
        pDropNode = pDropItem->mpNode;
        pCurrentNode->setAfter(pDropNode);
        pCurrentNode = pDropNode;
        --numItems;
    }
    return true;
}
