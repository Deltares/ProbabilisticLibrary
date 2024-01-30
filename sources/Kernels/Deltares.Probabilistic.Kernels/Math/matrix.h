#pragma once
#include <iosfwd>
#include <vector>
#include "vector1D.h"
#include "../Utils/probLibException.h"

namespace Deltares {
    namespace ProbLibCore {

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

            Matrix CholeskyDecomposition() const;
            void Transpose();
            Matrix matmul(const Matrix& m2) const;
            vector1D matvec(const vector1D& v) const;
            void get_dims(size_t& row, size_t& col) const { row = m_rows; col = m_columns; }

        private:
            size_t pos(size_t row, size_t column) const
            {
                //if (row >= m_rows || column >= m_columns) throw probLibException("array bound error");
                return row * m_columns + column;
            }

        private:
            std::vector<double> m_data;
            size_t m_rows;
            size_t m_columns;
        };
    }
}
