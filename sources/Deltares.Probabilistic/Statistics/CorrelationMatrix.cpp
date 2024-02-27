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
            bool fully;
            if (abs(value) < 1.0)
            {
                matrix(j, i) = value;
                fully = false;
            }
            else
            {
                fully = true;
            }
            auto p = correlationPair({ i, j, value, true });
            // TODO check on already added
            inputCorrelations.push_back(p);
        }

        void CorrelationMatrix::CholeskyDecomposition()
        {
            choleskyMatrix = matrix.CholeskyDecomposition();
        }

        bool CorrelationMatrix::checkFullyCorrelated(const int j) const
        {
            for (const auto& p : inputCorrelations)
            {
                if ((p.index1 == j or p.index2 == j) and p.isFullyCorrelated) return true;
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
            m->inputCorrelations = inputCorrelations;
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

        /// <summary>
        /// Checks whether there are correlations which should be fully correlated due to other correlations
        /// </summary>
        /// <returns></returns>
        bool CorrelationMatrix::HasConflictingCorrelations(bool onlyWithinStochasts) const
        {
            for (int i = 0; i < inputCorrelations.size(); i++)
            {
                auto correlation = inputCorrelations[i];

                if (correlation.isFullyCorrelated && IsCheckedWithinStochasts(onlyWithinStochasts, correlation))
                {
                    for (int j = i + 1; j < inputCorrelations.size(); j++)
                    {
                        auto otherCorrelation = inputCorrelations[j];

                        if (otherCorrelation.isFullyCorrelated && IsCheckedWithinStochasts(onlyWithinStochasts, otherCorrelation) && correlation.AreLinked(otherCorrelation))
                        {
                            // find the stochasts which are not the linking values
                            auto nonConnectingStochasts = GetLinkingCorrelationStochasts(correlation, otherCorrelation);

                            double correlationValue = GetCorrelation(nonConnectingStochasts[0], nonConnectingStochasts[1]);
                            double expectedCorrelation = correlation.correlation * otherCorrelation.correlation;

                            if (correlationValue != expectedCorrelation)
                            {
                                return true;
                            }
                        }
                    }
                }
            }

            return false;
        }

        bool CorrelationMatrix::IsWithinStochasts(correlationPair value) const
        {
            bool found1 = false;
            bool found2 = false;
            for (const auto & s : inputCorrelations)
            {
                if (s.index1 == value.index1 || s.index2 == value.index1) found1 = true;
                if (s.index1 == value.index2 || s.index2 == value.index2) found2 = true;
            }
            return found1 && found2;
        }

        bool CorrelationMatrix::IsCheckedWithinStochasts(bool checkWithinStochasts, correlationPair value) const
        {
            if (!checkWithinStochasts)
            {
                return true;
            }
            else
            {
                return IsWithinStochasts(value);
            }
        }

        /// <summary>
        /// Gets the <see cref="IHasStochast"/>s which are not part of both <see cref="StochastCorrelationValue"/>s
        /// </summary>
        /// <param name="correlation"></param>
        /// <param name="otherCorrelation"></param>
        /// <returns></returns>
        std::vector<int> CorrelationMatrix::GetLinkingCorrelationStochasts(correlationPair correlation, correlationPair otherCorrelation) const
        {
            auto nonConnectingStochasts = std::vector<int>();
            for(auto & stochast :
                {
                    correlation.index1,
                    correlation.index2,
                    otherCorrelation.index1,
                    otherCorrelation.index2
                })
            {
                if (correlation.index1 != stochast && correlation.index2 != stochast)
                {
                    nonConnectingStochasts.push_back(stochast);
                }
                else if (otherCorrelation.index1 != stochast && otherCorrelation.index2 != stochast)
                {
                    nonConnectingStochasts.push_back(stochast);
                }
            }

            return nonConnectingStochasts;
        }

    }
}
