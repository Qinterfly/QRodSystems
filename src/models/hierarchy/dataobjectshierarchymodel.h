/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
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

class DataObjectsHierarchyItem;
//! Tree model to represent and modify a hierarchy of data objects
class DataObjectsHierarchyModel : public AbstractHierarchyModel
{
    Q_OBJECT

public:
    DataObjectsHierarchyModel(Core::DataObjects& dataObjects, Core::HierarchyTree& hierarchyDataObjects,
                              QString const& mimeType, QTreeView* pView = nullptr);
    ~DataObjectsHierarchyModel() = default;
    void updateContent() override;
    void clearContent() override;
    bool isEmpty() const;
    void selectItem(int iRow);
    void selectItemByID(Core::DataIDType id);

signals:
    void selected(Core::DataIDType id);
    void selectionCleared();

public slots:
    void retrieveSelectedItem();
    void removeSelectedItems();

private slots:
    void renameItem(QStandardItem* pStandardItem);

private:
    DataObjectsHierarchyItem* findItemByID(DataObjectsHierarchyItem* pItem, Core::DataIDType const& id);
    void selectItem(DataObjectsHierarchyItem* pItem);

private:
    Core::DataObjects& mDataObjects;
    Core::HierarchyTree& mHierarchyDataObjects;
};

}

}

#endif // DATAOBJECTSHIERARCHYMODEL_H
