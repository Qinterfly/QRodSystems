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

}

#endif // HIERARCHYNODE_H
