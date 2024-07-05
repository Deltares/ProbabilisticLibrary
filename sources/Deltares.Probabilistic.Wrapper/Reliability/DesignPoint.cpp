#include "DesignPoint.h"
#include "../Model/Message.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
            using namespace System::Collections::Generic;

            void DesignPoint::setDesignPointInfo()
            {
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
            }

			void DesignPoint::setDesignPoint(IList<Stochast^>^ stochasts)
			{
				setStochastPoint(shared->object, stochasts);

                setDesignPointInfo();

				for (int i = 0; i < shared->object->ContributingDesignPoints.size(); i++)
				{
					DesignPoint^ designPointWrapper = gcnew DesignPoint(shared->object->ContributingDesignPoints[i], stochasts);
					this->ContributingDesignPoints->Add(designPointWrapper);
				}
			}

            void DesignPoint::setDesignPoints(IList<DesignPoint^>^ contributingDesignPoints)
			{
                List<Stochast^>^ stochasts = gcnew List<Stochast^>();

                for (int i = 0; i < contributingDesignPoints->Count; i++)
                {
                    for (int j = 0; j < contributingDesignPoints[i]->Alphas->Count; j++)
                    {
                        stochasts->Add(contributingDesignPoints[i]->Alphas[j]->Parameter);
                    }
                }

                setStochastPoint (shared->object, stochasts);

                setDesignPointInfo();

                for (int i = 0; i < shared->object->ContributingDesignPoints.size(); i++)
                {
                    for (int j = 0; j < contributingDesignPoints->Count; j++)
                    {
                        if (contributingDesignPoints[j]->getDesignPoint() == shared->object->ContributingDesignPoints[i])
                        {
                            this->ContributingDesignPoints->Add(contributingDesignPoints[j]);
                        }
                    }
                }
			}

            bool DesignPoint::HasMatchingAlphaValues()
            {
                if (this->Alphas->Count != shared->object->Alphas.size())
                {
                    return false;
                }

                for (int i = 0; i < this->Alphas->Count; i++)
                {
                    if (this->Alphas[i]->GetNativeAlpha() != shared->object->Alphas[i])
                    {
                        return false;
                    }
                }

                return true;
            }

            void DesignPoint::AssignTags(TagRepository^ tagRepository)
            {
                for (int i = 0; i < this->Evaluations->Count; i++)
                {
                    this->Evaluations[i]->AssignTag(tagRepository);
                }

                for (int i = 0; i < this->ContributingDesignPoints->Count; i++)
                {
                    this->ContributingDesignPoints[i]->AssignTags(tagRepository);
                }
            }
		}
	}
}

