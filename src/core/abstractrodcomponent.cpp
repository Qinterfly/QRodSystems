/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Definition of the AbstractRodComponent class
 */

#include "abstractrodcomponent.h"

using namespace QRS::Core;

quint32 AbstractRodComponent::smNumComponents = 0;

AbstractRodComponent::AbstractRodComponent(ComponentType componentType, QString name)
    : mComponentType(componentType)
    , mName(name)
{
    mID = ++smNumComponents;
}

AbstractRodComponent::~AbstractRodComponent()
{

}
