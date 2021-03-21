/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of Matrix class
 */

#include "matrix.h"

using namespace QRS;

template<typename T>
Matrix<T>::Matrix(uint numRows, uint numCols)
    : mNumRows(numRows)
    , mNumCols(numCols)
{
    mValues.resize(numRows * numCols);
}

