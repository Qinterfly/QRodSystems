/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of MainWindow class
 */

#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "DockManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mUi(new Ui::MainWindow)
{
    mUi->setupUi(this);
}

MainWindow::~MainWindow()
{
     delete mUi;
}

//! Initialize the window geometry
void MainWindow::initialize()
{

}


