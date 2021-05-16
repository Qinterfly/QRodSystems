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
namespace Ui
{
class DataObjectsManager;
}
class QTreeView;
class QSettings;
class QListWidget;
class QListWidgetItem;
QT_END_NAMESPACE

class TableModelInterface;
class BaseTableModel;
class MatrixTableModel;
class SurfaceTableModel;
class DataObjectsHierarchyModel;

namespace ads
{
class CDockManager;
class CDockWidget;
}
namespace QRS
{
class Project;
class AbstractDataObject;
}

using mapDataObjects = std::unordered_map<QRS::DataIDType, QRS::AbstractDataObject*>;

//! Manager to create objects of different types: scalars, vectors, matroces and surfaces
class DataObjectsManager : public QDialog
{
    Q_OBJECT

public:
    explicit DataObjectsManager(QRS::Project& project, QSettings& settings, QString& lastPath, QWidget* parent = nullptr);
    ~DataObjectsManager();
    void closeEvent(QCloseEvent* event) override;
    void selectDataObject(int iRow);
    mapDataObjects const& getDataObjects() { return mDataObjects; };

public slots:
    void apply();
    QRS::DataIDType addScalar();
    QRS::DataIDType addVector();
    QRS::DataIDType addMatrix();
    QRS::DataIDType addSurface();
    void insertItemAfterSelected();
    void insertLeadingItemAfterSelected();
    void removeSelectedItem();
    void removeSelectedLeadingItem();
    void importDataObjects();
    void representDataObject(QRS::DataIDType id);
    void clearDataObjectRepresentation();

private:
    // Content
    void createContent();
    ads::CDockWidget* createDataTableWidget();
    ads::CDockWidget* createDataObjectsWidget();
    ads::CDockWidget* createCodeWidget();
    QLayout* createDialogControls();
    void retrieveDataObjects();
    // Settings
    void restoreSettings();
    void saveSettings();
    // Helpers
    void emplaceDataObject(QRS::AbstractDataObject* pDataObject);
    void addListDataObjects(QRS::AbstractDataObject* pDataObject);
    bool isDataTableModifiable();
    void importDataObject(QString const& path, QString const& fileName);

private:
    Ui::DataObjectsManager* mpUi;
    // Docks
    ads::CDockManager* mpDockManager;
    // Widgets
    QTreeView* mpTreeDataObjects;
    QTreeView* mpDataTable;
    // Data
    QRS::Project& mProject;
    QSettings& mSettings;
    mapDataObjects mDataObjects;
    QRS::HierarchyTree mHierarchyDataObjects;
    QString& mLastPath;
    // Models
    TableModelInterface* mpTableModelInterface = nullptr;
    BaseTableModel* mpBaseTableModel;
    MatrixTableModel* mpMatrixTableModel;
    SurfaceTableModel* mpSurfaceTableModel;
    DataObjectsHierarchyModel* mpTreeDataObjectsModel;
};

#endif // DATAOBJECTSMANAGER_H
