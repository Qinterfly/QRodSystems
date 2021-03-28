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
    for (IndexType i = 0; i != newSize; ++i)
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

//! Remove a column by index
template<typename T>
void Array<T>::removeColumn(IndexType iRemoveColumn)
{
    if (iRemoveColumn >= mNumCols)
        return;
    IndexType numCols = mNumCols - 1;
    const IndexType newSize = mNumRows * numCols;
    T* pData = new T[newSize];
    IndexType k = 0;
    IndexType iCur;
    for (IndexType iRow = 0; iRow != mNumRows; ++iRow)
    {
        iCur = iRow * mNumCols;
        for (IndexType jCol = 0; jCol != mNumCols; ++jCol)
        {
            if (jCol != iRemoveColumn)
            {
                pData[k] = mpData[iCur + jCol];
                ++k;
            }
        }
    }
    delete[] mpData;
    mpData = pData;
    mNumCols = numCols;
}

//! Swap two columns
template<typename T>
void Array<T>::swapColumns(IndexType iFirstColumn, IndexType iSecondColumn)
{
    if (iFirstColumn >= mNumCols || iSecondColumn >= mNumCols)
        return;
    for (IndexType iRow = 0; iRow != mNumRows; ++iRow)
        std::swap(mpData[iRow + iFirstColumn], mpData[iRow + iSecondColumn]);
}
