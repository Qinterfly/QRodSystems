/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the RodComponentsManager class
 */

#include "rodcomponentsmanager.h"
#include "core/project.h"

using namespace QRS::Managers;
using namespace QRS::Core;

RodComponentsManager::RodComponentsManager(Project& project, QSettings& settings, QString& lastPath, QWidget* parent)
    : AbstractProjectManager(project, settings, lastPath, "RodComponentsManager", parent)
{
    setWindowTitle("Rod Components Manager[*]");
    setGeometry(0, 0, 700, 700);
    setWindowModified(false);
    restoreSettings();
}

RodComponentsManager::~RodComponentsManager()
{

}
