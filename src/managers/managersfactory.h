/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the ManagersFactory class
 */

#ifndef MANAGERSFACTORY_H
#define MANAGERSFACTORY_H

#include <QObject>
#include "abstractprojectmanager.h"

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

//! Fabric to create managers which utilize and modify project data
class ManagersFactory : public QObject
{
    Q_OBJECT

public:
    ManagersFactory(Core::Project& project, QString& lastPath, QSettings& settings, QWidget* parent);
    ~ManagersFactory();
    bool createManager(AbstractProjectManager::ManagerType type);
    bool deleteManager(AbstractProjectManager::ManagerType type);

private:
    Core::Project& mProject;
    QString& mLastPath;
    QSettings& mSettings;
    QWidget* mpParent;
    std::unordered_map<AbstractProjectManager::ManagerType, AbstractProjectManager*> mManagers;
};

}

}

#endif // MANAGERSFACTORY_H
