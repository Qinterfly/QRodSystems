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
#include <QVBoxLayout>
#include <QSettings>
#include <QXmlStreamWriter>
#include "DockManager.h"
#include "DockWidget.h"
#include "ads_globals.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controltabs.h"
#include "view3dwidget.h"
#include "logwidget.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::CDockAreaWidget;

const static QString kFileNameViewSettings = "ViewSettings.ini";
LogWidget* MainWindow::pLogger = nullptr;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , mpUi(new Ui::MainWindow)
{
    mpUi->setupUi(this);
    createContent();
    specifyMenuConnections();
    restoreViewSettings();
}

MainWindow::~MainWindow()
{
    delete mpUi;
}

//! Create all the widgets and corresponding actions
void MainWindow::createContent()
{
    // Configuration
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    QVBoxLayout* pLayout = new QVBoxLayout(mpUi->centralWidget);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->setSpacing(1);
    mpUi->centralWidget->setLayout(pLayout);
    // Tabbed toolbar
    QTabWidget* pTabControlWidget = new QTabWidget();
    pTabControlWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    pTabControlWidget->setMaximumHeight(95);
    pTabControlWidget->addTab(new ManagersTab(), tr("Managers"));
    pTabControlWidget->addTab(new QWidget(), tr("Model"));
    pTabControlWidget->addTab(new QWidget(), tr("Mesh"));
    pTabControlWidget->addTab(new QWidget(), tr("Analysis"));
    pTabControlWidget->addTab(new QWidget(), tr("Plot"));
    pTabControlWidget->addTab(new QWidget(), tr("View"));
    pLayout->addWidget(pTabControlWidget);
    // Dock manager
    mpDockManager = new CDockManager();
    pLayout->addWidget(mpDockManager);;
    ads::CDockWidget* pDockWidget = nullptr;
    // OpenGL window
    pDockWidget = createGLWindow();
    mpDockManager->addDockWidget(ads::TopDockWidgetArea, pDockWidget);
    // Log window
    pDockWidget = createLogWindow();
    mpDockManager->addDockWidget(ads::BottomDockWidgetArea, pDockWidget);
    // Project hierarchy
    pDockWidget = createProjectHierarchy();
    CDockAreaWidget* pArea = mpDockManager->addDockWidget(ads::LeftDockWidgetArea, pDockWidget);
    // Properties
    pDockWidget = createPropertiesWindow();
    mpDockManager->addDockWidget(ads::BottomDockWidgetArea, pDockWidget, pArea);
    // Views
    mpUi->menuWindow->addSeparator();
    mpUi->menuWindow->addAction("&Save View Settings", this, &MainWindow::saveViewSettings);
    mpUi->menuWindow->addAction("&Restore View Settings", this, &MainWindow::restoreViewSettings);
    qInfo() << "Application successfully started";
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
    pLogger = new LogWidget();
    CDockWidget* pDockWidget = new CDockWidget(tr("Logging"));
    pDockWidget->setWidget(pLogger);
    mpUi->menuWindow->addAction(pDockWidget->toggleViewAction());
    return pDockWidget;
}

//! Create a window to modify properies of selected objercts
CDockWidget* MainWindow::createPropertiesWindow()
{
    QTableWidget* pWidget = new QTableWidget();
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

//! Save the current view state
void MainWindow::saveViewSettings()
{
    QSettings settings(kFileNameViewSettings, QSettings::IniFormat);
    settings.setValue("mainWindow/Geometry", saveGeometry());
    settings.setValue("mainWindow/State", saveState());
    settings.setValue("mainWindow/DockingState", mpDockManager->saveState());
    if (settings.status() == QSettings::NoError)
        qInfo() << "View settings were written to the file" << kFileNameViewSettings;
}

//! Restore a view state from a file
void MainWindow::restoreViewSettings()
{
    QSettings settings(kFileNameViewSettings, QSettings::IniFormat);
    bool isOk = restoreGeometry(settings.value("mainWindow/Geometry").toByteArray())
                && restoreState(settings.value("mainWindow/State").toByteArray())
                && mpDockManager->restoreState(settings.value("mainWindow/DockingState").toByteArray());
    if (isOk)
        qInfo() << "View settings were restored from the file" << kFileNameViewSettings;
    else
        qWarning() << "An error occured while reading view settings from the file" << kFileNameViewSettings;;
}

//! Show a manager for designing data objects
void MainWindow::createDataObjectsManager()
{
    // TODO
}


