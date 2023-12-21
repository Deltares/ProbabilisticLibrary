#include <math.h>
#include "gaussianCorrelationRobust.h"
#include "../utils/matrix.h"
#include "../utils/basic_math.h"

namespace Deltares {
    namespace ProbLibCore {

        //> @file
        // Contains the module for handling correlations
        //<
        //>
        // Module for registering (basic) correlations between two stochasts
        //

        gaussianCorrelationRobust::gaussianCorrelationRobust(const int n, const std::vector<corrStruct>& pairs)
            : gaussianCorrelation(n)
        {
            updateCorrelationMatrixRobust(n, pairs);
        }

        // Method to update the correlation matrix
        void gaussianCorrelationRobust::updateCorrelationMatrixRobust(const size_t nStochasts, const std::vector<corrStruct>& probDb)
        {
            //
            // Fill the correlation matrix with default values
            //
            auto matrix = Matrix(nStochasts, nStochasts);
            auto full_correlation = Matrix(nStochasts, nStochasts);
            auto replace = std::vector<size_t>(nStochasts);
            auto sgn = std::vector<double>(nStochasts);
            independent = (int)nStochasts;

            for (size_t i = 0; i < nStochasts; i++)
            {
                matrix(i, i) = 1.0;
                full_correlation(i, i) = 1.0;
                replace[i] = i;
                sgn[i] = 1.0;
            }

            //
            // Now fill in the off-diagonal elements
            //
            for (auto c : probDb)  // loop over the entire set of correlation pairs in the list
            {
                size_t stochast1 = c.idx1;
                size_t stochast2 = c.idx2;
                size_t idx1 = min(stochast1, stochast2);  // force idx1 less than idx2
                size_t idx2 = max(stochast1, stochast2);

                double corr = c.correlation;

                if (idx1 >= 0 && idx1 < nStochasts && idx2 >= 0 && idx2 < nStochasts)
                {
                    if (fabs(corr) < 1.0)               // partial correlation: set in correlation matrix
                    {
                        matrix(idx2, idx1) = corr;
                        matrix(idx1, idx2) = corr;
                    }
                    else  // full correlation:
                    {
                        independent--;
                        for (size_t idx3 = 0; idx3 < nStochasts; idx3++)
                        {   // check ALL references in the array replace to the one we are about to replace, ...
                            if (replace[idx3] == idx2)          // .. col(1)
                            {
                                replace[idx3] = idx1;           // replace the reference into row(1)
                                sgn[idx3] = sgn[idx3] * corr;   // keep track of sign changes in the case of ANTI-correlations
                            }
                        }
                    }
                }
            }
            //
            // The array 'replace' is now the net effect of all required substitutions of the correlation pairs combined
            // The array 'sgn' is the corresponding net change of the sign.
            // Together, it tells us for index i the corresponding element is to be replaced by replace(i) with the sign sgn(i)
            // The fully_correlated matrix and the correlation matrix ('matrix') effectively eliminate redundant stochastics,
            // while preserving the size and indices of the original matrix
            //
            for (size_t idx3 = 0; idx3 < nStochasts; idx3++)          // loop over all elements i in the matrix again
            {
                if (replace[idx3] != idx3)                            // if element i is on the list for replacement by element j, update the full_correlation:
                {
                    auto idx_new = replace[idx3];
                    double corr = sgn[idx3];
                    full_correlation(idx3, idx3) = 0.0;                // set the diagonal value for element i to zero
                    full_correlation(idx3, idx_new) = corr;            // set the element [i,j] to 1 or -1, depending on the relation between elements i and j
                    for (size_t idx4 = 0; idx4 < nStochasts; idx4++)  // in the correlation matrix:
                    {
                        if (idx4 != idx3)                             // zero i-th row and column, except for the diagonal value:
                        {
                            matrix(idx3, idx4) = 0.0;
                            matrix(idx4, idx3) = 0.0;
                        }
                        else
                        {
                            matrix(idx3, idx4) = 1.0;                  // set THAT one to 1, if it wasn't already 1.
                        }
                    }
                }
            }

            matrix = matrix.CholeskyDecomposition();

            cholesky = full_correlation.matmul(matrix);
        }

        bool gaussianCorrelationRobust::isFullyCorrelated(const int i) const
        {
            return cholesky(i, i) == 0.0;
        }
    }
}
