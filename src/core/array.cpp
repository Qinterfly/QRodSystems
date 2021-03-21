/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of Matrix class
 */

#include "array.h"

template class QRS::Array<double>;

using namespace QRS;

template<typename T>
Array<T>::Array(IndexType numRows, IndexType numCols)
    : mNumRows(numRows)
    , mNumCols(numCols)
{
    mValues.resize(numRows * numCols);
}




