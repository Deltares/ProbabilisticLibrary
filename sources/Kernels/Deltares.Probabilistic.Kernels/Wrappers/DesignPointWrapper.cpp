#include "DesignPointWrapper.h"

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
}

std::string Deltares::Probabilistic::Kernels::DesignPointWrapper::MarshalString(System::String^ s)
{
	const char* chars =	(const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(s)).ToPointer();
	std::string os = chars;
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)chars));

	return os;
}

