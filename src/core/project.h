/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the QRS::Project class
 */

#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
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
    Project(QString const& name);
    Project(QString const& path, QString const& fileName);
    virtual ~Project() = default;
    // State
    bool isModified() const { return mNumModified == 0; }
    // Data objects
    std::shared_ptr<AbstractDataObject> getDataObject(DataIDType id);
    std::unordered_map<DataIDType, AbstractDataObject*> getDataObjects();
    void addDataObject(DataObjectType type);
    void removeDataObject(DataIDType id);
    void setDataObjects(std::unordered_map<DataIDType, AbstractDataObject*> dataObjects);
    // Getters and setters
    QString const& name() const { return mName; }
    QString const& filePath() const { return mFilePath; }
    static QString const& getFileExtension() { return skProjectExtension; }

private:
    void specifyConnections();

signals:
    void dataObjectAdded(QRS::DataIDType id);
    void dataObjectRemoved(QRS::DataIDType id);
    void allDataObjectsChanged();

public slots:
    bool save(QString const& dir, QString const& fileName);

private slots:
    void setModified();

private:
    //! Unique project identifier
    quint32 mID;
    //! Project name
    QString mName;
    //! Path to a file where a project is stored
    QString mFilePath;
    //! Number of modifications since last saving
    uint mNumModified = 0;
    //! Data objects
    DataObjects mDataObjects;
    //! File extensionn
    static const QString skProjectExtension;
};

}

#endif // PROJECT_H
