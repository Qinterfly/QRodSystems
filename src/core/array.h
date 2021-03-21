/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of Matrix class
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <vector>

namespace QRS
{

//! Numerical matrix
template<typename T>
class Array
{
public:
    using IndexType = unsigned int;
    Array(IndexType numRows = 1, IndexType numCols = 1);
    ~Array() = default;

private:
    std::vector<T> mValues;
    IndexType mNumRows;
    IndexType mNumCols;
};

}

#endif // ARRAY_H
