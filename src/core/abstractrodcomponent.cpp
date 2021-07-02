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
AbstractDataObject const* AbstractRodComponent::readDataObjectPointer(QDataStream& stream, DataObjectGetter const& getDataObject) const
{
    qint64 idDataObject;
    stream >> idDataObject;
    if (idDataObject < 0)
        return nullptr;
    return getDataObject(idDataObject);
}
