#pragma once
#include "matrix.h"

namespace Deltares
{
    namespace Numeric
    {
        class MatrixSupport
        {
        public:
            static Matrix* Inverse(Matrix* source);
        private:
            typedef struct order_array_elem
            {
                int old_row;
                double val;
            } oa_elem_t;

            static size_t count_leading_zeros(Matrix* mat, size_t n, size_t row);
            static void merge(oa_elem_t A[], int p, int q, int r);
            static void mergesort(oa_elem_t A[], int size);
            static void merge_sort(oa_elem_t A[], int p, int r);
            static void get_order(Matrix* mat, size_t n, double* order_arr);
            static Matrix* mergesort_mat(Matrix* mat, size_t n, double* order_arr);
            static void sort_mat(double* order_arr, size_t n, Matrix* mat);
            static void init_mat_inv(Matrix* mat_inv, size_t n);
            static bool check_leading_zeros(Matrix* mat, size_t n);

        };
    }
}

