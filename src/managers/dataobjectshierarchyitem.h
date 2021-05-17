/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the DataObjectsHierarchyItem class
 */

#ifndef DATAOBJECTSHIERARCHYITEM_H
#define DATAOBJECTSHIERARCHYITEM_H

#include <unordered_map>
#include "models/hierarchyitem.h"
#include "core/datatypes.h"

namespace QRS
{
class HierarchyTree;
class AbstractDataObject;
}

using mapDataObjects = std::unordered_map<QRS::DataIDType, QRS::AbstractDataObject*>;

//! Item to represent a hierarchy of data objects
class DataObjectsHierarchyItem : public HierarchyItem
{
    friend class DataObjectsHierarchyModel;

public:
    DataObjectsHierarchyItem() = default;
    DataObjectsHierarchyItem(mapDataObjects& dataObjects, QRS::HierarchyTree& hierarchyDataObjects, QString const& name = "Root");
    DataObjectsHierarchyItem(QRS::HierarchyNode* pNode, QRS::AbstractDataObject* pDataObject);
    DataObjectsHierarchyItem(QRS::HierarchyNode* pNode);
    int type() const override { return HierarchyItem::DataType::kDataObjects; }
    void write(QDataStream& out) const override;
    void read(QDataStream& in) override;

private:
    void appendItems(mapDataObjects& dataObjects, QRS::HierarchyNode* pNode);

private:
    QRS::AbstractDataObject* mpDataObject = nullptr;
};

#endif // DATAOBJECTSHIERARCHYITEM_H
