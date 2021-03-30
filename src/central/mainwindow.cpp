/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the MainWindow class
 */

#include <QDesktopWidget>
#include <QToolBar>
#include <QTableWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include "DockManager.h"
#include "DockWidget.h"
#include "ads_globals.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controltabs.h"
#include "view3d.h"
#include "logwidget.h"
#include "../managers/dataobjectsmanager.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::CDockAreaWidget;

void moveToCenter(QWidget*);
LogWidget* MainWindow::pLogger = nullptr;
const static QString skFileNameSettings = "Settings.ini";
const static QString skDefaultProjectName = "Project 1";
static QString const& skProjectExtension = QRS::Project::getFileExtension();

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , mpUi(new Ui::MainWindow)
{
    initializeWindow();
    createContent();
    specifyMenuConnections();
    restoreSettings();
}

MainWindow::~MainWindow()
{
    delete mpUi;
}

//! Set a state and geometry of MainWindow
void MainWindow::initializeWindow()
{
    mpUi->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    setWindowModified(true);
}

//! Create all the widgets and corresponding actions
void MainWindow::createContent()
{
    mLastPath = '.' + QDir::separator();
    mpProject = new QRS::Project(skDefaultProjectName);
    mpSettings = QSharedPointer<QSettings>(new QSettings(skFileNameSettings, QSettings::IniFormat));
    setProjectTitle();
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
    // Managers
    ManagersTab* pManagersTab = new ManagersTab();
    pTabControlWidget->addTab(pManagersTab, tr("Managers"));
    connect(pManagersTab, &ManagersTab::actionDataObjectsTriggered, this, &MainWindow::createDataObjectsManager);
    connect(pManagersTab, &ManagersTab::actionRodPropertiesTriggered, this, &MainWindow::createRodPropertiesManager);
    connect(pManagersTab, &ManagersTab::actionRodConstructorTriggered, this, &MainWindow::createRodConstructorManager);
    // Other
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
    pDockWidget = createGLWidget();
    mpDockManager->addDockWidget(ads::TopDockWidgetArea, pDockWidget);
    // Log window
    pDockWidget = createLogWidget();
    mpDockManager->addDockWidget(ads::BottomDockWidgetArea, pDockWidget);
    // Project hierarchy
    pDockWidget = createProjectHierarchyWidget();
    CDockAreaWidget* pArea = mpDockManager->addDockWidget(ads::LeftDockWidgetArea, pDockWidget);
    // Properties
    pDockWidget = createPropertiesWidget();
    mpDockManager->addDockWidget(ads::BottomDockWidgetArea, pDockWidget, pArea);
    // Views
    mpUi->menuWindow->addSeparator();
    mpUi->menuWindow->addAction(tr("&Save Settings"), this, &MainWindow::saveSettings);
    mpUi->menuWindow->addAction(tr("&Restore Settings"), this, &MainWindow::restoreSettings);
    qInfo() << tr("Application successfully started");
}

//! Create a widget to represent a project hierarchy
CDockWidget* MainWindow::createProjectHierarchyWidget()
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

//! Create an OpenGL widget to render rods
CDockWidget* MainWindow::createGLWidget()
{
    View3D* pWidget = new View3D();
    CDockWidget* pDockWidget = new CDockWidget(tr("Rod System"));
    pDockWidget->setWidget(pWidget);
    mpUi->menuWindow->addAction(pDockWidget->toggleViewAction());
    return pDockWidget;
}

//! Create a window for logging
CDockWidget* MainWindow::createLogWidget()
{
    pLogger = new LogWidget();
    CDockWidget* pDockWidget = new CDockWidget(tr("Logging"));
    pDockWidget->setWidget(pLogger);
    mpUi->menuWindow->addAction(pDockWidget->toggleViewAction());
    return pDockWidget;
}

//! Create a window to modify properies of selected objercts
CDockWidget* MainWindow::createPropertiesWidget()
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
    // Project
    connect(mpUi->actionNewProject, &QAction::triggered, this, &MainWindow::createProject);
    connect(mpUi->actionOpenProject, &QAction::triggered, this, &MainWindow::openProject);
    connect(mpUi->actionSaveProject, &QAction::triggered, this, &MainWindow::saveProject);
    connect(mpUi->actionSaveProject, &QAction::triggered, this, &MainWindow::saveProjectAs);
    connect(mpUi->actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(mpProject, &QRS::Project::modified, this, &MainWindow::setWindowModified);
    // Help
    connect(mpUi->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(mpUi->actionAboutProgram, &QAction::triggered, this, &MainWindow::aboutProgram);
}

//! Save the current window settings
void MainWindow::saveSettings()
{
    mpSettings->setValue("mainWindow/Geometry", saveGeometry());
    mpSettings->setValue("mainWindow/State", saveState());
    mpSettings->setValue("mainWindow/DockingState", mpDockManager->saveState());
    mpDockManager->savePerspectives(*mpSettings);
    if (mpSettings->status() == QSettings::NoError)
        qInfo() << tr("Settings were written to the file") << skFileNameSettings;
}

//! Restore window settings from a file
void MainWindow::restoreSettings()
{
    bool isOk = restoreGeometry(mpSettings->value("mainWindow/Geometry").toByteArray())
                && restoreState(mpSettings->value("mainWindow/State").toByteArray())
                && mpDockManager->restoreState(mpSettings->value("mainWindow/DockingState").toByteArray());
    mpDockManager->loadPerspectives(*mpSettings);
    if (isOk)
        qInfo() << tr("Settings were restored from the file") << skFileNameSettings;
    else
        qWarning() << tr("An error occured while reading settings from the file") << skFileNameSettings;;
}

//! Show a manager for designing data objects
void MainWindow::createDataObjectsManager()
{
    if (mpDataObjectsManager && mpDataObjectsManager->isVisible())
        return;
    mpDataObjectsManager = new DataObjectsManager(*mpProject, *mpSettings, mpUi->centralWidget);
    moveToCenter(mpDataObjectsManager);
    mpDataObjectsManager->show();
}

//! Show a manager to set rod properties based on the created data objects
void MainWindow::createRodPropertiesManager()
{
    // TODO
}

//! Show a manager to create a rod with assigned data properties
void MainWindow::createRodConstructorManager()
{
    // TODO
}

//! Create a project and substitute the current one with it
void MainWindow::createProject()
{
    saveProjectChangesDialog();
    delete mpProject;
    mpProject = new QRS::Project(skDefaultProjectName);
    connect(mpProject, &QRS::Project::modified, this, &MainWindow::setWindowModified);
    setProjectTitle();
}

//! Open a project
void MainWindow::openProject()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"),
                       mLastPath, tr("Project file format (*%1)").arg(skProjectExtension));
    if (!fileName.isEmpty())
    {
        QFileInfo info(fileName);
        QString path = info.path();
        fileName = info.baseName();
        delete mpProject;
        mpProject = new QRS::Project(path, fileName);
        connect(mpProject, &QRS::Project::modified, this, &MainWindow::setWindowModified);
        setWindowModified(false);
        mLastPath = path;
        setProjectTitle();
    }
}

//! Save the current project
bool MainWindow::saveProject()
{
    QString filePath = mpProject->filePath();
    if (filePath.isEmpty())
        return saveProjectAs();
    return saveProjectHelper(filePath);
}

//! Save the current project under a new name
bool MainWindow::saveProjectAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Project"), mLastPath,
                       tr("Project file format (*%1)").arg(skProjectExtension));
    return saveProjectHelper(filePath);
}

//! Helper method to perform saving of the current project
bool MainWindow::saveProjectHelper(QString const& filePath)
{
    if (filePath.isEmpty())
        return false;
    QFileInfo info(filePath);
    QString path = info.path();
    QString fileName = info.baseName();
    bool isSaved = mpProject->save(path, fileName);
    mLastPath = path;
    setProjectTitle();
    return isSaved;
}

//! Save project changes
void MainWindow::saveProjectChangesDialog()
{
    if (isWindowModified())
    {
        const QMessageBox::StandardButton res = QMessageBox::warning(this, tr("Save project changes"),
                                                tr(
                                                        "The project has been modified.\n"
                                                        "Would you like to save it"
                                                ),
                                                QMessageBox::Yes | QMessageBox::No);
        if (res == QMessageBox::Yes)
            saveProject();
    }
}

//! Save project and settings before exit
void MainWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();
    if (isWindowModified())
    {
        bool isClosed = false;
        const QMessageBox::StandardButton res = QMessageBox::warning(this, tr("Save project changes"),
                                                tr(
                                                        "The project has been modified.\n"
                                                        "Would you like to save it before exit?"
                                                ),
                                                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (res)
        {
        case QMessageBox::Save:
            isClosed = saveProject();
            break;
        case QMessageBox::Cancel:
            isClosed = false;
            break;
        case QMessageBox::Discard:
            isClosed = true;
            break;
        default:
            break;
        }
        if (isClosed)
            event->accept();
    }
    else
    {
        event->accept();
    }
}

//! Show information a name of a project
void MainWindow::setProjectTitle()
{
    QString title = APP_NAME;
    setWindowTitle(QString(title + ": %1[*]").arg(mpProject->name()));
}

//! Show information about a program
void MainWindow::aboutProgram()
{
    const QString aboutMsg = tr(
                                 "QRodSystems is a multiplatform wrapper to create rod systems by means of the KLPALGSYS core. "
                                 "You can download the code from <a href='https://github.com/qinterfly/QRodSystems'>GitHub</a>. If you find any "
                                 "bug or problem, please report it in <a href='https://github.com/qinterfly/QRodSystems/issues'>the issues "
                                 "page</a> so I can fix it as soon as possible.<br><br>"
                                 "Copyright &copy; 2021 QRodSystems (Pavel Lakiza) "
                                 "Copyright &copy; 2021 KLPALGSYS (Dmitriy Krasnorutskiy)"
                             );
    QMessageBox::about(this, tr("About QRodSystems v%1").arg(APP_VERSION), aboutMsg);
}

//! Helper function to situate widgets at the center of their parent widgets
void moveToCenter(QWidget* pWidget)
{
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(pWidget->parentWidget());
    int x = (screenGeometry.width() - pWidget->width()) / 2;
    int y = (screenGeometry.height() - pWidget->height()) / 2;
    pWidget->move(x, y);
}
