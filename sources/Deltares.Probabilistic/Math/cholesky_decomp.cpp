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
#include <cmath>
#include "matrix.h"
#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Numeric
    {
        // Cholesky decomposition
        Matrix Matrix::CholeskyDecomposition() const
        {
            if (m_rows != m_columns) throw probLibException("CholeskyDecomposition : input matrix must be square.");

            const size_t nStochasts = m_rows;        // Number of stochastic variables

            auto cholesky = Matrix(nStochasts, nStochasts);

            for (size_t j = 0; j < nStochasts; j++)
            {
                double sumrow = 0.0;
                for (size_t k = 0; k < j; k++)
                {
                    sumrow += pow(cholesky(j, k), 2);
                }
                double arg_sqrt = m_data[pos(j, j)] - sumrow;
                if (arg_sqrt >= 0.0)
                {
                    cholesky(j, j) = sqrt(arg_sqrt);
                }
                else
                {
                    throw probLibException("Cholesky decomposition fails: negative argument sqrt");
                }

                for (size_t i = j + 1; i < nStochasts; i++)
                {
                    double sumcol = 0.0;
                    if (cholesky(j, j) == 0.0)
                    {
                        throw probLibException("Cholesky decomposition fails: division by zero");
                    }
                    for (size_t k = 0; k < j; k++)
                    {
                        sumcol += cholesky(i, k) * cholesky(j, k);
                    }
                    cholesky(i, j) = (m_data[pos(i, j)] - sumcol) / cholesky(j, j);
                }
            }
            return cholesky;
        }
    }
}
