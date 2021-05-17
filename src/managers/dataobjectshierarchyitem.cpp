/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the DataObjectsHierarchyItem class
 */

#include "dataobjectshierarchyitem.h"
#include "core/abstractdataobject.h"
#include "core/hierarchytree.h"

using namespace QRS;

QIcon getDataObjectIcon(DataObjectType type);

//! Create the representer of the structure of data objects
DataObjectsHierarchyItem::DataObjectsHierarchyItem(mapDataObjects& dataObjects, QRS::HierarchyTree& hierarchyDataObjects, QString const& name)
    : AbstractHierarchyItem(QIcon(), name, hierarchyDataObjects.root())
{
    if (!mpNode->hasChild())
        return;
    appendItems(dataObjects, mpNode->firstChild());
}

void DataObjectsHierarchyItem::appendItems(mapDataObjects& dataObjects, QRS::HierarchyNode* pNode)
{
    HierarchyNode* pNextNode;
    while (pNode)
    {
        DataObjectsHierarchyItem* pItem = nullptr;
        switch (pNode->type())
        {
        case HierarchyNode::NodeType::kDirectory:
            pItem = new DataObjectsHierarchyItem(pNode);
            break;
        case HierarchyNode::NodeType::kObject:
            DataIDType id = pNode->value().toUInt();
            if (!dataObjects.contains(id))
                return;
            pItem = new DataObjectsHierarchyItem(pNode, dataObjects[id]);
            break;
        }
        if (pItem)
        {
            appendRow(pItem);
            if (pNode->hasChild())
                pItem->appendItems(dataObjects, pNode->firstChild());
        }
        pNextNode = pNode->nextSibling();
        pNode = pNextNode;
    }
}

//! Construct an item to represent a data object
DataObjectsHierarchyItem::DataObjectsHierarchyItem(QRS::HierarchyNode* pNode, QRS::AbstractDataObject* pDataObject)
    : AbstractHierarchyItem(getDataObjectIcon(pDataObject->type()), pDataObject->name(), pNode)
    , mpDataObject(pDataObject)
{

}

//! Construct an item to represent a directory
DataObjectsHierarchyItem::DataObjectsHierarchyItem(QRS::HierarchyNode* pNode)
    : AbstractHierarchyItem(QIcon(":/icons/folder.svg"), pNode->value().toString(), pNode)
{

}

//! Helper function to assign appropriate data object icon
QIcon getDataObjectIcon(DataObjectType type)
{
    switch(type)
    {
    case kScalar:
        return QIcon(":/icons/letter-s.svg");
    case kVector:
        return QIcon(":/icons/letter-v.svg");
    case kMatrix:
        return QIcon(":/icons/letter-m.svg");
    case kSurface:
        return QIcon(":/icons/letter-xy.svg");
    default:
        return QIcon();
    }
}




