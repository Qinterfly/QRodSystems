/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the AbstractRodComponent class
 */

#include "abstractrodcomponent.h"
#include "abstractdataobject.h"

using namespace QRS::Core;

DataIDType AbstractRodComponent::smMaxComponentID = 0;

AbstractRodComponent::AbstractRodComponent(ComponentType componentType, QString name)
    : mkComponentType(componentType)
    , mName(name)
{
    mID = ++smMaxComponentID;
}

AbstractRodComponent::~AbstractRodComponent()
{

}

//! Helper function to write the identifier of a data object
void AbstractRodComponent::writeDataObjectPointer(QDataStream& stream, AbstractDataObject const* pDataObject) const
{
    qint64 id = -1;
    if (pDataObject)
        id = (qint64)pDataObject->id();
    stream << id;
};

//! Helper function to retrieve the pointer to the data object by its identifier
AbstractDataObject const* AbstractRodComponent::readDataObjectPointer(QDataStream& stream, DataObjects const& dataObjects) const
{
    qint64 idDataObject;
    stream >> idDataObject;
    if (idDataObject < 0)
        return nullptr;
    return getDataObject(dataObjects, idDataObject);
}

//! Retrieve a data object from a set by id
AbstractDataObject const* AbstractRodComponent::getDataObject(DataObjects const& dataObjects, DataIDType id) const
{
    AbstractDataObject const* pDataObject = nullptr;
    auto iterFound = dataObjects.find(id);
    if (iterFound != dataObjects.end())
        pDataObject = iterFound->second;
    return pDataObject;
}

//! Substitute a data object with its updated version
AbstractDataObject const* AbstractRodComponent::substituteDataObject(DataObjects const& dataObjects,
                                                                     AbstractDataObject const* pDataObject) const
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
}
