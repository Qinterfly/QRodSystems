/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Specification of data types used in a project
 */

#ifndef DATATYPES_H
#define DATATYPES_H

namespace QRS::Core
{

using DataValueType = double;
using DataKeyType = double;
using DataIDType = unsigned int;

enum DataObjectType
{
    kScalar,
    kVector,
    kMatrix,
    kSurface
};

}

#endif // DATATYPES_H
