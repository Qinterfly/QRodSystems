/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the Array class
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <QDebug>

namespace QRS::Core
{

using IndexType = quint32;

//! Numerical array class
template<typename T>
class Array
{
private:
    template <typename U> struct Row;

public:
    Array(IndexType numRows = 0, IndexType numCols = 0);
    Array(Array<T> const& another);
    Array(Array<T>&& another);
    ~Array();
    T* data() { return mpData; }
    void resize(IndexType numRows, IndexType numCols);
    void removeColumn(IndexType iRemoveColumn);
    void swapColumns(IndexType iFirstColumn, IndexType iSecondColumn);
    IndexType rows() const { return mNumRows; };
    IndexType cols() const { return mNumCols; };
    IndexType size() const { return mNumRows * mNumCols; }
    Row<T> operator[](IndexType iRow) { return Row<T>(&mpData[mNumCols * iRow]); };
    Row<T> operator[](IndexType iRow) const { return Row<T>(&mpData[mNumCols * iRow]); };
    Array& operator=(Array<T> const& another);
    template<typename K> friend QDebug operator<<(QDebug stream, Array<K>& array);
    template<typename K> friend QDataStream& operator<<(QDataStream& stream, Array<K> const& array);
    template<typename K> friend QDataStream& operator>>(QDataStream& stream, Array<K>& array);

private:
    //! Number of rows
    IndexType mNumRows;
    //! Number of columns
    IndexType mNumCols;
    //! Pointer to the data stored
    T* mpData = nullptr;
    //! Proxy class to acquire a row by index
    template <typename U>
    struct Row
    {
        Row() = delete;
        Row(T* pData) : pRow(pData) { };
        ~Row() { }
        T& operator[](IndexType iCol) { return pRow[iCol]; }
        T const& operator[](IndexType iCol) const { return pRow[iCol]; }
        T* pRow;
    };
};

//! Print all array values using the matrix format
template<typename K>
inline QDebug operator<<(QDebug stream, Array<K>& array)
{
    IndexType const& nRows = array.mNumRows;
    IndexType const& nCols = array.mNumCols;
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

//! Write an array to a stream
template<typename K>
inline QDataStream& operator<<(QDataStream& stream, Array<K> const& array)
{
    stream << array.mNumRows << array.mNumCols;
    IndexType const& size = array.size();
    for (IndexType i = 0; i != size; ++i)
        stream << array.mpData[i];
    return stream;
}

//! Read an array from a stream
template<typename K>
inline QDataStream& operator>>(QDataStream& stream, Array<K>& array)
{
    delete[] array.mpData;
    stream >> array.mNumRows >> array.mNumCols;
    IndexType const& size = array.size();
    array.mpData = new K[size];
    for (IndexType i = 0; i != size; ++i)
        stream >> array.mpData[i];
    return stream;
}

}

#endif // ARRAY_H
