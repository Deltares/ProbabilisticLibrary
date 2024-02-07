#pragma once
#include "../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../Deltares.Probabilistic/Model/ModelRunner.h"
#include "../../Deltares.Probabilistic/Model/Sample.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Statistics/Stochast.h"
#include "../Utils/SharedPointerProvider.h"
#include "ProgressIndicator.h"
#include "RunSettings.h"
#include "Sample.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Reliability;
			using namespace Deltares::Statistics::Wrappers;

			public delegate void ZSampleDelegate(Sample^);

			public ref class ModelRunner
			{
			private:
				Models::ModelRunner* modelRunner;
				SharedPointerProvider<Models::ModelRunner>* sharedPointer = new SharedPointerProvider<Models::ModelRunner>();

				std::shared_ptr<Models::ZModel> getZModel();
				Models::ZLambda getZLambda();
				Models::ZMultipleLambda getZMultipleDelegate();
				ZSampleDelegate^ zFunction = nullptr;

				void CalcZValues(System::Collections::Generic::IList<Sample^>^ samples);
				void CalcZValue(Sample^ sample);

				void invokeSample(std::shared_ptr<Models::Sample> sample);
				void invokeMultipleSamples(std::vector<std::shared_ptr<Models::Sample>> samples);

				System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>^ handles = gcnew System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>();

			public:
				ModelRunner(ZSampleDelegate^ zFunction, System::Collections::Generic::List<Stochast^>^ stochasts, CorrelationMatrix^ correlationMatrix, ProgressIndicator^ progressIndicator);
				~ModelRunner() { this->!ModelRunner(); }
				!ModelRunner()
				{
					for (int i = 0; i < handles->Count; i++)
					{
						handles[i].Free();
					}
					delete sharedPointer;
				}

				System::Collections::Generic::List<Stochast^>^ Stochasts = gcnew System::Collections::Generic::List<Stochast^>();

				virtual void GetZValues(System::Collections::Generic::IList<Sample^>^ samples);
				virtual void GetZValue(Sample^ sample);

				RunSettings^ Settings = gcnew RunSettings();

				std::shared_ptr<Models::ModelRunner> GetModelRunner()
				{
					std::shared_ptr<Models::ModelRunner> m_modelRunner = sharedPointer->getSharedPointer(modelRunner);

					m_modelRunner->Settings = this->Settings->GetSettings();
					return m_modelRunner;
				}
			};
		}
	}
}




