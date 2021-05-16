/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the HierarchyItem class
 */

#ifndef HIERACHYITEM_H
#define HIERACHYITEM_H

#include <QStandardItem>

namespace QRS
{
class HierarchyNode;
class HierarchyTree;
class AbstractDataObject;
}

enum HierarchyItemType
{
    kBase = QStandardItem::UserType,
    kDataObjects,
};

//! Item to represent a hierarchy of elements of the same type
class HierarchyItem : public QStandardItem
{
public:
    HierarchyItem() = default;
    HierarchyItem(const QIcon& icon, const QString& text, QRS::HierarchyNode* pNode);
    int type() const override { return HierarchyItemType::kBase; };
    void write(QDataStream& out) const override;
    void read(QDataStream& in) override;

protected:
    QRS::HierarchyNode* mpNode = nullptr;
};

#endif // HIERACHYITEM_H
