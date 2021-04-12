/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
 * \brief Implementation of the HierarchyTree class
 */

#include "hierarchytree.h"

using namespace QRS;

static const QString kRootName = "Root";

//! Base tree constructor
HierarchyTree::HierarchyTree()
{
    mpRootNode = new HierarchyNode(HierarchyNode::NodeType::kDirectory, kRootName);
}

//! Take the user defined node as the root
HierarchyTree::HierarchyTree(HierarchyNode* pRootNode)
{
    mpRootNode = pRootNode;
}

//! Copy assignment operator
HierarchyTree& HierarchyTree::operator=(HierarchyTree const& another)
{
    removeNode(mpRootNode);
    mpRootNode = copyNode(another.mpRootNode, 0);
    return *this;
}

//! Tree destructor
HierarchyTree::~HierarchyTree()
{
    removeNode(mpRootNode);
}

//! Delete all nodes except the root node
void HierarchyTree::clear()
{
    removeNodeSiblings(mpRootNode->mpFirstChild);
}

//! Append a node to the root node
void HierarchyTree::appendNode(HierarchyNode* pNode)
{
    mpRootNode->appendChild(pNode);
}

//! Remove a node by type and value
bool HierarchyTree::removeNode(HierarchyNode::NodeType type, QVariant const& value)
{
    HierarchyNode* pNode = findNode(mpRootNode, type, value);
    if (!pNode)
        return false;
    removeNode(pNode);
    return true;
}

//! Change the value of a node
void HierarchyTree::changeNodeValue(HierarchyNode::NodeType type, QVariant const& oldValue, QVariant const& newValue)
{
    HierarchyNode* pNode = findNode(mpRootNode, type, oldValue);
    if (!pNode)
        return;
    pNode->mValue = newValue;
}

//! Clone a tree
HierarchyTree HierarchyTree::clone() const
{
    return HierarchyTree(copyNode(mpRootNode, 0));
}

//! Find a node by type and value
HierarchyNode* HierarchyTree::findNode(HierarchyNode* pBaseNode, HierarchyNode::NodeType type, QVariant const& value) const
{
    HierarchyNode* pNextNode;
    HierarchyNode* pFoundNode = nullptr;
    while (pBaseNode)
    {
        pNextNode = pBaseNode->mpNextSibling;
        if (pBaseNode->mpFirstChild)
        {
            pFoundNode = findNode(pBaseNode->mpFirstChild, type, value);
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

//! Copy a node
HierarchyNode* HierarchyTree::copyNode(HierarchyNode* pBaseNode, uint relativeLevel) const
{
    HierarchyNode* pNewNode;
    HierarchyNode* pFirstNewNode = nullptr;
    HierarchyNode* pPrevNewNode = nullptr;
    HierarchyNode* pNode = pBaseNode;
    bool isFirst = true;
    while (pNode)
    {
        pNewNode = new HierarchyNode(pNode->mType, pNode->mValue);
        if (pNode->mpFirstChild)
        {
            pNewNode->mpFirstChild = copyNode(pNode->mpFirstChild, relativeLevel + 1);
            pNewNode->mpFirstChild->mpParent = pNewNode;
        }
        if (isFirst)
        {
            isFirst = !isFirst;
            pFirstNewNode = pNewNode;
        }
        if (relativeLevel == 0)
            break;
        pNode = pNode->mpNextSibling;
        if (pPrevNewNode)
            pPrevNewNode->mpNextSibling = pNewNode;
        pPrevNewNode = pNewNode;
    }
    return pFirstNewNode;
}

//! Remove a node and all its subnodes
void HierarchyTree::removeNode(HierarchyNode* pNode)
{
    removeNodeSiblings(pNode->mpFirstChild);
    if (pNode->mpParent && pNode->mpParent->mpFirstChild == pNode)
        pNode->mpParent->mpFirstChild = pNode->mpNextSibling;
    delete pNode;
}

//! Remove all subnodes
void HierarchyTree::removeNodeSiblings(HierarchyNode* pNode)
{
    HierarchyNode* pNextNode;
    while (pNode)
    {
        pNextNode = pNode->mpNextSibling;
        if (pNode->mpFirstChild)
            removeNodeSiblings(pNode->mpFirstChild);
        delete pNode;
        pNode = pNextNode;
    }
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


