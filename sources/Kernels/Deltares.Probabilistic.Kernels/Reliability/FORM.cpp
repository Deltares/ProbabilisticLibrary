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
		std::shared_ptr<DesignPoint> FORM::getDesignPoint(std::shared_ptr<Models::ZModelRunner> modelRunner)
		{
			modelRunner->updateStochastSettings(this->Settings->StochastSet);

			std::vector<std::shared_ptr<DesignPoint>> previousDesignPoints;

			// initialize
			StartPointCalculator* startPointCalculator = new StartPointCalculator();
			startPointCalculator->Settings = this->Settings->StartPointSettings;
			startPointCalculator->Settings->StochastSet = this->Settings->StochastSet;

			std::shared_ptr<Sample> startPoint = startPointCalculator->getStartPoint(modelRunner);

			if (Settings->StartPointSettings->StartMethod != StartMethodType::None)
			{
				const std::shared_ptr<DesignPoint> startDesignPoint = modelRunner->getDesignPoint(startPoint, startPoint->getBeta());
				startDesignPoint->Identifier = "Start point";
				previousDesignPoints.push_back(startDesignPoint);
			}

			std::shared_ptr<DesignPoint> designPoint = nullptr;

			double dzduLength = 0;
			double relaxationFactor = this->Settings->RelaxationFactor;

			// perform iterations
			for (int relaxationIndex = 0; relaxationIndex < Settings->RelaxationLoops; relaxationIndex++)
			{
				modelRunner->clear();

				designPoint = getDesignPoint(modelRunner, startPoint, relaxationFactor);

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

		std::shared_ptr<DesignPoint> FORM::getDesignPoint(std::shared_ptr<Models::ZModelRunner> modelRunner, std::shared_ptr<Sample> startSample, double relaxationFactor)
		{
			constexpr double minDzduLength = 1E-08;

			const int nStochasts = modelRunner->getVaryingStochastCount();

			std::vector<double> zGradient(nStochasts);

			// initialization
			std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();

			if (this->Settings->RelaxationLoops > 1)
			{
				convergenceReport->RelaxationFactor = relaxationFactor;
			}

			std::shared_ptr<Sample> u = startSample->clone();

			int iteration = 0;
			double beta = nan("");

			Models::GradientCalculator* gradientCalculator = new Models::GradientCalculator();
			gradientCalculator->Settings = this->Settings->GradientSettings;

			while (!convergenceReport->IsConverged && iteration < this->Settings->MaximumIterations && !this->isStopped())
			{
				u->IterationIndex = iteration;
				zGradient = gradientCalculator->getGradient(modelRunner, u);

				// check whether there are valid results

				if (!areResultsValid(zGradient))
				{
					// return the result so far
#ifdef __cpp_lib_format
					modelRunner->reportMessage(Models::MessageType::Error, std::format("Model did not provide valid results, limit state value = {0:.5G}", u->Z));
#else
					auto pl = Deltares::ProbLibCore::probLibString();
					modelRunner->reportMessage(Models::MessageType::Error, "Model did not provide valid results, limit state value = " + pl.double2str( u->Z));
#endif

					std::shared_ptr<ReliabilityReport> reportInvalid = std::make_shared<ReliabilityReport>();
					reportInvalid->Step = iteration;
					reportInvalid->MaxSteps = this->Settings->MaximumIterations;
					reportInvalid->Reliability = nan("");
					reportInvalid->ReportMatchesEvaluation = false;

					modelRunner->reportResult(reportInvalid);

					return modelRunner->getDesignPoint(u, nan(""), convergenceReport);
				}

				// repair for failed evaluations
				repairResults(zGradient);

				// Mean value Z at u = 0
				double zSum = 0;
				for (int i = 0; i < nStochasts; i++)
				{
					zSum += u->Values[i] * zGradient[i];
				}

				const double z0 = u->Z - zSum;

				// Standard deviation Z 
				double zGradientLength = NumericSupport::GetLength(zGradient);

				if (zGradientLength < minDzduLength && iteration == 0)
				{
					modelRunner->reportMessage(Models::MessageType::Error, "No variation in model results found at start point");

					std::shared_ptr<ReliabilityReport> reportTooSmall = std::make_shared<ReliabilityReport>();

					reportTooSmall->Step = iteration;
					reportTooSmall->MaxSteps = this->Settings->MaximumIterations;
					reportTooSmall->Reliability = beta;
					reportTooSmall->ReportMatchesEvaluation = false;

					modelRunner->reportResult(reportTooSmall);

					double beta = this->getZFactor(u->Z) * Statistics::StandardNormal::BetaMax;

					return modelRunner->getDesignPoint(u, beta, convergenceReport);
				}

				//   compute beta
				if (zGradientLength == 0)
				{
					beta = this->getZFactor(z0) * Statistics::StandardNormal::BetaMax;
				}
				else
				{
					beta = z0 / zGradientLength;
				}

				if (std::abs(beta) >= Statistics::StandardNormal::BetaMax)
				{
					modelRunner->reportMessage(Models::MessageType::Error, "No convergence found");

					std::shared_ptr<ReliabilityReport> reportTooHigh = std::make_shared<ReliabilityReport>();

					reportTooHigh->Step = iteration;
					reportTooHigh->MaxSteps = this->Settings->MaximumIterations;
					reportTooHigh->Reliability = beta;
					reportTooHigh->ReportMatchesEvaluation = false;

					modelRunner->reportResult(reportTooHigh);

					return modelRunner->getDesignPoint(u, beta, convergenceReport);
				}

				convergenceReport->IsConverged = checkConvergence(modelRunner, u, convergenceReport, beta, zGradientLength);

				// no convergence, next iteration
				if (!convergenceReport->IsConverged)
				{
					std::shared_ptr<Sample> uNew = std::make_shared<Sample>(nStochasts);

					for (int k = 0; k < nStochasts; k++)
					{
						double alpha = zGradient[k] / zGradientLength;
						double uNewValue = -alpha * beta;

						uNew->Values[k] = relaxationFactor * uNewValue + (1 - relaxationFactor) * u->Values[k];
					}

					u = uNew;
				}

				iteration++;
			}

			return modelRunner->getDesignPoint(u, beta, convergenceReport);
		}

		void FORM::repairResults(std::vector<double> dzdu)
		{
			for (int k = 0; k < dzdu.size(); k++)
			{
				if (std::isnan(dzdu[k]))
				{
					dzdu[k] = 0;
				}
			}
		}

		bool FORM::areResultsValid(std::vector<double> dzdu)
		{
			bool validResults = false;
			for (int k = 0; k < dzdu.size(); k++)
			{
				if (!std::isnan(dzdu[k]))
				{
					validResults = true;
					break;
				}
			}

			return validResults;
		}

		bool FORM::checkConvergence(std::shared_ptr<Models::ZModelRunner> modelRunner, std::shared_ptr<Sample> u, std::shared_ptr<ConvergenceReport> convergenceReport, double beta, double zGradientLength)
		{
			//   compute alpha vector
			const double uSquared = NumericSupport::GetSquaredSum(u->Values);

			const double fromZeroDiff = uSquared > 0 ? std::abs(beta * beta - uSquared) / uSquared : 0;
			const double localDiff = std::abs(u->Z / zGradientLength);
			const double betaDiff = std::max(fromZeroDiff, localDiff);

			convergenceReport->Convergence = betaDiff;
			convergenceReport->IsConverged = betaDiff <= this->Settings->EpsilonBeta;

			std::shared_ptr<ReliabilityReport> report = std::make_shared<ReliabilityReport>();
			report->Step = u->IterationIndex;
			report->MaxSteps = this->Settings->MaximumIterations;
			report->Reliability = beta;
			report->ConvBeta = betaDiff;
			report->ReportMatchesEvaluation = false;

			modelRunner->reportResult(report);

			//   check for convergence
			return convergenceReport->IsConverged;
		}
	}
}


