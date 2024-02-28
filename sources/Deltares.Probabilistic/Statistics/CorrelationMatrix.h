#pragma once
#include <memory>

#include "../Math/matrix.h"
#include <vector>

namespace Deltares
{
    namespace Statistics
    {
        struct indexWithCorrelation
        {
            int index;
            double correlation;
        };

        class correlationPair
        {
        public:
            int index1;
            int index2;
            double correlation;
            bool isFullyCorrelated;
            bool AreLinked(correlationPair other)
            {
                return index1 == other.index1 || index1 == other.index2 || index2 == other.index1 || index2 == other.index2;
            }
        };

        enum class correlationCheckResult
        {
            OK,
            missingCorrelation,
            inconsistentCorrelation
        };

        class CorrelationMatrix
        {
        public:
            void init(const int maxStochasts);
            std::vector<double> Cholesky(const std::vector<double>& uValues);
            void SetCorrelation(const int i, const int j, double value);
            double GetCorrelation(const int i, const int j) const { return matrix(i, j); }
            bool IsIdentity() const;
            int CountCorrelations() const;
            bool HasConflictingCorrelations(bool onlyWithinStochasts = false) const;
            void resolveConflictingCorrelations(bool onlyWithinStochasts = false);
            void CholeskyDecomposition();
            bool checkFullyCorrelated(const int i) const;
            void filter(const std::shared_ptr<CorrelationMatrix> m, const std::vector<int>& index);
            indexWithCorrelation findDependent(const int i) const;
        private:
            Deltares::ProbLibCore::Matrix matrix = Deltares::ProbLibCore::Matrix(0, 0);
            Deltares::ProbLibCore::Matrix choleskyMatrix = Deltares::ProbLibCore::Matrix(0, 0);
            int findNewIndex(const std::vector<int> index, const size_t i);
            std::vector<indexWithCorrelation> indexer;
            std::vector<correlationPair> inputCorrelations;
            bool IsCheckedWithinStochasts(const bool checkWithinStochasts, const correlationPair & value) const;
            bool IsWithinStochasts(const correlationPair & value) const;
            std::vector<int> GetLinkingCorrelationStochasts(correlationPair correlation, correlationPair otherCorrelation) const;
            size_t dim = 0;
        };
    }
}

