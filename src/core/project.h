/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the Project class
 */

#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <unordered_map>
#include <memory>
#include "abstractdataobject.h"
#include "array.h"
#include "hierarchytree.h"

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace QRS::HierarchyModels
{
class ProjectHierarchyModel;
}

namespace QRS::Core
{

using DataObjects = std::unordered_map<DataIDType, std::shared_ptr<AbstractDataObject>>;

//! Project class to interact with a created system of rods
class Project : public QObject
{
    Q_OBJECT
    friend class QRS::HierarchyModels::ProjectHierarchyModel;

public:
    Project(QString const& name);
    Project(QString const& path, QString const& fileName);
    virtual ~Project() = default;
    // State
    bool isModified() const { return mIsModified; }
    // Data objects
    DataIDType numberDataObjects() const { return mDataObjects.size(); }
    std::unordered_map<DataIDType, AbstractDataObject*> cloneDataObjects() const;
    DataIDType addDataObject(DataObjectType type);
    void removeDataObject(DataIDType id);
    void setDataObjects(std::unordered_map<DataIDType, AbstractDataObject*> dataObjects, HierarchyTree const& hierarchyDataObjects);
    // Hierarchy of data objects
    HierarchyTree cloneHierarchyDataObjects() const;
    // Getters and setters
    QString const& name() const { return mName; }
    QString const& filePath() const { return mFilePath; }
    static QString const& getFileExtension() { return skProjectExtension; }
    void importDataObjects(QString const& path, QString const& fileName);

signals:
    void dataObjectAdded(QRS::Core::DataIDType id);
    void dataObjectRemoved(QRS::Core::DataIDType id);
    void allDataObjectsChanged();
    void modified(bool modifiedState);

public slots:
    bool save(QString const& dir, QString const& fileName);

private slots:
    void setModified(bool modifiedState = true);

private:
    //! Unique project identifier
    quint32 mID;
    //! Project name
    QString mName;
    //! Path to a file where a project is stored
    QString mFilePath;
    //! Flag whether a project has been modified since last saving
    bool mIsModified;
    //! Data objects
    DataObjects mDataObjects;
    //! Hierarchy of data objects
    HierarchyTree mHierarchyDataObjects;
    //! File extensionn
    static const QString skProjectExtension;
};

}

#endif // PROJECT_H
