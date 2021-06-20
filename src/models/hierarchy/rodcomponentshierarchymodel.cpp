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
