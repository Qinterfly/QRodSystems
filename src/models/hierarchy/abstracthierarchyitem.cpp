/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the AbstractHierarchyItem class
 */

#include "abstracthierarchyitem.h"
#include "core/hierarchynode.h"

using namespace QRS::HierarchyModels;
using namespace QRS::Core;

AbstractHierarchyItem::AbstractHierarchyItem(QIcon const& icon, QString const& text, HierarchyNode* pNode)
    : QStandardItem(icon, text)
    , mpNode(pNode)
{

}

AbstractHierarchyItem::~AbstractHierarchyItem()
{

}

//! Write the pointer to the current item to a stream
void AbstractHierarchyItem::writePointer(QDataStream& out) const
{
    out << reinterpret_cast<quintptr>(this);
}

//! Retrieve a pointer to an item from a stream
AbstractHierarchyItem* AbstractHierarchyItem::readPointer(QDataStream& in)
{
    quintptr itemAddress;
    in >> itemAddress;
    return reinterpret_cast<AbstractHierarchyItem*>(itemAddress);
}
