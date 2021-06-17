/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Specification of types of datasets used in a project
 */

#ifndef ALIASDATASET_H
#define ALIASDATASET_H

#include <unordered_map>
#include "aliasdata.h"

namespace QRS::Core
{

class AbstractDataObject;
class AbstractRodComponent;

using DataObjects = std::unordered_map<DataIDType, AbstractDataObject*>;
using RodComponents = std::unordered_map<DataIDType, AbstractRodComponent*>;

}

#endif // ALIASDATASET_H
