#include "ModelRunner.h"
#include "../Math/NumericSupport.h"
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
		int ModelRunner::getVaryingStochastCount()
		{
			return this->uConverter->getVaryingStochastCount();
		}

		int ModelRunner::getStochastCount()
		{
			return this->uConverter->getStochastCount();
		}

		void ModelRunner::updateStochastSettings(std::shared_ptr<Reliability::StochastSettingsSet> settings)
		{
			this->uConverter->updateStochastSettings(settings);
		}


		void ModelRunner::initializeForRun()
		{
			this->uConverter->initializeForRun();
			this->zModel->setMaxProcesses(this->Settings->MaxParallelProcesses);
		}

		void ModelRunner::clear()
		{
			this->reliabilityResults.clear();
			this->evaluations.clear();
			this->messages.clear();
		}

		std::shared_ptr<Sample> ModelRunner::getXSample(std::shared_ptr<Sample> sample)
		{
			auto xValues = this->uConverter->getXValues(sample);

			// create a sample with values in x-space
			std::shared_ptr<Sample> xSample = std::make_shared<Sample>(xValues);

			xSample->AllowProxy = sample->AllowProxy;
			xSample->IterationIndex = sample->IterationIndex;
			xSample->Weight = sample->Weight;

			return xSample;
		}

		double ModelRunner::getZValue(std::shared_ptr<Sample> sample)
		{
			std::shared_ptr<Sample> xSample = getXSample(sample);

			this->zModel->invoke(xSample);

			registerEvaluation(xSample);

			sample->Z = xSample->Z;
			sample->Tag = xSample->Tag;

			return sample->Z;
		}

		double* ModelRunner::getZValues(std::vector<std::shared_ptr<Sample>> samples)
		{
			std::vector<std::shared_ptr<Sample>> xSamples;

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

			return zValues;
		}

		void ModelRunner::registerEvaluation(std::shared_ptr<Sample> sample)
		{
			if (this->Settings->SaveEvaluations)
			{
				std::shared_ptr<Evaluation> evaluation = std::make_shared<Evaluation>();

				evaluation->Z = sample->Z;
				evaluation->Tag = sample->Tag;
				evaluation->X = NumericSupport::getCopy(sample->Values);
				evaluation->SizeX = sample->getSize();
				evaluation->Iteration = sample->IterationIndex;

				this->evaluations.push_back(evaluation);
			}
		}

		bool ModelRunner::shouldExitPrematurely(double* zValues, double z0Fac, std::vector<std::shared_ptr<Sample>> samples, double beta)
		{
			return false;
		}

		bool ModelRunner::shouldExitPrematurely(bool final)
		{
			return false;
		}

		void ModelRunner::reportResult(std::shared_ptr<Reliability::ReliabilityReport> report)
		{
			if (Settings->SaveConvergence)
			{
				bool hasPreviousReport = this->reliabilityResults.size() > 0;

				std::shared_ptr<Reliability::ReliabilityResult> previousReport = nullptr;
				if (hasPreviousReport)
				{
					previousReport = this->reliabilityResults.back();
				}

				std::shared_ptr<Reliability::ReliabilityResult> result = std::make_shared<Reliability::ReliabilityResult>();
				result->Reliability = report->Reliability;
				result->ConvBeta = report->ConvBeta;
				result->Variation = report->Variation;
				result->Contribution = report->Contribution;
				result->Index = hasPreviousReport ? previousReport->Index + 1 : 0;

				if (report->ReportMatchesEvaluation && previousReport != nullptr)
				{
					std::shared_ptr<Reliability::ReliabilityResult> previousPreviousReport = this->reliabilityResults.size() > 1
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
				const double progressIndicator = NumericSupport::Divide(report->Step, report->MaxSteps);

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

		void ModelRunner::reportMessage(MessageType type, std::string text)
		{
			if (Settings->SaveMessages && this->messages.size() < this->Settings->MaxMessages && type >= this->Settings->LowestMessageType)
			{
				this->messages.push_back(std::make_shared<Message>(type, text));
			}
		}

		std::shared_ptr<Reliability::DesignPoint> ModelRunner::getDesignPoint(std::shared_ptr<Sample> sample, double beta, std::shared_ptr<Reliability::ConvergenceReport> convergenceReport, std::string identifier)
		{
			std::shared_ptr<StochastPoint> stochastPoint = uConverter->GetStochastPoint(sample, beta);

			std::shared_ptr<Reliability::DesignPoint> designPoint = std::make_shared<Reliability::DesignPoint>();

			designPoint->Beta = stochastPoint->Beta;

			for (size_t i = 0; i < stochastPoint->Alphas.size(); i++)
			{
				designPoint->Alphas.push_back(stochastPoint->Alphas[i]);
			}

			designPoint->Identifier = identifier;
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



