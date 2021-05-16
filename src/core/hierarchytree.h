/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
 * \brief Declaration of the HierarchyTree class
 */

#ifndef HIERARCHYTREE_H
#define HIERARCHYTREE_H

#include <QDebug>

#include "hierarchynode.h"

namespace QRS
{

//! Hierarchy of data objects (n-aray tree)
class HierarchyTree
{
public:
    HierarchyTree();
    HierarchyTree(HierarchyNode* pRootNode);
    HierarchyTree(QDataStream& stream, int numNodes);
    HierarchyTree& operator=(HierarchyTree const& another);
    HierarchyTree& operator=(HierarchyTree&& another);
    ~HierarchyTree();
    void clear();
    void appendNode(HierarchyNode* pNode);
    bool removeNode(HierarchyNode::NodeType type, QVariant const& value);
    void removeNode(HierarchyNode* pNode);
    void changeNodeValue(HierarchyNode::NodeType type, QVariant const& oldValue, QVariant const& newValue);
    HierarchyNode* root() { return mpRootNode; }
    HierarchyTree clone() const;
    HierarchyNode* findNode(HierarchyNode* pBaseNode, HierarchyNode::NodeType type, QVariant const& value) const;
    int size() const;
    HierarchyNode* groupNodes(HierarchyNode* pBaseNode, HierarchyNode* pChildNode);
    void setBefore(HierarchyNode* pBaseNode, HierarchyNode* pSetNode);
    void setAfter(HierarchyNode* pBaseNode, HierarchyNode* pSetNode);
    friend QDebug operator<<(QDebug stream, HierarchyTree& tree);
    friend QDataStream& operator<<(QDataStream& stream, HierarchyTree const& tree);

private:
    HierarchyNode* copyNode(HierarchyNode* pBaseNode, uint relativeLevel) const;
    void removeNodeSiblings(HierarchyNode* pNode);
    void printNode(uint level, HierarchyNode* pNode, QDebug stream) const;
    void writeNode(HierarchyNode* pNode, QDataStream& stream) const;
    int countNodes(HierarchyNode* pNode, int& numNodes) const;
    void excludeNodeFromHierarchy(HierarchyNode* pNode);

private:
    HierarchyNode* mpRootNode = nullptr;
};

//! Print a tree structure
inline QDebug operator<<(QDebug stream, HierarchyTree& tree)
{
    tree.printNode(0, tree.mpRootNode, stream);
    return stream;
}

//! Write a tree structure to a stream
inline QDataStream& operator<<(QDataStream& stream, HierarchyTree const& tree)
{
    tree.writeNode(tree.mpRootNode, stream);
    return stream;
}

}

#endif // HIERARCHYTREE_H
