/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of MainWindow class
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace ads
{
class CDockManager;
class CDockWidget;
}

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

//! The main window of the program
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    // Content
    void createContent();
    ads::CDockWidget* createProjectHierarchy();
    ads::CDockWidget* createGLWindow();
    ads::CDockWidget* createLogWindow();
    ads::CDockWidget* createPropertiesWindow();
    // Signals&Slots
    void specifyMenuConnections();

private slots:
    void createDataObjectsManager();

private:
    Ui::MainWindow* mpUi;
    ads::CDockManager* mpDockManager;
};
#endif // MAINWINDOW_H
