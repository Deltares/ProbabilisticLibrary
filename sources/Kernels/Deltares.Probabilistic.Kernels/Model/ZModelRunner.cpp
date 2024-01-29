#include "ZModelRunner.h"
#include "DesignPoint.h"
#include "../Math/NumericSupport.h"
#include <stdarg.h>
#include <cmath>
#if __has_include(<format>)
#include <format>
#else
#include "../Utils/probLibString.h"
#endif

namespace Deltares
{
	namespace Models
	{
		int ZModelRunner::getVaryingStochastCount()
		{
			return this->uConverter->getVaryingStochastCount();
		}

		int ZModelRunner::getStochastCount()
		{
			return this->uConverter->getStochastCount();
		}

		void ZModelRunner::updateStochastSettings(Reliability::StochastSettingsSet* settings)
		{
			this->uConverter->updateStochastSettings(settings);
		}


		void ZModelRunner::initializeForRun()
		{
			this->uConverter->initializeForRun();
			this->zModel->setMaxProcesses(this->Settings->MaxParallelProcesses);
		}

		void ZModelRunner::clear()
		{
			this->reliabilityResults.clear();
			this->evaluations.clear();
			this->messages.clear();
		}

		Sample* ZModelRunner::getXSample(Sample* sample)
		{
			auto xValues = this->uConverter->getXValues(sample);

			// create a sample with values in x-space
			Sample* xSample = new Sample(xValues);

			xSample->AllowProxy = sample->AllowProxy;
			xSample->IterationIndex = sample->IterationIndex;
			xSample->ScenarioIndex = sample->ScenarioIndex;
			xSample->Weight = sample->Weight;

			return xSample;
		}

		double ZModelRunner::getZValue(Sample* sample)
		{
			Sample* xSample = getXSample(sample);

			this->zModel->invoke(xSample);

			registerEvaluation(xSample);

			sample->Z = xSample->Z;
			sample->Tag = xSample->Tag;

			delete xSample;

			return sample->Z;
		}

		double* ZModelRunner::getZValues(std::vector<Sample*> samples)
		{
			std::vector<Sample*> xSamples;

			for (int i = 0; i < samples.size(); i++)
			{
				xSamples.push_back(getXSample(samples[i]));
			}

			this->zModel->invoke(xSamples);

			double* zValues = new double[xSamples.size()];

			for (int i = 0; i < xSamples.size(); i++)
			{
				registerEvaluation(xSamples[i]);

				samples[i]->Z = xSamples[i]->Z;
				samples[i]->Tag = xSamples[i]->Tag;
				zValues[i] = xSamples[i]->Z;
			}

			for (size_t i = 0; i < xSamples.size(); i++)
			{
				delete(xSamples[i]);
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
				evaluation->X = NumericSupport::getCopy(sample->Values);
				evaluation->SizeX = sample->getSize();
				evaluation->Iteration = sample->IterationIndex;

				this->evaluations.push_back(evaluation);
			}
		}

		bool ZModelRunner::shouldExitPrematurely(double* zValues, double z0Fac, std::vector<Sample*> samples, double beta)
		{
			return false;
		}

		bool ZModelRunner::shouldExitPrematurely(bool final)
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

				if (report->ReportMatchesEvaluation && previousReport != nullptr)
				{
					ReliabilityResult* previousPreviousReport = this->reliabilityResults.size() > 1
						? this->reliabilityResults[this->reliabilityResults.size() - 2]
						: nullptr;

					// remove the last result
					if (!previousReport->IsMeaningful(previousPreviousReport, result))
					{
						this->reliabilityResults.pop_back();
					}
				}

				this->reliabilityResults.push_back(result);
			}

			if (this->progressIndicator != nullptr)
			{
				double progressIndicator = NumericSupport::Divide(report->Step, report->MaxSteps);

				double convergence = report->ConvBeta;
				if (std::isnan(convergence))
				{
					convergence = report->Variation;
				}

				this->progressIndicator->doProgress(progressIndicator);
				this->progressIndicator->doDetailedProgress(report->Step, report->Loop, report->Reliability, convergence);

#ifdef __cpp_lib_format
				auto text = std::format("{}/{}, Reliability = {:.3f}, Convergence = {:.3f}", report->Step, report->MaxSteps, report->Reliability, convergence);
#else
				auto pl = Deltares::ProbLibCore::probLibString();
				auto text = "Reliability = " + pl.double2str(report->Reliability);
#endif

				this->progressIndicator->doTextualProgress(ProgressType::Detailed, text);
			}
		}

		void ZModelRunner::reportMessage(MessageType type, std::string text)
		{
			if (Settings->SaveMessages && this->messages.size() < this->Settings->MaxMessages && type >= this->Settings->LowestMessageType)
			{
				this->messages.push_back(new Message(type, text));
			}
		}

		DesignPoint* ZModelRunner::getDesignPoint(double beta, std::vector<double> alpha, ConvergenceReport* convergenceReport, int scenarioIndex, std::string identifier)
		{
			int count = getVaryingStochastCount();

			StochastPoint* stochastPoint = uConverter->GetStochastPoint(beta, alpha, count);

			DesignPoint* designPoint = new DesignPoint();

			designPoint->Beta = stochastPoint->Beta;

			for (int i = 0; i < stochastPoint->Alphas.size(); i++)
			{
				designPoint->Alphas.push_back(stochastPoint->Alphas[i]);
			}

			designPoint->Identifier = identifier;
			designPoint->ScenarioIndex = scenarioIndex;
			designPoint->convergenceReport = convergenceReport;

			for (size_t i = 0; i < this->reliabilityResults.size(); i++)
			{
				designPoint->ReliabililityResults.push_back(this->reliabilityResults[i]);
			}

			for (size_t i = 0; i < this->evaluations.size(); i++)
			{
				designPoint->Evaluations.push_back(this->evaluations[i]);
			}

			for (size_t i = 0; i < this->messages.size(); i++)
			{
				designPoint->Messages.push_back(this->messages[i]);
			}

			return designPoint;
		}
	}
}



