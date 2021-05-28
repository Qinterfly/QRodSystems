/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the DataObjectsHierarchyItem class
 */

#include "dataobjectshierarchyitem.h"
#include "core/abstractdataobject.h"
#include "core/hierarchytree.h"

using namespace QRS::HierarchyModels;
using namespace QRS::Core;

QIcon getDataObjectIcon(AbstractDataObject::ObjectType type);

//! Create the representer of the structure of data objects
DataObjectsHierarchyItem::DataObjectsHierarchyItem(DataObjects& dataObjects, HierarchyTree& hierarchyDataObjects, QString const& text, QIcon const& icon)
    : AbstractHierarchyItem(icon, text, hierarchyDataObjects.root())
{
    if (!mpNode->hasChild())
        return;
    appendItems(dataObjects, mpNode->firstChild());
}

void DataObjectsHierarchyItem::appendItems(DataObjects& dataObjects, HierarchyNode* pNode)
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
DataObjectsHierarchyItem::DataObjectsHierarchyItem(HierarchyNode* pNode, AbstractDataObject* pDataObject)
    : AbstractHierarchyItem(getDataObjectIcon(pDataObject->type()), pDataObject->name(), pNode)
    , mpDataObject(pDataObject)
{
    setFlags(flags() | Qt::ItemIsEditable);
}

//! Construct an item to represent a directory
DataObjectsHierarchyItem::DataObjectsHierarchyItem(HierarchyNode* pNode)
    : AbstractHierarchyItem(QIcon(":/icons/folder.svg"), pNode->value().toString(), pNode)
{
    setFlags(flags() | Qt::ItemIsEditable);
}

//! Helper function to assign appropriate data object icon
QIcon getDataObjectIcon(AbstractDataObject::ObjectType type)
{
    switch(type)
    {
    case AbstractDataObject::ObjectType::kScalar:
        return QIcon(":/icons/letter-s.svg");
    case AbstractDataObject::ObjectType::kVector:
        return QIcon(":/icons/letter-v.svg");
    case AbstractDataObject::ObjectType::kMatrix:
        return QIcon(":/icons/letter-m.svg");
    case AbstractDataObject::ObjectType::kSurface:
        return QIcon(":/icons/letter-xy.svg");
    default:
        return QIcon();
    }
}




