/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the Array class
 */

#include "array.h"

template class QRS::Array<double>;
using namespace QRS;

template<typename T>
Array<T>::Array(IndexType numRows, IndexType numCols)
    : mNumRows(numRows)
    , mNumCols(numCols)
{
    const IndexType& curSize = size();
    mpData = new T[curSize];
    for (IndexType i = 0; i != curSize; ++i)
        mpData[i] = 0.0;
}

//! Copy constructor
template<typename T>
Array<T>::Array(Array<T> const& another)
{
    delete[] mpData;
    const IndexType& newSize = another.size();
    mpData = new T[newSize];
    for (int i = 0; i != newSize; ++i)
        mpData[i] = another.mpData[i];
    mNumRows = another.mNumRows;
    mNumCols = another.mNumCols;
}

//! Move constructor
template<typename T>
Array<T>::Array(Array<T>&& another)
{
    std::swap(mpData, another.mpData);
    mNumRows = std::exchange(another.mNumRows, 0);
    mNumCols = std::exchange(another.mNumCols, 0);
}

template<typename T>
Array<T>::~Array()
{
    delete[] mpData;
}

//! Resize and copy previous values if possible
template<typename T>
void Array<T>::resize(IndexType numRows, IndexType numCols)
{
    if (!numRows || !numCols)
    {
        delete[] mpData;
        mpData = nullptr;
        mNumRows = 0;
        mNumCols = 0;
        return;
    }
    if (numRows == mNumRows && numCols == mNumCols)
        return;
    const IndexType newSize = numRows * numCols;
    T* pData = new T[newSize];
    // Filling with zeros
    for (IndexType i = 0; i != newSize; ++i)
        pData[i] = 0.0;
    // Copying previous values
    IndexType iCur;
    IndexType iNew;
    IndexType minNumRows = std::min(mNumRows, numRows);
    IndexType minNumCols = std::min(mNumCols, numCols);
    for (IndexType iRow = 0; iRow != minNumRows; ++iRow)
    {
        iCur = iRow * mNumCols;
        iNew = iRow * numCols;
        for (IndexType jCol = 0; jCol != minNumCols; ++jCol)
            pData[iNew + jCol] = mpData[iCur + jCol];
    }
    // Swapping
    delete[] mpData;
    mpData = pData;
    mNumRows = numRows;
    mNumCols = numCols;
}
