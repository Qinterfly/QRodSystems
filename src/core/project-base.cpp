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
#include "materialrodcomponent.h"

using namespace QRS::Core;

template<typename T>
void clearDataMap(std::unordered_map<DataIDType, T*>& dataMap);
AbstractDataObject* createDataObject(AbstractDataObject::ObjectType type);
AbstractDataObject const* substituteDataObject(DataObjects const& dataObjects, AbstractDataObject const* pObject);
void resolveGeometryRodComponentReferences(DataObjects const& dataObjects, GeometryRodComponent* pGeometry);
void resolveSectionRodComponentReferences(DataObjects const& dataObjects, AbstractSectionRodComponent* pBaseSection);
void resolveMaterialRodComponentReferences(DataObjects const& dataObjects, MaterialRodComponent* pMaterial);

//! Construct a clean project with the user specified name
Project::Project(QString const& name)
    : mID(QRandomGenerator::global()->generate())
    , mName(name)
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
            resolveGeometryRodComponentReferences(dataObjects, (GeometryRodComponent*)pRodComponent);
            break;
        case AbstractRodComponent::ComponentType::kSection:
            resolveSectionRodComponentReferences(dataObjects, (AbstractSectionRodComponent*)pRodComponent);
            break;
        case AbstractRodComponent::ComponentType::kMaterial:
            resolveMaterialRodComponentReferences(dataObjects, (MaterialRodComponent*)pRodComponent);
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

//! Add a material rod component
AbstractRodComponent* Project::addMaterial()
{
    QString name = "Material " + QString::number(MaterialRodComponent::numberInstances() + 1);
    AbstractRodComponent* pRodComponent = new MaterialRodComponent(name);
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

//! Emplace a rod component into a project
void Project::emplaceRodComponent(AbstractRodComponent* pRodComponent)
{
    if (pRodComponent)
    {
        DataIDType id = pRodComponent->id();
        mRodComponents.emplace(id, pRodComponent);
        mHierarchyRodComponents.appendNode(new HierarchyNode(HierarchyNode::NodeType::kObject, id));
    }
}

//! Helper function to clear a map consisted of data pointers
template<typename T>
void clearDataMap(std::unordered_map<DataIDType, T*>& dataMap)
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

//! Helper function to substitute a data object with its updated version
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

//! Helper function to resolve references of a geometrical rod component
void resolveGeometryRodComponentReferences(DataObjects const& dataObjects, GeometryRodComponent* pGeometry)
{
    pGeometry->setRadiusVector((VectorDataObject const*)substituteDataObject(dataObjects, pGeometry->radiusVector()));
    pGeometry->setRotationMatrix((MatrixDataObject const*)substituteDataObject(dataObjects, pGeometry->rotationMatrix()));
}

//! Helper function to resolve references of a cross section
void resolveSectionRodComponentReferences(DataObjects const& dataObjects, AbstractSectionRodComponent* pBaseSection)
{
    switch (pBaseSection->sectionType())
    {
    case AbstractSectionRodComponent::SectionType::kUserDefined:
        UserSectionRodComponent* pUserSection = (UserSectionRodComponent*)pBaseSection;
        pUserSection->setArea((ScalarDataObject const*)substituteDataObject(dataObjects, pUserSection->area()));
        pUserSection->setInertiaMomentTorsional((ScalarDataObject const*)substituteDataObject(dataObjects, pUserSection->inertiaMomentTorsional()));
        pUserSection->setInertiaMomentX((ScalarDataObject const*)substituteDataObject(dataObjects, pUserSection->inertiaMomentX()));
        pUserSection->setInertiaMomentY((ScalarDataObject const*)substituteDataObject(dataObjects, pUserSection->inertiaMomentY()));
        pUserSection->setCenterCoordinateX((ScalarDataObject const*)substituteDataObject(dataObjects, pUserSection->centerCoordinateX()));
        pUserSection->setCenterCoordinateY((ScalarDataObject const*)substituteDataObject(dataObjects, pUserSection->centerCoordinateY()));
        break;
    }
}

//! Helper function to resolve references of a material rod component
void resolveMaterialRodComponentReferences(DataObjects const& dataObjects, MaterialRodComponent* pMaterial)
{
    pMaterial->setElasticModulus((ScalarDataObject const*)substituteDataObject(dataObjects, pMaterial->elasticModulus()));
    pMaterial->setShearModulus((ScalarDataObject const*)substituteDataObject(dataObjects, pMaterial->shearModulus()));
    pMaterial->setPoissonsRatio((ScalarDataObject const*)substituteDataObject(dataObjects, pMaterial->poissonsRatio()));
    pMaterial->setDensity((ScalarDataObject const*)substituteDataObject(dataObjects, pMaterial->density()));
}
