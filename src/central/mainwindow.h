/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the MainWindow class
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logwidget.h"
#include "../core/project.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
class QSettings;
class QLabel;
QT_END_NAMESPACE

namespace ads
{
class CDockManager;
class CDockWidget;
}
class DataObjectsManager;

//! The main window of the program
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void openProject(QString const& filePath);
    bool saveProject();

private:
    // Content
    void initializeWindow();
    void createContent();
    void closeEvent(QCloseEvent* event) override;
    ads::CDockWidget* createProjectHierarchyWidget();
    ads::CDockWidget* createGLWidget();
    ads::CDockWidget* createLogWidget();
    ads::CDockWidget* createPropertiesWidget();
    void setProjectTitle();
    void retrieveRecentProjects();
    void addToRecentProjects();
    // Signals & Slots
    void specifyMenuConnections();
    // Project
    bool saveProjectChangesDialog();
    bool saveProjectHelper(QString const& filePath);

private slots:
    // Project
    void createProject();
    void openProjectDialog();
    void openRecentProject();
    bool saveAsProject();
    void projectModified();
    // Settings
    void saveSettings();
    void restoreSettings();
    // Managers
    void createDataObjectsManager();
    void createRodPropertiesManager();
    void createRodConstructorManager();
    // Help
    void aboutProgram();

private:
    Ui::MainWindow* mpUi;
    ads::CDockManager* mpDockManager;
    QLabel* mpStatusLabel;
    QSharedPointer<QSettings> mpSettings;
    DataObjectsManager* mpDataObjectsManager = nullptr;
    QRS::Project* mpProject;
    QString mLastPath;
    QList<QString> mPathRecentProjects;

public:
    static LogWidget* pLogger;
};

//! Log all the messages
inline void throwMessage(QtMsgType type, const QMessageLogContext& /*context*/, const QString& message)
{
    MainWindow::pLogger->log(type, message);
}

#endif // MAINWINDOW_H
