/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of data objects
 */

#include "dataobjects.h"

using namespace QRS;

int AbstractDataObject::smID = 0;

AbstractDataObject::AbstractDataObject()
{
    ++smID;
}

