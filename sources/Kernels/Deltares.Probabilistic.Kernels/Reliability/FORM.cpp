#include "FORM.h"

#include "StartPointCalculator.h"
#include "../Model/DesignPoint.h"
#include "../Model/GradientCalculator.h"
#include <format>


namespace Deltares
{
	namespace Reliability
	{
		DesignPoint* FORM::getDesignPoint(Deltares::Models::ZModelRunner* modelRunner)
		{
			modelRunner->updateStochastSettings(this->Settings->StochastSet);

			std::vector<DesignPoint*> previousDesignPoints;

			// initialize
			StartPointCalculator* startPointCalculator = new StartPointCalculator();
			startPointCalculator->Settings = this->Settings->StartPointSettings;
			startPointCalculator->Settings->StochastSet = this->Settings->StochastSet;

			Sample* startPoint = startPointCalculator->getStartPoint(modelRunner);

			if (Settings->StartPointSettings->StartMethod != StartMethodType::None)
			{
				DesignPoint* startDesignPoint = this->getDesignPointFromSample(startPoint, modelRunner, 1);
				startDesignPoint->Identifier = "Start point";
				previousDesignPoints.push_back(startDesignPoint);
			}

			DesignPoint* designPoint = nullptr;

			double dzduLength = 0;
			double relaxationFactor = this->Settings->RelaxationFactor;

			// perform iterations
			for (int relaxationIndex = 0; relaxationIndex < Settings->RelaxationLoops; relaxationIndex++)
			{
				modelRunner->clear();

				designPoint = getDesignPoint(modelRunner, startPoint, relaxationFactor);

				if (designPoint->ConvergenceReport->IsConverged)
				{
					break;
				}
				else
				{
					relaxationFactor /= 2;

					int modifiedRelaxationIndex = relaxationIndex + 1;

					if (modifiedRelaxationIndex < Settings->RelaxationLoops)
					{

						designPoint->Identifier = std::format("Relaxation loop {0:}", modifiedRelaxationIndex);
						previousDesignPoints.push_back(designPoint);
					}
				}
			}

			for (DesignPoint* previousDesignPoint : previousDesignPoints)
			{
				designPoint->ContributingDesignPoints.push_back(previousDesignPoint);
			}

			return designPoint;
		}

		DesignPoint* FORM::getDesignPoint(Models::ZModelRunner* modelRunner, Sample* startSample, double relaxationFactor)
		{
			constexpr double minDzduLength = 1E-08;

			const int nStochasts = modelRunner->getVaryingStochastCount();

			std::vector<double> zGradient(nStochasts);

			// initialization
			ConvergenceReport* convergenceReport = new ConvergenceReport();

			if (this->Settings->RelaxationLoops > 1)
			{
				convergenceReport->RelaxationFactor = relaxationFactor;
			}

			Sample* u = startSample->clone();

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
					modelRunner->reportMessage(Models::MessageType::Error, std::format("Model did not provide valid results, limit state value = {0:.5G}", u->Z));

					ReliabilityReport* reportInvalid = new ReliabilityReport();
					reportInvalid->Step = iteration;
					reportInvalid->MaxSteps = this->Settings->MaximumIterations;
					reportInvalid->Reliability = nan("");
					reportInvalid->ReportMatchesEvaluation = false;

					modelRunner->reportResult(reportInvalid);

					return this->getDesignPointFromSampleAndBeta(modelRunner, u, nan(""), convergenceReport);
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

					ReliabilityReport* reportTooSmall = new ReliabilityReport();

					reportTooSmall->Step = iteration;
					reportTooSmall->MaxSteps = this->Settings->MaximumIterations;
					reportTooSmall->Reliability = beta;
					reportTooSmall->ReportMatchesEvaluation = false;

					modelRunner->reportResult(reportTooSmall);

					double beta = this->getZFactor(u->Z) * Statistics::StandardNormal::BetaMax;

					return this->getDesignPointFromSampleAndBeta(modelRunner, u, beta, convergenceReport);
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

					ReliabilityReport* reportTooHigh = new ReliabilityReport();
					reportTooHigh->Step = iteration;
					reportTooHigh->MaxSteps = this->Settings->MaximumIterations;
					reportTooHigh->Reliability = beta;
					reportTooHigh->ReportMatchesEvaluation = false;

					modelRunner->reportResult(reportTooHigh);

					return this->getDesignPointFromSampleAndBeta(modelRunner, u, beta, convergenceReport);
				}

				//   compute alpha vector
				double uSquared = NumericSupport::GetSquaredSum(u->Values);

				double fromZeroDiff = uSquared > 0 ? std::abs(beta * beta - uSquared) / uSquared : 0;
				double localDiff = std::abs(u->Z / zGradientLength);

				double betaDiff = std::max(fromZeroDiff, localDiff);

				convergenceReport->Convergence = betaDiff;

				ReliabilityReport* report = new ReliabilityReport();
				report->Step = iteration;
				report->MaxSteps = this->Settings->MaximumIterations;
				report->Reliability = beta;
				report->ConvBeta = std::max(fromZeroDiff, localDiff);
				report->ReportMatchesEvaluation = false;

				modelRunner->reportResult(report);

				//   check for convergence
				convergenceReport->IsConverged = betaDiff <= this->Settings->EpsilonBeta;

				// no convergence, next iteration
				if (!convergenceReport->IsConverged)
				{
					Sample* uNew = new Sample(nStochasts);

					for (int k = 0; k < nStochasts; k++)
					{
						double alpha = zGradient[k] / zGradientLength;
						double uNewValue = -alpha * beta;

						uNew->Values[k] = relaxationFactor * uNewValue + (1 - relaxationFactor) * u->Values[k];
					}

					delete u;

					u = uNew;
				}

				iteration++;
			}

			DesignPoint* designPoint = this->getDesignPointFromSampleAndBeta(modelRunner, u, beta, convergenceReport);

			delete u;

			return designPoint;
		}

		void FORM::repairResults(std::vector<double> dzdu)
		{
			for (int k = 0; k < dzdu.size(); k++)
			{
				if (isnan(dzdu[k]))
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
				if (!isnan(dzdu[k]))
				{
					validResults = true;
					break;
				}
			}

			return validResults;
		}
	}
}


