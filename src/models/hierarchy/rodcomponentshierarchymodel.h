/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the RodComponentsHierarchyModel class
 */

#ifndef RODCOMPONENTSHIERARCHYMODEL_H
#define RODCOMPONENTSHIERARCHYMODEL_H

#include "models/hierarchy/abstracthierarchymodel.h"
#include "core/aliasdataset.h"

namespace QRS
{

namespace HierarchyModels
{

//! Tree model to represent and modify a hierarchy of rod components
class RodComponentsHierarchyModel : public AbstractHierarchyModel
{
public:
    RodComponentsHierarchyModel(Core::RodComponents& rodComponents, Core::HierarchyTree& hierarchyRodComponents,
                                QTreeView* pView = nullptr);
    ~RodComponentsHierarchyModel() = default;
    void updateContent() override;
    void clearContent() override;
    bool isEmpty() const;
    void selectItem(int iRow);

public slots:
    void retrieveSelectedRodComponent();

private slots:
    void renameRodComponent(QStandardItem* pStandardItem);

private:
    Core::RodComponents& mRodComponents;
    Core::HierarchyTree& mHierarchyRodComponents;
};

}

}

#endif // RODCOMPONENTSHIERARCHYMODEL_H
