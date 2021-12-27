/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the AbstractHierarchyItem class
 */

#ifndef ABSTRACTHIERARCHYITEM_H
#define ABSTRACTHIERARCHYITEM_H

#include <QStandardItem>

namespace QRS
{

namespace Core
{
class HierarchyNode;
class HierarchyTree;
}

namespace PropertiesModels
{
class AbstractPropertiesModel;
}

namespace HierarchyModels
{

//! Item to represent a hierarchy of elements of the same type
class AbstractHierarchyItem : public QStandardItem
{
    friend class AbstractHierarchyModel;
    friend class PropertiesModels::AbstractPropertiesModel;

public:
    enum ItemType
    {
        kDataObjects = QStandardItem::UserType,
        kRodComponents
    };
    AbstractHierarchyItem(QIcon const& icon, QString const& text, Core::HierarchyNode* pNode);
    virtual ~AbstractHierarchyItem() = 0;
    void writePointer(QDataStream& out) const;
    static AbstractHierarchyItem* readPointer(QDataStream& in);
    virtual int type() const = 0;

protected:
    Core::HierarchyNode* mpNode = nullptr;
};

}

}

#endif // ABSTRACTHIERARCHYITEM_H
