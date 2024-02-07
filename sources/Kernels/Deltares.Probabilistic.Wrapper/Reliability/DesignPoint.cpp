#include "DesignPoint.h"
#include "../Model/Message.h"


namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			void DesignPoint::SetDesignPoint(std::shared_ptr<Reliability::DesignPoint> designPoint, System::Collections::Generic::List<Stochast^>^ stochasts)
			{
				this->m_designPoint = designPoint.get();

				this->sharedPointer->setSharedPointer(designPoint);

				SetStochastPoint(designPoint, stochasts);

				if (designPoint->convergenceReport != nullptr)
				{
					this->ConvergenceReport = gcnew Wrappers::ConvergenceReport(designPoint->convergenceReport);
				}

				for (int i = 0; i < designPoint->ReliabililityResults.size(); i++)
				{
					Wrappers::ReliabilityResult^ resultWrapper = gcnew Wrappers::ReliabilityResult(designPoint->ReliabililityResults[i]);
					this->ReliabilityResults->Add(resultWrapper);
				}

				for (int i = 0; i < designPoint->Evaluations.size(); i++)
				{
					Wrappers::Evaluation^ evaluationWrapper = gcnew Wrappers::Evaluation(designPoint->Evaluations[i]);
					this->Evaluations->Add(evaluationWrapper);
				}

				for (int i = 0; i < designPoint->Messages.size(); i++)
				{
					Wrappers::Message^ messageWrapper = gcnew Wrappers::Message(designPoint->Messages[i]);
					this->Messages->Add(messageWrapper);
				}

				for (int i = 0; i < designPoint->ContributingDesignPoints.size(); i++)
				{
					DesignPoint^ designPointWrapper = gcnew DesignPoint(designPoint->ContributingDesignPoints[i], stochasts);
					this->ContributingDesignPoints->Add(designPointWrapper);
				}
			}
		}
	}
}

