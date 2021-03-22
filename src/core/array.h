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

using IndexType = unsigned int;

//! Proxy class to acquire a row by index
template<typename T>
class Row
{
public:
    Row(T* pData) : mpData(pData) { };
    T& operator[](IndexType iCol) { return mpData[iCol]; }

private:
    T* mpData;
};

//! Numerical array class
template<typename T>
class Array
{
public:
    Array(IndexType numRows = 1, IndexType numCols = 1);
    ~Array() = default;
    std::vector<T> const& data() const { return mData; }
    IndexType rows() const { return mNumRows; };
    IndexType cols() const { return mNumCols; };
    Row<T> operator[](IndexType iRow);

private:
    std::vector<T> mData;
    IndexType mNumRows;
    IndexType mNumCols;
};


}

#endif // ARRAY_H
