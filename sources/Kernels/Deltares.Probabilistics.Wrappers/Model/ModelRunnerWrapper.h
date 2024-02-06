#pragma once
#include "../../Deltares.Probabilistic.Kernels/Model/ZModel.h"
#include "../../Deltares.Probabilistic.Kernels/Model/ModelRunner.h"
#include "../../Deltares.Probabilistic.Kernels/Model/Sample.h"
#include "../Statistics/CorrelationMatrixWrapper.h"
#include "../Statistics/StochastWrapper.h"
#include "../Utils/SharedPointerProvider.h"
#include "ProgressIndicatorWrapper.h"
#include "RunSettingsWrapper.h"
#include "SampleWrapper.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Reliability;
			using namespace Deltares::Statistics::Wrappers;

			public delegate void ZSampleDelegate(SampleWrapper^);

			public ref class ModelRunnerWrapper
			{
			private:
				Models::ModelRunner* modelRunner;
				SharedPointerProvider<Models::ModelRunner>* sharedPointer = new SharedPointerProvider<Models::ModelRunner>();

				std::shared_ptr<Models::ZModel> getZModel();
				Models::ZLambda getZLambda();
				Models::ZMultipleLambda getZMultipleDelegate();
				ZSampleDelegate^ zFunction = nullptr;

				void CalcZValues(System::Collections::Generic::IList<SampleWrapper^>^ samples);
				void CalcZValue(SampleWrapper^ sample);

				void invokeSample(std::shared_ptr<Sample> sample);
				void invokeMultipleSamples(std::vector<std::shared_ptr<Sample>> samples);

				System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>^ handles = gcnew System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>();

			public:
				ModelRunnerWrapper(ZSampleDelegate^ zFunction, System::Collections::Generic::List<StochastWrapper^>^ stochasts, CorrelationMatrixWrapper^ correlationMatrix, ProgressIndicatorWrapper^ progressIndicator);
				~ModelRunnerWrapper() { this->!ModelRunnerWrapper(); }
				!ModelRunnerWrapper()
				{
					for (int i = 0; i < handles->Count; i++)
					{
						handles[i].Free();
					}
					delete sharedPointer;
				}

				System::Collections::Generic::List<StochastWrapper^>^ Stochasts = gcnew System::Collections::Generic::List<StochastWrapper^>();

				virtual void GetZValues(System::Collections::Generic::IList<SampleWrapper^>^ samples);
				virtual void GetZValue(SampleWrapper^ sample);

				RunSettingsWrapper^ Settings = gcnew RunSettingsWrapper();

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




