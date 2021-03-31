/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
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
#include "uiconstants.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::CDockAreaWidget;

LogWidget* MainWindow::pLogger = nullptr;
const static QString skDefaultProjectName = "Default";
static QString const& skProjectExtension = QRS::Project::getFileExtension();
const static QString skSettingsFileName = "Settings.ini";
const static QString skMainWindow = "MainWindow";
const static QString skRecentProjects = "RecentProjects";

void moveToCenter(QWidget*);

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
    setWindowModified(false);
}

//! Create all the widgets and corresponding actions
void MainWindow::createContent()
{
    mLastPath = '.' + QDir::separator();
    mpProject = new QRS::Project(skDefaultProjectName);
    mpSettings = QSharedPointer<QSettings>(new QSettings(skSettingsFileName, QSettings::IniFormat));
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
    connect(mpUi->actionSaveAsProject, &QAction::triggered, this, &MainWindow::saveAsProject);
    connect(mpUi->actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(mpProject, &QRS::Project::modified, this, &MainWindow::projectModified);
    // Help
    connect(mpUi->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(mpUi->actionAboutProgram, &QAction::triggered, this, &MainWindow::aboutProgram);
}

//! Save the current window settings
void MainWindow::saveSettings()
{
    mpSettings->beginGroup(skMainWindow);
    mpSettings->setValue(UiConstants::Settings::skGeometry, saveGeometry());
    mpSettings->setValue(UiConstants::Settings::skState, saveState());
    mpSettings->setValue(UiConstants::Settings::skDockingState, mpDockManager->saveState());
    mpSettings->endGroup();
    if (mpSettings->status() == QSettings::NoError)
        qInfo() << tr("Settings were written to the file") << skSettingsFileName;
}

//! Restore window settings from a file
void MainWindow::restoreSettings()
{
    mpSettings->beginGroup(skMainWindow);
    bool isOk = restoreGeometry(mpSettings->value(UiConstants::Settings::skGeometry).toByteArray())
                && restoreState(mpSettings->value(UiConstants::Settings::skState).toByteArray())
                && mpDockManager->restoreState(mpSettings->value(UiConstants::Settings::skDockingState).toByteArray());
    mpSettings->endGroup();
    retrieveRecentProjects();
    if (isOk)
        qInfo() << tr("Settings were restored from the file") << skSettingsFileName;
    else
        qWarning() << tr("An error occured while reading settings from the file") << skSettingsFileName;
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
    if (!saveProjectChangesDialog())
        return;
    delete mpProject;
    mpProject = new QRS::Project(skDefaultProjectName);
    connect(mpProject, &QRS::Project::modified, this, &MainWindow::projectModified);
    setWindowModified(false);
    setProjectTitle();
}

//! Open a project
void MainWindow::openProject()
{
    if (!saveProjectChangesDialog())
        return;
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Project"),
                       mLastPath, tr("Project file format (*%1)").arg(skProjectExtension));
    openProjectHelper(filePath);
}

//! Open the project which was selected from the Recent Projects menu
void MainWindow::openRecentProject()
{
    if (!saveProjectChangesDialog())
        return;
    QAction* pAction = (QAction*)sender();
    if (pAction)
        openProjectHelper(pAction->text());
}

//! Helper method to perform opening of the specified project
void MainWindow::openProjectHelper(QString const& filePath)
{
    if (filePath.isEmpty())
        return;
    QFileInfo info(filePath);
    QString path = info.path();
    QString baseName = info.baseName();
    delete mpProject;
    mpProject = new QRS::Project(path, baseName);
    connect(mpProject, &QRS::Project::modified, this, &MainWindow::projectModified);
    setWindowModified(false);
    mLastPath = path;
    setProjectTitle();
    addToRecentProjects();
}

//! Whenever a project has been modified
void MainWindow::projectModified()
{
    setWindowModified(mpProject->isModified());
    setProjectTitle();
}

//! Save the current project
bool MainWindow::saveProject()
{
    QString filePath = mpProject->filePath();
    if (filePath.isEmpty())
        return saveAsProject();
    return saveProjectHelper(filePath);
}

//! Save the current project under a new name
bool MainWindow::saveAsProject()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Project"), mLastPath,
                       tr("Project file format (*%1)").arg(skProjectExtension));
    bool isSaved = saveProjectHelper(filePath);
    if (isSaved)
        addToRecentProjects();
    return isSaved;
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
bool MainWindow::saveProjectChangesDialog()
{
    if (isWindowModified())
    {
        const QMessageBox::StandardButton res = QMessageBox::warning(this, tr("Save project changes"),
                                                tr(
                                                        "The project containes unsaved changes.\n"
                                                        "Would you like to save it?"
                                                ),
                                                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (res)
        {
        case QMessageBox::Yes:
            saveProject();
            break;
        case QMessageBox::Cancel:
            return false;
        default:
            break;
        }
    }
    return true;
}

//! Save project and settings before exit
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (saveProjectChangesDialog())
        event->accept();
    else
        event->ignore();
}

//! Show information a name of a project
void MainWindow::setProjectTitle()
{
    QString title = APP_NAME;
    setWindowTitle(QString(title + ": %1[*]").arg(mpProject->name()));
}

//! Retrieve recent projects from the settings file
void MainWindow::retrieveRecentProjects()
{
    QList<QVariant> listSettingsProjects = mpSettings->value(skRecentProjects).toList();
    mPathRecentProjects.clear();
    mpUi->menuRecentProjects->clear();
    QString pathProject;
    QList<QVariant> listUpdatedPaths;
    for (auto& varPath : listSettingsProjects)
    {
        pathProject = varPath.toString();
        if (QFileInfo::exists(pathProject))
        {
            listUpdatedPaths.push_back(pathProject);
            QAction* pAction = mpUi->menuRecentProjects->addAction(pathProject);
            connect(pAction, &QAction::triggered, this, &MainWindow::openRecentProject);
            mPathRecentProjects.push_back(pathProject);
        }
    }
    mpSettings->setValue(skRecentProjects, listUpdatedPaths);
}

//! Add the current project to the recent ones
void MainWindow::addToRecentProjects()
{
    static const int kMaxRecentProjects = 5;
    QString const& filePath = mpProject->filePath();
    if (!filePath.isEmpty())
    {
        if (!mPathRecentProjects.contains(filePath))
            mPathRecentProjects.push_back(filePath);
        while (mPathRecentProjects.count() > kMaxRecentProjects)
            mPathRecentProjects.pop_front();
        mpUi->menuRecentProjects->clear();
        QList<QVariant> listSettingsProjects;
        for (auto& path : mPathRecentProjects)
        {
            listSettingsProjects.push_back(path);
            QAction* pAction = mpUi->menuRecentProjects->addAction(path);
            connect(pAction, &QAction::triggered, this, &MainWindow::openRecentProject);
        }
        mpSettings->setValue(skRecentProjects, listSettingsProjects);
    }
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
