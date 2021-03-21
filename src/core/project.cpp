/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of QRS::Project class
 */

#include "project.h"

using namespace QRS;

//! Construct a clean project with the user specified name
Project::Project(QString name)
    : mID(QUuid::createUuid())
    , mName(name)
{

}

