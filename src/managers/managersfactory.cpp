/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Definition of the ManagersFactory class
 */

#include <QApplication>
#include <QDesktopWidget>
#include "managersfactory.h"
#include "core/project.h"
#include "managers/dataobjectsmanager.h"
#include "managers/rodcomponentsmanager.h"

using namespace QRS::Core;
using namespace QRS::Managers;

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
bool ManagersFactory::createManager(AbstractManager::ManagerType type)
{
    AbstractManager* pManager = nullptr;
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
    case AbstractManager::kDataObjects:
    {
        DataObjectsManager* pDataObjectManager = new DataObjectsManager(mProject.cloneDataObjects(), mProject.cloneHierarchyDataObjects(),
                                                                        mLastPath, mSettings, mpParent);
        connect(pDataObjectManager, &DataObjectsManager::applied, &mProject, &Project::setDataObjects);
        pManager = pDataObjectManager;
        break;
    }
    case AbstractManager::kRodComponents:
    {
        RodComponentsManager* pRodComponentsManager = new RodComponentsManager(mProject.mDataObjects, mProject.mHierarchyDataObjects,
                                                                               mProject.cloneRodComponents(), mProject.cloneHierarchyRodComponents(),
                                                                               mLastPath, mSettings, mpParent);
        connect(pRodComponentsManager, &RodComponentsManager::applied, &mProject, &Project::setRodComponents);
        connect(&mProject, &Project::dataObjectsSubstituted, pRodComponentsManager, &RodComponentsManager::updateDataObjects);
        connect(&mProject, &Project::projectHierarchyChanged, pRodComponentsManager, &RodComponentsManager::updateDataObjects);
        pManager = pRodComponentsManager;
        break;
    }
    default:
        return false;
    }
    if (!pManager)
        return false;
    mManagers.emplace(type, pManager);
    pManager->show();
    connect(pManager, &AbstractManager::closed, this, &ManagersFactory::deleteManager);
    return true;
}

//! Destroy a manager by given type
bool ManagersFactory::deleteManager(AbstractManager::ManagerType type)
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

//! Retrieve a manager of a given type
AbstractManager* ManagersFactory::manager(AbstractManager::ManagerType type)
{
    if (mManagers.contains(type))
        return mManagers[type];
    else
        return nullptr;
}
