#include "DesignPointWrapper.h"

void Deltares::Probabilistic::Kernels::DesignPointWrapper::SetDesignPoint(DesignPoint* designPoint, System::Collections::Generic::List<Deltares::Probabilistic::Kernels::StochastWrapper^>^ stochasts)
{
	this->m_designPoint = designPoint;

	SetStochastPoint(designPoint, stochasts);

	this->ConvergenceReport = gcnew Deltares::Probabilistic::Kernels::ConvergenceReportWrapper(designPoint->ConvergenceReport);

	for (int i = 0; i < designPoint->ReliabililityResults.size(); i++)
	{
		Deltares::Probabilistic::Kernels::ReliabilityResultWrapper^ resultWrapper = gcnew Deltares::Probabilistic::Kernels::ReliabilityResultWrapper(designPoint->ReliabililityResults[i]);
		this->ReliabilityResults->Add(resultWrapper);
	}
}

