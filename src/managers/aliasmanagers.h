/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Specification of data types used in managers
 */

#ifndef ALIASMANAGERS_H
#define ALIASMANAGERS_H

#include "core/aliasdata.h"

namespace QRS
{

namespace Core
{
class ScalarDataObject;
class VectorDataObject;
class MatrixDataObject;
class SurfaceDataObject;
}

namespace Managers
{

using ScalarDataObjects = std::unordered_map<Core::DataIDType, Core::ScalarDataObject const* >;
using VectorDataObjects = std::unordered_map<Core::DataIDType, Core::VectorDataObject const* >;
using MatrixDataObjects = std::unordered_map<Core::DataIDType, Core::MatrixDataObject const* >;
using SurfaceDataObjects = std::unordered_map<Core::DataIDType, Core::SurfaceDataObject const* >;

}

}


#endif // ALIASMANAGERS_H
