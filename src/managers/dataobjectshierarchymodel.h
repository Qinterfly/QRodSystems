/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the DataObjectsHierarchyModel class
 */

#ifndef DATAOBJECTSHIERARCHYMODEL_H
#define DATAOBJECTSHIERARCHYMODEL_H

#include "models/abstracthierarchymodel.h"
#include "dataobjectshierarchyitem.h"

namespace QRS
{

namespace Core
{
class HierarchyTree;
class AbstractDataObject;
}

namespace HierarchyModels
{

//! Tree model to represent and modify a hierarchy of data objects
class DataObjectsHierarchyModel : public AbstractHierarchyModel
{
    Q_OBJECT

public:
    DataObjectsHierarchyModel(mapDataObjects& dataObjects, Core::HierarchyTree& hierarchyDataObjects, QTreeView* pView = nullptr);
    ~DataObjectsHierarchyModel() = default;
    void updateContent() override;
    void clearContent() override;
    bool isEmpty() const;
    void selectItem(int iRow);

signals:
    void dataObjectSelected(Core::DataIDType id);
    void selectionCleared();

public slots:
    void retrieveSelectedDataObject();
    void removeSelectedItems();

private slots:
    void renameDataObject(QStandardItem* pStandardItem);

private:
    mapDataObjects& mDataObjects;
    Core::HierarchyTree& mHierarchyDataObjects;
};

}

}

#endif // DATAOBJECTSHIERARCHYMODEL_H
