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

QT_BEGIN_NAMESPACE
namespace Ui
{
class DataObjectsManager;
}
class QTreeView;
class QSettings;
class QListWidget;
QT_END_NAMESPACE

class InterfaceTableModel;
class BaseTableModel;
class MatrixTableModel;

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

//! Manager to create objects of different types: scalars, vectors, matroces and surfaces
class DataObjectsManager : public QDialog
{
    Q_OBJECT

    using mapDataObjects = std::unordered_map<QRS::DataIDType, QRS::AbstractDataObject*>;

public:
    explicit DataObjectsManager(QRS::Project& project, QSettings& settings, QWidget* parent = nullptr);
    ~DataObjectsManager();
    void closeEvent(QCloseEvent* ) override;
    void selectDataObject(int index);

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

public slots:
    void apply();
    void addScalar();
    void addVector();
    void addMatrix();
    void addSurface();
    void insertItemAfterSelected();
    void removeSelectedItem();
    const mapDataObjects& getDataObjects() { return mDataObjects; };
    void removeSelectedDataObject();

private slots:
    void representSelectedDataObject();

private:
    void emplaceDataObject(QRS::AbstractDataObject* dataObject, QIcon const& icon, QString const& name);

private:
    Ui::DataObjectsManager* mpUi;
    // Docks
    ads::CDockManager* mpDockManager;
    // Widgets
    QListWidget* mpListObjects;
    QTreeView* mpDataTable;
    // Data
    QRS::Project& mProject;
    QSettings& mSettings;
    mapDataObjects mDataObjects;
    // Models
    InterfaceTableModel* mpInterfaceTableModel = nullptr;
    BaseTableModel* mpBaseTableModel;
    MatrixTableModel* mpMatrixTableModel;
};

#endif // DATAOBJECTSMANAGER_H
