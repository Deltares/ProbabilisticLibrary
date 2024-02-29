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
#include "UConverter.h"

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
				SharedPointerProvider<Models::ModelRunner>* shared = nullptr;

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
					delete shared;
				}

				System::Collections::Generic::List<Stochast^>^ Stochasts = gcnew System::Collections::Generic::List<Stochast^>();

				virtual void CalcSamples(System::Collections::Generic::IList<Sample^>^ samples);
				virtual void CalcSample(Sample^ sample);

				virtual property int VaryingStochastCount
				{
					int get() { return shared->object->getVaryingStochastCount(); }
				}

				bool IsVaryingStochast(int index)
				{
					return shared->object->isVaryingStochast(index);
				}

				/**
				 * \brief Calculates a z-value for a reliability method in .net
				 * \param sample 
				 * \return 
				 */
				virtual double GetZValue(Sample^ sample)
				{
					return shared->object->getZValue(sample->GetSample());
				}

				/**
				 * \brief Calculates multiple z-values for a reliability method in .net
				 * \param samples 
				 * \return 
				 */
				virtual array<double>^ GetZValues(System::Collections::Generic::List<Sample^>^ samples)
				{
					std::vector<std::shared_ptr<Models::Sample>> nativeSamples(samples->Count);

					for (int i = 0; i < samples->Count; i++)
					{
						nativeSamples[i] = samples[i]->GetSample();
					}

					std::vector<double> zValues = shared->object->getZValues(nativeSamples);

					return NativeSupport::toManaged(zValues);
				}

				RunSettings^ Settings = gcnew RunSettings();

				virtual void InitializeForRun()
				{
					shared->object->initializeForRun();
				}

				Sample^ GetXSample(Sample^ sample);
				Sample^ GetOnlyVaryingSample(Sample^ sample);

				std::shared_ptr<Models::ModelRunner> GetModelRunner()
				{
					shared->object->Settings = this->Settings->GetSettings();
					return shared->object;
				}
			};
		}
	}
}




