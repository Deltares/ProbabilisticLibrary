#include "CrudeMonteCarlo.h"
#include <iostream>
#include <vector>
#include <cmath>

#include "../Statistics/StandardNormal.h"
#include "../Model/Sample.h"
#include "../Model/ConvergenceReport.h"
#include "../Model/ReliabilityReport.h"
#include "../Model/RandomGenerator.h"
#include "../Model/DesignPoint.h"
#include "DesignPointBuilder.h"

DesignPoint* CrudeMonteCarlo::getDesignPoint(ZModelRunner* modelRunner)
{
	//RemainingReliability remainingReliability = RemainingReliability();
	//PartialProbability remaining = remainingReliability.GetRemainingProbability(modelRunner);
	//return GetReducedDesignPoint(modelRunner, remaining.FailureProbability, 1 - remaining.Probability);

	return GetReducedDesignPoint(modelRunner, 0, 1);
}

DesignPoint* CrudeMonteCarlo::GetReducedDesignPoint(ZModelRunner* modelRunner, double qFail, double qRange)
{
	int nParameters = modelRunner->getVaryingStochastCount();
	double* zValues = new double[0]; // copy of z for all parallel threads as double
	DesignPointBuilder* uMean = new DesignPointBuilder(nParameters, Settings->DesignPointMethod, modelRunner);

	RandomGenerator* randomGenerator = new RandomGenerator();
	randomGenerator->Settings = this->Settings->RandomSettings;
	randomGenerator->initialize();

	Sample* uMin = new Sample(nParameters);
	double rmin = std::numeric_limits<double>::infinity();
	double pf = 0.0;
	bool initial = true;
	double z0Fac = 0;
	int nFailed = 0;
	int nSamples = 0;
	ConvergenceReport* convergenceReport = new ConvergenceReport();
	std::vector<Sample*> samples;
	int zIndex = 0;

	for (int sampleIndex = 0; sampleIndex < Settings->MaximumSamples + 1 && !isStopped(); sampleIndex++)
	{
		zIndex++;

		if (initial || zIndex >= samples.size())
		{
			samples.clear();
			int chunkSize = Settings->RunSettings->MaxChunkSize;
			int runs = std::min(chunkSize, Settings->MaximumSamples + 1 - sampleIndex);

			if (initial)
			{
				samples.push_back(new Sample(nParameters));
				runs = runs - 1;
			}

			for (int i = 0; i < runs; i++)
			{
				samples.push_back(randomGenerator->getRandomSample(nParameters));
			}

			zValues = modelRunner->getZValues(samples);

			if (initial)
			{
				z0Fac = getZFactor(zValues[0]);
				uMin->setBeta(z0Fac * StandardNormal::BetaMax);
				uMean->Initialize(z0Fac * StandardNormal::BetaMax);
			}

			if (modelRunner->shouldExitPrematurely(zValues, z0Fac, samples, rmin))
			{
				// return the result so far
				return GetRealizationFromP(modelRunner, pf, uMin, z0Fac, convergenceReport);
			}

			zIndex = 0;
		}

		if (initial)
		{
			z0Fac = zValues[zIndex] > 0.0 ? 1 : -1;
			initial = false;
			continue;
		}

		double z = zValues[zIndex];
		Sample* u = samples[zIndex];

		if (isnan(z))
		{
			continue;
		}

		nSamples++;
		if (z < 0)
		{
			nFailed = nFailed + 1;
		}

		convergenceReport->FailedSamples = nFailed;
		convergenceReport->FailFraction = NumericSupport::Divide(nFailed, nSamples);

		if (z * z0Fac < 0)
		{
			uMean->AddSample(u);
			double rbeta = u->getBeta();
			if (rbeta < rmin)
			{
				rmin = rbeta;
				uMin = u;
			}
		}
		pf = NumericSupport::Divide(nFailed, nSamples);
		pf = qFail + qRange * pf;
		if (CheckConvergence(modelRunner, pf, nSamples, sampleIndex))
		{
			break;
		}
	}

	double beta = StandardNormal::getUFromQ(pf);
	uMin = uMean->GetSample();

	double* alpha = GetAlphas(uMin, nParameters, z0Fac);
	convergenceReport->Convergence = GetConvergence(pf, nSamples);
	return modelRunner->getRealization(beta, alpha, convergenceReport, uMin->ScenarioIndex);
}

bool CrudeMonteCarlo::CheckConvergence(ZModelRunner* modelRunner, double pf, int samples, int nmaal)
{
	ReliabilityReport* report = new ReliabilityReport();
	report->Step = nmaal;
	report->MaxSteps = Settings->MaximumSamples;

	if (pf > 0 && pf < 1)
	{
		double convergence = GetConvergence(pf, samples);
		report->Reliability = StandardNormal::getUFromQ(pf);
		report->Variation = convergence;
		modelRunner->reportResult(report);
		bool enoughSamples = nmaal >= Settings->MinimumSamples;
		return enoughSamples && convergence < Settings->VariationCoefficient;
	}
	else
	{
		modelRunner->reportResult(report);
		return false;
	}
}

double CrudeMonteCarlo::GetConvergence(double pf, int samples)
{
	if (pf > 0 && pf < 1)
	{
		if (pf > 0.5)
		{
			pf = 1 - pf;
		}
		double varPf = sqrt((1 - pf) / (samples * pf));
		return varPf;
	}
	else
	{
		return nan("");
	}
}



