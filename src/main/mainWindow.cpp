/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of MainWindow class
 */

#include <QToolBar>
#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "DockManager.h"

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
    // Toolbars
    createManagersToolBar();
}

//! Create a toolbar consisted of object designers
void MainWindow::createManagersToolBar()
{
    QHBoxLayout *pLayout = new QHBoxLayout();
    pLayout->setContentsMargins(3, 0, 3, 0);
    QToolBar *pToolBar = new QToolBar();
    pToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // Object manager
    QAction *pAction = pToolBar->addAction("Object Manager");
    pAction->setIcon(QIcon(":/icons/Edit-Table.svg"));
    connect(pAction, &QAction::triggered, this, &MainWindow::createObjectManager);
    pToolBar->addSeparator();
    // Rod Data
    pAction = pToolBar->addAction("Rod Data");
    pAction->setIcon(QIcon(":/icons/Data-Binary.svg"));
    // Rod Contructor
    pAction = pToolBar->addAction("Rod Constructor");
    pAction->setIcon(QIcon(":/icons/Std-Placement.svg"));
    pToolBar->addSeparator();
    // Adding the layout to the tab
    pLayout->setMenuBar(pToolBar);
    mpUi->tabManagers->setLayout(pLayout);
}

//! Set signals and slots for menu actions
void MainWindow::specifyMenuConnections()
{
    // File
    connect(mpUi->actionExit, &QAction::triggered, this, &QMainWindow::close);
    // Help
    connect(mpUi->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
}

//! Show an object manager
void MainWindow::createObjectManager()
{
    // TODO
}
