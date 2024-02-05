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
	namespace Statistics
	{
		class VariableStochast;
	}
}

UConverter::UConverter(std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> stochasts, std::shared_ptr<CorrelationMatrix> correlationMatrix)
{
	this->stochasts.clear();

	for (int i = 0; i < stochasts.size(); i++)
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
		if (this->stochasts[i]->isVarying() && ! checkFullyCorrelated(i))
		{
			this->varyingStochastIndex.push_back(this->varyingStochastIndex.size());
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

	if (stochasts.size() == varyingStochasts.size())
	{
		varyingCorrelationMatrix = correlationMatrix;
	}
	else
	{
		varyingCorrelationMatrix = std::make_shared<CorrelationMatrix>();
		varyingCorrelationMatrix->init(varyingStochasts.size());
		varyingCorrelationMatrix->filter(correlationMatrix, varyingStochastIndex);
	}
	varyingCorrelationMatrix->CholeskyDecomposition();
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

void UConverter::updateDependedParameter(std::vector<double> & uValues, const int i)
{
	auto r = varyingCorrelationMatrix->findDependent(i);
	if (r.index >= 0)
	{
		uValues[i] = r.correlation * uValues[r.index];
	}
}

std::vector<double> UConverter::getExpandedValues(const std::vector<double> & values)
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

std::vector<double> UConverter::getExpandedValues(const std::vector<double> & values, double defaultValue)
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


std::vector<double> UConverter::getXValues(std::shared_ptr<Sample> sample)
{
	auto uCorrelated = varyingCorrelationMatrix->Cholesky(sample->Values);

	auto expandedUValues = getExpandedValues(uCorrelated);

	auto xValues = std::vector<double>(this->stochasts.size());

	for (int i = 0; i < this->stochasts.size(); i++)
	{
		xValues[i] = this->stochasts[i]->getXFromU(expandedUValues[i]);
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

	return qualitativeExcludedSample->normalize(sample->getBeta());
}

void UConverter::assignVariableStochasts(std::shared_ptr<StochastPoint> realization, std::vector<double>& uCorrelated)
{
	std::set<int> assignedAlphas;
	std::vector<int> unAssignedAlphas;

	// build up initial administration which stochasts have been assigned and which not
	for (int i = 0; i < realization->Alphas.size(); i++)
	{
		if (!realization->Alphas[i]->Stochast->IsVariableStochast)
		{
			assignedAlphas.insert(i);
		}
		else
		{
			unAssignedAlphas.push_back(i);
		}
	}

	bool ready = unAssignedAlphas.size() == 0;

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
					double xSource = realization->Alphas[sourceIndex]->X;
					realization->Alphas[alphaIndex]->X = stochasts[i]->getXFromUAndSource(xSource, uCorrelated[alphaIndex]);
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
}

std::shared_ptr<StochastPoint> UConverter::GetStochastPoint(std::shared_ptr<Sample> sample, double beta)
{
	std::shared_ptr<StochastPoint> realization = std::make_shared<StochastPoint>();
	realization->Beta = beta;

	std::shared_ptr<Sample> betaSample = sample->normalize(abs(beta));

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

		// correct for realized stochasts
		if (this->hasVariableStochasts)
		{
			assignVariableStochasts(realization, uCorrelated);
		}
	}

	return realization;
}


