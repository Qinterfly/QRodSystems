/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the HierarchyTree class
 */

#ifndef HIERARCHYTREE_H
#define HIERARCHYTREE_H

#include <QDebug>
#include "hierarchynode.h"

namespace QRS::Core
{

//! Hierarchy of data objects (n-aray tree)
class HierarchyTree
{
public:
    HierarchyTree();
    HierarchyTree(HierarchyTree& another);
    HierarchyTree(HierarchyTree&& another);
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
    quint32 size() const;
    friend QDebug operator<<(QDebug stream, HierarchyTree& tree);
    friend QDataStream& operator<<(QDataStream& stream, HierarchyTree const& tree);

private:
    HierarchyNode* copyNode(HierarchyNode* pBaseNode, quint32 relativeLevel) const;
    void removeNodeSiblings(HierarchyNode* pNode);
    void printNode(quint32 level, HierarchyNode* pNode, QDebug stream) const;
    void writeNode(HierarchyNode* pNode, QDataStream& stream) const;

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
