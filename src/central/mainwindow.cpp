/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Implementation of the MainWindow class
 */

#include <QToolBar>
#include <QTreeView>
#include <QTableView>
#include <QHeaderView>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"
#include "ads_globals.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controltabs.h"
#include "logwidget.h"
#include "uiconstants.h"
#include "models/hierarchy/projecthierarchymodel.h"
#include "models/properties/dataobjectspropertiesmodel.h"
#include "managers/managersfactory.h"
#include "render/view3d.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::CDockAreaWidget;

using namespace QRS::App;
using namespace QRS::Core;
using namespace QRS::Managers;
using namespace QRS::Graph;
using namespace QRS::HierarchyModels;
using namespace QRS::PropertiesModels;

LogWidget* MainWindow::pLogger = nullptr;
const static QString skDefaultProjectName = "Default";
static QString const& skProjectExtension = Project::getFileExtension();
const static QString skSettingsFileName = "Settings.ini";
const static QString skMainWindow = "MainWindow";
const static QString skRecentProjects = "RecentProjects";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , mpUi(new Ui::MainWindow)
{
    initializeWindow();
    createContent();
    specifyMenuConnections();
    specifyProjectConnections();
    restoreSettings();
}

MainWindow::~MainWindow()
{
    delete mpManagersFactory;
    delete mpProject;
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
    mpProject = new Project(skDefaultProjectName);
    mpSettings = QSharedPointer<QSettings>(new QSettings(skSettingsFileName, QSettings::IniFormat));
    setProjectTitle();
    mpManagersFactory = new ManagersFactory(*mpProject, mLastPath, *mpSettings, this);
    // Configuration
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    QVBoxLayout* pLayout = new QVBoxLayout(mpUi->centralWidget);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->setSpacing(1);
    mpUi->centralWidget->setLayout(pLayout);
    // Tabbed toolbar
    QTabWidget* pTabControlWidget = new QTabWidget();
    pTabControlWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    pTabControlWidget->setMaximumHeight(92);
    // Managers
    ManagersTab* pManagersTab = new ManagersTab();
    pTabControlWidget->addTab(pManagersTab, tr("Managers"));
    connect(pManagersTab, &ManagersTab::actionDataObjectsTriggered, this, &MainWindow::createDataObjectsManager);
    connect(pManagersTab, &ManagersTab::actionRodPropertiesTriggered, this, &MainWindow::createRodComponentsManager);
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
    mpDockManager->setStyleSheet("");
    pLayout->addWidget(mpDockManager);;
    ads::CDockWidget* pDockWidget = nullptr;
    // OpenGL window
    pDockWidget = createGLWidget();
    mpDockManager->addDockWidget(ads::TopDockWidgetArea, pDockWidget);
    // Code Widget
    pDockWidget = createCodeWidget();
    mpDockManager->addDockWidgetTab(ads::TopDockWidgetArea, pDockWidget);
    // Log window
    pDockWidget = createLogWidget();
    mpDockManager->addDockWidget(ads::BottomDockWidgetArea, pDockWidget);
    // Project hierarchy
    pDockWidget = createProjectHierarchyWidget();
    CDockAreaWidget* pArea = mpDockManager->addDockWidget(ads::LeftDockWidgetArea, pDockWidget);
    // Status bar
    mpStatusLabel = new QLabel();
    mpUi->statusBar->addWidget(mpStatusLabel);
    // Properties
    pDockWidget = createPropertiesWidget();
    mpDockManager->addDockWidget(ads::BottomDockWidgetArea, pDockWidget, pArea);
    qInfo() << tr("Application successfully started");
}

//! Create a widget to represent a project hierarchy
CDockWidget* MainWindow::createProjectHierarchyWidget()
{
    QSize const kIconSize = QSize(16, 16);
    QTreeView* pWidget = new QTreeView();
    // Specify properties
    pWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    pWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    pWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pWidget->setHeaderHidden(true);
    pWidget->setAcceptDrops(true);
    pWidget->setDragEnabled(true);
    pWidget->setSortingEnabled(false);
    pWidget->setStyleSheet("padding: 3px 0px 0px 0px");
    pWidget->setIconSize(kIconSize);
    // Set the hierarchy model
    mpProjectHierarchyModel = new ProjectHierarchyModel("central/projectHierarchy", pWidget);
    mpProjectHierarchyModel->setProject(mpProject);
    pWidget->setModel(mpProjectHierarchyModel);
    connect(pWidget->selectionModel(), &QItemSelectionModel::selectionChanged,
            mpProjectHierarchyModel, &ProjectHierarchyModel::validateItemSelection);
    connect(mpProjectHierarchyModel, &ProjectHierarchyModel::selectionValidated,
            this, &MainWindow::representHierarchyProperties);
    // Create a dock widget
    CDockWidget* pDockWidget = new CDockWidget(tr("Project Hierarchy"));
    pDockWidget->setStyleSheet("background-color: white");
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

//! Create a widget enables to code
CDockWidget* MainWindow::createCodeWidget()
{
    QSize const kToolBarIconSize(18, 18);
    CDockWidget* pDockWidget = new CDockWidget(tr("Code"));
    QTextEdit* pWidget = new QTextEdit();
    pDockWidget->setWidget(pWidget);
    // ToolBar
    QToolBar* pToolBar = pDockWidget->createDefaultToolBar();
    pToolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    pDockWidget->setToolBarIconSize(kToolBarIconSize, CDockWidget::StateDocked);
    pToolBar->addAction(QIcon(":/icons/debug-start.svg"), tr("Start"));
    pToolBar->addAction(QIcon(":/icons/debug-stop.svg"), tr("Stop"));
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
    mpPropertiesWidget = new QTableView();
    mpPropertiesWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    mpPropertiesWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    mpPropertiesWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    mpPropertiesWidget->setSortingEnabled(false);
    mpPropertiesWidget->setSizeAdjustPolicy(QAbstractItemView::AdjustToContents);
    // Specify headers
    QHeaderView* pHeader = mpPropertiesWidget->horizontalHeader();
    pHeader->setStretchLastSection(true);
    pHeader->setHidden(true);
    pHeader->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    mpPropertiesWidget->verticalHeader()->setHidden(true);
    // Create a dock widget
    CDockWidget* pDockWidget = new CDockWidget(tr("Properties"));
    pDockWidget->setWidget(mpPropertiesWidget);
    pDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromContent);
    mpUi->menuWindow->addAction(pDockWidget->toggleViewAction());
    return pDockWidget;
}

//! Set signals and slots for menu actions
void MainWindow::specifyMenuConnections()
{
    // Project
    connect(mpUi->actionNewProject, &QAction::triggered, this, &MainWindow::createProject);
    connect(mpUi->actionOpenProject, &QAction::triggered, this, &MainWindow::openProjectDialog);
    connect(mpUi->actionSaveProject, &QAction::triggered, this, &MainWindow::saveProject);
    connect(mpUi->actionSaveAsProject, &QAction::triggered, this, &MainWindow::saveAsProject);
    connect(mpUi->actionExit, &QAction::triggered, this, &QMainWindow::close);
    // Help
    connect(mpUi->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(mpUi->actionAboutProgram, &QAction::triggered, this, &MainWindow::aboutProgram);
}

//! Set signals and slots for a project
void MainWindow::specifyProjectConnections()
{
    // Update models
    connect(mpProject, &Project::propertiesDataObjectsChanged, mpProjectHierarchyModel, &ProjectHierarchyModel::updateContent);
    connect(mpProject, &Project::propertiesRodComponentsChanged, mpProjectHierarchyModel, &ProjectHierarchyModel::updateContent);
    connect(mpProject, &Project::dataObjectsSubstituted, mpProjectHierarchyModel, &ProjectHierarchyModel::updateContent);
    connect(mpProject, &Project::rodComponentsSubstituted, mpProjectHierarchyModel, &ProjectHierarchyModel::updateContent);
    // Update the project through models
    connect(mpProjectHierarchyModel, &ProjectHierarchyModel::hierarchyChanged, mpProject, &Project::projectHierarchyChanged);
    // Set the modified state when the project has been changed
    std::function<void()> funProjectChanged = [this]() { setModified(true); };
    connect(mpProject, &Project::projectHierarchyChanged, funProjectChanged);
    connect(mpProject, &Project::propertiesDataObjectsChanged, funProjectChanged);
    connect(mpProject, &Project::propertiesRodComponentsChanged, funProjectChanged);
    connect(mpProject, &Project::dataObjectsSubstituted, funProjectChanged);
    connect(mpProject, &Project::rodComponentsSubstituted, funProjectChanged);
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
    if (!mpManagersFactory->createManager(AbstractManager::ManagerType::kDataObjects))
        qWarning() << tr("Failed to open a manager to create data objects");
}

//! Show a manager to set rod components based on the created data objects
void MainWindow::createRodComponentsManager()
{
    if (!mpManagersFactory->createManager(AbstractManager::ManagerType::kRodComponents))
        qWarning() << tr("Failed to open a manager to produce rod components");
}

//! Show a manager to assemble a rod by using rod components
void MainWindow::createRodConstructorManager()
{
    if (!mpManagersFactory->createManager(AbstractManager::ManagerType::kRodConstructor))
        qWarning() << tr("Failed to open a manager to construct rods");
}

//! Create a project and substitute the current one with it
void MainWindow::createProject()
{
    if (!saveProjectChangesDialog())
        return;
    delete mpProject;
    mpProject = new Project(skDefaultProjectName);
    specifyProjectConnections();
    setModified(false);
    delete mpManagersFactory;
    mpManagersFactory = new ManagersFactory(*mpProject, mLastPath, *mpSettings, this);
}

//! Open a project by using a dialog
void MainWindow::openProjectDialog()
{
    if (!saveProjectChangesDialog())
        return;
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Project"), mLastPath,
                                                    tr("Project file format (*%1)").arg(skProjectExtension));
    openProject(filePath);
}

//! Open the specific project
void MainWindow::openProject(QString const& filePath)
{
    // Read a project from a file
    if (filePath.isEmpty())
        return;
    QFileInfo info(filePath);
    if (!info.exists() || !info.isFile())
        return;
    QString path = info.path();
    QString baseName = info.baseName();
    delete mpProject;
    // Open a project and specify connections
    mpProject = new Project(path, baseName);
    mpProjectHierarchyModel->setProject(mpProject);
    specifyProjectConnections();
    // Modify GUI
    mLastPath = path;
    setModified(false);
    addToRecentProjects();
    // Update managers
    delete mpManagersFactory;
    mpManagersFactory = new ManagersFactory(*mpProject, mLastPath, *mpSettings, this);
}

//! Open the project which was selected from the Recent Projects menu
void MainWindow::openRecentProject()
{
    if (!saveProjectChangesDialog())
        return;
    QAction* pAction = (QAction*)sender();
    if (pAction)
        openProject(pAction->text());
}

//! Whenever a project has been modified
void MainWindow::setModified(bool flag)
{
    setWindowModified(flag);
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
    setModified(!isSaved);
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
    setModified(!isSaved);
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

//! Show information about the selected project items
void MainWindow::representHierarchyProperties(QVector<AbstractHierarchyItem*> items)
{
    delete mpPropertiesWidget->model();
    mpPropertiesWidget->setModel(nullptr);
    if (items.isEmpty())
        return;
    AbstractHierarchyItem::ItemType itemType = (AbstractHierarchyItem::ItemType)items[0]->type();
    switch (itemType)
    {
    case AbstractHierarchyItem::ItemType::kDataObjects:
    {
        DataObjectsPropertiesModel* pModel = new DataObjectsPropertiesModel(mpPropertiesWidget, items);
        connect(pModel, &DataObjectsPropertiesModel::propertyChanged, mpProject, &Project::propertiesDataObjectsChanged);
        mpPropertiesWidget->setModel(pModel);
        break;
    }
    case AbstractHierarchyItem::ItemType::kRodComponents:
        // TODO
        break;
    }
}

//! Save project and settings before exit
void MainWindow::closeEvent(QCloseEvent* pEvent)
{
    if (saveProjectChangesDialog())
    {
        saveSettings();
        pEvent->accept();
    }
    else
    {
        pEvent->ignore();
    }
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
                                 "Copyright &copy; 2021 QRodSystems (Pavel Lakiza)\n"
                                 "Copyright &copy; 2021 KLPALGSYS (Dmitriy Krasnorutskiy)"
                             );
    QMessageBox::about(this, tr("About QRodSystems v%1").arg(APP_VERSION), aboutMsg);
}
