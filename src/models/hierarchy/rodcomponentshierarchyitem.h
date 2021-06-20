/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the RodComponentsHierarchyItem class
 */

#ifndef RODCOMPONENTSHIERARCHYITEM_H
#define RODCOMPONENTSHIERARCHYITEM_H

#include "models/hierarchy/abstracthierarchyitem.h"
#include "core/aliasdataset.h"

namespace QRS
{

namespace HierarchyModels
{

//! Item to represent a hierarchy of rod components
class RodComponentsHierarchyItem : public AbstractHierarchyItem
{
public:
    RodComponentsHierarchyItem(Core::RodComponents& rodComponents, Core::HierarchyTree& hierarchyRodComponents,
                               QString const& text = "Root", QIcon const& icon = QIcon());
    RodComponentsHierarchyItem(Core::HierarchyNode* pNode, Core::AbstractRodComponent* pRodComponent);
    RodComponentsHierarchyItem(Core::HierarchyNode* pNode);
    int type() const override { return AbstractHierarchyItem::ItemType::kRodComponents; }

private:
    void appendItems(Core::RodComponents& rodComponents, Core::HierarchyNode* pNode);

private:
    Core::AbstractRodComponent* mpRodComponent = nullptr;
};

}

}

#endif // RODCOMPONENTSHIERARCHYITEM_H
