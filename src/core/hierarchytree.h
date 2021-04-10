/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
 * \brief Declaration of the HierarchyNode and HierarchyTree classes
 */

#ifndef HIERARCHYTREE_H
#define HIERARCHYTREE_H

#include <QVariant>
#include <QDebug>

namespace QRS
{

//! Hierarchy representative
class HierarchyNode
{
public:
    friend class HierarchyTree;
    enum NodeType
    {
        kObject,
        kDirectory
    };
    HierarchyNode(NodeType type, QVariant value);
    ~HierarchyNode() = default;
    void appendChild(HierarchyNode* node);

private:
    HierarchyNode* mpParent = nullptr;
    HierarchyNode* mpFirstChild = nullptr;
    HierarchyNode* mpNextSibling = nullptr;
    NodeType mType;
    QVariant mValue;
};

//! Hierarchy of data objects (n-aray tree)
class HierarchyTree
{
public:
    HierarchyTree();
    ~HierarchyTree();
    void clear();
    void appendNode(HierarchyNode* pNode);
    bool removeNode(HierarchyNode::NodeType type, QVariant const& value);
    void changeNodeValue(HierarchyNode::NodeType type, QVariant const& oldValue, QVariant const& newValue);
    HierarchyNode& root() { return *mpRootNode; }
    friend QDebug operator<<(QDebug stream, HierarchyTree& tree);

private:
    HierarchyNode* find(HierarchyNode* pBaseNode, HierarchyNode::NodeType type, QVariant const& value) const;
    void removeNode(HierarchyNode* pNode);
    void removeChildren(HierarchyNode* pNode);
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
