/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
 * \brief Implementation of the HierarchyNode class
 */

#include "hierarchynode.h"

using namespace QRS;

//! Node constructor
HierarchyNode::HierarchyNode(NodeType type, QVariant value)
    : mType(type)
    , mValue(value)
{

}

//! Add a child node
void HierarchyNode::appendChild(HierarchyNode* node)
{
    if (mType != NodeType::kDirectory)
    {
        delete node;
        return;
    }
    if (!mpFirstChild)
    {
        mpFirstChild = node;
    }
    else
    {
        HierarchyNode* pLastNode = mpFirstChild;
        while (pLastNode->mpNextSibling)
            pLastNode = pLastNode->mpNextSibling;
        pLastNode->mpNextSibling = node;
    }
    node->mpParent = this;
}
