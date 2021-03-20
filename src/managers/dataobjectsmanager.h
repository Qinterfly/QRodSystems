/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of DataObjectsManager class
 */

#ifndef DATAOBJECTSMANAGER_H
#define DATAOBJECTSMANAGER_H

#include <QDialog>

namespace Ui
{
class DataObjectsManager;
}

namespace ads
{
class CDockManager;
class CDockWidget;
}

class QTabWidget;
class QSettings;

//! Manager to create objects of different types: scalars, vectors, matroces and surfaces
class DataObjectsManager : public QDialog
{
    Q_OBJECT

public:
    explicit DataObjectsManager(QSettings& settings, QWidget* parent = nullptr);
    ~DataObjectsManager();
    void closeEvent(QCloseEvent* ) override;

private:
    void createContent();
    ads::CDockWidget* createDataTablesWidget();
    ads::CDockWidget* createDataObjectsWidget();
    ads::CDockWidget* createCodeWidget();
    QLayout* createDialogControls();
    void restoreSettings();
    void saveSettings();

private slots:
    void apply();

private:
    Ui::DataObjectsManager* mpUi;
    ads::CDockManager* mpDockManager;
    QTabWidget* mpDataTables;
    QSettings& mSettings;
};

#endif // DATAOBJECTSMANAGER_H
