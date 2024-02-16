#pragma once

#include <set>
#include <vector>

#include "Sample.h"
#include "StochastPoint.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Reliability/StochastSettingsSet.h"

namespace Deltares
{
	namespace Models
	{
		class UConverter
		{
		private:
			bool hasQualitiveStochasts = false;
			bool hasVariableStochasts = false;

			std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> stochasts;
			std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> varyingStochasts;
			std::vector<int> varyingStochastIndex;

			std::vector<int> variableStochastIndex; // reference of the stochast index to the variable source index
			std::vector<int> variableStochastList; // list of all stochast indices in order how they should be assigned

			std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix;
			std::shared_ptr<Statistics::CorrelationMatrix> varyingCorrelationMatrix;

			std::vector<double> getExpandedValues(const std::vector<double>& values);
			std::vector<double> getExpandedValues(const std::vector<double>& values, double defaultValue);

			bool checkFullyCorrelated(const int i);
			void updateDependedParameter(std::vector<double>& uValues, const int i);
			std::shared_ptr<Sample> getQualitativeExcludedSample(std::shared_ptr<Sample> sample);
			std::vector<int> getVariableStochastIndex();

		public:

			UConverter(std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> stochasts, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix);
			void initializeForRun();
			std::vector<double> getXValues(std::shared_ptr<Sample> sample);
			int getStochastCount();
			int getVaryingStochastCount();
			std::shared_ptr<StochastPoint> GetStochastPoint(std::shared_ptr<Sample> sample, double beta);
			void updateStochastSettings(std::shared_ptr<Deltares::Reliability::StochastSettingsSet> settings);
		};
	}
}

