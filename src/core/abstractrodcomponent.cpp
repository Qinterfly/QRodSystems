/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Definition of the AbstractRodComponent class
 */

#include "abstractrodcomponent.h"

using namespace QRS::Core;

quint32 AbstractRodComponent::smNumObjects = 0;

AbstractRodComponent::AbstractRodComponent(ComponentType type, QString name)
    : mType(type)
    , mName(name)
{
    mID = ++smNumObjects;
}

AbstractRodComponent::~AbstractRodComponent()
{

}
