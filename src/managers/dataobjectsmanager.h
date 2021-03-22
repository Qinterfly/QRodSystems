/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of DataObjectsManager class
 */

#ifndef DATAOBJECTSMANAGER_H
#define DATAOBJECTSMANAGER_H

#include <QSignalMapper>
#include <QDialog>
#include <unordered_map>
#include "../core/datatypes.h"

namespace Ui
{
class DataObjectsManager;
}

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
class QTabWidget;
class QSettings;
class QListWidget;


//! Manager to create objects of different types: scalars, vectors, matroces and surfaces
class DataObjectsManager : public QDialog
{
    Q_OBJECT

public:
    explicit DataObjectsManager(QRS::Project& project, QSettings& settings, QWidget* parent = nullptr);
    ~DataObjectsManager();
    void closeEvent(QCloseEvent* ) override;

private:
    // Content
    void createContent();
    ads::CDockWidget* createDataTablesWidget();
    ads::CDockWidget* createDataObjectsWidget();
    ads::CDockWidget* createCodeWidget();
    QLayout* createDialogControls();
    void retrieveDataObjects();
    // Settings
    void restoreSettings();
    void saveSettings();

private slots:
    void apply();
    void addScalar();
    void addVector();
    void addMatrix();
    void addSurface();

private:
    Ui::DataObjectsManager* mpUi;
    ads::CDockManager* mpDockManager;
    QTabWidget* mpDataTables;
    QRS::Project& mProject;
    QSettings& mSettings;
    std::unordered_map<QRS::DataIDType, QRS::AbstractDataObject*> mDataObjects;
    QListWidget* mpListObjects;
};

#endif // DATAOBJECTSMANAGER_H
