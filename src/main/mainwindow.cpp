/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of MainWindow class
 */

#include <QToolBar>
#include <QTreeView>
#include <QFileSystemModel>
#include "DockManager.h"
#include "DockAreaWidget.h"
#include "mainwindow.h"
#include "ui_mainWindow.h"
#include "controltabs.h"
#include "view3dwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mpUi(new Ui::MainWindow)
{
    mpUi->setupUi(this);
    createContent();
    specifyMenuConnections();
}

MainWindow::~MainWindow()
{
    delete mpUi;
}

//! Create all the widgets and corresponding actions
void MainWindow::createContent()
{
    //  Configuration
    ads::CDockManager::setConfigFlag(ads::CDockManager::FocusHighlighting, true);
    // Tabbed toolbar
    mpUi->tabControlWidget->addTab(new ManagersTab(), tr("Managers"));
    // Dock manager
    mpDockManager = new ads::CDockManager();
    QHBoxLayout* pLayout = new QHBoxLayout();
    pLayout->addWidget(mpDockManager);
    pLayout->setContentsMargins(0, 0, 0, 0);
    mpUi->dockHolderWidget->setLayout(pLayout);
    ads::CDockWidget* pDockWidget = nullptr;
    // Project hierarchy
    pDockWidget = createProjectHierarchy();
    mpDockManager->addDockWidget(ads::LeftDockWidgetArea, pDockWidget);
    // OpenGL window
    pDockWidget = createGLWindow();
    mpDockManager->addDockWidget(ads::RightDockWidgetArea, pDockWidget);
}

//! Create a widget to represent a project hierarchy
ads::CDockWidget* MainWindow::createProjectHierarchy()
{
    QTreeView* pWidget = new QTreeView();
    pWidget->setFrameShape(QFrame::NoFrame);
    QFileSystemModel* pModel = new QFileSystemModel(pWidget);
    pModel->setRootPath(QDir::currentPath());
    pWidget->setModel(pModel);
    ads::CDockWidget* pDockWidget = new ads::CDockWidget(tr("Project Hierarchy"));
    pDockWidget->setWidget(pWidget);
    return pDockWidget;
}

//! Create an OpenGL widget
ads::CDockWidget* MainWindow::createGLWindow()
{
    View3DWidget* pWidget = new View3DWidget();
    ads::CDockWidget* pDockWidget = new ads::CDockWidget(tr("Rod System"));
    pDockWidget->setWidget(pWidget);
    return pDockWidget;
}

//! Set signals and slots for menu actions
void MainWindow::specifyMenuConnections()
{
    // File
    connect(mpUi->actionExit, &QAction::triggered, this, &QMainWindow::close);
    // Help
    connect(mpUi->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
}

//! Show a manager for designing data objects
void MainWindow::createDataObjectsManager()
{
    // TODO
}
