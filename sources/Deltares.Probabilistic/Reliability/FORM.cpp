#include "FORM.h"

#include "StartPointCalculator.h"
#include "DesignPoint.h"
#include "../Model/GradientCalculator.h"

#if __has_include(<format>)
#include <format>
#else
#include "../Utils/probLibString.h"
#endif


namespace Deltares
{
	namespace Reliability
	{
		using namespace Deltares::Numeric;

		std::shared_ptr<DesignPoint> FORM::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
		{
			modelRunner->updateStochastSettings(this->Settings->StochastSet);

			std::vector<std::shared_ptr<DesignPoint>> previousDesignPoints;

			// initialize
			const std::shared_ptr <StartPointCalculator> startPointCalculator = std::make_shared<StartPointCalculator>();
			startPointCalculator->Settings = this->Settings->StartPointSettings;
			startPointCalculator->Settings->StochastSet = this->Settings->StochastSet;

			const std::shared_ptr<Sample> startPoint = startPointCalculator->getStartPoint(modelRunner);

			if (Settings->StartPointSettings->StartMethod != StartMethodType::None)
			{
				const std::shared_ptr<DesignPoint> startDesignPoint = modelRunner->getDesignPoint(startPoint, startPoint->getBeta());
				startDesignPoint->Identifier = "Start point";
				previousDesignPoints.push_back(startDesignPoint);
			}

			std::shared_ptr<DesignPoint> designPoint = nullptr;

			double relaxationFactor = this->Settings->RelaxationFactor;

			// perform iterations
			for (int relaxationIndex = 0; relaxationIndex < Settings->RelaxationLoops; relaxationIndex++)
			{
				modelRunner->clear();

				designPoint = getDesignPoint(modelRunner, startPoint->clone(), relaxationFactor);

				if (designPoint->convergenceReport->IsConverged)
				{
					break;
				}
				else
				{
					relaxationFactor /= 2;

					int modifiedRelaxationIndex = relaxationIndex + 1;

					if (modifiedRelaxationIndex < Settings->RelaxationLoops)
					{
#ifdef __cpp_lib_format
						designPoint->Identifier = std::format("Relaxation loop {0:}", modifiedRelaxationIndex);
#else
						designPoint->Identifier = "Relaxation loop " + std::to_string(modifiedRelaxationIndex);
#endif
						previousDesignPoints.push_back(designPoint);
					}
				}
			}

			for (std::shared_ptr<DesignPoint> previousDesignPoint : previousDesignPoints)
			{
				designPoint->ContributingDesignPoints.push_back(previousDesignPoint);
			}

			return designPoint;
		}

		std::shared_ptr<DesignPoint> FORM::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> startPoint, double relaxationFactor)
		{
			constexpr double minGradientLength = 1E-08;

			const int nStochasts = modelRunner->getVaryingStochastCount();

			std::vector<double> zGradient(nStochasts);

			// initialization
			const std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();

			if (this->Settings->RelaxationLoops > 1)
			{
				convergenceReport->RelaxationFactor = relaxationFactor;
			}

			int iteration = 0;
			double beta = nan("");

			std::shared_ptr<Sample> sample = startPoint->clone();
			std::shared_ptr<Sample> resultSample = startPoint->clone();

			const std::shared_ptr <Models::GradientCalculator> gradientCalculator = std::make_shared<Models::GradientCalculator>();
			gradientCalculator->Settings = this->Settings->GradientSettings;

			while (!convergenceReport->IsConverged && iteration < this->Settings->MaximumIterations && !this->isStopped())
			{
				sample->IterationIndex = iteration;
				zGradient = gradientCalculator->getGradient(modelRunner, sample);

				// check whether there are valid results

				if (!areAllResultsValid(zGradient))
				{
					// return the result so far
#ifdef __cpp_lib_format
					modelRunner->reportMessage(Models::MessageType::Error, std::format("Model did not provide valid results, limit state value = {0:.5G}", sample->Z));
#else
					auto pl = Deltares::ProbLibCore::probLibString();
					modelRunner->reportMessage(Models::MessageType::Error, "Model did not provide valid results, limit state value = " + pl.double2str( u->Z));
#endif

					std::shared_ptr<ReliabilityReport> reportInvalid = getReport(iteration, nan(""));

					modelRunner->reportResult(reportInvalid);

					return modelRunner->getDesignPoint(sample, nan(""), convergenceReport);
				}

				// Mean value Z at u = 0
				double zSum = 0;
				for (int i = 0; i < nStochasts; i++)
				{
					zSum += sample->Values[i] * zGradient[i];
				}

				const double z0 = sample->Z - zSum;

				// Standard deviation Z 
				double zGradientLength = NumericSupport::GetLength(zGradient);

				if (zGradientLength < minGradientLength && iteration == 0)
				{
					modelRunner->reportMessage(Models::MessageType::Error, "No variation in model results found at start point");

					const std::shared_ptr<ReliabilityReport> reportTooSmall = getReport(iteration, beta);

					modelRunner->reportResult(reportTooSmall);

					const double betaNoVariation = ReliabilityMethod::getZFactor(sample->Z) * Statistics::StandardNormal::BetaMax;

					return modelRunner->getDesignPoint(sample, betaNoVariation, convergenceReport);
				}

				//   compute beta
				if (zGradientLength == 0)
				{
					beta = ReliabilityMethod::getZFactor(z0) * Statistics::StandardNormal::BetaMax;
				}
				else
				{
					beta = z0 / zGradientLength;
				}

				if (std::abs(beta) >= Statistics::StandardNormal::BetaMax)
				{
					modelRunner->reportMessage(Models::MessageType::Error, "No convergence found");

					const std::shared_ptr<ReliabilityReport> reportTooHigh = std::make_shared<ReliabilityReport>();

					reportTooHigh->Step = iteration;
					reportTooHigh->MaxSteps = this->Settings->MaximumIterations;
					reportTooHigh->Reliability = beta;
					reportTooHigh->ReportMatchesEvaluation = false;

					modelRunner->reportResult(reportTooHigh);

					return modelRunner->getDesignPoint(sample, beta, convergenceReport);
				}

				convergenceReport->IsConverged = isConverged(modelRunner, sample, convergenceReport, beta, zGradientLength);

				// no convergence, next iteration
				if (!convergenceReport->IsConverged)
				{
					const std::shared_ptr<Sample> newSample = std::make_shared<Sample>(nStochasts);

					for (int k = 0; k < nStochasts; k++)
					{
						const double alpha = zGradient[k] / zGradientLength;
						const double uNewValue = -alpha * beta;

						newSample->Values[k] = relaxationFactor * uNewValue + (1 - relaxationFactor) * sample->Values[k];
					}

					sample = newSample;
				}

				// compute design values
				std::vector<double> uValues = NumericSupport::select(zGradient, [beta, zGradientLength](double p) { return - beta *  p / zGradientLength; });

				resultSample = std::make_shared<Sample>(uValues);

				iteration++;
			}

			return modelRunner->getDesignPoint(resultSample, beta, convergenceReport);
		}

		bool FORM::areAllResultsValid(std::vector<double> values)
		{
			for (int k = 0; k < values.size(); k++)
			{
				if (std::isnan(values[k]))
				{
					return false;
				}
			}

			return true;
		}

		bool FORM::isConverged(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> sample, std::shared_ptr<ConvergenceReport> convergenceReport, double beta, double zGradientLength)
		{
			const double uSquared = NumericSupport::GetSquaredSum(sample->Values);

			const double fromZeroDiff = uSquared > 0 ? std::abs(beta * beta - uSquared) / uSquared : 0;
			const double localDiff = std::abs(sample->Z / zGradientLength);
			const double betaDiff = std::max(fromZeroDiff, localDiff);

			convergenceReport->Convergence = betaDiff;
			convergenceReport->IsConverged = betaDiff <= this->Settings->EpsilonBeta;

			std::shared_ptr<ReliabilityReport> report = getReport(sample->IterationIndex, beta);
			report->ConvBeta = betaDiff;

			modelRunner->reportResult(report);

			//   check for convergence
			return convergenceReport->IsConverged;
		}

		std::shared_ptr<ReliabilityReport> FORM::getReport(int iteration, double reliability)
		{
			std::shared_ptr<ReliabilityReport> report = std::make_shared<ReliabilityReport>();

			report->Step = iteration;
			report->Reliability = reliability;

			report->MaxSteps = this->Settings->MaximumIterations;
			report->ReportMatchesEvaluation = false;

			return report;
		}
	}
}


