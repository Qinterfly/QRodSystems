/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the AbstractManager class
 */

#include <QMessageBox>
#include <QSettings>
#include <QToolBar>

#include "abstractmanager.h"
#include "central/uiconstants.h"
#include "DockManager.h"

using namespace QRS::Managers;
using ads::CDockManager;

AbstractManager::AbstractManager(QString& lastPath, QSettings& settings,
                                 ManagerType type, QString groupName, QWidget* parent)
    : QDialog(parent)
    , mLastPath(lastPath)
    , mSettings(settings)
    , mkType(type)
    , mkGroupName(groupName)
{
    mpDockManager = new CDockManager(this);
    mpDockManager->setStyleSheet("");
}

AbstractManager::~AbstractManager()
{
    if (mpDockManager->parent() == nullptr)
    {
        delete mpDockManager;
        mpDockManager = nullptr;
    }
}

//! Save settings to a file
void AbstractManager::saveSettings()
{
    mSettings.beginGroup(mkGroupName);
    mSettings.setValue(UiConstants::Settings::skGeometry, saveGeometry());
    mSettings.setValue(UiConstants::Settings::skDockingState, mpDockManager->saveState());
    mSettings.endGroup();
}

//! Restore settings from a file
void AbstractManager::restoreSettings()
{
    mSettings.beginGroup(mkGroupName);
    restoreGeometry(mSettings.value(UiConstants::Settings::skGeometry).toByteArray());
    mpDockManager->restoreState(mSettings.value(UiConstants::Settings::skDockingState).toByteArray());
    mSettings.endGroup();
}

//! Save settings and delete handling widgets before closing the window
void AbstractManager::closeEvent(QCloseEvent* pEvent)
{
    pEvent->ignore();
    bool isClosed = false;
    if (isWindowModified())
    {
        auto dialogResult = QMessageBox::question(this
                                                  , tr("Close confirmation")
                                                  , tr("Manager containes unsaved changes. Would you like to save them?")
                                                  , QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (dialogResult)
        {
        case QMessageBox::Save:
            isClosed = true;
            apply();
            break;
        case QMessageBox::Discard:
            isClosed = true;
            break;
        default:
            break;
        }
    }
    else
    {
        isClosed = true;
    }
    if (isClosed)
    {
        saveSettings();
        emit closed(mkType);
        pEvent->accept();
    }
}

//! Helper function to add a shortcut hint to all actions which a toolbar contains
void AbstractManager::setToolBarShortcutHints(QToolBar* pToolBar)
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
