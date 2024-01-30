#include "UConverter.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Reliability/StochastSettings.h"
#include "../Reliability/StochastSettingsSet.h"

#include <memory>

UConverter::UConverter(std::vector<Deltares::Statistics::Stochast*> stochasts, CorrelationMatrix* correlationMatrix)
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

	for (Deltares::Statistics::Stochast* stochast : this->stochasts)
	{
		stochast->initializeForRun();
	}

	for (size_t i = 0; i < this->stochasts.size(); i++)
	{
		if (this->stochasts[i]->isVarying() && ! checkFullyCorrelated(i))
		{
			this->varyingStochastIndex.push_back(this->varyingStochastIndex.size());
			this->varyingStochasts.push_back(this->stochasts[i]);
		}
		else
		{
			int type = (this->stochasts[i]->isVarying() ? -1 : -2);
			this->varyingStochastIndex.push_back(type);
		}
	}

	if (stochasts.size() == varyingStochasts.size())
	{
		varyingCorrelationMatrix = correlationMatrix;
	}
	else
	{
		varyingCorrelationMatrix = new CorrelationMatrix();
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

void UConverter::updateStochastSettings(Deltares::Reliability::StochastSettingsSet* settings)
{
	for (size_t i = settings->getStochastCount(); i < stochasts.size(); i++)
	{
		Deltares::Reliability::StochastSettings* stochastSettings = new Deltares::Reliability::StochastSettings();
		settings->StochastSettings.push_back(stochastSettings);
	}

	settings->VaryingStochastSettings.clear();

	int j = 0;
	for (size_t i = 0; i < stochasts.size(); i++)
	{
		if (stochasts[i]->isVarying() && !checkFullyCorrelated(i))
		{
			Deltares::Reliability::StochastSettings* varyingStochastSettings = settings->StochastSettings[i];

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
	if (r.first >= 0)
	{
		uValues[i] = r.second * uValues[r.first];
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


std::vector<double> UConverter::getXValues(Sample* sample)
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

StochastPoint* UConverter::GetStochastPoint(double beta, std::vector<double> & alphas, int count)
{
	StochastPoint* realization = new StochastPoint();
	realization->Beta = beta;

	if (count > 0)
	{
		auto uValues = std::vector<double>(count);
		for (int i = 0; i < count; i++)
		{
			uValues[i] = -beta * alphas[i];
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

		//HashSet<StochastPointAlpha> hasDerivedValues = new HashSet<StochastPointAlpha>();

		for (int i = 0; i < this->stochasts.size(); i++)
		{
			StochastPointAlpha* alpha = new StochastPointAlpha();
			alpha->Stochast = this->stochasts[i];
			alpha->Alpha = alphas[i];
			alpha->AlphaCorrelated = alphaCorrelated[i];
			alpha->U = uValues[i];
			alpha->X = stochasts[i]->getXFromU(uCorrelated[i]);

			realization->Alphas.push_back(alpha);

			//if (stochasts[i].GetStochast() is RealizedStochast)
			//{
			//	hasDerivedValues.Add(stochastRealization);
			//}
		}
	}

	return realization;
}


