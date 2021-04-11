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
    ~HierarchyTree();
    void clear();
    void appendNode(HierarchyNode* pNode);
    bool removeNode(HierarchyNode::NodeType type, QVariant const& value);
    void changeNodeValue(HierarchyNode::NodeType type, QVariant const& oldValue, QVariant const& newValue);
    HierarchyNode& root() { return *mpRootNode; }
    HierarchyTree clone() const;
    friend QDebug operator<<(QDebug stream, HierarchyTree& tree);

private:
    HierarchyNode* findNode(HierarchyNode* pBaseNode, HierarchyNode::NodeType type, QVariant const& value) const;
    HierarchyNode* copyNode(HierarchyNode* pBaseNode, uint relativeLevel) const;
    void removeNode(HierarchyNode* pNode);
    void removeNodeSiblings(HierarchyNode* pNode);
    void printNode(uint level, HierarchyNode* pNode, QDebug stream);

private:
    HierarchyNode* mpRootNode;
};

//! Print a tree structure
inline QDebug operator<<(QDebug stream, HierarchyTree& tree)
{
    tree.printNode(0, tree.mpRootNode, stream);
    return stream;
}

}

#endif // HIERARCHYTREE_H
