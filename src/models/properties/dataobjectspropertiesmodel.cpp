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
#include "models/hierarchy/abstracthierarchymodel.h"
#include "models/hierarchy/dataobjectshierarchyitem.h"

using namespace QRS::PropertiesModels;
using namespace QRS::HierarchyModels;
using namespace QRS::Core;

DataObjectsPropertiesModel::DataObjectsPropertiesModel(QTableView* pView, QVector<AbstractHierarchyItem*> items)
    : AbstractPropertiesModel(pView, items)
{
    if (mItems.isEmpty())
        return;
    // Specify properties for all directories or objects
    if (mIsDirectory)
        setDirectoryAttributes();
    else
        setObjectAttributes();
}

//! Set attributes of selected data objects
void DataObjectsPropertiesModel::setObjectAttributes()
{
    // Create the intersection flags
    bool isSameName = true;
    bool isSameType = true;
    bool isSameNumberItems = true;
    bool isSameIdentifier = true;
    // Check if properties intersect
    int numItems = mItems.size();
    DataObjectsHierarchyItem* pItem = (DataObjectsHierarchyItem*)mItems[0];
    AbstractDataObject* pCurrentDataObject = pItem->mpDataObject;
    AbstractDataObject* pNextDataObject;
    for (int i = 0; i != numItems - 1; ++i)
    {
        pItem = (DataObjectsHierarchyItem*)mItems[i + 1];
        pNextDataObject = pItem->mpDataObject;
        isSameName = isSameName && pCurrentDataObject->name() == pNextDataObject->name();
        isSameType = isSameType && pCurrentDataObject->type() == pNextDataObject->type();
        isSameNumberItems = isSameNumberItems && pCurrentDataObject->numberItems() == pNextDataObject->numberItems();
        isSameIdentifier = isSameIdentifier && pCurrentDataObject->id() == pNextDataObject->id();
        pCurrentDataObject = pNextDataObject;
    }
    // Set only common properties by flags
    QString name = isSameName ? pCurrentDataObject->name() : mkEmptyProperty;
    QString type;
    QString numberEntities;
    if (isSameType)
    {
        switch (pCurrentDataObject->type())
        {
        case AbstractDataObject::ObjectType::kScalar:
            type = tr("Scalar");
            numberEntities = QString::number(1);
            break;
        case AbstractDataObject::ObjectType::kVector:
            type = tr("Vector");
            numberEntities = QString::number(3);
            break;
        case AbstractDataObject::ObjectType::kMatrix:
            type = tr("Matrix");
            numberEntities = QString::number(9);
            break;
        case AbstractDataObject::ObjectType::kSurface:
            type = tr("Surface");
            SurfaceDataObject* pSurfaceObject = (SurfaceDataObject*)pCurrentDataObject;
            numberEntities = QString::number(pSurfaceObject->numberLeadingItems());
            break;
        }
    }
    QString numberItems = isSameNumberItems ? QString::number(pCurrentDataObject->numberItems()) : mkEmptyProperty;
    QString identifier = isSameIdentifier ? QString::number(pCurrentDataObject->id()) : mkEmptyProperty;
    // Insert the properties into the table
    QStandardItem* pRoot = invisibleRootItem();
    pRoot->appendRow(preparePropertyRow(PropertyDataObject::kName, tr("Name"), name, true));
    pRoot->appendRow(preparePropertyRow(PropertyDataObject::kType, tr("Type"), type, false));
    pRoot->appendRow(preparePropertyRow(PropertyDataObject::kNumberItems, tr("Number of items"), numberItems, false));
    pRoot->appendRow(preparePropertyRow(PropertyDataObject::kNumberEntities, tr("Number of entities"), numberEntities, false));
    pRoot->appendRow(preparePropertyRow(PropertyDataObject::kID, tr("Identifier"), identifier, false));
}

//! Modify the selected property of all items
void DataObjectsPropertiesModel::modifyProperty(QStandardItem* pChangedProperty)
{
    QString newName = pChangedProperty->data(Qt::DisplayRole).toString();
    if (mIsDirectory)
    {
        modifyDirectoryName(newName);
    }
    else
    {
        int numItems = mItems.size();
        DataObjectsHierarchyItem* pItem;
        for (int i = 0; i != numItems; ++i)
        {
            pItem = (DataObjectsHierarchyItem*)mItems[i];
            pItem->mpDataObject->setName(newName);
            pItem->setText(newName);
        }
    }
    emit propertyChanged();
}
