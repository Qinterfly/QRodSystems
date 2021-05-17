/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
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

//! Move assignment operator
HierarchyTree& HierarchyTree::operator=(HierarchyTree&& another)
{
    if (this != &another)
    {
        removeNode(mpRootNode);
        mpRootNode = another.mpRootNode;
        another.mpRootNode = nullptr;
    }
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
    HierarchyNode* pFoundNode = nullptr;
    while (pBaseNode)
    {
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
        pBaseNode = pBaseNode->mpNextSibling;
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
            HierarchyNode* pChild = pNewNode->mpFirstChild;
            while (pChild)
            {
                pChild->mpParent = pNewNode;
                pChild = pChild->mpNextSibling;
            }
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
        {
            pPrevNewNode->mpNextSibling = pNewNode;
            pNewNode->mpPreviousSibling = pPrevNewNode;
        }
        pPrevNewNode = pNewNode;
    }
    return pFirstNewNode;
}

//! Remove a node and all its subnodes
void HierarchyTree::removeNode(HierarchyNode* pNode)
{
    if (!pNode)
        return;
    removeNodeSiblings(pNode->mpFirstChild);
    excludeNodeFromHierarchy(pNode);
    delete pNode;
}

//! Remove all links to a given node
void HierarchyTree::excludeNodeFromHierarchy(HierarchyNode* pNode)
{
    HierarchyNode* pPreviousNode = pNode->mpPreviousSibling;
    HierarchyNode* pNextNode = pNode->mpNextSibling;
    if (pNode->mpParent && pNode->mpParent->mpFirstChild == pNode)
        pNode->mpParent->mpFirstChild = pNextNode;
    if (pNextNode)
        pNextNode->mpPreviousSibling = pNode->mpPreviousSibling;
    if (pPreviousNode)
        pPreviousNode->mpNextSibling = pNode->mpNextSibling;
}

//! Remove all subnodes
void HierarchyTree::removeNodeSiblings(HierarchyNode* pNode)
{
    HierarchyNode* pNextNode;
    while (pNode)
    {
        if (pNode->mpFirstChild)
            removeNodeSiblings(pNode->mpFirstChild);
        pNextNode = pNode->mpNextSibling;
        delete pNode;
        pNode = pNextNode;
    }
}

//! Print a current node and all its subnodes
void HierarchyTree::printNode(uint level, HierarchyNode* pNode, QDebug stream) const
{
    QString nodeIndentation;
    if (level > 0)
        nodeIndentation = '|' + QString('-').repeated(level);
    while (pNode)
    {
        stream << nodeIndentation + pNode->mValue.toString() << Qt::endl;
        if (pNode->mpFirstChild)
            printNode(level + 1, pNode->mpFirstChild, stream);
        pNode = pNode->mpNextSibling;
    }
}

//! Get a number of nodes
int HierarchyTree::size() const
{
    int numNodes = 0;
    return countNodes(mpRootNode, numNodes);
}

//! Count all nodes
int HierarchyTree::countNodes(HierarchyNode* pNode, int& numNodes) const
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

//! Print a current node and all its subnodes
void HierarchyTree::writeNode(HierarchyNode* pNode, QDataStream& stream) const
{
    while (pNode)
    {
        stream << reinterpret_cast<quintptr>(pNode);
        stream << (quint32)pNode->mType;
        stream << pNode->mValue;
        stream << reinterpret_cast<quintptr>(pNode->mpParent);
        stream << reinterpret_cast<quintptr>(pNode->mpFirstChild);
        stream << reinterpret_cast<quintptr>(pNode->mpNextSibling);
        stream << reinterpret_cast<quintptr>(pNode->mpPreviousSibling);
        if (pNode->mpFirstChild)
            writeNode(pNode->mpFirstChild, stream);
        pNode = pNode->mpNextSibling;
    }
}

//! Read a tree from a stream
HierarchyTree::HierarchyTree(QDataStream& stream, int numNodes)
{
    std::map<HierarchyNode*, HierarchyNode*> mapNodes;
    quint32 iType;
    quint64 addressHolder;
    auto retrieveAddress = [&stream, &addressHolder]()
    {
        stream >> addressHolder;
        return reinterpret_cast<HierarchyNode*>(addressHolder);
    };
    // Read nodes with unresolved links between them
    for (int i = 0; i != numNodes; ++i)
    {
        HierarchyNode* nodeAddress = retrieveAddress();
        stream >> iType;
        QVariant nodeValue;
        stream >> nodeValue;
        HierarchyNode* pNode = new HierarchyNode((HierarchyNode::NodeType)iType, nodeValue);
        pNode->mpParent = retrieveAddress();
        pNode->mpFirstChild = retrieveAddress();
        pNode->mpNextSibling = retrieveAddress();
        pNode->mpPreviousSibling = retrieveAddress();
        mapNodes.emplace(nodeAddress, pNode);
    }
    // Resolve references
    for (auto& iter : mapNodes)
    {
        HierarchyNode* pCurrentNode = iter.second;
        HierarchyNode* pLink = pCurrentNode->mpParent;
        // Parent
        if (!pLink)
            mpRootNode = pCurrentNode;
        else
            pCurrentNode->mpParent = mapNodes[pLink];
        // First child
        pLink = pCurrentNode->mpFirstChild;
        if (pLink)
            pCurrentNode->mpFirstChild = mapNodes[pLink];
        // Next sibling
        pLink = pCurrentNode->mpNextSibling;
        if (pLink)
            pCurrentNode->mpNextSibling = mapNodes[pLink];
        // Previous sibling
        pLink = pCurrentNode->mpPreviousSibling;
        if (pLink)
            pCurrentNode->mpPreviousSibling = mapNodes[pLink];
    }
}

//! Encompass two nodes
HierarchyNode* HierarchyTree::groupNodes(HierarchyNode* pBaseNode, HierarchyNode* pChildNode)
{
    QString const kNameDirectory = "Group";
    if (pBaseNode == pChildNode)
        return nullptr;
    switch (pBaseNode->type())
    {
    case HierarchyNode::NodeType::kDirectory:
        excludeNodeFromHierarchy(pChildNode);
        pBaseNode->appendChild(pChildNode);
        return pBaseNode;
        break;
    case HierarchyNode::NodeType::kObject:
    {
        HierarchyNode* pDirectory = new HierarchyNode(HierarchyNode::NodeType::kDirectory, kNameDirectory);
        HierarchyNode* pParentNode = pBaseNode->mpParent;
        HierarchyNode* pNextNode = pBaseNode->mpNextSibling;
        HierarchyNode* pPreviousNode = pBaseNode->mpPreviousSibling;
        // Initialize a directory by base node
        pDirectory->mpParent = pParentNode;
        pDirectory->mpNextSibling = pNextNode;
        pDirectory->mpPreviousSibling = pPreviousNode;
        // Switch the newly created directory with the base node
        if (pParentNode && pParentNode->mpFirstChild == pBaseNode)
            pParentNode->mpFirstChild = pDirectory;
        if (pNextNode)
            pNextNode->mpPreviousSibling = pDirectory;
        if (pPreviousNode)
            pPreviousNode->mpNextSibling = pDirectory;
        // Insert the base node into the directory
        pBaseNode->mpParent = nullptr;
        pBaseNode->mpPreviousSibling = nullptr;
        pBaseNode->mpNextSibling = nullptr;
        pDirectory->appendChild(pBaseNode);
        // Insert the child into the directory
        excludeNodeFromHierarchy(pChildNode);
        pDirectory->appendChild(pChildNode);
        return pDirectory;
        break;
    }
    default:
        return nullptr;
    }
}

//! Set a given node before a base one
void HierarchyTree::setBefore(HierarchyNode* pBaseNode, HierarchyNode* pSetNode)
{
    if (pBaseNode == pSetNode)
        return;
    excludeNodeFromHierarchy(pSetNode);
    HierarchyNode* pParentNode = pBaseNode->mpParent;
    if (pParentNode && pParentNode->mpFirstChild == pBaseNode)
        pParentNode->mpFirstChild = pSetNode;
    pSetNode->mpParent = pParentNode;
    pSetNode->mpNextSibling = pBaseNode;
    pSetNode->mpPreviousSibling = pBaseNode->mpPreviousSibling;
    pBaseNode->mpPreviousSibling = pSetNode;
}

//! Set a given node after a base one
void HierarchyTree::setAfter(HierarchyNode* pBaseNode, HierarchyNode* pSetNode)
{
    if (pBaseNode == pSetNode)
        return;
    excludeNodeFromHierarchy(pSetNode);
    pSetNode->mpParent = pBaseNode->mpParent;
    pSetNode->mpPreviousSibling = pBaseNode;
    pSetNode->mpNextSibling = pBaseNode->mpNextSibling;
    pBaseNode->mpNextSibling = pSetNode;
}
