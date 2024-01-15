#include "ZModelRunner.h"
#include "DesignPoint.h"
#include "../Math/NumericSupport.h"
#include <stdarg.h>
#include <format>

namespace Deltares
{
	namespace Models
	{
		int ZModelRunner::getVaryingStochastCount()
		{
			return this->uConverter->getVaryingStochastCount();
		}

		void ZModelRunner::updateStochastSettings(Reliability::StochastListSettings* settings)
		{
			this->uConverter->updateStochastSettings(settings);
		}


		void ZModelRunner::initializeForRun()
		{
			this->uConverter->initializeForRun();
			this->zModel->setMaxProcesses(this->Settings->MaxParallelProcesses);
		}

		double ZModelRunner::getZValue(Sample* sample)
		{
			sample->XValues = this->uConverter->getXValues(sample);

			this->zModel->invoke(sample);

			registerEvaluation(sample);

			return sample->Z;

		}

		double* ZModelRunner::getZValues(std::vector<Sample*> samples)
		{
			for (int i = 0; i < samples.size(); i++)
			{
				samples[i]->XValues = this->uConverter->getXValues(samples[i]);
			}

			this->zModel->invoke(samples);

			double* zValues = new double[samples.size()];

			for (int i = 0; i < samples.size(); i++)
			{
				registerEvaluation(samples[i]);
				zValues[i] = samples[i]->Z;
			}

			return zValues;
		}

		void ZModelRunner::registerEvaluation(Sample* sample)
		{
			if (this->Settings->SaveEvaluations)
			{
				Evaluation* evaluation = new Evaluation();

				evaluation->Z = sample->Z;
				evaluation->Tag = sample->Tag;
				evaluation->X = sample->XValues;
				evaluation->SizeX = sample->getSize();
				evaluation->Iteration = sample->IterationIndex;

				this->evaluations.push_back(evaluation);
			}
		}

		bool ZModelRunner::shouldExitPrematurely(double* zValues, double z0Fac, std::vector<Sample*> samples, double beta)
		{
			return false;
		}

		void ZModelRunner::reportResult(ReliabilityReport* report)
		{
			if (Settings->SaveConvergence)
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

			if (this->progressIndicator != nullptr)
			{
				double progressIndicator = NumericSupport::Divide(report->Step, report->MaxSteps);

				double convergence = report->ConvBeta;
				if (isnan(convergence))
				{
					convergence = report->Variation;
				}

				this->progressIndicator->doProgress(progressIndicator);
				this->progressIndicator->doDetailedProgress(report->Step, report->Loop, report->Reliability, convergence);

				auto text = std::format("{}/{}, Reliability = {:.3f}, Convergence = {:.3f}", report->Step, report->MaxSteps, report->Reliability, convergence);

				this->progressIndicator->doTextualProgress(ProgressType::Detailed, text);
			}
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

			for (int i = 0; i < this->evaluations.size(); i++)
			{
				design_point->Evaluations.push_back(this->evaluations[i]);
			}

			//realization.Messages.AddRange(Messages);

			return design_point;
		}
	}
}



