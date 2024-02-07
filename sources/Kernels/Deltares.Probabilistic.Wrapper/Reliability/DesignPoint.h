#pragma once

#include "../../Deltares.Probabilistic/Reliability/DesignPoint.h"
#include "../Model/Evaluation.h"
#include "../Model/StochastPointAlpha.h"
#include "../Model/StochastPoint.h"
#include "../Model/Message.h"
#include "ConvergenceReport.h"
#include "ReliabilityResult.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models::Wrappers;
			using namespace Deltares::Statistics::Wrappers;

			public ref class DesignPoint : public Wrappers::StochastPoint
			{
			private:
				Reliability::DesignPoint* m_designPoint;
				SharedPointerProvider<Reliability::DesignPoint>* sharedPointer = new SharedPointerProvider<Reliability::DesignPoint>();
				ConvergenceReport^ convergenceReport = gcnew Wrappers::ConvergenceReport();
				System::Collections::Generic::List<Wrappers::ReliabilityResult^>^ reliabilityResults = gcnew System::Collections::Generic::List<ReliabilityResult^>();
				System::Collections::Generic::List<Wrappers::Evaluation^>^ evaluations = gcnew System::Collections::Generic::List<Wrappers::Evaluation^>();
				System::Collections::Generic::List<Wrappers::Message^>^ messages = gcnew System::Collections::Generic::List<Wrappers::Message^>();
				System::Collections::Generic::List<Wrappers::DesignPoint^>^ contributingDesignPoints = gcnew System::Collections::Generic::List<Wrappers::DesignPoint^>();

				void SetDesignPoint(std::shared_ptr<Reliability::DesignPoint> designPoint, System::Collections::Generic::List<Stochast^>^ stochasts);
			public:
				DesignPoint()
				{
					std::shared_ptr<Reliability::DesignPoint> designPoint = std::make_shared<Reliability::DesignPoint>();
					SetDesignPoint(designPoint, gcnew System::Collections::Generic::List<Stochast^>());
				}
				DesignPoint(std::shared_ptr<Reliability::DesignPoint> designPoint, System::Collections::Generic::List<Stochast^>^ stochasts)
				{
					SetDesignPoint(designPoint, stochasts);
				}
				~DesignPoint() { this->!DesignPoint(); }
				!DesignPoint() { delete sharedPointer; }

				property System::String^ Identifier
				{
					System::String^ get() { return NativeSupport::toManaged(m_designPoint->Identifier); }
					void set (System::String^ value) { m_designPoint->Identifier = NativeSupport::toNative(value); }
				}

				property double ProbabilityFailure
				{
					double get() { return Statistics::StandardNormal::getQFromU(this->Beta); }
				}

				property double ProbabilityNonFailure
				{
					double get() { return Statistics::StandardNormal::getPFromU(this->Beta); }
				}

				property double ReturnPeriod
				{
					double get() { return Statistics::StandardNormal::getRFromP(this->ProbabilityFailure); }
				}

				property Wrappers::ConvergenceReport^ ConvergenceReport
				{
					Wrappers::ConvergenceReport^ get() { return convergenceReport; }
					void set(Wrappers::ConvergenceReport^ value) { convergenceReport = value; }
				}

				property System::Collections::Generic::List<ReliabilityResult^>^ ReliabilityResults
				{
					System::Collections::Generic::List<ReliabilityResult^>^ get() { return reliabilityResults; }
				}

				property System::Collections::Generic::List<Wrappers::Evaluation^>^ Evaluations
				{
					System::Collections::Generic::List<Wrappers::Evaluation^>^ get() { return evaluations; }
				}

				property System::Collections::Generic::List<Wrappers::Message^>^ Messages
				{
					System::Collections::Generic::List<Wrappers::Message^>^ get() { return messages; }
				}

				property System::Collections::Generic::List<DesignPoint^>^ ContributingDesignPoints
				{
					System::Collections::Generic::List<DesignPoint^>^ get() { return contributingDesignPoints; }
				}

				std::shared_ptr<Reliability::DesignPoint> getDesignPoint()
				{
					return sharedPointer->getSharedPointer(m_designPoint);
				}
			};
		}
	}
}


