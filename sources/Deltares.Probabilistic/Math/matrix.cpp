// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
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

        Matrix Matrix::identity(size_t n)                                   // n x n Identity matrix
        {
            Matrix I(n, n);
            for (size_t i = 0; i < n; i++) I.setValue(i, i, 1.0);
            return I;
        }

        //bool Matrix::QR(const Matrix& A, Matrix& Q, Matrix& R)
        //{
        //    // see https://cplusplus.com/forum/general/242116/

        //    // https://github.com/hosseinmoein/Tiger

        //    // https://libeigen.gitlab.io/

        //    const double SMALL = 1.0E-30;          // used to stop divide-by-zero

        //    size_t rows = A.getRowCount();
        //    size_t cols = A.getColumnCount();

        //    if (rows < cols)
        //    {
        //        throw probLibException("Algorithm only works for rows >= cols");
        //    }

        //    R = A;
        //    Matrix QT = identity(rows);

        //    for (size_t k = 0; k < cols - 1; k++)          // k is the working column
        //    {
        //        // X vector, based on the elements from k down in the kth column
        //        double alpha = 0;
        //        for (size_t i = k; i < rows; i++) alpha += R(i, k) * R(i, k);
        //        alpha = sqrt(alpha);                     // alpha is the Euclidean norm of Xk

        //        // V vector ( normalise Xk - alpha e_k )
        //        vector1D V = vector1D(rows);

        //        double Vnorm = 0.0;
        //        for (size_t i = k + 1; i < rows; i++)
        //        {
        //            V(i) = R(i, k);
        //            Vnorm += V(k) * V(k);
        //        }
        //        V(k) = R(k, k) - alpha;
        //        Vnorm += V(k) * V(k);
        //        Vnorm = sqrt(Vnorm);

        //        if (Vnorm > SMALL)
        //        {
        //            for (size_t i = k; i < rows; i++)
        //            {
        //                V(i) /= Vnorm;
        //            }
        //        }

        //        // Householder matrix: Qk = I - 2 V VT
        //        Matrix Qk = identity(rows);
        //        for (size_t i = k; i < rows; i++)
        //        {
        //            for (size_t j = k; j < rows; j++)
        //            {
        //                Qk(i, j) -= 2.0 * V(i) * V(j);
        //            }
        //        }

        //        QT= Qk.matmul(QT);
        //        R = Qk.matmul(R);
        //    }

        //    Q = QT;
        //    Q.Transpose();

        //    return true;
        //}

        // compute minor
        Matrix Matrix::compute_minor(const Matrix& mat, size_t d)
        {
            Matrix minor = Matrix(mat.getRowCount(), mat.getColumnCount());

            for (size_t i = 0; i < d; i++)
                minor(i, i) = 1.0;
            for (size_t i = d; i < mat.getRowCount(); i++)
                for (size_t j = d; j < mat.getColumnCount(); j++)
                    minor(i, j) = mat(i, j);

            return minor;
        }

        vector1D Matrix::extract_column(size_t c)
        {
            vector1D v = vector1D(m_rows);

            for (size_t i = 0; i < m_rows; i++)
            {
                v(i) = (*this)(i, c);
            }

            return v;
        }

        std::vector<Matrix> Matrix::QR(const Matrix& mat)
        {
            // https://rosettacode.org/wiki/QR_decomposition#C++

            size_t m = mat.getRowCount();
            size_t n = mat.getColumnCount();

            // array of factor Q1, Q2, ... Qm
            std::vector<Matrix> qv_matrices;

            // temp array
            Matrix z(mat);
            Matrix z1(m,n);

            for (size_t k = 0; k < n && k < m - 1; k++) {

                vector1D e = vector1D(m);
                vector1D x = vector1D(m);

                // compute minor
                z1 = compute_minor(z, k);

                // extract k-th column into x
                x = z1.extract_column(k);

                double a = x.norm();
                if (mat(k, k) > 0) a = -a;

                for (size_t i = 0; i < e.size(); i++)
                {
                    e(i) = i == k ? 1.0 : 0.0;
                }

                // e = x + a*e
                e = x + a * e;
                //vmadd(x, e, a, e);

                // e = e / ||e||
                e.normalize();

                // compute household factor
                // qv[k] = I - 2 *e*e^T
                // compute_householder_factor(qv[k], e);

                Matrix qv = Matrix(n, n);
                for (size_t i = 0; i < n; i++)
                    for (size_t j = 0; j < n; j++)
                        qv(i, j) = -2 * e(i) * e(j);
                for (size_t i = 0; i < n; i++)
                    qv(i, i) += 1;

                qv_matrices.push_back(qv);

                // z = qv[k] * z1
                // z.mult(qv[k], z1);
                z = qv.matmul(z1);
            }

            Matrix Q = qv_matrices[0];

            // after this loop, we will obtain Q (up to a transpose operation)
            for (size_t i = 1; i < n && i < m - 1; i++) {

                z1 = qv_matrices[i].matmul(Q);
                Q = z1;
            }

            Matrix R = Q.matmul(mat);
            Q.Transpose();

            return std::vector<Matrix> {R, Q};
        }
    }
}
