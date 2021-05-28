/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the DataObjectsManager class
 */

#ifndef DATAOBJECTSMANAGER_H
#define DATAOBJECTSMANAGER_H

#include <QSignalMapper>
#include <QDialog>
#include <unordered_map>
#include "../core/datatypes.h"
#include "../core/hierarchytree.h"

QT_BEGIN_NAMESPACE
class QTreeView;
class QSettings;
class QListWidget;
class QListWidgetItem;
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
class AbstractDataObject;
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

using mapDataObjects = std::unordered_map<Core::DataIDType, Core::AbstractDataObject*>;

//! Manager to create objects of different types: scalars, vectors, matroces and surfaces
class DataObjectsManager : public QDialog
{
    Q_OBJECT

public:
    explicit DataObjectsManager(Core::Project& project, QSettings& settings, QString& lastPath, QWidget* parent = nullptr);
    ~DataObjectsManager();
    void selectDataObject(int iRow);
    mapDataObjects const& getDataObjects() { return mDataObjects; };

signals:
    void closed();

public slots:
    void apply();
    Core::DataIDType addScalar();
    Core::DataIDType addVector();
    Core::DataIDType addMatrix();
    Core::DataIDType addSurface();
    void insertItemAfterSelected();
    void insertLeadingItemAfterSelected();
    void removeSelectedItem();
    void removeSelectedLeadingItem();
    void importDataObjects();
    void representDataObject(Core::DataIDType id);
    void clearDataObjectRepresentation();

private:
    void closeEvent(QCloseEvent* pEvent) override;
    // Content
    void createContent();
    ads::CDockWidget* createDataTableWidget();
    ads::CDockWidget* createDataObjectsWidget();
    QLayout* createDialogControls();
    void retrieveDataObjects();
    // Settings
    void restoreSettings();
    void saveSettings();
    // Helpers
    void emplaceDataObject(Core::AbstractDataObject* pDataObject);
    bool isDataTableModifiable();
    void importDataObject(QString const& path, QString const& fileName);

private:
    // Docks
    ads::CDockManager* mpDockManager;
    // Widgets
    QTreeView* mpTreeDataObjects;
    QTreeView* mpDataTable;
    // Delegates
    DoubleSpinBoxItemDelegate* mpItemDelegate = nullptr;
    // Data
    Core::Project& mProject;
    QSettings& mSettings;
    mapDataObjects mDataObjects;
    Core::HierarchyTree mHierarchyDataObjects;
    QString& mLastPath;
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
