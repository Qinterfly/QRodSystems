/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of Matrix class
 */

#include "array.h"

template class QRS::Row<double>;
template class QRS::Array<double>;

using namespace QRS;

template<typename T>
Array<T>::Array(IndexType numRows, IndexType numCols)
    : mNumRows(numRows)
    , mNumCols(numCols)
{
    mData.resize(numRows * numCols);
}

//! Get a row by index
template<typename T>
Row<T> Array<T>::operator[](IndexType iRow)
{
    return Row<T>(&mData[mNumRows * iRow]);
}


