#include "DesignPoint.h"
#include "../Model/Message.h"


namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			void DesignPoint::setDesignPoint(System::Collections::Generic::List<Stochast^>^ stochasts)
			{
				setStochastPoint(shared->object, stochasts);

				if (shared->object->convergenceReport != nullptr)
				{
					this->ConvergenceReport = gcnew Wrappers::ConvergenceReport(shared->object->convergenceReport);
				}

				for (int i = 0; i < shared->object->ReliabililityResults.size(); i++)
				{
					Wrappers::ReliabilityResult^ resultWrapper = gcnew Wrappers::ReliabilityResult(shared->object->ReliabililityResults[i]);
					this->ReliabilityResults->Add(resultWrapper);
				}

				for (int i = 0; i < shared->object->Evaluations.size(); i++)
				{
					Wrappers::Evaluation^ evaluationWrapper = gcnew Wrappers::Evaluation(shared->object->Evaluations[i]);
					this->Evaluations->Add(evaluationWrapper);
				}

				for (int i = 0; i < shared->object->Messages.size(); i++)
				{
					Wrappers::Message^ messageWrapper = gcnew Wrappers::Message(shared->object->Messages[i]);
					this->Messages->Add(messageWrapper);
				}

				for (int i = 0; i < shared->object->ContributingDesignPoints.size(); i++)
				{
					DesignPoint^ designPointWrapper = gcnew DesignPoint(shared->object->ContributingDesignPoints[i], stochasts);
					this->ContributingDesignPoints->Add(designPointWrapper);
				}
			}
		}
	}
}

