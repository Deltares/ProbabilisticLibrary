#include "ModelRunner.h"
#include "../Math/NumericSupport.h"
#include <cmath>

#include "ModelSample.h"
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

		bool ModelRunner::isVaryingStochast(int index)
		{
			return this->uConverter->isVaryingStochast(index);
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

		std::shared_ptr<ModelSample> ModelRunner::getModelSample(std::shared_ptr<Sample> sample)
		{
			std::vector<double> xValues = this->uConverter->getXValues(sample);

			// create a sample with values in x-space
			std::shared_ptr<ModelSample> xSample = std::make_shared<ModelSample>(xValues);

			xSample->AllowProxy = sample->AllowProxy;
			xSample->IterationIndex = sample->IterationIndex;
			xSample->Weight = sample->Weight;
			xSample->IsRestartRequired = sample->IsRestartRequired;
			xSample->Beta = sample->getBeta();

			return xSample;
		}

		double ModelRunner::getZValue(std::shared_ptr<Sample> sample)
		{
			std::shared_ptr<ModelSample> xSample = getModelSample(sample);

			this->zModel->invoke(xSample);

			registerEvaluation(xSample);

			sample->Z = xSample->Z;

			return sample->Z;
		}

		std::vector<double> ModelRunner::getZValues(std::vector<std::shared_ptr<Sample>> samples)
		{
			std::vector<std::shared_ptr<ModelSample>> xSamples;

			for (size_t i = 0; i < samples.size(); i++)
			{
				xSamples.push_back(getModelSample(samples[i]));
			}

			this->zModel->invoke(xSamples);

			std::vector<double> zValues(xSamples.size());

			for (size_t i = 0; i < xSamples.size(); i++)
			{
				registerEvaluation(xSamples[i]);

				samples[i]->Z = xSamples[i]->Z;
				samples[i]->AllowProxy = xSamples[i]->AllowProxy;
				samples[i]->IsRestartRequired = xSamples[i]->IsRestartRequired;
				zValues[i] = xSamples[i]->Z;
			}

			return zValues;
		}

		void ModelRunner::registerEvaluation(std::shared_ptr<ModelSample> sample)
		{
			if (this->Settings->SaveEvaluations)
			{
				std::shared_ptr<Evaluation> evaluation = std::make_shared<Evaluation>();

				evaluation->Z = sample->Z;
				evaluation->X = sample->Values;
				evaluation->Iteration = sample->IterationIndex;
				evaluation->Tag = sample->Tag;

				this->evaluations.push_back(evaluation);
			}
		}

		bool ModelRunner::shouldExitPrematurely(std::vector<std::shared_ptr<Sample>> samples)
		{
			for (std::shared_ptr<Sample> sample : samples)
			{
				if (sample->IsRestartRequired)
				{
					return true;
				}
			}

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
				const double progressIndicator = Numeric::NumericSupport::Divide(report->Step, report->MaxSteps);

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
				auto pl = Deltares::Reliability::probLibString();
				auto text = "Reliability = " + pl.double2str(report->Reliability);
#endif

				this->progressIndicator->doTextualProgress(ProgressType::Detailed, text);
			}
		}

		void ModelRunner::reportMessage(MessageType type, std::string text)
		{
			if (Settings->SaveMessages && this->messages.size() < (size_t)this->Settings->MaxMessages && type >= this->Settings->LowestMessageType)
			{
				this->messages.push_back(std::make_shared<Message>(type, text));
			}
		}

		void ModelRunner::doTextualProgress(ProgressType type, std::string text)
		{
			if (this->progressIndicator != nullptr) 
			{
				this->progressIndicator->doTextualProgress(type, text);
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

		std::vector<double> ModelRunner::getOnlyVaryingValues(std::vector<double> values)
		{
			return this->uConverter->getVaryingValues(values);
		}
	}
}



