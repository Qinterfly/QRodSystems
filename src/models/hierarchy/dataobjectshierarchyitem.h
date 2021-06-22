/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the DataObjectsHierarchyItem class
 */

#ifndef DATAOBJECTSHIERARCHYITEM_H
#define DATAOBJECTSHIERARCHYITEM_H

#include "models/hierarchy/abstracthierarchyitem.h"
#include "core/aliasdataset.h"

namespace QRS
{

namespace PropertiesModels
{
class DataObjectsPropertiesModel;
}

namespace HierarchyModels
{

//! Item to represent a hierarchy of data objects
class DataObjectsHierarchyItem : public AbstractHierarchyItem
{
    friend class DataObjectsHierarchyModel;
    friend class PropertiesModels::DataObjectsPropertiesModel;

public:
    DataObjectsHierarchyItem(Core::DataObjects& dataObjects, Core::HierarchyTree& hierarchyDataObjects,
                             QString const& text = "Root", QIcon const& icon = QIcon());
    DataObjectsHierarchyItem(Core::HierarchyNode* pNode, Core::AbstractDataObject* pDataObject);
    DataObjectsHierarchyItem(Core::HierarchyNode* pNode);
    int type() const override { return AbstractHierarchyItem::ItemType::kDataObjects; }

private:
    void appendItems(Core::DataObjects& dataObjects, Core::HierarchyNode* pNode);

private:
    Core::AbstractDataObject* mpDataObject = nullptr;
};

}

}

#endif // DATAOBJECTSHIERARCHYITEM_H
