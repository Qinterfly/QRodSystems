/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
 * \brief Implementation of the HierarchyNode and HierarchyTree classes
 */

#include "hierarchytree.h"

using namespace QRS;

static const QString kRootName = "Root";

//! Tree constructor
HierarchyTree::HierarchyTree()
{
    mpRootNode = new HierarchyNode(HierarchyNode::NodeType::kDirectory, kRootName);
}

//! Tree destructor
HierarchyTree::~HierarchyTree()
{
    removeNode(mpRootNode);
}

//! Delete all nodes except the root node
void HierarchyTree::clear()
{
    removeChildren(mpRootNode);
}

//! Append a node to the root node
void HierarchyTree::appendNode(HierarchyNode* pNode)
{
    mpRootNode->appendChild(pNode);
}

//! Remove a node by type and value
bool HierarchyTree::removeNode(HierarchyNode::NodeType type, QVariant const& value)
{
    HierarchyNode* pNode = find(mpRootNode, type, value);
    if (!pNode)
        return false;
    removeNode(pNode);
    return true;
}

//! Find a node by type and value
HierarchyNode* HierarchyTree::find(HierarchyNode* pBaseNode, HierarchyNode::NodeType type, QVariant const& value) const
{
    HierarchyNode* pNextNode;
    HierarchyNode* pFoundNode = nullptr;
    while (pBaseNode)
    {
        pNextNode = pBaseNode->mpNextSibling;
        if (pBaseNode->mpFirstChild)
        {
            pFoundNode = find(pBaseNode->mpFirstChild, type, value);
            if (pFoundNode)
                break;
        }
        if (pBaseNode->mType == type && pBaseNode->mValue == value)
        {
            pFoundNode = pBaseNode;
            break;
        }
        pBaseNode = pNextNode;
    }
    return pFoundNode;
}

//! Remove all subnodes
void HierarchyTree::removeChildren(HierarchyNode* pNode)
{
    HierarchyNode* pNextNode;
    while (pNode)
    {
        pNextNode = pNode->mpNextSibling;
        if (pNode->mpFirstChild)
            removeChildren(pNode->mpFirstChild);
        delete pNode;
        pNode = pNextNode;
    }
}

//! Remove a node and all its subnodes
void HierarchyTree::removeNode(HierarchyNode* pNode)
{
    removeChildren(pNode->mpFirstChild);
    if (pNode->mpParent && pNode->mpParent->mpFirstChild == pNode)
        pNode->mpParent->mpFirstChild = pNode->mpNextSibling;
    delete pNode;
}

//! Change the value of a node
void HierarchyTree::changeNodeValue(HierarchyNode::NodeType type, QVariant const& oldValue, QVariant const& newValue)
{
    HierarchyNode* pNode = find(mpRootNode, type, oldValue);
    if (!pNode)
        return;
    pNode->mValue = newValue;
}

//! Print a current node and all its subnodes
void HierarchyTree::printNode(uint level, HierarchyNode* pNode, QDebug stream)
{
    HierarchyNode* pNextNode;
    QString nodeIndentation;
    if (level > 0)
        nodeIndentation = '|' + QString('-').repeated(level);
    while (pNode)
    {
        pNextNode = pNode->mpNextSibling;
        stream << nodeIndentation + pNode->mValue.toString() << Qt::endl;
        if (pNode->mpFirstChild)
            printNode(level + 1, pNode->mpFirstChild, stream);
        pNode = pNextNode;
    }
}

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
