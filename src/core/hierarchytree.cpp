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
    if (!pNode)
        return;
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
void HierarchyTree::printNode(uint level, HierarchyNode* pNode, QDebug stream) const
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

//! Get a number of nodes
int HierarchyTree::size() const
{
    int numNodes = 0;
    return countNodes(mpRootNode, numNodes);
}

//! Count all nodes
int HierarchyTree::countNodes(HierarchyNode* pNode, int& numNodes) const
{
    HierarchyNode* pNextNode;
    while (pNode)
    {
        ++numNodes;
        pNextNode = pNode->mpNextSibling;
        if (pNode->mpFirstChild)
            countNodes(pNode->mpFirstChild, numNodes);
        pNode = pNextNode;
    }
    return numNodes;
}

//! Print a current node and all its subnodes
void HierarchyTree::writeNode(HierarchyNode* pNode, QDataStream& stream) const
{
    HierarchyNode* pNextNode;
    while (pNode)
    {
        pNextNode = pNode->mpNextSibling;
        stream << reinterpret_cast<quintptr>(pNode);
        stream << (quint32)pNode->mType;
        stream << pNode->mValue;
        stream << reinterpret_cast<quintptr>(pNode->mpParent);
        stream << reinterpret_cast<quintptr>(pNode->mpFirstChild);
        stream << reinterpret_cast<quintptr>(pNode->mpNextSibling);
        if (pNode->mpFirstChild)
            writeNode(pNode->mpFirstChild, stream);
        pNode = pNextNode;
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
    }
}
