#pragma once
#include <iosfwd>
#include <vector>
#include "vector1D.h"
#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Numeric
    {
        /**
         * \brief Class for a matrix of doubles
         */
        class Matrix
        {
        public:
            /**
             * Constructor
             */
            Matrix(size_t rows, size_t columns);

            /**
             * Copy Constructor
             */
            Matrix(const Matrix& m);

            /**
             * Move Constructor
             */
            Matrix(Matrix&& m);

            /**
             * Destructor
             */
            ~Matrix();

            /**
             * the assignment operator
             */
            Matrix& operator=(const Matrix& m);

            /**
             * the assignment operator
             */
            Matrix& operator=(Matrix&& m);

            /**
             * the + operator
             */
            Matrix operator+(const Matrix& m) const;

            /**
             * the * operator
             */
            Matrix operator*(double d) const;

            /**
             * the * operator
             */
            friend Matrix operator*(double d, const Matrix& m);

            /**
             * the << operator
             */
            friend std::ostream& operator<<(std::ostream& os, const Matrix& m);

            /**
             * the () operator
             */
            double& operator()(size_t row, size_t col) { return m_data[pos(row, col)]; }

            /**
             * the () operator
             */
            const double& operator()(size_t row, size_t col) const { return m_data[pos(row, col)]; }

            /**
             * gets one element of the matrix
             */
            double getValue(size_t row, size_t col) const { return m_data[pos(row, col)]; }

            /**
             * sets one element of the matrix
             */
            void setValue(size_t row, size_t col, double value) { m_data[pos(row, col)] = value; }

            /**
             * returns the Cholesky decomposition of the matrix
             * matrix must symmetric and positive definite, otherwise an exception is thrown
             */
            Matrix CholeskyDecomposition() const;

            /**
             * returns the inverse of the matrix
             * matrix must positive definite, otherwise an exception is thrown
             */
            Matrix Inverse() const;

            /**
             * checks whether or not the matrix is positive definite
             */
            bool IsPositiveDefinite() const;

            /**
             * checks whether or not the matrix is symmetric
             */
            bool IsSymmetric() const;

            /**
             * returns the tranpose of the matrix
             */
            void Transpose();

            /**
             * returns the matrix * matrix product
             */
            Matrix matmul(const Matrix& m2) const;

            /**
             * returns the matrix * vector product
             */
            vector1D matvec(const vector1D& v) const;

            /**
             * sets the two dimensions of the matrix
             */
            void get_dims(size_t& row, size_t& col) const { row = m_rows; col = m_columns; }

            /**
             * returns the number of rows
             */
            size_t getRowCount() const { return this->m_rows; }

            /**
             * returns the number of columns
             */
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
