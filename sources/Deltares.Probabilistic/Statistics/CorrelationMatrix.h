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

        struct correlationPair
        {
            int index1;
            int index2;
            double correlation;
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
            void SetCorrelation(const int i, const int j, const double value);
            double GetCorrelation(const int i, const int j) const { return matrix(i, j); }
            bool IsIdentity() const;
            int CountCorrelations() const;
            bool HasConflictingCorrelations() const;
            void CholeskyDecomposition();
            bool checkFullyCorrelated(const int i) const;
            void resolveConflictingCorrelations();
            void filter(const std::shared_ptr<CorrelationMatrix> m, const std::vector<int>& index);
            indexWithCorrelation findDependent(const int i) const;
        private:
            Deltares::ProbLibCore::Matrix matrix = Deltares::ProbLibCore::Matrix(0, 0);
            Deltares::ProbLibCore::Matrix choleskyMatrix = Deltares::ProbLibCore::Matrix(0, 0);
            int findNewIndex(const std::vector<int> index, const size_t i);
            std::vector<indexWithCorrelation> indexer;
            std::vector<correlationPair> fullCorrelatedCorrelations;
            correlationCheckResult checkFullyCorrelatedOneStochast(const size_t i) const;
            void fixInconsistent(size_t i);
            void fixMissing(size_t i);
            size_t dim = 0;
        };
    }
}

