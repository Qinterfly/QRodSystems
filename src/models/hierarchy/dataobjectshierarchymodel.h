/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the DataObjectsHierarchyModel class
 */

#ifndef DATAOBJECTSHIERARCHYMODEL_H
#define DATAOBJECTSHIERARCHYMODEL_H

#include "models/hierarchy/abstracthierarchymodel.h"
#include "core/aliasdataset.h"

namespace QRS
{

namespace Core
{
class HierarchyTree;
}

namespace HierarchyModels
{

//! Tree model to represent and modify a hierarchy of data objects
class DataObjectsHierarchyModel : public AbstractHierarchyModel
{
    Q_OBJECT

public:
    DataObjectsHierarchyModel(Core::DataObjects& dataObjects, Core::HierarchyTree& hierarchyDataObjects, QTreeView* pView = nullptr);
    ~DataObjectsHierarchyModel() = default;
    void updateContent() override;
    void clearContent() override;
    bool isEmpty() const;
    void selectItem(int iRow);

signals:
    void selected(Core::DataIDType id);
    void selectionCleared();

public slots:
    void retrieveSelectedItem();
    void removeSelectedItems();

private slots:
    void renameItem(QStandardItem* pStandardItem);

private:
    Core::DataObjects& mDataObjects;
    Core::HierarchyTree& mHierarchyDataObjects;
};

}

}

#endif // DATAOBJECTSHIERARCHYMODEL_H
