/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of ControlTabs class
 */

#include <QLayout>
#include <QToolBar>
#include <QIcon>
#include "controltabs.h"

//! Managers tab constructor
ManagersTab::ManagersTab(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* pLayout = new QHBoxLayout();
    pLayout->setContentsMargins(3, 0, 3, 0);
    QToolBar* pToolBar = new QToolBar();
    pToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // Data Objects
    pToolBar->addAction(QIcon(":/icons/Edit-Table.svg"), tr("Data Objects"));
    pToolBar->addSeparator();
    // Rod Properties
    pToolBar->addAction(QIcon(":/icons/Packet.svg"), tr("Rod Properties"));
    // Rod Contructor
    pToolBar->addAction(QIcon(":/icons/Std-Placement.svg"), tr("Rod Constructor"));
    pToolBar->addSeparator();
    // Adding the layout to the tab
    pLayout->setMenuBar(pToolBar);
    setLayout(pLayout);
}
