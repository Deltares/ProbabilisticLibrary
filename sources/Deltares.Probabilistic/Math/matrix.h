#pragma once
#include <iosfwd>
#include <vector>
#include "vector1D.h"
#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Numeric
    {
        class Matrix
        {
        public:
            Matrix(size_t rows, size_t columns);
            Matrix(const Matrix& m);
            Matrix(Matrix&& m);
            ~Matrix();

            Matrix& operator=(const Matrix& m);
            Matrix& operator=(Matrix&& m);

            Matrix operator+(const Matrix& m) const;
            Matrix operator*(double d) const;

            friend Matrix operator*(double d, const Matrix& m);
            friend std::ostream& operator<<(std::ostream& os, const Matrix& m);

            double& operator()(size_t row, size_t col) { return m_data[pos(row, col)]; }
            const double& operator()(size_t row, size_t col) const { return m_data[pos(row, col)]; }

            double getValue(size_t row, size_t col) { return m_data[pos(row, col)]; }
            void setValue(size_t row, size_t col, double value) { m_data[pos(row, col)] = value; }

            Matrix CholeskyDecomposition() const;
            Matrix Inverse();
            void Transpose();
            Matrix matmul(const Matrix& m2) const;
            vector1D matvec(const vector1D& v) const;
            void get_dims(size_t& row, size_t& col) const { row = m_rows; col = m_columns; }

            size_t getRowCount() const { return this->m_rows; }
            size_t getColumnCount() const { return this->m_columns; }

        private:
            size_t pos(size_t row, size_t column) const
            {
                if (row >= m_rows || column >= m_columns)
                {
                    throw Reliability::probLibException("array bound error");
                }
                return row * m_columns + column;
            }

            std::vector<double> m_data;
            size_t m_rows;
            size_t m_columns;
        };
    }
}
