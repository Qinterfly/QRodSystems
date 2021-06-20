/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Definition of the RodComponentsHierarchyItem class
 */

#include "rodcomponentshierarchyitem.h"
#include "core/abstractrodcomponent.h"
#include "core/hierarchytree.h"

using namespace QRS::HierarchyModels;
using namespace QRS::Core;

QIcon getDataObjectIcon(AbstractRodComponent::ComponentType type);

//! Create the representative of the structure of rod components
RodComponentsHierarchyItem::RodComponentsHierarchyItem(RodComponents& rodComponents, HierarchyTree& hierarchyRodComponents,
                                                       QString const& text, QIcon const& icon)
    : AbstractHierarchyItem(icon, text, hierarchyRodComponents.root())
{
    if (!mpNode->hasChild())
        return;
    appendItems(rodComponents, mpNode->firstChild());
}

//! Create items based on the position in the tree structure
void RodComponentsHierarchyItem::appendItems(RodComponents& rodComponents, HierarchyNode* pNode)
{
    HierarchyNode* pNextNode;
    while (pNode)
    {
        RodComponentsHierarchyItem* pItem = nullptr;
        switch (pNode->type())
        {
        case HierarchyNode::NodeType::kDirectory:
            pItem = new RodComponentsHierarchyItem(pNode);
            break;
        case HierarchyNode::NodeType::kObject:
            DataIDType id = pNode->value().toUInt();
            if (!rodComponents.contains(id))
                return;
            pItem = new RodComponentsHierarchyItem(pNode, rodComponents[id]);
            break;
        }
        if (pItem)
        {
            appendRow(pItem);
            if (pNode->hasChild())
                pItem->appendItems(rodComponents, pNode->firstChild());
        }
        pNextNode = pNode->nextSibling();
        pNode = pNextNode;
    }
}

//! Construct an item to represent a rod component
RodComponentsHierarchyItem::RodComponentsHierarchyItem(HierarchyNode* pNode, AbstractRodComponent* pRodComponent)
    : AbstractHierarchyItem(getDataObjectIcon(pRodComponent->componentType()), pRodComponent->name(), pNode)
    , mpRodComponent(pRodComponent)
{
    setFlags(flags() | Qt::ItemIsEditable);
}

//! Construct an item to represent a directory
RodComponentsHierarchyItem::RodComponentsHierarchyItem(HierarchyNode* pNode)
    : AbstractHierarchyItem(QIcon(":/icons/folder.svg"), pNode->value().toString(), pNode)
{
    setFlags(flags() | Qt::ItemIsEditable);
}

//! Helper function to assign an appropriate rod component icon
QIcon getDataObjectIcon(AbstractRodComponent::ComponentType type)
{
    switch (type)
    {
    case AbstractRodComponent::ComponentType::kGeometry:
        return QIcon(":/icons/axis.svg");
    default:
        return QIcon();
    }
}
