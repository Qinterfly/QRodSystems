/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
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

private:
    // Content
    void initializeWindow();
    void createContent();
    ads::CDockWidget* createProjectHierarchyWidget();
    ads::CDockWidget* createGLWidget();
    ads::CDockWidget* createLogWidget();
    ads::CDockWidget* createPropertiesWidget();
    // Signals&Slots
    void specifyMenuConnections();

private slots:
    void aboutProgram();
    void saveSettings();
    void restoreSettings();
    void createDataObjectsManager();
    void createRodPropertiesManager();
    void createRodConstructorManager();

private:
    Ui::MainWindow* mpUi;
    ads::CDockManager* mpDockManager;
    QSharedPointer<QSettings> mpSettings;
    DataObjectsManager* mpDataObjectsManager = nullptr;
    QRS::Project mProject;
    QString mLastPath;

public:
    static LogWidget* pLogger;
};
#endif // MAINWINDOW_H
