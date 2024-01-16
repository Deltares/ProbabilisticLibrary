#include "UConverter.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Reliability/StochastSettings.h"
#include "../Reliability/StochastSettingsSet.h"

UConverter::UConverter(std::vector<Stochast*> stochasts, CorrelationMatrix* correlationMatrix)
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

	for (int i = 0; i < this->stochasts.size(); i++)
	{
		if (this->stochasts[i]->isVarying())
		{
			this->varyingStochastIndex.push_back(this->varyingStochastIndex.size());
			this->varyingStochasts.push_back(this->stochasts[i]);
		}
		else
		{
			this->varyingStochastIndex.push_back(-1);
		}
	}

	varyingCorrelationMatrix = new CorrelationMatrix();
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
	settings->VaryingStochastCount = this->varyingStochasts.size();
	settings->VaryingStochastSettings = new Deltares::Reliability::StochastSettings * [settings->VaryingStochastCount];

	int j = 0;
	for (int i = 0; i < stochasts.size(); i++)
	{
		if (stochasts[i]->isVarying())
		{
			settings->VaryingStochastSettings[j] = i < settings->StochastCount ? settings->StochastSettings[i] : new Deltares::Reliability::StochastSettings();
			settings->VaryingStochastSettings[j]->StochastIndex = i;
			settings->VaryingStochastSettings[j]->IsQualitative = varyingStochasts[i]->isQualitative();
			settings->VaryingStochastSettings[j]->setStochast(varyingStochasts[i]);

			settings->VaryingStochastSettings[j]->initializeForRun();

			j++;
		}
	}

	settings->StochastCount = this->stochasts.size();
}


double* UConverter::getExpandedUValues(double* values)
{
	double* uValues = new double[this->stochasts.size()];

	for (int i = 0; i < this->stochasts.size(); i++)
	{
		const int varyingIndex = this->varyingStochastIndex[i];
		if (varyingIndex >= 0)
		{
			uValues[i] = values[varyingIndex];
		}
		else
		{
			uValues[i] = 0;
		}
	}

	return uValues;
}


double* UConverter::getXValues(Sample* sample)
{
	double* uCorrelated = varyingCorrelationMatrix->Cholesky(sample->Values, sample->getSize());

	double* expandedUValues = getExpandedUValues(uCorrelated);

	double* xValues = new double[this->stochasts.size()];

	for (int i = 0; i < this->stochasts.size(); i++)
	{
		xValues[i] = this->stochasts[i]->getXFromU(expandedUValues[i]);
	}

	delete[] expandedUValues;

	return xValues;
}

StochastPoint* UConverter::GetStochastPoint(double beta, double* alphas, int count)
{
	StochastPoint* realization = new StochastPoint();
	realization->Beta = beta;

	if (count > 0)
	{
		double* uValues = new double[count];
		for (int i = 0; i < count; i++)
		{
			uValues[i] = -beta * alphas[i];
		}

		double* uCorrelated = varyingCorrelationMatrix->Cholesky(uValues, count);

		double* alphaCorrelated = new double[count];
		for (int i = 0; i < count; i++)
		{
			alphaCorrelated[i] = -uCorrelated[i] / beta;
		}

		if (this->varyingStochasts.size() < this->stochasts.size())
		{
			uValues = getExpandedUValues(uValues);
			alphas = getExpandedUValues(alphas);
			uCorrelated = getExpandedUValues(uCorrelated);
			alphaCorrelated = getExpandedUValues(alphaCorrelated);
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


