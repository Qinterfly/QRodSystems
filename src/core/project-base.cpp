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

using namespace QRS::Core;

AbstractDataObject* createDataObject(AbstractDataObject::ObjectType type);
AbstractRodComponent* createRodComponent(AbstractRodComponent::ComponentType type);

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
    clearDataMap(mDataObjects);
    AbstractDataObject* pDataObject;
    for (auto& item : dataObjects)
    {
        pDataObject = item.second;
        mDataObjects.emplace(pDataObject->id(), pDataObject->clone());
    }
    mHierarchyDataObjects = hierarchyDataObjects;
    emit dataObjectsChanged();
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

//! Create a rod component of a given type
AbstractRodComponent* Project::addRodComponent(AbstractRodComponent::ComponentType type)
{
    AbstractRodComponent* pComponent = createRodComponent(type);
    if (pComponent)
    {
        DataIDType id = pComponent->id();
        mRodComponents.emplace(id, pComponent);
        mHierarchyRodComponents.appendNode(new HierarchyNode(HierarchyNode::NodeType::kObject, id));
        setModified(true);
    }
    return pComponent;
}

//! Clone rod components
RodComponents Project::cloneRodComponents() const
{
    RodComponents result;
    for (auto& pItem : mRodComponents)
    {
        AbstractRodComponent* pComponent = pItem.second->clone();
        result.emplace(pComponent->id(), pComponent);
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

//! Helper function to create RodComponent by a type and name
AbstractRodComponent* createRodComponent(AbstractRodComponent::ComponentType type)
{
    AbstractRodComponent* pComponent = nullptr;
    QString name;
    switch (type)
    {
    case AbstractRodComponent::ComponentType::kGeometry:
        name = "Geometry " + QString::number(GeometryRodComponent::numberInstances() + 1);
        pComponent = new GeometryRodComponent(name);
        break;
    }
    return pComponent;
}
