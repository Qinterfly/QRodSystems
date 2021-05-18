/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the ControlTabs class
 */

#include <QLayout>
#include <QToolBar>
#include <QIcon>
#include "controltabs.h"

using namespace QRS::App;

//! Managers tab constructor
ManagersTab::ManagersTab(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* pLayout = new QHBoxLayout();
    pLayout->setContentsMargins(3, 0, 3, 0);
    QToolBar* pToolBar = new QToolBar();
    pToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // Data Objects
    pToolBar->addAction(QIcon(":/icons/edit-table.svg"), tr("Data Objects"), this, &ManagersTab::actionDataObjectsTriggered);
    pToolBar->addSeparator();
    // Rod Properties
    pToolBar->addAction(QIcon(":/icons/packet.svg"), tr("Rod Properties"), this, &ManagersTab::actionRodPropertiesTriggered);
    // Rod Contructor
    pToolBar->addAction(QIcon(":/icons/std-placement.svg"), tr("Rod Constructor"), this, &ManagersTab::actionRodConstructorTriggered);
    pToolBar->addSeparator();
    // Adding the layout to the tab
    pLayout->setMenuBar(pToolBar);
    setLayout(pLayout);
}
