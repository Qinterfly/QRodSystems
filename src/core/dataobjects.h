/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of data objects
 */

#ifndef ABSTRACTDATAOBJECT_H
#define ABSTRACTDATAOBJECT_H

namespace QRS
{

//! Base class for all data objects
class AbstractDataObject
{
public:
    AbstractDataObject();
    virtual ~AbstractDataObject() = 0;

private:
    static int smID;
};

//! Scalar data object
class Scalar : AbstractDataObject
{
public:
    Scalar() = default;
    ~Scalar() = default;

private:
    double mValue = 0.0;
};

//! Vector (1x3) data object
class Vector : AbstractDataObject
{

};

//! Matrix (3x3) data object
class Matrix : AbstractDataObject
{

};

//! Surface (Nx2) data object
class Surface : AbstractDataObject
{

};

}

#endif // ABSTRACTDATAOBJECT_H
