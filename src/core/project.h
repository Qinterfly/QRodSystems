/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of QRS::Project class
 */

#ifndef PROJECT_H
#define PROJECT_H

#include <QUuid>
#include <QSharedPointer>
#include <list>
#include "dataobjects.h"

class QString;

namespace QRS
{
using ListDataObjects = std::list<QSharedPointer<AbstractDataObject>>;

//! Project class to interact with a created system of rods
class Project
{
public:
    Project(QString name);
    ~Project() = default;

private:
    //! Unique project identifier
    const QUuid mID;
    //! Project name
    QString mName;
    //! Path to a file where a project is stored
    QString mFilePath;
    //! Number of modifications since last saving
    uint mNumModified = 0;
    //! Data objects
    ListDataObjects mDataObjects;
};

}

#endif // PROJECT_H
