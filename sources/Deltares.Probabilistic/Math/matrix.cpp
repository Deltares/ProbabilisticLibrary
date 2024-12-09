// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "matrix.h"
#include <iostream>

#include "MatrixSupport.h"

namespace Deltares
{
    namespace Numeric
    {
        Matrix::Matrix(size_t rows, size_t columns)
            : m_data(std::vector<double>(rows* columns)),
            m_rows(rows),
            m_columns(columns)
        {
            for (size_t pos = 0; pos < m_rows * m_columns; pos++)
            {
                m_data[pos] = 0;
            }
        }

        Matrix::Matrix(const Matrix& m)
            : m_data(std::vector<double>(m.m_rows* m.m_columns)),
            m_rows(m.m_rows),
            m_columns(m.m_columns)
        {
            for (size_t pos = 0; pos < m_rows * m_columns; pos++)
            {
                m_data[pos] = m.m_data[pos];
            }
        }

        Matrix::Matrix(Matrix&& m)
            : m_data(m.m_data),
            m_rows(m.m_rows),
            m_columns(m.m_columns)
        {
            m.m_data = std::vector<double>(0);
            m.m_rows = 0;
            m.m_columns = 0;
        }

        Matrix::~Matrix()
        {
        }

        Matrix& Matrix::operator=(const Matrix& m)
        {
            if (this != &m)
            {
                m_rows = m.m_rows;
                m_columns = m.m_columns;

                m_data = std::vector<double>(m_rows * m_columns);

                for (size_t pos = 0; pos < m_rows * m_columns; pos++)
                {
                    m_data[pos] = m.m_data[pos];
                }
            }

            return *this;
        }

        Matrix& Matrix::operator=(Matrix&& m)
        {
            std::swap(m_data, m.m_data);
            std::swap(m_rows, m.m_rows);
            std::swap(m_columns, m.m_columns);
            return *this;
        }

        Matrix Matrix::operator+(const Matrix& m) const
        {
            if (m_rows != m.m_rows || m_columns != m.m_columns)
            {
                throw Reliability::probLibException("#rows <> #colums in matrix addition");
            }

            Matrix result(m_rows, m_columns);

            for (size_t pos = 0; pos < m_rows * m_columns; pos++)
            {
                result.m_data[pos] = m_data[pos] + m.m_data[pos];
            }

            return result;
        }

        Matrix Matrix::operator*(double d) const
        {
            Matrix result(m_rows, m_columns);

            for (size_t pos = 0; pos < m_rows * m_columns; pos++)
            {
                result.m_data[pos] = m_data[pos] * d;
            }

            return result;
        }

        Matrix operator*(double d, const Matrix& m)
        {
            return m * d;
        }

        std::ostream& operator<<(std::ostream& os, const Matrix& m)
        {
            for (size_t row = 0; row < m.m_rows; row++)
            {
                for (size_t col = 0; col < m.m_columns; col++)
                {
                    os << m(row, col) << ", ";
                }

                os << std::endl;
            }

            return os;
        }

        void Matrix::Transpose()
        {
            for (size_t row = 0; row < m_rows; row++)
            {
                for (size_t col = 0; col < row; col++)
                {
                    std::swap(m_data[pos(row, col)], m_data[pos(col, row)]);
                }
            }
        }

        Matrix Matrix::matmul(const Matrix& m2) const
        {
            if (m_columns != m2.m_rows) throw Reliability::probLibException("dimension mismatch in matmul");

            auto result = Matrix(m_rows, m2.m_columns);
            for (size_t row = 0; row < m_rows; row++)
            {
                for (size_t col = 0; col < m2.m_columns; col++)
                {
                    double sum = 0.0;
                    for (size_t k = 0; k < m_columns; k++)
                    {
                        sum += m_data[pos(row, k)] * m2(k, col);
                    }
                    result(row, col) = sum;
                }
            }
            return result;
        }

        vector1D Matrix::matvec(const vector1D& v) const
        {
            if (m_columns != v.size())
            {
                throw Reliability::probLibException("dimension mismatch in matvec");
            }

            auto result = Numeric::vector1D(m_rows);
            for (size_t row = 0; row < m_rows; row++)
            {
                double sum = 0.0;
                for (size_t k = 0; k < m_columns; k++)
                {
                    sum += m_data[pos(row, k)] * v(k);
                }
                result(row) = sum;
            }
            return result;
        }

        Matrix Matrix::Inverse() const
        {
            return MatrixSupport::Inverse(this);
        }

        bool Matrix::IsSymmetric() const
        {
            if (m_columns != m_rows) return false;
            for (size_t row = 0; row < m_rows; row++)
            {
                for (size_t col = 0; col < row; col++)
                {
                    if (m_data[pos(row, col)] != m_data[pos(col, row)]) return false;
                }
            }
            return true;
        }

        bool Matrix::IsPositiveDefinite() const
        {
            try
            {
                if (IsSymmetric())
                {
                    auto m = CholeskyDecomposition();
                }
                else
                {
                    auto m = Inverse();
                }
                return true;
            }
            catch (const std::exception&)
            {
                return false;
            }
        }
    }
}
