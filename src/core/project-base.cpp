/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Implementation of the Project class
 *
 * Implementation of the methods to operate with data objects, components and rods
 */

#include <QRandomGenerator>

#include "project.h"
#include "scalardataobject.h"
#include "vectordataobject.h"
#include "matrixdataobject.h"
#include "surfacedataobject.h"
#include "geometryrodcomponent.h"
#include "usersectionrodcomponent.h"

using namespace QRS::Core;

AbstractDataObject* createDataObject(AbstractDataObject::ObjectType type);
AbstractDataObject const* substituteDataObject(DataObjects const& dataObjects, AbstractDataObject const* pObject);

//! Construct a clean project with the user specified name
Project::Project(QString const& name)
    : mID(QRandomGenerator::global()->generate())
    , mName(name)
    , mIsModified(false)
{

}

Project::~Project()
{
    clearDataMap(mDataObjects);
    clearDataMap(mRodComponents);
}

//! Create a data object with the specified type
AbstractDataObject* Project::addDataObject(AbstractDataObject::ObjectType type)
{
    AbstractDataObject* pObject = createDataObject(type);
    if (pObject)
    {
        DataIDType id = pObject->id();
        mDataObjects.emplace(id, pObject);
        mHierarchyDataObjects.appendNode(new HierarchyNode(HierarchyNode::NodeType::kObject, id));
        setModified(true);
    }
    return pObject;
}

//! Substitute current data objects with new ones
void Project::setDataObjects(DataObjects const& dataObjects, HierarchyTree const& hierarchyDataObjects)
{
    // Resolve references of rod components
    for (auto& item : mRodComponents)
    {
        AbstractRodComponent* pRodComponent = item.second;
        switch (pRodComponent->componentType())
        {
        case AbstractRodComponent::ComponentType::kGeometry:
        {
            GeometryRodComponent* pGeometry = (GeometryRodComponent*)pRodComponent;
            pGeometry->setRadiusVector((VectorDataObject const*)substituteDataObject(dataObjects, pGeometry->radiusVector()));
            pGeometry->setRotationMatrix((MatrixDataObject const*)substituteDataObject(dataObjects, pGeometry->rotationMatrix()));
            break;
        }
        case AbstractRodComponent::ComponentType::kSection:
            break;
        }
    }
    // Substitute data objects
    clearDataMap(mDataObjects);
    AbstractDataObject* pDataObject;
    for (auto& item : dataObjects)
    {
        pDataObject = item.second;
        mDataObjects.emplace(pDataObject->id(), pDataObject->clone());
    }
    mHierarchyDataObjects = hierarchyDataObjects;
    emit dataObjectsSubstituted();
    setModified(true);
}

//! Clone data objects
DataObjects Project::cloneDataObjects() const
{
    DataObjects result;
    for (auto& pItem : mDataObjects)
    {
        AbstractDataObject* pObject = pItem.second->clone();
        result.emplace(pObject->id(), pObject);
    }
    return result;
}

//! Create a geometrical rod component
AbstractRodComponent* Project::addGeometry()
{
    QString name = "Geometry " + QString::number(GeometryRodComponent::numberInstances() + 1);
    AbstractRodComponent* pRodComponent = new GeometryRodComponent(name);
    emplaceRodComponent(pRodComponent);
    return pRodComponent;
}

//! Create a cross section
AbstractRodComponent* Project::addCrossSection(AbstractSectionRodComponent::SectionType sectionType)
{
    QString name = "Cross section " + QString::number(AbstractSectionRodComponent::numberInstances() + 1);
    AbstractRodComponent* pRodComponent = nullptr;
    switch (sectionType)
    {
    case AbstractSectionRodComponent::SectionType::kUserDefined:
        pRodComponent = new UserSectionRodComponent(name);
        break;
    }
    emplaceRodComponent(pRodComponent);
    return pRodComponent;
}

//! Substitute current rod components with new ones
void Project::setRodComponents(RodComponents const& rodComponents, HierarchyTree const& hierarchyRodComponents)
{
    clearDataMap(mRodComponents);
    AbstractRodComponent* pRodComponent;
    for (auto& item : rodComponents)
    {
        pRodComponent = item.second;
        mRodComponents.emplace(pRodComponent->id(), pRodComponent->clone());
    }
    mHierarchyRodComponents = hierarchyRodComponents;
    emit rodComponentsSubstituted();
    setModified(true);
}

//! Clone rod components
RodComponents Project::cloneRodComponents() const
{
    RodComponents result;
    for (auto& pItem : mRodComponents)
    {
        AbstractRodComponent* pRodComponent = pItem.second->clone();
        result.emplace(pRodComponent->id(), pRodComponent);
    }
    return result;
}

//! Set a modification state
void Project::setModified(bool modifiedState)
{
    mIsModified = modifiedState;
    emit modified(mIsModified);
}

//! Helper function to clear a map consisted of data pointers
template<typename T>
void Project::clearDataMap(std::unordered_map<DataIDType, T*>& dataMap)
{
    for (auto iter = dataMap.begin(); iter != dataMap.end(); ++iter)
        delete iter->second;
    dataMap.clear();
}

//! Helper function to create DataObject instance by a type and name
AbstractDataObject* createDataObject(AbstractDataObject::ObjectType type)
{
    AbstractDataObject* pObject = nullptr;
    QString name;
    switch (type)
    {
    case AbstractDataObject::ObjectType::kScalar:
        name = "Scalar " + QString::number(ScalarDataObject::numberInstances() + 1);
        pObject = new ScalarDataObject(name);
        break;
    case AbstractDataObject::ObjectType::kVector:
        name = "Vector " + QString::number(VectorDataObject::numberInstances() + 1);
        pObject = new VectorDataObject(name);
        break;
    case AbstractDataObject::ObjectType::kMatrix:
        name = "Matrix " + QString::number(MatrixDataObject::numberInstances() + 1);
        pObject = new MatrixDataObject(name);
        break;
    case AbstractDataObject::ObjectType::kSurface:
        name = "Surface " + QString::number(SurfaceDataObject::numberInstances() + 1);
        pObject = new SurfaceDataObject(name);
        break;
    }
    return pObject;
}

//! Substitute a data object with its updated version
AbstractDataObject const* substituteDataObject(DataObjects const& dataObjects, AbstractDataObject const* pDataObject)
{
    if (!pDataObject)
        return nullptr;
    DataIDType id = pDataObject->id();
    if (dataObjects.contains(id))
    {
        AbstractDataObject const* pFoundDataObject = dataObjects.at(id);
        if (pFoundDataObject->type() == pDataObject->type())
            return pFoundDataObject;
    }
    return nullptr;
};

//! Emplace a rod component into a project
void Project::emplaceRodComponent(AbstractRodComponent* pRodComponent)
{
    if (pRodComponent)
    {
        DataIDType id = pRodComponent->id();
        mRodComponents.emplace(id, pRodComponent);
        mHierarchyRodComponents.appendNode(new HierarchyNode(HierarchyNode::NodeType::kObject, id));
        setModified(true);
    }
}
