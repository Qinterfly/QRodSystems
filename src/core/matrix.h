/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of Matrix class
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <QtGlobal>
#include <vector>

namespace QRS
{

//! Numerical matrix
template<typename T>
class Matrix
{
public:
    Matrix() = default;
    Matrix(uint numRows, uint numCols);
    ~Matrix() = default;
    void setValue(T&);

private:
    std::vector<T> mValues;
    uint mNumRows;
    uint mNumCols;
};

}

#endif // MATRIX_H
