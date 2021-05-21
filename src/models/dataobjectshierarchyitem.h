/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the DataObjectsHierarchyItem class
 */

#ifndef DATAOBJECTSHIERARCHYITEM_H
#define DATAOBJECTSHIERARCHYITEM_H

#include <unordered_map>
#include "models/abstracthierarchyitem.h"
#include "core/datatypes.h"

namespace QRS
{

namespace Core
{
class HierarchyTree;
class AbstractDataObject;
}


namespace HierarchyModels
{

using DataObjects = std::unordered_map<Core::DataIDType, Core::AbstractDataObject*>;

//! Item to represent a hierarchy of data objects
class DataObjectsHierarchyItem : public AbstractHierarchyItem
{
    friend class DataObjectsHierarchyModel;

public:
    DataObjectsHierarchyItem(DataObjects& dataObjects, Core::HierarchyTree& hierarchyDataObjects,
                             QString const& text = "Root", QIcon const& icon = QIcon());
    DataObjectsHierarchyItem(Core::HierarchyNode* pNode, Core::AbstractDataObject* pDataObject);
    DataObjectsHierarchyItem(Core::HierarchyNode* pNode);
    int type() const override { return HierarchyItemType::kDataObjects; }

private:
    void appendItems(DataObjects& dataObjects, Core::HierarchyNode* pNode);

private:
    Core::AbstractDataObject* mpDataObject = nullptr;
};

}

}

#endif // DATAOBJECTSHIERARCHYITEM_H
