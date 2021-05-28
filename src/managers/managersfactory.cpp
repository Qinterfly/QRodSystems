/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the ManagersFactory class
 */

#include <QApplication>
#include <QDesktopWidget>

#include "managersfactory.h"
#include "managers/dataobjectsmanager.h"
#include "managers/rodcomponentsmanager.h"

using namespace QRS::Managers;
using namespace QRS::Core;

void moveToCenter(QWidget*);

ManagersFactory::ManagersFactory(Project& project, QString& lastPath, QSettings& settings, QWidget* parent)
    : mProject(project)
    , mLastPath(lastPath)
    , mSettings(settings)
    , mpParent(parent)
{

}

ManagersFactory::~ManagersFactory()
{
    for (auto iter = mManagers.begin(); iter != mManagers.end(); ++iter)
        delete iter->second;
    mManagers.clear();
}

//! Create a manager according to a given type
bool ManagersFactory::createManager(AbstractProjectManager::ManagerType type)
{
    AbstractProjectManager* pManager = nullptr;
    bool isExisted = mManagers.contains(type);
    if (isExisted)
    {
        if (mManagers[type]->isVisible())
            return false;
        delete mManagers[type];
        mManagers.erase(type);
    }
    switch (type)
    {
    case AbstractProjectManager::kDataObjects:
        pManager = new DataObjectsManager(mProject, mLastPath, mSettings, mpParent);
        break;
    case AbstractProjectManager::kRodComponents:
        pManager = new RodComponentsManager(mProject, mLastPath, mSettings, mpParent);
        break;
    default:
        return false;
    }
    if (!pManager)
        return false;
    mManagers.emplace(type, pManager);
    pManager->show();
    connect(pManager, &AbstractProjectManager::closed, this, &ManagersFactory::deleteManager);
    return true;
}

//! Destroy a manager by given type
bool ManagersFactory::deleteManager(AbstractProjectManager::ManagerType type)
{
    if (!mManagers.contains(type))
        return false;
    delete mManagers[type];
    mManagers.erase(type);
    return true;
}

//! Helper function to situate widgets at the center of their parent widgets
void moveToCenter(QWidget* pWidget)
{
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(pWidget->parentWidget());
    int x = (screenGeometry.width() - pWidget->width()) / 2;
    int y = (screenGeometry.height() - pWidget->height()) / 2;
    pWidget->move(x, y);
}
