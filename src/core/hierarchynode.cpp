/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Implementation of the HierarchyNode class
 */

#include "hierarchynode.h"

using namespace QRS::Core;

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
        node->excludeNodeFromHierarchy();
        HierarchyNode* pLastNode = mpFirstChild;
        while (pLastNode->mpNextSibling)
            pLastNode = pLastNode->mpNextSibling;
        pLastNode->mpNextSibling = node;
        node->mpPreviousSibling = pLastNode;
        node->mpNextSibling = nullptr;
    }
    node->mpParent = this;
}

//! Merge two nodes into one entity
HierarchyNode* HierarchyNode::groupNodes(HierarchyNode* pChildNode)
{
    QString const kNameDirectory = "Group";
    if (!pChildNode || this == pChildNode || isParentOf(pChildNode) || pChildNode->isParentOf(this))
        return nullptr;
    switch (mType)
    {
    case HierarchyNode::NodeType::kDirectory:
        pChildNode->excludeNodeFromHierarchy();
        appendChild(pChildNode);
        return this;
        break;
    case HierarchyNode::NodeType::kObject:
    {
        HierarchyNode* pDirectory = new HierarchyNode(HierarchyNode::NodeType::kDirectory, kNameDirectory);
        // Initialize a directory by base node
        pDirectory->mpParent = mpParent;
        pDirectory->mpNextSibling = mpNextSibling;
        pDirectory->mpPreviousSibling = mpPreviousSibling;
        // Switch the newly created directory with the base node
        if (mpParent && mpParent->mpFirstChild == this)
            mpParent->mpFirstChild = pDirectory;
        if (mpNextSibling)
            mpNextSibling->mpPreviousSibling = pDirectory;
        if (mpPreviousSibling)
            mpPreviousSibling->mpNextSibling = pDirectory;
        // Insert the base node into the directory
        mpParent = nullptr;
        mpPreviousSibling = nullptr;
        mpNextSibling = nullptr;
        pDirectory->appendChild(this);
        // Insert the child into the directory
        pChildNode->excludeNodeFromHierarchy();
        pDirectory->appendChild(pChildNode);
        return pDirectory;
        break;
    }
    default:
        return nullptr;
    }
}

//! Set a given node before the current one
bool HierarchyNode::setBefore(HierarchyNode* pSetNode)
{
    if (!isSetAllowed(pSetNode))
        return false;
    pSetNode->excludeNodeFromHierarchy();
    if (mpParent && mpParent->mpFirstChild == this)
        mpParent->mpFirstChild = pSetNode;
    pSetNode->mpParent = mpParent;
    pSetNode->mpNextSibling = this;
    pSetNode->mpPreviousSibling = mpPreviousSibling;
    if (mpPreviousSibling)
        mpPreviousSibling->mpNextSibling = pSetNode;
    mpPreviousSibling = pSetNode;
    return true;
}

//! Set a given node after the current one
bool HierarchyNode::setAfter(HierarchyNode* pSetNode)
{
    if (!isSetAllowed(pSetNode))
        return false;
    pSetNode->excludeNodeFromHierarchy();
    pSetNode->mpParent = mpParent;
    pSetNode->mpPreviousSibling = this;
    pSetNode->mpNextSibling = mpNextSibling;
    if (mpNextSibling)
        mpNextSibling->mpPreviousSibling = pSetNode;
    mpNextSibling = pSetNode;
    return true;
}

//! Retrieve a number of children of the current node
quint32 HierarchyNode::numberChildren() const
{
    quint32 numberNodes = 0;
    return countNodes(mpFirstChild, numberNodes);
}

//! Check whether it is possible to place a given item before or after the current one
bool HierarchyNode::isSetAllowed(HierarchyNode const* pNode) const
{
    return pNode && this != pNode && !pNode->isParentOf(this);
}

//! Remove all links to the node
void HierarchyNode::excludeNodeFromHierarchy()
{
    if (mpParent && mpParent->mpFirstChild == this)
        mpParent->mpFirstChild = mpNextSibling;
    if (mpNextSibling)
        mpNextSibling->mpPreviousSibling = mpPreviousSibling;
    if (mpPreviousSibling)
        mpPreviousSibling->mpNextSibling = mpNextSibling;
    mpParent = nullptr;
    mpNextSibling = nullptr;
    mpPreviousSibling = nullptr;
}

//! Check whether the current item containes a given node as a child
bool HierarchyNode::isParentOf(HierarchyNode const* pNode) const
{
    HierarchyNode* pParentNode = pNode->mpParent;
    while (pParentNode)
    {
        if (pParentNode == this)
            return true;
        pParentNode = pParentNode->mpParent;
    }
    return false;
}

//! Count all children and siblings of a given node
quint32 HierarchyNode::countNodes(HierarchyNode* pNode, quint32& numNodes) const
{
    while (pNode)
    {
        ++numNodes;
        if (pNode->mpFirstChild)
            countNodes(pNode->mpFirstChild, numNodes);
        pNode = pNode->mpNextSibling;
    }
    return numNodes;
}
