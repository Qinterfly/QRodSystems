/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the DataObjectsManager class
 */

#ifndef DATAOBJECTSMANAGER_H
#define DATAOBJECTSMANAGER_H

#include <unordered_map>
#include "abstractmanager.h"
#include "core/aliasdata.h"
#include "core/aliasdataset.h"
#include "core/hierarchytree.h"

QT_BEGIN_NAMESPACE
class QTreeView;
class QSettings;
QT_END_NAMESPACE

namespace ads
{
class CDockManager;
class CDockWidget;
}

namespace QRS
{

namespace TableModels
{
class TableModelInterface;
class BaseTableModel;
class MatrixTableModel;
class SurfaceTableModel;
}

namespace HierarchyModels
{
class DataObjectsHierarchyModel;
}

namespace Managers
{

//! Manager to create objects of different types: scalars, vectors, matroces and surfaces
class DataObjectsManager : public AbstractManager
{
    Q_OBJECT

public:
    explicit DataObjectsManager(Core::DataObjects&& dataObjects, Core::HierarchyTree&& hierarchyDataObjects,
                                QString& lastPath, QSettings& settings, QWidget* parent = nullptr);
    ~DataObjectsManager();
    void selectDataObject(int iRow);
    void selectDataObjectByID(Core::DataIDType id);
    Core::DataObjects const& getDataObjects() { return mDataObjects; };

signals:
    void applied(Core::DataObjects const& dataObjects, Core::HierarchyTree const& hierarchyDataObjects);

public slots:
    void apply() override;
    Core::AbstractDataObject* addScalar();
    Core::AbstractDataObject* addVector();
    Core::AbstractDataObject* addMatrix();
    Core::AbstractDataObject* addSurface();
    void insertItemAfterSelected();
    void insertLeadingItemAfterSelected();
    void removeSelectedItem();
    void removeSelectedLeadingItem();
    void importDataObjects();

private:
    // Content
    void createContent();
    ads::CDockWidget* createDataTableWidget();
    ads::CDockWidget* createHierarchyWidget();
    QLayout* createDialogControls();
    // Helpers
    void emplaceDataObject(Core::AbstractDataObject* pDataObject);
    bool isDataTableModifiable();
    void importDataObject(QString const& path, QString const& fileName);
    // Selection
    void representDataObject(Core::DataIDType id);
    void clearDataObjectRepresentation();

private:
    // Widgets
    QTreeView* mpTreeDataObjects;
    QTreeView* mpDataTable;
    // Data
    Core::DataObjects mDataObjects;
    Core::HierarchyTree mHierarchyDataObjects;
    // Models
    TableModels::TableModelInterface* mpTableModelInterface = nullptr;
    TableModels::BaseTableModel* mpBaseTableModel;
    TableModels::MatrixTableModel* mpMatrixTableModel;
    TableModels::SurfaceTableModel* mpSurfaceTableModel;
    HierarchyModels::DataObjectsHierarchyModel* mpTreeDataObjectsModel;
};

}

}

#endif // DATAOBJECTSMANAGER_H
