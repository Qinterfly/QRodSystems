/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the HierarchyNode class
 */

#ifndef HIERARCHYNODE_H
#define HIERARCHYNODE_H

#include <QVariant>
#include <QDataStream>
#include <QSharedPointer>

namespace QRS::Core
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
    bool hasParent() const { return mpParent; }
    bool hasChild() const { return mpFirstChild; }
    bool hasNextSibling() const { return mpNextSibling; }
    HierarchyNode* parent() { return mpParent; }
    HierarchyNode* firstChild() { return mpFirstChild; }
    HierarchyNode* nextSibling() { return mpNextSibling; }
    NodeType type() const { return mType; }
    QVariant& value() { return mValue; }
    HierarchyNode* groupNodes(HierarchyNode* pChildNode);
    bool setBefore(HierarchyNode* pSetNode);
    bool setAfter(HierarchyNode* pSetNode);
    uint numberChildren() const;

private:
    void excludeNodeFromHierarchy();
    bool isSetAllowed(HierarchyNode const* pNode) const;
    bool isParentOf(HierarchyNode const* pNode) const;
    uint countNodes(HierarchyNode* pNode, uint& numNodes) const;

private:
    HierarchyNode* mpParent = nullptr;
    HierarchyNode* mpFirstChild = nullptr;
    HierarchyNode* mpNextSibling = nullptr;
    HierarchyNode* mpPreviousSibling = nullptr;
    NodeType mType;
    QVariant mValue;
};

}

#endif // HIERARCHYNODE_H
