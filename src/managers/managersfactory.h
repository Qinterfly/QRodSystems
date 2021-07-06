/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the ManagersFactory class
 */

#ifndef MANAGERSFACTORY_H
#define MANAGERSFACTORY_H

#include <QObject>
#include "abstractmanager.h"

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace QRS
{

namespace Core
{
class Project;
}

namespace Managers
{

class DataObjectsManager;
class RodComponentsManager;

//! Factory to create managers which utilize and modify project data
class ManagersFactory : public QObject
{
    Q_OBJECT

public:
    ManagersFactory(Core::Project& project, QString& lastPath, QSettings& settings, QWidget* parent);
    ~ManagersFactory();
    bool createManager(AbstractManager::ManagerType type);
    bool deleteManager(AbstractManager::ManagerType type);
    AbstractManager* manager(AbstractManager::ManagerType type);

private:
    void specifyConnections(DataObjectsManager* pManager);
    void specifyConnections(RodComponentsManager* pManager);

private:
    Core::Project& mProject;
    QString& mLastPath;
    QSettings& mSettings;
    QWidget* mpParent;
    std::unordered_map<AbstractManager::ManagerType, AbstractManager*> mManagers;
};

}

}

#endif // MANAGERSFACTORY_H
