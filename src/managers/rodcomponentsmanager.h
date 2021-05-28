/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the RodComponentsManager class
 */

#ifndef RODCOMPONENTSMANAGER_H
#define RODCOMPONENTSMANAGER_H

#include "managers/abstractprojectmanager.h"

namespace QRS::Managers
{

//! Manager to construct components of a rod
class RodComponentsManager : public AbstractProjectManager
{
    Q_OBJECT

public:
    RodComponentsManager(Core::Project& project, QString& lastPath, QSettings& settings, QWidget* parent = nullptr);
    ~RodComponentsManager();
};

}

#endif // RODCOMPONENTSMANAGER_H
