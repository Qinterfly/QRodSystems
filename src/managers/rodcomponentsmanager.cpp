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

RodComponentsManager::RodComponentsManager(Project& project, QString& lastPath, QSettings& settings, QWidget* parent)
    : AbstractProjectManager(project, lastPath, settings, kRodComponents, "RodComponentsManager", parent)
{
    setWindowTitle("Rod Components Manager[*]");
    setGeometry(0, 0, 700, 700);
    setWindowModified(false);
    restoreSettings();
}

RodComponentsManager::~RodComponentsManager()
{

}
