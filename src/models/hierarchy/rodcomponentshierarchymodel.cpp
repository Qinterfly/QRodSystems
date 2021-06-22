/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Definition of the RodComponentsHierarchyModel class
 */

#include <QTreeView>
#include <QMimeData>
#include "rodcomponentshierarchymodel.h"
#include "rodcomponentshierarchyitem.h"
#include "core/abstractrodcomponent.h"
#include "core/hierarchytree.h"

using namespace QRS::HierarchyModels;
using namespace QRS::Core;

RodComponentsHierarchyModel::RodComponentsHierarchyModel(RodComponents& rodComponents, HierarchyTree& hierarchyRodComponents,
                                                         QTreeView* pView)
    : AbstractHierarchyModel("rodcomponentsmanager/hierarchy", pView)
    , mRodComponents(rodComponents)
    , mHierarchyRodComponents(hierarchyRodComponents)
{
    connect(this, &QStandardItemModel::itemChanged, this, &RodComponentsHierarchyModel::renameItem);
    RodComponentsHierarchyModel::updateContent();
}

//! Update all the content
void RodComponentsHierarchyModel::updateContent()
{
    RodComponentsHierarchyModel::clearContent();
    if (isEmpty())
        return;
    QStandardItem* pRootItem = invisibleRootItem();
    AbstractHierarchyItem* pHierarchyItem = new RodComponentsHierarchyItem(mRodComponents, mHierarchyRodComponents);
    int nRows = pHierarchyItem->rowCount();
    for (int i = 0; i != nRows; ++i)
        pRootItem->appendRow(pHierarchyItem->takeChild(i));
    delete pHierarchyItem;
}

//! Clear all the items
void RodComponentsHierarchyModel::clearContent()
{
    removeRows(0, rowCount());
}

//! Check if there are data objcects to represent
inline bool RodComponentsHierarchyModel::isEmpty() const
{
    return mRodComponents.size() == 0 || mHierarchyRodComponents.size() <= 1;
}

//! Rename a rod component after editing
void RodComponentsHierarchyModel::renameItem(QStandardItem* pStandardItem)
{
    RodComponentsHierarchyItem* pItem = (RodComponentsHierarchyItem*)pStandardItem;
    QString newName = pItem->data(Qt::DisplayRole).toString();
    if (pItem->mpRodComponent)
        pItem->mpRodComponent->setName(newName);
    else if (pItem->mpNode->type() == HierarchyNode::NodeType::kDirectory)
        pItem->mpNode->value() = newName;
    emit dataModified(true);
}

//! Select an item by row index
void RodComponentsHierarchyModel::selectItem(int iRow)
{
    if (iRow > invisibleRootItem()->rowCount())
        return;
    RodComponentsHierarchyItem* pItem = (RodComponentsHierarchyItem*)invisibleRootItem()->child(iRow);
    QModelIndex const& selectionIndex = pItem->index();
    QTreeView* pView = (QTreeView*)parent();
    pView->selectionModel()->select(selectionIndex, QItemSelectionModel::SelectionFlag::SelectCurrent);
    AbstractRodComponent const* pRodComponent = pItem->mpRodComponent;
    if (pRodComponent)
        emit selected(pRodComponent->id());
    else
        emit selectionCleared();
}

//! Retrieve a selected rod component
void RodComponentsHierarchyModel::retrieveSelectedItem()
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
    RodComponentsHierarchyItem* pItem = (RodComponentsHierarchyItem*)itemFromIndex(indices[0]);
    AbstractRodComponent* pRodComponent = pItem->mpRodComponent;
    if (pRodComponent)
        emit selected(pRodComponent->id());
    else
        emit selectionCleared();
}

//! Remove rod components under selection
void RodComponentsHierarchyModel::removeSelectedItems()
{
    QTreeView* pView = (QTreeView*)parent();
    if (pView->selectionModel()->selection().isEmpty())
        return;
    QModelIndexList indices = pView->selectionModel()->selectedIndexes();
    for (QModelIndex const& index : indices)
    {
        RodComponentsHierarchyItem* pItem = (RodComponentsHierarchyItem*)itemFromIndex(index);
        AbstractRodComponent* pRodComponent = pItem->mpRodComponent;
        if (pRodComponent)
        {
            mRodComponents.erase(pRodComponent->id());
            delete pRodComponent;
        }
        mHierarchyRodComponents.removeNode(pItem->mpNode);
    }
    updateContent();
    emit dataModified(true);
    emit selectionCleared();
}


