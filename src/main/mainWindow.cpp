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
}

MainWindow::~MainWindow()
{
     delete mpUi;
}

//! Create all the widgets inside
void MainWindow::createContent()
{
    // Toolbars
    createManagersToolBar();
}

//! Create the toolbar consisted of the object designers
void MainWindow::createManagersToolBar()
{
    QHBoxLayout *pLayout = new QHBoxLayout();
    pLayout->setContentsMargins(3, 0, 3, 0);
    QToolBar *pToolBar = new QToolBar();
    pToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QAction *pAction = pToolBar->addAction("Object Creator");
    pAction->setIcon(QIcon(":/icons/Edit-Table.svg"));
    pLayout->setMenuBar(pToolBar);
    mpUi->tabManagers->setLayout(pLayout);
}


