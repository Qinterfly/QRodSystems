/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of MainWindow class
 */

#include <QToolBar>
#include <QTableWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QTextEdit>

#include "DockManager.h"

#include "mainwindow.h"
#include "ui_mainWindow.h"
#include "controltabs.h"
#include "view3dwidget.h"

using ads::CDockManager;
using ads::CDockWidget;

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
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    // Tabbed toolbar
    mpUi->tabControlWidget->addTab(new ManagersTab(), tr("Managers"));
    mpUi->tabControlWidget->addTab(new QWidget(), tr("Model"));
    mpUi->tabControlWidget->addTab(new QWidget(), tr("Mesh"));
    mpUi->tabControlWidget->addTab(new QWidget(), tr("Analysis"));
    mpUi->tabControlWidget->addTab(new QWidget(), tr("Plot"));
    mpUi->tabControlWidget->addTab(new QWidget(), tr("View"));
    // Dock manager
    mpDockManager = new CDockManager();
    QHBoxLayout* pLayout = new QHBoxLayout();
    pLayout->addWidget(mpDockManager);
    pLayout->setContentsMargins(0, 0, 0, 0);
    mpUi->dockHolderWidget->setLayout(pLayout);
    ads::CDockWidget* pDockWidget = nullptr;
    // OpenGL window
    pDockWidget = createGLWindow();
    mpDockManager->addDockWidget(ads::TopDockWidgetArea, pDockWidget);
    // Log window
    pDockWidget = createLogWindow();
    mpDockManager->addDockWidget(ads::BottomDockWidgetArea, pDockWidget);
    // Project hierarchy
    pDockWidget = createProjectHierarchy();
    mpDockManager->addDockWidget(ads::LeftDockWidgetArea, pDockWidget);
    // Properties
    pDockWidget = createPropertiesWindow();
    mpDockManager->addDockWidget(ads::LeftDockWidgetArea, pDockWidget);
}

//! Create a widget to represent a project hierarchy
CDockWidget* MainWindow::createProjectHierarchy()
{
    QTreeView* pWidget = new QTreeView();
    pWidget->setFrameShape(QFrame::NoFrame);
    QFileSystemModel* pModel = new QFileSystemModel(pWidget);
    pModel->setRootPath(QDir::currentPath());
    pWidget->setModel(pModel);
    CDockWidget* pDockWidget = new CDockWidget(tr("Project Hierarchy"));
    pDockWidget->setWidget(pWidget);
    pDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromContent);
    mpUi->menuWindow->addAction(pDockWidget->toggleViewAction());
    return pDockWidget;
}

//! Create an OpenGL widget
CDockWidget* MainWindow::createGLWindow()
{
    View3DWidget* pWidget = new View3DWidget();
    CDockWidget* pDockWidget = new CDockWidget(tr("Rod System"));
    pDockWidget->setWidget(pWidget);
    mpUi->menuWindow->addAction(pDockWidget->toggleViewAction());
    return pDockWidget;
}

//! Create a window for logging
CDockWidget* MainWindow::createLogWindow()
{
    QTextEdit * pWidget = new QTextEdit();
    pWidget->setReadOnly(true);
    CDockWidget* pDockWidget = new CDockWidget(tr("Logging"));
    pDockWidget->setWidget(pWidget);
    mpUi->menuWindow->addAction(pDockWidget->toggleViewAction());
    return pDockWidget;
}

//! Create a window to modify properies of selected objercts
CDockWidget* MainWindow::createPropertiesWindow()
{
    QTableWidget * pWidget = new QTableWidget();
    pWidget->setColumnCount(3);
    pWidget->setRowCount(50);
    CDockWidget* pDockWidget = new CDockWidget(tr("Properties"));
    pDockWidget->setWidget(pWidget);
    pDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromContent);
    mpUi->menuWindow->addAction(pDockWidget->toggleViewAction());
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
