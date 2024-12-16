// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
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
#pragma once
#include <vector>
#include "matrix.h"

namespace Deltares
{
    namespace Numeric
    {
        class MatrixSupport
        {
        public:
            static Matrix Inverse(const Matrix* source);
        private:
            typedef struct order_array_elem
            {
                int old_row;
                double val;
            } oa_elem_t;

            static size_t count_leading_zeros(const Matrix& mat, const size_t n, const size_t row);
            static void merge(std::vector<oa_elem_t>& A, int p, int q, int r);
            static void mergesort(std::vector<oa_elem_t>& A, int size);
            static void merge_sort(std::vector<oa_elem_t>& A, int p, int r);
            static void get_order(Matrix& mat, size_t n, std::vector<double>& order_arr);
            static Matrix mergesort_mat(Matrix& mat, size_t n, std::vector<double>& order_arr);
            static void sort_mat(std::vector<double>& order_arr, size_t n, Matrix& mat);
            static void init_mat_inv(Matrix& mat_inv, size_t n);
            static bool check_leading_zeros(const Matrix& mat, const size_t n);

        };
    }
}

