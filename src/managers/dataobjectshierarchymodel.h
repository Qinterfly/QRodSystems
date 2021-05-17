/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the DataObjectsHierarchyModel class
 */

#ifndef DATAOBJECTSHIERARCHYMODEL_H
#define DATAOBJECTSHIERARCHYMODEL_H

#include <unordered_map>
#include "models/abstracthierarchymodel.h"
#include "core/datatypes.h"

namespace QRS
{
class HierarchyTree;
class AbstractDataObject;
}

using mapDataObjects = std::unordered_map<QRS::DataIDType, QRS::AbstractDataObject*>;

//! Tree model to represent and modify a hierarchy of data objects
class DataObjectsHierarchyModel : public AbstractHierarchyModel
{
    Q_OBJECT

public:
    DataObjectsHierarchyModel(mapDataObjects& dataObjects, QRS::HierarchyTree& hierarchyDataObjects, QTreeView* pView = nullptr);
    ~DataObjectsHierarchyModel() = default;
    void updateContent() override;
    void clearContent() override;
    bool isEmpty() const;
    void selectItem(int iRow);

signals:
    void dataObjectSelected(QRS::DataIDType id);
    void selectionCleared();

public slots:
    void retrieveSelectedDataObject();
    void removeSelectedItems();

private slots:
    void renameDataObject(QStandardItem* pStandardItem);

private:
    mapDataObjects& mDataObjects;
    QRS::HierarchyTree& mHierarchyDataObjects;
};

#endif // DATAOBJECTSHIERARCHYMODEL_H
