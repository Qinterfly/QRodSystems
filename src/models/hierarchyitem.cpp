/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the HierarchyItem class
 */

#include "hierarchyitem.h"
#include "core/hierarchynode.h"

using namespace QRS;

HierarchyItem::HierarchyItem(const QIcon& icon, const QString& text, HierarchyNode* pNode)
    : QStandardItem(icon, text)
    , mpNode(pNode)
{
    setFlags(flags() | Qt::ItemIsEditable);
}

//! Write a hierarchy item to a stream
void HierarchyItem::write(QDataStream& out) const
{
    QStandardItem::write(out);
    out << reinterpret_cast<quintptr>(mpNode);
}

//! Read a hierarchy item from a stream
void HierarchyItem::read(QDataStream& in)
{
    QStandardItem::read(in);
    quintptr nodeAddress;
    in >> nodeAddress;
    mpNode = reinterpret_cast<HierarchyNode*>(nodeAddress);
}
