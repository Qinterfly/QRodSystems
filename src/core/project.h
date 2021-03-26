/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the QRS::Project class
 */

#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QUuid>
#include <unordered_map>
#include <memory>
#include "abstractdataobject.h"
#include "array.h"

class QString;

namespace QRS
{

using DataObjects = std::unordered_map<DataIDType, std::shared_ptr<AbstractDataObject>>;

//! Project class to interact with a created system of rods
class Project : public QObject
{
    Q_OBJECT

public:
    Project(QString name);
    virtual ~Project() = default;
    // Data objects
    std::shared_ptr<AbstractDataObject> getDataObject(DataIDType id);
    std::unordered_map<DataIDType, AbstractDataObject*> getDataObjects();

signals:
    void dataObjectAdded();

public slots:
    void addDataObject(DataObjectType type);

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
    DataObjects mDataObjects;
};

}

#endif // PROJECT_H
