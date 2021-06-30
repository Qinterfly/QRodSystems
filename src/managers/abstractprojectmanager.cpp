/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the AbstractProjectManager class
 */

#include <QMessageBox>
#include <QSettings>
#include <QToolBar>

#include "abstractprojectmanager.h"
#include "central/uiconstants.h"
#include "core/project.h"
#include "DockManager.h"

using namespace QRS::Managers;
using namespace QRS::Core;
using ads::CDockManager;

AbstractProjectManager::AbstractProjectManager(QString& lastPath, QSettings& settings,
                                               ManagerType type, QString groupName, QWidget* parent)
    : QDialog(parent)
    , mLastPath(lastPath)
    , mSettings(settings)
    , mType(type)
    , mGroupName(groupName)
{
    mpDockManager = new CDockManager();
    mpDockManager->setStyleSheet("");
}

AbstractProjectManager::~AbstractProjectManager()
{
    if (mpDockManager->parent() == nullptr)
    {
        delete mpDockManager;
        mpDockManager = nullptr;
    }
}

//! Save settings to a file
void AbstractProjectManager::saveSettings()
{
    mSettings.beginGroup(mGroupName);
    mSettings.setValue(UiConstants::Settings::skGeometry, saveGeometry());
    mSettings.setValue(UiConstants::Settings::skDockingState, mpDockManager->saveState());
    mSettings.endGroup();
}

//! Restore settings from a file
void AbstractProjectManager::restoreSettings()
{
    mSettings.beginGroup(mGroupName);
    restoreGeometry(mSettings.value(UiConstants::Settings::skGeometry).toByteArray());
    mpDockManager->restoreState(mSettings.value(UiConstants::Settings::skDockingState).toByteArray());
    mSettings.endGroup();
}

//! Save settings and delete handling widgets before closing the window
void AbstractProjectManager::closeEvent(QCloseEvent* pEvent)
{
    pEvent->ignore();
    bool isClosed = false;
    if (isWindowModified())
    {
        auto dialogResult = QMessageBox::question(this
                                                  , tr("Close confirmation")
                                                  , tr("Manager containes unsaved changes. Would you like to close it anyway?")
                                                  , QMessageBox::Yes | QMessageBox::No);
        isClosed = QMessageBox::Yes == dialogResult;
    }
    else
    {
        isClosed = true;
    }
    if (isClosed)
    {
        saveSettings();
        emit closed(mType);
        pEvent->accept();
    }
}

//! Helper function to add a shortcut hint to all actions which a toolbar contains
void AbstractProjectManager::setToolBarShortcutHints(QToolBar* pToolBar)
{
    QList<QAction*> listActions = pToolBar->actions();
    QString shortCut;
    for (auto& action : listActions)
    {
        shortCut = action->shortcut().toString();
        if (shortCut.isEmpty())
            action->setText(QString(action->text()));
        else
            action->setText(QString(action->text() + " (%1)").arg(shortCut));
    }
}
