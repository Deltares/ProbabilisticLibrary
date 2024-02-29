#include "UConverter.h"

#include <map>

#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Reliability/StochastSettings.h"
#include "../Reliability/StochastSettingsSet.h"

#include <memory>
#include <set>


namespace Deltares
{
	namespace Models
	{
		UConverter::UConverter(std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> stochasts, std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix)
		{
			id = ++UConverter::counter;

			this->stochasts.clear();

			for (size_t i = 0; i < stochasts.size(); i++)
			{
				this->stochasts.push_back(stochasts[i]);
			}

			this->correlationMatrix = correlationMatrix;
		}


		void UConverter::initializeForRun()
		{
			this->varyingStochasts.clear();
			this->varyingStochastIndex.clear();
			this->variableStochastIndex.clear();

			this->hasQualitiveStochasts = false;
			this->hasVariableStochasts = false;

			for (std::shared_ptr<Deltares::Statistics::Stochast> stochast : this->stochasts)
			{
				stochast->initializeForRun();
			}

			for (size_t i = 0; i < this->stochasts.size(); i++)
			{
				if (this->stochasts[i]->isVarying() && !checkFullyCorrelated(i))
				{
					this->varyingStochastIndex.push_back(i);
					this->varyingStochasts.push_back(this->stochasts[i]);
					this->hasQualitiveStochasts |= this->stochasts[i]->isQualitative();
				}
				else
				{
					int type = (this->stochasts[i]->isVarying() ? -1 : -2);
					this->varyingStochastIndex.push_back(type);
				}

				variableStochastIndex.push_back(-1);

				if (this->stochasts[i]->IsVariableStochast)
				{
					this->hasVariableStochasts = true;
					for (size_t j = 0; j < this->stochasts.size(); j++)
					{
						if (stochasts[j] == this->stochasts[i]->VariableSource)
						{
							variableStochastIndex[i] = j;
							break;
						}
					}
				}
			}

			if (this->hasVariableStochasts)
			{
				this->variableStochastList = getVariableStochastIndex();
			}

			if (stochasts.size() == varyingStochasts.size())
			{
				varyingCorrelationMatrix = correlationMatrix;
			}
			else
			{
				varyingCorrelationMatrix = std::make_shared<Statistics::CorrelationMatrix>();
				varyingCorrelationMatrix->init(varyingStochasts.size());
				varyingCorrelationMatrix->filter(correlationMatrix, varyingStochastIndex);
			}
			varyingCorrelationMatrix->CholeskyDecomposition();
		}

		bool UConverter::isVaryingStochast(int index)
		{
			return this->varyingStochastIndex[index] >= 0;
		}

		std::vector<double> UConverter::getVaryingValues(std::vector<double> values)
		{
			std::vector<double> varyingValues;

			for (size_t i = 0; i < this->stochasts.size(); i++)
			{
				if (varyingStochastIndex[i] >= 0)
				{
					varyingValues.push_back(values[i]);
				}
			}

			return varyingValues;
		}


		bool UConverter::checkFullyCorrelated(const int i)
		{
			return correlationMatrix->checkFullyCorrelated(i);
		}

		int UConverter::getStochastCount()
		{
			return this->stochasts.size();
		}

		int UConverter::getVaryingStochastCount()
		{
			return this->varyingStochasts.size();
		}

		void UConverter::updateStochastSettings(std::shared_ptr<Deltares::Reliability::StochastSettingsSet> settings)
		{
			for (size_t i = settings->getStochastCount(); i < stochasts.size(); i++)
			{
				std::shared_ptr<Deltares::Reliability::StochastSettings> stochastSettings = std::make_shared<Deltares::Reliability::StochastSettings>();
				settings->StochastSettings.push_back(stochastSettings);
			}

			settings->VaryingStochastSettings.clear();

			int j = 0;
			for (size_t i = 0; i < stochasts.size(); i++)
			{
				if (stochasts[i]->isVarying() && !checkFullyCorrelated(i))
				{
					std::shared_ptr<Deltares::Reliability::StochastSettings> varyingStochastSettings = settings->StochastSettings[i];

					varyingStochastSettings->StochastIndex = i;
					varyingStochastSettings->IsQualitative = varyingStochasts[j]->isQualitative();
					varyingStochastSettings->setStochast(varyingStochasts[j]);

					varyingStochastSettings->initializeForRun();

					settings->VaryingStochastSettings.push_back(varyingStochastSettings);

					j++;
				}
			}
		}

		void UConverter::updateDependedParameter(std::vector<double>& uValues, const int i)
		{
			auto r = varyingCorrelationMatrix->findDependent(i);
			if (r.index >= 0)
			{
				uValues[i] = r.correlation * uValues[r.index];
			}
		}

		std::vector<double> UConverter::getExpandedValues(const std::vector<double>& values)
		{
			auto uValues = std::vector<double>(this->stochasts.size());
			int ii = 0;
			for (int i = 0; i < this->stochasts.size(); i++)
			{
				const int varyingIndex = this->varyingStochastIndex[i];
				if (varyingIndex >= 0)
				{
					uValues[i] = values[ii];
					ii++;
				}
			}

			for (int i = 0; i < this->stochasts.size(); i++)
			{
				const int varyingIndex = this->varyingStochastIndex[i];
				if (varyingIndex < 0)
				{
					updateDependedParameter(uValues, i);
				}
			}

			return uValues;
		}

		std::vector<double> UConverter::getExpandedValues(const std::vector<double>& values, double defaultValue)
		{
			auto uValues = std::vector<double>(this->stochasts.size());
			int ii = 0;
			for (int i = 0; i < this->stochasts.size(); i++)
			{
				const int varyingIndex = this->varyingStochastIndex[i];
				if (varyingIndex >= 0)
				{
					uValues[i] = values[ii];
					ii++;
				}
				else
				{
					uValues[i] = 0;
				}
			}

			return uValues;
		}

		std::vector<double> UConverter::getUValues(std::shared_ptr<Sample> sample)
		{
			return varyingCorrelationMatrix->Cholesky(sample->Values);
		}

		std::vector<double> UConverter::getXValues(std::shared_ptr<Sample> sample)
		{
			std::vector<double> unexpandedUValues = sample->Values;

			if (sample->Values.size() > varyingStochasts.size())
			{
				unexpandedUValues.clear();
				for (size_t i = 0; i < varyingStochasts.size(); i++)
				{
					unexpandedUValues.push_back(sample->Values[i]);
				}
			}

			auto uCorrelated = varyingCorrelationMatrix->Cholesky(unexpandedUValues);

			auto expandedUValues = getExpandedValues(uCorrelated);

			auto xValues = std::vector<double>(this->stochasts.size());

			for (size_t i = 0; i < this->stochasts.size(); i++)
			{
				if (!this->hasVariableStochasts || !stochasts[i]->IsVariableStochast)
				{
					xValues[i] = this->stochasts[i]->getXFromU(expandedUValues[i]);
				}
			}

			if (this->hasVariableStochasts)
			{
				for (int i = 0; i < variableStochastList.size(); i++)
				{
					int stochastIndex = variableStochastList[i];
					int sourceIndex = variableStochastIndex[stochastIndex];

					double xSource = xValues[sourceIndex];
					xValues[stochastIndex] = stochasts[stochastIndex]->getXFromUAndSource(xSource, expandedUValues[stochastIndex]);
				}
			}

			return xValues;
		}

		std::shared_ptr<Sample> UConverter::getQualitativeExcludedSample(std::shared_ptr<Sample> sample)
		{
			std::shared_ptr<Sample> qualitativeExcludedSample = sample->clone();

			for (int i = 0; i < sample->Values.size(); i++)
			{
				if (this->varyingStochasts[i]->isQualitative())
				{
					qualitativeExcludedSample->Values[i] = 0;
				}
			}

			return qualitativeExcludedSample->getSampleAtBeta(sample->getBeta());
		}

		/**
		 * \brief Gets the indices of the variable stochasts in order of assignment
		 */
		std::vector<int> UConverter::getVariableStochastIndex()
		{
			std::vector<int> newVariableStochastList;

			std::set<int> assignedAlphas;
			std::vector<int> unAssignedAlphas;

			// build up initial administration which stochasts have been assigned and which not
			for (int i = 0; i < this->stochasts.size(); i++)
			{
				if (!this->stochasts[i]->IsVariableStochast)
				{
					assignedAlphas.insert(i);
				}
				else
				{
					unAssignedAlphas.push_back(i);
				}
			}

			bool ready = unAssignedAlphas.empty();

			// loop until all stochasts have been assigned
			while (!ready)
			{
				ready = true;
				bool modified = false;

				for (int i = 0; i < unAssignedAlphas.size(); i++)
				{
					int alphaIndex = unAssignedAlphas[i];

					if (!assignedAlphas.contains(alphaIndex))
					{
						int sourceIndex = variableStochastIndex[alphaIndex];
						if (assignedAlphas.contains(sourceIndex))
						{
							newVariableStochastList.push_back(alphaIndex);

							assignedAlphas.insert(alphaIndex);
							modified = true;
						}
						else
						{
							ready = false;
						}
					}
				}

				// when no progress and not everything has been assigned => error
				if (!ready && !modified)
				{
					throw new std::exception("circular reference");
				}
			}

			return newVariableStochastList;
		}

		std::shared_ptr<StochastPoint> UConverter::GetStochastPoint(std::shared_ptr<Sample> sample, double beta)
		{
			std::shared_ptr<StochastPoint> realization = std::make_shared<StochastPoint>();
			realization->Beta = beta;

			std::shared_ptr<Sample> betaSample = sample->getSampleAtBeta(abs(beta));

			if (this->hasQualitiveStochasts)
			{
				betaSample = getQualitativeExcludedSample(betaSample);
			}

			const int count = sample->getSize();

			if (count > 0)
			{
				std::vector<double> alphas = std::vector<double>(count);

				auto uValues = std::vector<double>(count);
				for (int i = 0; i < count; i++)
				{
					uValues[i] = betaSample->Values[i]; // - beta * alphas[i];
					alphas[i] = -uValues[i] / beta;
				}

				auto uCorrelated = varyingCorrelationMatrix->Cholesky(uValues);

				auto alphaCorrelated = std::vector<double>(count);
				for (int i = 0; i < count; i++)
				{
					alphaCorrelated[i] = -uCorrelated[i] / beta;
				}

				if (this->varyingStochasts.size() < this->stochasts.size())
				{
					uValues = getExpandedValues(uValues, 0);
					alphas = getExpandedValues(alphas, 0);
					uCorrelated = getExpandedValues(uCorrelated);
					alphaCorrelated = getExpandedValues(alphaCorrelated);
				}

				for (size_t i = 0; i < this->stochasts.size(); i++)
				{
					std::shared_ptr<StochastPointAlpha> alpha = std::make_shared<StochastPointAlpha>();
					alpha->Stochast = this->stochasts[i];
					alpha->Alpha = alphas[i];
					alpha->AlphaCorrelated = alphaCorrelated[i];
					alpha->U = uValues[i];

					if (this->hasQualitiveStochasts && this->stochasts[i]->isQualitative())
					{
						int varyingIndex = this->varyingStochastIndex[i];
						alpha->X = stochasts[i]->getXFromU(sample->Values[varyingIndex]);
					}
					else if (!this->hasVariableStochasts || !stochasts[i]->IsVariableStochast)
					{
						alpha->X = stochasts[i]->getXFromU(uCorrelated[i]);
					}

					realization->Alphas.push_back(alpha);
				}

				// assign variable stochasts
				if (this->hasVariableStochasts)
				{
					for (int i = 0; i < variableStochastList.size(); i++)
					{
						int stochastIndex = variableStochastList[i];
						int sourceIndex = variableStochastIndex[stochastIndex];

						double xSource = realization->Alphas[sourceIndex]->X;
						realization->Alphas[stochastIndex]->X = stochasts[stochastIndex]->getXFromUAndSource(xSource, uCorrelated[stochastIndex]);
					}
				}
			}

			return realization;
		}
	}
}


