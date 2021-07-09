/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the Project class
 */

#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include "aliasdataset.h"
#include "array.h"
#include "hierarchytree.h"
#include "abstractdataobject.h"
#include "abstractrodcomponent.h"
#include "abstractsectionrodcomponent.h"

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace QRS::HierarchyModels
{
class ProjectHierarchyModel;
}

namespace QRS::Managers
{
class ManagersFactory;
}

namespace QRS::Core
{

//! Project class to interact with a created system of rods
class Project : public QObject
{
    Q_OBJECT

    friend class QRS::HierarchyModels::ProjectHierarchyModel;
    friend class QRS::Managers::ManagersFactory;

public:
    Project(QString const& name);
    Project(QString const& path, QString const& fileName);
    virtual ~Project();
    // Data objects
    DataIDType numberDataObjects() const { return mDataObjects.size(); }
    AbstractDataObject* addDataObject(AbstractDataObject::ObjectType type);
    DataObjects cloneDataObjects() const;
    HierarchyTree cloneHierarchyDataObjects() const { return mHierarchyDataObjects.clone(); }
    // Rod components
    DataIDType numberRodComponents() const { return mRodComponents.size(); }
    AbstractRodComponent* addGeometry();
    AbstractRodComponent* addCrossSection(AbstractSectionRodComponent::SectionType sectionType);
    AbstractRodComponent* addMaterial();
    AbstractRodComponent* addLoad();
    AbstractRodComponent* addConstraint();
    AbstractRodComponent* addMechanical();
    RodComponents cloneRodComponents() const;
    HierarchyTree cloneHierarchyRodComponents() const { return mHierarchyRodComponents.clone(); }
    // Getters and setters
    QString const& name() const { return mName; }
    QString const& filePath() const { return mFilePath; }
    static QString const& getFileExtension() { return skProjectExtension; }
    void importDataObjects(QString const& path, QString const& fileName);

signals:
    // Data objects
    void dataObjectsSubstituted();
    void propertiesDataObjectsChanged();
    // Rod components
    void rodComponentsSubstituted();
    void propertiesRodComponentsChanged();
    // Project hierarchy
    void projectHierarchyChanged();

public slots:
    bool save(QString const& dir, QString const& fileName);
    void setDataObjects(QRS::Core::DataObjects const& dataObjects, QRS::Core::HierarchyTree const& hierarchyDataObjects);
    void setRodComponents(QRS::Core::RodComponents const& rodComponents, QRS::Core::HierarchyTree const& hierarchyRodComponents);

private:
    void emplaceRodComponent(AbstractRodComponent* pRodComponent);

private:
    //! Unique project identifier
    quint32 mID;
    //! Project name
    QString mName;
    //! Path to a file where a project is stored
    QString mFilePath;
    //! Data objects
    DataObjects mDataObjects;
    //! Hierarchy of data objects
    HierarchyTree mHierarchyDataObjects;
    //! Rod components
    RodComponents mRodComponents;
    //! Hierarchy of rod components
    HierarchyTree mHierarchyRodComponents;
    //! File extensionn
    static const QString skProjectExtension;
};

}

#endif // PROJECT_H
