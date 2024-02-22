#include "CorrelationMatrix.h"
#include <memory>
#include <algorithm>

namespace Deltares
{
    namespace Statistics
    {
        using namespace Deltares::ProbLibCore;

        std::vector<double> CorrelationMatrix::Cholesky(const std::vector<double>& uValues)
        {
            auto count = uValues.size();
            auto correlatedValues = std::vector<double>(count);
            if (dim == 0)
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
            dim = maxStochasts;
            matrix = Matrix(maxStochasts, maxStochasts);
            for (size_t i = 0; i < maxStochasts; i++)
            {
                matrix(i, i) = 1.0;
            }
        }

        void CorrelationMatrix::SetCorrelation(const int i, const int j, const double value)
        {
            if (std::max(i, j) >= dim)
            {
                throw probLibException("dimension mismatch in SetCorrelation");
            }
            matrix(i, j) = std::min(std::max(value, -1.0), 1.0);
            if (abs(value) < 1.0)
            {
                matrix(j, i) = value;
            }
            else
            {
                auto p = correlationPair({ i, j, value });
                // TODO check on already added
                fullCorrelatedCorrelations.push_back(p);
            }
        }

        void CorrelationMatrix::CholeskyDecomposition()
        {
            choleskyMatrix = matrix.CholeskyDecomposition();
        }

        bool CorrelationMatrix::checkFullyCorrelated(const int j) const
        {
            for (const auto& p : fullCorrelatedCorrelations)
            {
                if (p.index1 == j or p.index2 == j) return true;
            }
            return false;
        }

        void CorrelationMatrix::filter(const std::shared_ptr<CorrelationMatrix> m, const std::vector<int>& index)
        {
            for (size_t i = 0; i < dim; i++)
            {
                auto ii = findNewIndex(index, i);
                for (size_t j = 0; j < dim; j++)
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

        indexWithCorrelation CorrelationMatrix::findDependent(const int j) const
        {
            if (indexer.size() > 0)
            {
                return indexer[j];
            }

            for (size_t i = 0; i < dim; i++)
            {
                if (i != j && abs(matrix(i, j)) >= 1.0)
                {
                    return { (int)i, matrix(i,j) };
                }
            }
            return { -1, -999.0 };
        }

        bool CorrelationMatrix::IsIdentity() const
        {
            for (size_t i = 0; i < dim; i++)
            {
                for (size_t j = 0; j < dim; j++)
                {
                    if (i == j && matrix(i, j) != 1.0) return false;
                    if (i != j && matrix(i, j) != 0.0) return false;
                }
            }
            return true;
        }

        int CorrelationMatrix::CountCorrelations() const
        {
            int count = 0;
            for (size_t i = 0; i < dim; i++)
            {
                for (size_t j = i+1; j < dim; j++)
                {
                    if (matrix(i, j) != 0.0 || matrix(j, i) != 0.0) count++;
                }
            }
            return count;
        }

        correlationCheckResult CorrelationMatrix::checkFullyCorrelatedOneStochast(const size_t i) const
        {
            auto indexes = std::vector<correlationPair>();
            for (const auto & p : fullCorrelatedCorrelations)
            {
                if (p.index1 == i)
                {
                    indexes.push_back(p);
                }
                else if (p.index2 == i)
                {
                    indexes.push_back(p);
                }
            }
            if (indexes.size() > 1)
            {
                auto ii =  std::vector<int>();
                double product = 1.0;
                for (size_t j = 0; j < 2; j++)
                {
                    if (indexes[j].index1 == i)
                    {
                        ii.push_back(indexes[j].index2);
                    }
                    else
                    {
                        ii.push_back(indexes[j].index1);
                    }
                    product *= indexes[j].correlation;
                }
                for (const auto& p : fullCorrelatedCorrelations)
                {
                    if (std::find(ii.begin(), ii.end(), p.index1) != ii.end() &&
                        std::find(ii.begin(), ii.end(), p.index2) != ii.end())
                    {
                        return (abs(product - p.correlation) < 1e-6 ? correlationCheckResult::OK : correlationCheckResult::inconsistentCorrelation);
                    }
                }
                return correlationCheckResult::missingCorrelation;
            }
            return correlationCheckResult::OK;
        }

        bool CorrelationMatrix::HasConflictingCorrelations() const
        {
            if (fullCorrelatedCorrelations.size() > 0)
            {
                for (size_t i = 0; i < dim; i++)
                {
                    auto r = checkFullyCorrelatedOneStochast(i);
                    if (r != correlationCheckResult::OK) return true;
                }
            }
            return false;
        }
    }
}
