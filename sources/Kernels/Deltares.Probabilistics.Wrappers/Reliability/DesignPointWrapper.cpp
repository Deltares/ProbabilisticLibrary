#include "DesignPointWrapper.h"
#include "../Model/MessageWrapper.h"


void Deltares::Probabilistic::Kernels::DesignPointWrapper::SetDesignPoint(DesignPoint* designPoint, System::Collections::Generic::List<Deltares::Probabilistic::Kernels::StochastWrapper^>^ stochasts)
{
	this->m_designPoint = designPoint;

	SetStochastPoint(designPoint, stochasts);

	this->ConvergenceReport = gcnew ConvergenceReportWrapper(designPoint->ConvergenceReport);

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

