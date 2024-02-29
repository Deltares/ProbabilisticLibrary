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

		bool CorrelationMatrix::checkFullyCorrelated(const int j) const
		{
			size_t m1; size_t m2;
			matrix.get_dims(m1, m2);
			for (size_t i = 0; i < j; i++)
			{
				if (abs(matrix(i, j)) >= 1.0 || abs(matrix(j, i)) >= 1.0)
				{
					return true;
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
			if (index[i] == -1)
			{
				return -1;
			}
			else 
			{
				int newIndex = 0;
				for (size_t j = 0; j < i; j++)
				{
					if (index[j] >= 0) newIndex++;
				}

				return newIndex;
			}
		}

		indexWithCorrelation CorrelationMatrix::findDependent(const int j) const
		{
			if (indexer.size() > 0)
			{
				return indexer[j];
			}

			size_t m1; size_t m2;
			matrix.get_dims(m1, m2);
			for (size_t i = 0; i < j; i++)
			{
				double correlation = matrix(i, j);
				if (abs(correlation) < 1.0)
				{
					correlation = matrix(j, i);
				}

				if (abs(correlation) >= 1.0)
				{
					return { (int)i, correlation };
				}
			}

			return { -1, -999.0 };
		}

		bool CorrelationMatrix::IsIdentity() const
		{
			size_t m1; size_t m2;
			matrix.get_dims(m1, m2);
			for (size_t i = 0; i < m1; i++)
			{
				for (size_t j = 0; j < m2; j++)
				{
					if (i == j && matrix(i, j) != 1.0) return false;
					if (i != j && matrix(i, j) != 0.0) return false;
				}
			}
			return true;
		}

		int CorrelationMatrix::CountCorrelations() const
		{
			size_t m1; size_t m2;
			matrix.get_dims(m1, m2);
			int count = 0;
			for (size_t i = 0; i < m1; i++)
			{
				for (size_t j = i + 1; j < m2; j++)
				{
					if (matrix(i, j) != 0.0 || matrix(j, i) != 0.0) count++;
				}
			}
			return count;
		}

		correlationCheckResult checkFullyCorrelatedB(const size_t i, const std::vector<correlationPair>& pairs)
		{
			auto indexes = std::vector<correlationPair>();
			for (const auto& p : pairs)
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
				auto ii = std::vector<int>();
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
				for (const auto& p : pairs)
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
			auto full = std::vector<correlationPair>();
			size_t m1; size_t m2;
			matrix.get_dims(m1, m2);
			for (size_t i = 0; i < m1; i++)
			{
				for (size_t j = 0; j < m2; j++)
				{
					if (i != j && std::abs(matrix(i, j)) == 1.0)
					{
						correlationPair pair;
						pair.index1 = i;
						pair.index2 = j;
						pair.correlation = matrix(i, j);
						full.push_back(pair);
					}
				}
			}
			for (size_t i = 0; i < m1; i++)
			{
				auto r = checkFullyCorrelatedB(i, full);
				if (r != correlationCheckResult::OK) return true;
			}

			return false;
		}
	}
}
