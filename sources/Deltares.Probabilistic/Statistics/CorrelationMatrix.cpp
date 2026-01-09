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
#include "CorrelationMatrix.h"
#include <memory>
#include <algorithm>
#include <cmath>

#include "../Math/MatrixSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        using namespace Deltares::Reliability;
        using namespace Deltares::Numeric;

        bool CorrelationMatrix::IsValid() const
        {
            auto report = Logging::ValidationReport();
            Validate(report);
            return report.isValid();
        }

        std::vector<double> CorrelationMatrix::ApplyCorrelation(const std::vector<double>& uValues)
        {
            auto count = uValues.size();
            auto correlatedValues = std::vector<double>(count);
            if (dim == 0)
            {
                for (size_t i = 0; i < count; i++)
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

                for (size_t i = 0; i < count; i++)
                {
                    correlatedValues[i] = uNew(i);
                }
            }

            return correlatedValues;
        }

        std::vector<double> CorrelationMatrix::InverseCholesky(const std::vector<double>& uValues)
        {
            auto count = uValues.size();
            auto inverseValues = std::vector<double>(count);

            if (dim == 0)
            {
                for (size_t i = 0; i < count; i++)
                {
                    inverseValues[i] = uValues[i];
                }
            }
            else
            {
                size_t c1; size_t c2;
                inverseCholeskyMatrix.get_dims(c1, c2);
                if (c1 == 0) InverseCholeskyDecomposition();

                auto u = vector1D(count);
                for (size_t i = 0; i < count; i++)
                {
                    u(i) = uValues[i];
                }

                auto uNew = inverseCholeskyMatrix.matvec(u);

                for (size_t i = 0; i < count; i++)
                {
                    inverseValues[i] = uNew(i);
                }
            }

            return inverseValues;
        }

        void CorrelationMatrix::Init(const int maxStochasts)
        {
            dim = maxStochasts;
            matrix = Matrix(maxStochasts, maxStochasts);
            for (int i = 0; i < maxStochasts; i++)
            {
                matrix(i, i) = 1.0;
            }
        }

        void CorrelationMatrix::Init(const std::vector<std::shared_ptr<Stochast>>& stochastList)
        {
            Init(static_cast<int>(stochastList.size()));

            for (size_t i = 0; i < stochastList.size(); i++)
            {
                stochasts.push_back(stochastList[i]);
                stochastIndex.insert({ stochastList[i], i });
            }
        }

        std::shared_ptr<Stochast> CorrelationMatrix::GetStochast(int index)
        {
            if (index < static_cast<int>(stochasts.size()))
            {
                return stochasts[index];
            }
            else
            {
                return nullptr;
            }
        }


        CorrelationValueAndType CorrelationMatrix::GetCorrelation(const int i, const int j) const
        {
            auto type = CorrelationType::Gaussian;
            return { matrix(i, j), type };
        }

        CorrelationValueAndType CorrelationMatrix::GetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2)
        {
            if (stochastIndex.contains(stochast1) && stochastIndex.contains(stochast2))
            {
                const int index1 = stochastIndex.at(stochast1);
                const int index2 = stochastIndex.at(stochast2);

                return GetCorrelation(index1, index2);
            }
            else
            {
                return { 0, CorrelationType::Gaussian };
            }
        }

        void CorrelationMatrix::SetCorrelation(const int i, const int j, double value, CorrelationType type)
        {
            if (std::max(i, j) >= static_cast<int>(dim))
            {
                throw probLibException("dimension mismatch in SetCorrelation");
            }

            value = std::min(std::max(value, -1.0), 1.0);
            matrix(i, j) = value;
            matrix(j, i) = value;
            bool fully = (std::fabs(value) == 1.0);
            for (auto& c : inputCorrelations)
            {
                if ((c.index1 == i && c.index2 == j) || (c.index1 == j && c.index2 == i))
                {
                    c.correlation = value;
                    c.isFullyCorrelated = fully;
                    return;
                }
            }
            auto p = correlationPair({ i, j, value, fully });
            inputCorrelations.push_back(p);
        }

        void CorrelationMatrix::SetCorrelation(const std::shared_ptr<Stochast>& stochast1, const std::shared_ptr<Stochast>& stochast2, double value, CorrelationType type)
        {
            if (stochastIndex.contains(stochast1) && stochastIndex.contains(stochast2))
            {
                const int index1 = stochastIndex.at(stochast1);
                const int index2 = stochastIndex.at(stochast2);

                SetCorrelation(index1, index2, value, type);
            }
        }

        bool CorrelationMatrix::hasFullyCorrelated() const
        {
            return std::ranges::any_of(inputCorrelations, [](const auto& corr) {return corr.isFullyCorrelated; });
        }

        void CorrelationMatrix::Validate(Logging::ValidationReport& report) const
        {
            if ( ! allow_validation) return;

            bool validation_result;
            std::string validation_message;
            if (hasFullyCorrelated())
            {
                validation_result = ! HasConflictingCorrelations();
                if ( ! validation_result)
                {
                    validation_message = "found conflicting correlations";
                }
            }
            else
            {
                try
                {
                    auto cholesky_matrix = matrix.CholeskyDecomposition();
                    validation_result = true;
                }
                catch (const probLibException& e)
                {
                    validation_result = false;
                    validation_message = e.what();
                }
            }

            if ( ! validation_result)
            {
                auto message = std::make_shared<Logging::Message>();
                message->Type = Logging::MessageType::Error;
                message->Text = validation_message;
                message->Subject = "Correlation Matrix";
                report.messages.push_back(message);
            }
        }

        void CorrelationMatrix::InitializeForRun()
        {
            CholeskyDecomposition();
            InverseCholeskyDecomposition();
        }

        void CorrelationMatrix::CholeskyDecomposition()
        {
            choleskyMatrix = matrix.CholeskyDecomposition();
        }

        void CorrelationMatrix::InverseCholeskyDecomposition()
        {
            inverseCholeskyMatrix = MatrixSupport::Inverse(&choleskyMatrix);
        }

        bool CorrelationMatrix::IsFullyCorrelated(const int index, const std::vector<int>& varyingIndices) const
        {
            if (dim == 0) return false;

            for (int varyingIndex : varyingIndices)
            {
                if (varyingIndex >= 0 && varyingIndex < index)
                {
                    if (fabs(matrix(varyingIndex, index)) >= 1.0 || fabs(matrix(index, varyingIndex)) >= 1.0)
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        void CorrelationMatrix::Filter(const std::shared_ptr<BaseCorrelation> source, const std::vector<int>& index)
        {
            if (source->GetDimension() == 0) return;
            auto corrM = std::dynamic_pointer_cast<CorrelationMatrix> (source);
            if (corrM == nullptr) throw probLibException("error casting a correlation matrix in filter method.");

            auto nrAllStochasts = index.size();

            for (size_t i = 0; i < nrAllStochasts; i++)
            {
                auto ii = FindNewIndex(index, i);
                for (size_t j = 0; j < nrAllStochasts; j++)
                {
                    auto jj = FindNewIndex(index, j);
                    if (index[i] >= 0 && index[j] >= 0)
                    {
                        SetCorrelation(ii, jj, corrM->matrix(i, j), CorrelationType::Gaussian);
                    }
                }
            }

            auto newIndexer = std::vector<IndexWithCorrelation>(nrAllStochasts);
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
                        auto dependent = source->FindDependent(static_cast<int>(ii));
                        if (dependent.index < 0) break;
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

        IndexWithCorrelation CorrelationMatrix::FindDependent(const int j) const
        {
            if (indexer.size() > 0)
            {
                return indexer[j];
            }

            for (int i = 0; i < j; i++)
            {
                double correlation = matrix(i, j);
                if (std::abs(correlation) < 1.0)
                {
                    correlation = matrix(j, i);
                }

                if (std::abs(correlation) >= 1.0)
                {
                    return { (int)i, correlation };
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
            return static_cast<int>(inputCorrelations.size());
        }

        /// <summary>
        /// Checks whether there are correlations which should be fully correlated due to other correlations
        /// </summary>
        /// <returns> true if it has conflicting correlations </returns>
        bool CorrelationMatrix::HasConflictingCorrelations() const
        {
            for (size_t i = 0; i < inputCorrelations.size(); i++)
            {
                auto correlation = inputCorrelations[i];

                if (correlation.isFullyCorrelated)
                {
                    for (size_t j = i + 1; j < inputCorrelations.size(); j++)
                    {
                        auto otherCorrelation = inputCorrelations[j];

                        if (otherCorrelation.isFullyCorrelated && correlation.AreLinked(otherCorrelation))
                        {
                            // find the stochasts which are not the linking values
                            auto nonConnectingStochasts = GetLinkingCorrelationStochasts(correlation, otherCorrelation);

                            auto correlationWithType = GetCorrelation(nonConnectingStochasts[0], nonConnectingStochasts[1]);
                            double expectedCorrelation = correlation.correlation * otherCorrelation.correlation;

                            if (correlationWithType.value != expectedCorrelation)
                            {
                                return true;
                            }
                        }
                    }
                }
            }

            return false;
        }

        /// <summary>
        /// Sets correlations to fully correlated if they should be so due to other correlations
        /// </summary>
        void CorrelationMatrix::resolveConflictingCorrelations()
        {
            bool modified = true;

            while (modified)
            {
                modified = false;

                for (size_t i = 0; i < inputCorrelations.size(); i++)
                {
                    auto correlation = inputCorrelations[i];

                    if (correlation.isFullyCorrelated)
                    {
                        for (size_t j = i + 1; j < inputCorrelations.size(); j++)
                        {
                            auto otherCorrelation = inputCorrelations[j];

                            if (otherCorrelation.isFullyCorrelated && correlation.AreLinked(otherCorrelation))
                            {
                                auto nonConnectingStochasts = GetLinkingCorrelationStochasts(correlation, otherCorrelation);

                                auto correlationWithType = GetCorrelation(nonConnectingStochasts[0], nonConnectingStochasts[1]);
                                double expectedCorrelation = correlation.correlation * otherCorrelation.correlation;

                                if (correlationWithType.value != expectedCorrelation)
                                {
                                    SetCorrelation(nonConnectingStochasts[0], nonConnectingStochasts[1], expectedCorrelation, CorrelationType::Gaussian);
                                    modified = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Gets the <see cref="IHasStochast"/>s which are not part of both <see cref="StochastCorrelationValue"/>s
        /// </summary>
        /// <param name="correlation"></param>
        /// <param name="otherCorrelation"></param>
        /// <returns></returns>
        std::vector<int> CorrelationMatrix::GetLinkingCorrelationStochasts(correlationPair correlation, correlationPair otherCorrelation)
        {
            auto nonConnectingStochasts = std::vector<int>();
            for (auto& stochast :
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

