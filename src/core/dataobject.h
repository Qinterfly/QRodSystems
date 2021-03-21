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

namespace QRS
{

//! Data object
//! \tparam double
//! \tparam vector<double>
//! \tparam matrix<double>
//! \tparam surface<double>
template <typename T>
class DataObject
{
public:
    DataObject(QString name);
    ~DataObject() = default;
    void addEntity(double key, const T& entity);

private:
    std::unordered_map<double, T> mEntities;
    QString mName;
    static uint smID;
};

}

#endif // DATAOBJECT_H
