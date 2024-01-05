#include "pch.h"
#include "ZModelRunner.h"
#include "DesignPoint.h"

int ZModelRunner::getVaryingStochastCount()
{
	return this->uConverter->getVaryingStochastCount();
}

double ZModelRunner::getZValue(Sample* sample)
{
	double* xValues = this->uConverter->getXValues(sample);

	sample->Z = this->zModel->getZValue(xValues);

	delete[] xValues;

	return sample->Z;

}

double* ZModelRunner::getZValues(std::vector<Sample*> samples)
{
	double* zValues = new double[samples.size()];

	for (int i = 0; i < samples.size(); i++)
	{
		zValues[i] = getZValue(samples[i]);
	}

	return zValues;
}

bool ZModelRunner::shouldExitPrematurely(double* zValues, double z0Fac, std::vector<Sample*> samples, double beta)
{
	return false;
}

void ZModelRunner::reportResult(ReliabilityReport* report)
{
	if (RunSettings->SaveConvergence)
	{
		bool hasPreviousReport = this->reliabilityResults.size() > 0;

		ReliabilityResult* previousReport = nullptr;
		if (hasPreviousReport)
		{
			previousReport = this->reliabilityResults.back();
		}

		ReliabilityResult* result = new ReliabilityResult();
		result->Reliability = report->Reliability;
		result->ConvBeta = report->ConvBeta;
		result->Variation = report->Variation;
		result->Contribution = report->Contribution;
		result->Index = hasPreviousReport ? previousReport->Index + 1 : 0;

		//if (report.ReportMatchesEvaluation)
		//	if (previousReport != null)
		//	{
		//		var previousPreviousReport = ReliabilityResults.Count > 1
		//			? ReliabilityResults[ReliabilityResults.Count - 2]
		//			: null;

		//		if (!previousReport.IsMeaningful(previousPreviousReport, result))
		//			ReliabilityResults.Remove(previousReport);
		//	}

		this->reliabilityResults.push_back(result);
	}

	//if (this.GetProgressIndicator() != null)
	//{
	//    var progressIndicator = NumericSupport.GetQuotient(report.Step, report.MaxSteps);

	//    var convergence = double.NaN;
	//    if (!double.IsNaN(report.ConvBeta))
	//        convergence = report.ConvBeta;
	//    else if (!double.IsNaN(report.Variation)) convergence = report.Variation;

	//    DoProgress(progressIndicator);
	//    DoDetailedProgress(report.Step, report.Loop, report.Reliability, convergence);
	//    DoTextualProgress(ProgressType.Detailed,
	//        $"{report.Step}/{report.MaxSteps}, Reliability = {report.Reliability:F3}, Convergence = {convergence:F3}");
	//}
}

DesignPoint* ZModelRunner::getRealization(double beta, double* alpha, ConvergenceReport* convergenceReport, int scenarioIndex, std::string identifier)
{
	int count = getVaryingStochastCount();

	StochastPoint* stochast_point = uConverter->GetStochastPoint(beta, alpha, count);

	DesignPoint* design_point = new DesignPoint();

	design_point->Beta = stochast_point->Beta;

	for (int i = 0; i < stochast_point->Alphas.size(); i++)
	{
		design_point->Alphas.push_back(stochast_point->Alphas[i]);
	}

	design_point->Identifier = identifier;
	design_point->ScenarioIndex = scenarioIndex;
	design_point->ConvergenceReport = convergenceReport;

	for (int i = 0; i < this->reliabilityResults.size(); i++)
	{
		design_point->ReliabililityResults.push_back(this->reliabilityResults[i]);
	}

	//realization.Evaluations.AddRange(Evaluations);
	//realization.Messages.AddRange(Messages);

	return design_point;
}



