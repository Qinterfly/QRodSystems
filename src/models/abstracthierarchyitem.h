/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the AbstractHierarchyItem class
 */

#ifndef ABSTRACTHIERARCHYITEM_H
#define ABSTRACTHIERARCHYITEM_H

#include <QStandardItem>

namespace QRS
{
class HierarchyNode;
class HierarchyTree;
class AbstractDataObject;
}

enum HierarchyItemType
{
    kDataObjects = QStandardItem::UserType + 1
};

//! Item to represent a hierarchy of elements of the same type
class AbstractHierarchyItem : public QStandardItem
{
    friend class AbstractHierarchyModel;

public:
    AbstractHierarchyItem(const QIcon& icon, const QString& text, QRS::HierarchyNode* pNode);
    virtual ~AbstractHierarchyItem() = 0;
    void writePointer(QDataStream& out);
    static AbstractHierarchyItem* readPointer(QDataStream& in);

protected:
    QRS::HierarchyNode* mpNode = nullptr;
};

#endif // ABSTRACTHIERARCHYITEM_H
