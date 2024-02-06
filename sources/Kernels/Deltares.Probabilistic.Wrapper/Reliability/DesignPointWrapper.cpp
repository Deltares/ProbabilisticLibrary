#include "DesignPointWrapper.h"
#include "../Model/MessageWrapper.h"


namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			void DesignPointWrapper::SetDesignPoint(std::shared_ptr<DesignPoint> designPoint, System::Collections::Generic::List<StochastWrapper^>^ stochasts)
			{
				this->m_designPoint = designPoint.get();

				this->sharedPointer->setSharedPointer(designPoint);

				SetStochastPoint(designPoint, stochasts);

				if (designPoint->convergenceReport != nullptr)
				{
					this->ConvergenceReport = gcnew ConvergenceReportWrapper(designPoint->convergenceReport);
				}

				for (int i = 0; i < designPoint->ReliabililityResults.size(); i++)
				{
					ReliabilityResultWrapper^ resultWrapper = gcnew ReliabilityResultWrapper(designPoint->ReliabililityResults[i]);
					this->ReliabilityResults->Add(resultWrapper);
				}

				for (int i = 0; i < designPoint->Evaluations.size(); i++)
				{
					EvaluationWrapper^ evaluationWrapper = gcnew EvaluationWrapper(designPoint->Evaluations[i]);
					this->Evaluations->Add(evaluationWrapper);
				}

				for (int i = 0; i < designPoint->Messages.size(); i++)
				{
					MessageWrapper^ messageWrapper = gcnew MessageWrapper(designPoint->Messages[i]);
					this->Messages->Add(messageWrapper);
				}

				for (int i = 0; i < designPoint->ContributingDesignPoints.size(); i++)
				{
					DesignPointWrapper^ designPointWrapper = gcnew DesignPointWrapper(designPoint->ContributingDesignPoints[i], stochasts);
					this->ContributingDesignPoints->Add(designPointWrapper);
				}
			}
		}
	}
}

