/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the DataObjectsPropertiesModel class
 */

#include <QTableView>
#include "dataobjectspropertiesmodel.h"
#include "core/abstractdataobject.h"
#include "core/surfacedataobject.h"
#include "core/hierarchynode.h"
#include "models/hierarchy/dataobjectshierarchyitem.h"

using namespace QRS::PropertiesModels;
using namespace QRS::HierarchyModels;
using namespace QRS::Core;

static QString const skEmptyProperty = "";

DataObjectsPropertiesModel::DataObjectsPropertiesModel(QTableView* pView, QVector<AbstractHierarchyItem*> items)
    : QStandardItemModel(pView)
{
    if (items.isEmpty())
        return;
    for (AbstractHierarchyItem* pItem : items)
        mItems.push_back((DataObjectsHierarchyItem*)pItem);
    // Check if the set of items consists of directories and objects
    int numItems = mItems.size();
    HierarchyNode::NodeType nodeType = mItems[0]->mpNode->type();
    for (int i = 1; i != numItems; ++i)
    {
        if (mItems[i]->mpNode->type() != nodeType)
            return;
    }
    // Specify properties for all directories or objects
    if (nodeType == HierarchyNode::NodeType::kDirectory)
        setDirectoryAttributes();
    else
        setObjectAttributes();
    // Create connections
    connect(this, &DataObjectsPropertiesModel::itemChanged, this, &DataObjectsPropertiesModel::modifyProperty);
}

//! Set directory characteristic attributes
void DataObjectsPropertiesModel::setDirectoryAttributes()
{
    // Create the intersection flags
    bool isSameName = true;
    bool isSameNumberChildren = true;
    // Check if properties intersect
    int numItems = mItems.size();
    HierarchyNode* pCurrentNode = mItems[0]->mpNode;
    HierarchyNode* pNextNode;
    for (int i = 0; i != numItems - 1; ++i)
    {
        pNextNode = mItems[i + 1]->mpNode;
        isSameName = isSameName && pCurrentNode->value() == pNextNode->value();
        isSameNumberChildren = isSameNumberChildren && pCurrentNode->numberChildren() == pNextNode->numberChildren();
        pCurrentNode = pNextNode;
    }
    // Set only common properties by flags
    QString name = isSameName ? pCurrentNode->value().toString() : skEmptyProperty;
    QString numberChildren = isSameNumberChildren ? QString::number(pCurrentNode->numberChildren()) : skEmptyProperty;
    // Insert the properties into the table
    QStandardItem* pRoot = invisibleRootItem();
    pRoot->appendRow(preparePropertyRow(kName, tr("Name"), name, true));
    pRoot->appendRow(preparePropertyRow(kNumberChildren, tr("Number of children"), numberChildren, false));
}

//! Set objects characteristic attributes
void DataObjectsPropertiesModel::setObjectAttributes()
{
    // Create the intersection flags
    bool isSameName = true;
    bool isSameType = true;
    bool isSameNumberItems = true;
    bool isSameIdentifier = true;
    // Check if properties intersect
    int numItems = mItems.size();
    AbstractDataObject* pCurrentDataObject = mItems[0]->mpDataObject;
    AbstractDataObject* pNextDataObject;
    for (int i = 0; i != numItems - 1; ++i)
    {
        pNextDataObject = mItems[i + 1]->mpDataObject;
        isSameName = isSameName && pCurrentDataObject->name() == pNextDataObject->name();
        isSameType = isSameType && pCurrentDataObject->type() == pNextDataObject->type();
        isSameNumberItems = isSameNumberItems && pCurrentDataObject->numberItems() == pNextDataObject->numberItems();
        isSameIdentifier = isSameIdentifier && pCurrentDataObject->id() == pNextDataObject->id();
        pCurrentDataObject = pNextDataObject;
    }
    // Set only common properties by flags
    QString name = isSameName ? pCurrentDataObject->name() : skEmptyProperty;
    QString type;
    QString numberEntities;
    if (isSameType)
    {
        switch (pCurrentDataObject->type())
        {
        case DataObjectType::kScalar:
            type = tr("Scalar");
            numberEntities = QString::number(1);
            break;
        case DataObjectType::kVector:
            type = tr("Vector");
            numberEntities = QString::number(3);
            break;
        case DataObjectType::kMatrix:
            type = tr("Matrix");
            numberEntities = QString::number(9);
            break;
        case DataObjectType::kSurface:
            type = tr("Surface");
            SurfaceDataObject* pSurfaceObject = (SurfaceDataObject*)pCurrentDataObject;
            numberEntities = QString::number(pSurfaceObject->numberLeadingItems());
            break;
        }
    }
    QString numberItems = isSameNumberItems ? QString::number(pCurrentDataObject->numberItems()) : skEmptyProperty;
    QString identifier = isSameIdentifier ? QString::number(pCurrentDataObject->id()) : skEmptyProperty;
    // Insert the properties into the table
    QStandardItem* pRoot = invisibleRootItem();
    pRoot->appendRow(preparePropertyRow(kName, tr("Name"), name, true));
    pRoot->appendRow(preparePropertyRow(kType, tr("Type"), type, false));
    pRoot->appendRow(preparePropertyRow(kNumberItems, tr("Number of items"), numberItems, false));
    pRoot->appendRow(preparePropertyRow(kNumberEntities, tr("Number of entities"), numberEntities, false));
    pRoot->appendRow(preparePropertyRow(kID, tr("Identifier"), identifier, false));
}

//! Modify the selected property of all items
void DataObjectsPropertiesModel::modifyProperty(QStandardItem* pChangedProperty)
{
    PropertyType propertyType = (PropertyType)pChangedProperty->data(Qt::UserRole).toInt();
    // Only the name is modifiable
    if (propertyType != kName)
        return;
    // Since all nodes of items have the same type we apply changes to all of them
    DataObjectsHierarchyItem* pFirstItem = mItems[0];
    QString newName = pChangedProperty->data(Qt::DisplayRole).toString();
    bool isDataObject = pFirstItem->mpNode->type() == HierarchyNode::NodeType::kObject;
    // Set the new hierarchial name
    QStandardItemModel* pModel = pFirstItem->model();
    QVector<QModelIndex> indices;
    for (DataObjectsHierarchyItem* pItem : qAsConst(mItems))
    {
        indices.push_back(pItem->index());
        if (isDataObject)
            pItem->mpDataObject->setName(newName);
        else
            pItem->mpNode->value() = newName;
    }
    // Modify names of items
    for (QModelIndex const& index : indices)
        pModel->setData(index, newName, Qt::DisplayRole);
    emit propertyChanged(true);
}

//! Prepare a row to insert into the table
QList<QStandardItem*> DataObjectsPropertiesModel::preparePropertyRow(PropertyType type, QString const& title, QVariant const& value, bool isValueEditable) const
{
    QColor const kAlternateColor = QColor(236, 236, 236);
    QList<QStandardItem*> result;
    // Title
    QStandardItem* pTitleItem = new QStandardItem(title);
    pTitleItem->setFlags(Qt::ItemFlag::ItemIsEnabled);
    result.push_back(pTitleItem);
    // Value
    QStandardItem* pValueItem = new QStandardItem();
    pValueItem->setData(value, Qt::DisplayRole);
    pValueItem->setData(type, Qt::UserRole);
    if (!isValueEditable)
    {
        pValueItem->setFlags(pValueItem->flags() & (~Qt::ItemFlag::ItemIsEditable));
        pValueItem->setBackground(kAlternateColor);
    }
    result.push_back(pValueItem);
    return result;
}
