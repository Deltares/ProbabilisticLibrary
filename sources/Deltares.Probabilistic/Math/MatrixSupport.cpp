#include "MatrixSupport.h"
#include <cmath>

// Implementation taken from https://github.com/mndxpnsn/gauss-jordan/blob/master/gauss-jordan/main.cpp

namespace Deltares
{
    namespace Numeric
    {
        const double SMALL_NUM = 1e-10;
        const int MAX_INT = 1215752192;

        size_t MatrixSupport::count_leading_zeros(const Matrix& mat, const size_t n, const size_t row)
        {
            size_t count = 0;

            while (fabs(mat.getValue(row, count)) <= SMALL_NUM && count < n) 
            {
                count++;
            }

            return count;
        }


        void MatrixSupport::merge(std::vector<oa_elem_t> & A, int p, int q, int r)
        {
            int size_r, size_l;
            int i, j;
            size_l = q - p + 1;
            size_r = r - q;
            auto L = std::vector<oa_elem_t>(size_l + 1);
            auto R = std::vector<oa_elem_t>(size_r + 1);
            i = 0;
            j = 0;

            for (int n = p; n < q + 1; ++n) 
            {
                L[i] = A[n];
                ++i;
            }

            L[size_l].val = MAX_INT;
            for (int n = q + 1; n < r + 1; ++n) 
            {
                R[j] = A[n];
                ++j;
            }

            R[size_r].val = MAX_INT;
            i = 0;
            j = 0;
            for (int n = p; n < r + 1; ++n) 
            {
                if (L[i].val < R[j].val) 
                {
                    A[n] = L[i];
                    ++i;
                }
                else 
                {
                    A[n] = R[j];
                    ++j;
                }
            }
        }

        void MatrixSupport::merge_sort(std::vector<oa_elem_t> &A, int p, int r)
        {
            if (p < r)
            {
                int q = (p + r) / 2;
                merge_sort(A, p, q);
                merge_sort(A, q + 1, r);
                merge(A, p, q, r);
            }
        }

        void MatrixSupport::mergesort(std::vector<oa_elem_t>& A, int size)
        {
            merge_sort(A, 0, size - 1);
        }

        Matrix MatrixSupport::mergesort_mat(Matrix& mat, size_t n, std::vector<double>& order_arr)
        {
            auto order_array = std::vector<oa_elem_t>(n);

            for (int row = 0; row < n; ++row) 
            {
                order_array[row].old_row = row;
                order_array[row].val = order_arr[row];
            }

            mergesort(order_array, n);

            Matrix ordered_mat = Matrix(n, n);

            for (size_t row = 0; row < n; ++row) 
            {
                for (size_t c = 0; c < n; ++c)
                {
                    int old_row = order_array[row].old_row;
                    ordered_mat.setValue(row, c, mat.getValue(old_row, c));
                }
            }

            return ordered_mat;
        }

        void MatrixSupport::sort_mat(std::vector<double>& order_arr, size_t n, Matrix& mat)
        {
            auto mat_ordered = mergesort_mat(mat, n, order_arr);

            for (size_t row = 0; row < n; ++row) 
            {
                for (size_t c = 0; c < n; ++c)
                {
                    double newValue = mat_ordered.getValue(row, c);

                    if (fabs(newValue) <= SMALL_NUM)
                    {
                        newValue = 0.0;
                    }

                    mat.setValue(row, c, newValue);
                }
            }
        }

        void MatrixSupport::get_order(Matrix& mat, size_t n, std::vector<double>& order_arr)
        {
            for (int row = 0; row < n; ++row) 
            {
                size_t order = 0;
                while (fabs(mat.getValue(row, order)) <= SMALL_NUM && order < n) 
                {
                    mat.setValue(row, order, 0.0);
                    order++;
                }

                order_arr[row] = order;
            }
        }


        void MatrixSupport::init_mat_inv(Matrix& mat_inv, size_t n)
        {
            for (size_t row = 0; row < n; ++row)
            {
                for (size_t c = 0; c < n; ++c)
                {
                    if (c == row) 
                    {
                        mat_inv.setValue(row, c, 1.0);
                    }
                    else 
                    {
                        mat_inv.setValue(row, c, 0.0);
                    }
                }
            }
        }

        bool MatrixSupport::check_leading_zeros(const Matrix& mat, const size_t n)
        {
            // Check if matrix is singular
            for (size_t row = 0; row < n; ++row)
            {
                const size_t num_lead_zeros = count_leading_zeros(mat, n, row);

                if (num_lead_zeros >= row + 1) 
                {
                    return true;
                }
            }

            return false;
        }


        Matrix MatrixSupport::Inverse(const Matrix* src)
        {
            if (src->getRowCount() != src->getColumnCount()) throw Reliability::probLibException("Matrix inverse: input matrix must be square.");

            const size_t n = src->getRowCount();        // Number of stochastic variables
            Matrix source = *src;

            Matrix inverse = Matrix(n, n);

            auto order_arr = std::vector<double>(n);

            // Initialize matrix inverse
            init_mat_inv(inverse, n);

            bool is_singular = false;

            // Convert to row echelon form
            for (size_t c = 0; c < n; ++c) {

                // Sort if under threshold
                if (fabs(source.getValue(c, c)) <= SMALL_NUM)
                {
                    get_order(source, n, order_arr);

                    sort_mat(order_arr, n, source);

                    sort_mat(order_arr, n, inverse);

                    is_singular |= check_leading_zeros(source, n);
                }

                // Normalize matrix row
                for (size_t col = c + 1; col < n; ++col)
                {
                    double newValue = fabs(source.getValue(c, c)) <= SMALL_NUM ? 0.0 : source.getValue(c, col) / source.getValue(c, c);
                    source.setValue(c, col, newValue);
                }

                // Update row matrix inverse
                for (int col = 0; col < n; ++col)
                {
                    double newValue = fabs(source.getValue(c, c)) <= SMALL_NUM ? 0.0 : inverse.getValue(c, col) / source.getValue(c, c);
                    inverse.setValue(c, col, newValue);
                }

                source.setValue(c, c, 1.0);

                // Delete elements in rows below
                for (int row = c + 1; row < n; ++row) 
                {
                    if (source.getValue(row, c) != 0) 
                    {
                        for (int col = c + 1; col < n; ++col) 
                        {
                            const double newValue = -1.0 * source.getValue(row, c) * source.getValue(c, col) + source.getValue(row, col);
                            source.setValue(row, col, newValue);
                        }

                        for (int col = 0; col < n; ++col) 
                        {
                            const double newValue = -1.0 * source.getValue(row, c) * inverse.getValue(c, col) + inverse.getValue(row, col);
                            inverse.setValue(row, col, newValue);
                        }

                        source.setValue(row, c, 0);
                    }
                }
            }

            // Backtrace to convert to reduced row echelon form
            for (int c = n - 1; c > 0; --c) 
            {
                for (int row = c - 1; row > -1; --row) 
                {
                    if (source.getValue(row, c) != 0) 
                    {
                        for (int col = 0; col < n; ++col) 
                        {
                            const double newValue = -1.0 * source.getValue(row, c) * inverse.getValue(c, col) + inverse.getValue(row, col);
                            inverse.setValue(row, col, newValue);
                        }

                        source.setValue(row, c, 0);
                    }
                }
            }

            return inverse;
        }

    }
}

