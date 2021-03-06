/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the ProjectHierarchyModel class
 */

#ifndef PROJECTHIERARCHYMODEL_H
#define PROJECTHIERARCHYMODEL_H

#include "models/hierarchy/abstracthierarchymodel.h"
#include "core/aliasdata.h"
#include "core/project.h"

namespace QRS::HierarchyModels
{

class DataObjectsHierarchyItem;
class RodComponentsHierarchyItem;

//! Project hierarchy representative
class ProjectHierarchyModel : public AbstractHierarchyModel
{
    Q_OBJECT

public:
    ProjectHierarchyModel(QString const& mimeType, QTreeView* pView = nullptr);
    void updateContent() override;
    void clearContent() override;
    void setProject(Core::Project* pProject);

signals:
    void selectionValidated(QVector<QRS::HierarchyModels::AbstractHierarchyItem*> validatedItems);

public slots:
    void validateItemSelection();

private:
    DataObjectsHierarchyItem* retrieveDataObjectsItem();
    RodComponentsHierarchyItem* retrieveRodComponentsItem();

private:
    Core::Project* mpProject = nullptr;
};

}

#endif // PROJECTHIERARCHYMODEL_H
