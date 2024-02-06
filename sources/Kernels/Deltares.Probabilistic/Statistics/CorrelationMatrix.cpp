#include "CorrelationMatrix.h"
#include <memory>

namespace Deltares
{
    namespace Statistics
    {
        using namespace Deltares::ProbLibCore;

        std::vector<double> CorrelationMatrix::Cholesky(const std::vector<double>& uValues)
        {
            size_t m1; size_t m2;
            matrix.get_dims(m1, m2);
            auto count = uValues.size();
            auto correlatedValues = std::vector<double>(count);
            if (m1 == 0)
            {
                for (int i = 0; i < count; i++)
                {
                    correlatedValues[i] = uValues[i];
                }
            }
            else
            {
                size_t c1; size_t c2;
                choleskyMatrix.get_dims(c1, c2);
                if (c1 == 0) CholeskyDecomposition();

                auto u = vector1D(count);
                for (size_t i = 0; i < count; i++)
                {
                    u(i) = uValues[i];
                }

                auto uNew = choleskyMatrix.matvec(u);

                for (int i = 0; i < count; i++)
                {
                    correlatedValues[i] = uNew(i);
                }
            }

            return correlatedValues;
        }

        void CorrelationMatrix::init(const int maxStochasts)
        {
            matrix = Matrix(maxStochasts, maxStochasts);
            for (size_t i = 0; i < maxStochasts; i++)
            {
                matrix(i, i) = 1.0;
            }
        }

        void CorrelationMatrix::SetCorrelation(const int i, const int j, const double value)
        {
            size_t m1; size_t m2;
            matrix.get_dims(m1, m2);
            if (std::max(i, j) >= m1)
            {
                throw probLibException("dimension mismatch in SetCorrelation");
            }
            matrix(i, j) = std::min(std::max(value, -1.0), 1.0);
            if (abs(value) < 1.0)
            {
                matrix(j, i) = value;
            }
        }

        void CorrelationMatrix::CholeskyDecomposition()
        {
            choleskyMatrix = matrix.CholeskyDecomposition();
        }

        bool CorrelationMatrix::checkFullyCorrelated(const int j)
        {
            size_t m1; size_t m2;
            matrix.get_dims(m1, m2);
            for (size_t i = 0; i < m1; i++)
            {
                if (i != j)
                {
                    if (abs(matrix(i, j)) >= 1.0)
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        void CorrelationMatrix::filter(const std::shared_ptr<CorrelationMatrix> m, const std::vector<int>& index)
        {
            size_t m1; size_t m2;
            m->matrix.get_dims(m1, m2);
            for (size_t i = 0; i < m1; i++)
            {
                auto ii = findNewIndex(index, i);
                for (size_t j = 0; j < m1; j++)
                {
                    auto jj = findNewIndex(index, j);
                    if (index[i] >= 0 && index[j] >= 0)
                    {
                        matrix(ii, jj) = m->matrix(i, j);
                    }
                }
            }

            auto nrAllStochasts = index.size();
            auto newIndexer = std::vector<indexWithCorrelation>(nrAllStochasts);
            for (size_t i = 0; i < nrAllStochasts; i++)
            {
                if (index[i] == -2)
                {
                    newIndexer[i].index = -2;  // not varying stochast
                }
                else if (index[i] == -1)
                {
                    auto ii = i;
                    double correlation = 1.0;
                    for (;;)
                    {
                        auto dependent = m->findDependent(ii);
                        dependent.correlation *= correlation;
                        if (index[dependent.index] >= 0)
                        {
                            newIndexer[i] = dependent;
                            break;
                        }
                        ii = dependent.index;
                        correlation = dependent.correlation;
                    }
                }
            }
            indexer = newIndexer;
        }

        void CorrelationMatrix::resolveConflictingCorrelations()
        {
            // to be implemented
        }


        int CorrelationMatrix::findNewIndex(const std::vector<int> index, const size_t i)
        {
            int newIndex = 0;
            for (size_t j = 0; j <= i; j++)
            {
                if (index[j] >= 0) newIndex++;
            }
            return newIndex - 1;
        }

        indexWithCorrelation CorrelationMatrix::findDependent(const int j)
        {
            if (indexer.size() > 0)
            {
                return indexer[j];
            }

            size_t m1; size_t m2;
            matrix.get_dims(m1, m2);
            for (size_t i = 0; i < m2; i++)
            {
                if (i != j && abs(matrix(i, j)) >= 1.0)
                {
                    return { (int)i, matrix(i,j) };
                }
            }
            return { -1, -999.0 };
        }
    }
}
