#include <math.h>
#include "gaussianCorrelation.h"
#include "../utils/matrix.h"

namespace Deltares {
    namespace ProbLibCore {

        //> @file
        // Contains the module for handling correlations
        //<
        //>
        // Module for registering (basic) correlations between two stochasts
        //

        gaussianCorrelation::gaussianCorrelation(const int n, const std::vector<corrStruct>& pairs) : cholesky(n, n)
        {
            updateCorrelationMatrix(n, pairs);
        }

        void gaussianCorrelation::updateU(vector1D& u) const
        {
            u = cholesky.matvec(u);
        }

        int gaussianCorrelation::cntIndependent() const
        {
            return independent;
        }

        // Method to update the correlation matrix
        void gaussianCorrelation::updateCorrelationMatrix(const size_t nStochasts, const std::vector<corrStruct>& probDb)
        {
            //
            // Fill the correlation matrix with default values
            //
            auto matrix = Matrix(nStochasts, nStochasts);
            auto full_correlation = Matrix(nStochasts, nStochasts);
            independent = (int)nStochasts;
            for (size_t i = 0; i < nStochasts; i++)
            {
                matrix(i, i) = 1.0;
                full_correlation(i, i) = 1.0;
            }

            //
            // Now fill in the off-diagonal elements
            //
            for (auto c : probDb)
            {
                if (fabs(c.correlation) < 1.0)
                {
                    matrix(c.idx2, c.idx1) = c.correlation;
                    matrix(c.idx1, c.idx2) = c.correlation;
                }
                else
                {
                    full_correlation(c.idx2, c.idx1) = c.correlation;
                    full_correlation(c.idx2, c.idx2) = 0.0;
                    independent--;
                }
            }

            matrix = matrix.CholeskyDecomposition();

            cholesky = full_correlation.matmul(matrix);
        }

        bool gaussianCorrelation::isFullyCorrelated(const int i) const
        {
            return cholesky(i, i) == 0.0;
        }
    }
}
