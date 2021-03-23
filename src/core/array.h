/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the Array class
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <QDebug>

namespace QRS
{

using IndexType = unsigned int;


//! Numerical array class
template<typename T>
class Array
{
private:
    template <typename U> struct Row;

public:
    Array(IndexType numRows = 1, IndexType numCols = 1);
    ~Array();
    T* data() { return mpData; }
    void resize(IndexType numRows, IndexType numCols);
    IndexType rows() const { return mNumRows; };
    IndexType cols() const { return mNumCols; };
    IndexType size() const { return mNumRows * mNumCols; }
    Row<T> operator[](IndexType iRow) { return Row<T>(&mpData[mNumCols * iRow]); };
    template<typename K> friend QDebug operator<<(QDebug stream, Array<K>& array);
private:
    IndexType mNumRows;
    IndexType mNumCols;
    T* mpData;
    //! Proxy class to acquire a row by index
    template <typename U>
    struct Row
    {
        Row(T* pData) : pRow(pData) { };
        T& operator[](IndexType iCol) { return pRow[iCol]; }
        T* pRow;
    };
};

//! Print all the values as well as their indices
template<typename K>
QDebug operator<<(QDebug stream, Array<K>& array)
{
    IndexType const& nRows = array.rows();
    IndexType const& nCols = array.cols();
    stream = stream.noquote();
    stream << QString("Array size: %1 x %2").arg(QString::number(nRows), QString::number(nCols));
    stream << Qt::endl;
    for (IndexType iRow = 0; iRow != nRows; ++iRow)
    {
        for (IndexType jCol = 0; jCol != nCols; ++jCol)
            stream << QString::number(array[iRow][jCol]);
        stream << Qt::endl;
    }
    return stream;
}

}



#endif // ARRAY_H
