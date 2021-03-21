/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of DataObject class
 */

#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <QString>
#include <unordered_map>
#include "matrix.h"

namespace QRS
{
using ScalarDataType = double;
using VectorDataType = std::vector<ScalarDataType>;
using MatrixDataType = Matrix<ScalarDataType>;

/*!
 * \brief Data object which is designied in the way to be represented in a table easily
 * \tparam double
 * \tparam vector<double>
 * \tparam matrix<double>
 * \tparam surface<double>
 */
template <typename T>
class DataObject
{
    using DataHolder = std::unordered_map<ScalarDataType, T>;

public:
    DataObject(const QString& name);
    ~DataObject() = default;
    static uint getID();
    // Modification
    void addEntity(double keyParameter, const T& entity);
    void modifyEntity(double keyParameter, const T& entity);
    void removeEntity(double keyParameter);

private:
    static uint smID;
    QString mName;
    DataHolder mEntities;
};


}

#endif // DATAOBJECT_H
