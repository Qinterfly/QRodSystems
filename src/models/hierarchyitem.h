/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the HierarchyItem class
 */

#ifndef HIERARCHYITEM_H
#define HIERARCHYITEM_H

#include <QStandardItem>

namespace QRS
{
class HierarchyNode;
class HierarchyTree;
class AbstractDataObject;
}

//! Item to represent a hierarchy of elements of the same type
class HierarchyItem : public QStandardItem
{
    friend class AbstractHierarchyModel;

public:
    enum DataType
    {
        kBase = QStandardItem::UserType,
        kDataObjects,
    };
    HierarchyItem() = default;
    HierarchyItem(const QIcon& icon, const QString& text, QRS::HierarchyNode* pNode);
    virtual ~HierarchyItem() = default;
    int type() const override { return DataType::kBase; };
    void write(QDataStream& out) const override;
    void read(QDataStream& in) override;

protected:
    QRS::HierarchyNode* mpNode = nullptr;
};

#endif // HIERARCHYITEM_H
