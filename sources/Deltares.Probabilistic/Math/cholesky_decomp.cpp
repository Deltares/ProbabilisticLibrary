#include <math.h>
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
