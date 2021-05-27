/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Specification of data types used in a project
 */

#ifndef DATATYPES_H
#define DATATYPES_H

#include <QtGlobal>

namespace QRS::Core
{

using DataValueType = double;
using DataKeyType = double;
using DataIDType = quint32;

enum DataObjectType
{
    kScalar,
    kVector,
    kMatrix,
    kSurface
};

}

#endif // DATATYPES_H
