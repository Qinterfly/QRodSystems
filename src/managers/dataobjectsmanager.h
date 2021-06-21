/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the DataObjectsManager class
 */

#ifndef DATAOBJECTSMANAGER_H
#define DATAOBJECTSMANAGER_H

#include <unordered_map>
#include "abstractprojectmanager.h"
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

namespace Core
{
class Project;
}

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

class DoubleSpinBoxItemDelegate;

//! Manager to create objects of different types: scalars, vectors, matroces and surfaces
class DataObjectsManager : public AbstractProjectManager
{
    Q_OBJECT

public:
    explicit DataObjectsManager(Core::Project& project, QString& lastPath, QSettings& settings, QWidget* parent = nullptr);
    ~DataObjectsManager();
    void selectDataObject(int iRow);
    Core::DataObjects const& getDataObjects() { return mDataObjects; };

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
    void retrieveDataObjects();
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
    // Delegates
    DoubleSpinBoxItemDelegate* mpItemDelegate = nullptr;
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
