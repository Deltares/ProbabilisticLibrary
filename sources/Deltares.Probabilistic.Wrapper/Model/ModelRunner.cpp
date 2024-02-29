#include "ModelRunner.h"

#include "ProgressIndicator.h"
#include "../../Deltares.Probabilistic/Model/Sample.h"
#include "../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Models;

			delegate void ManagedSampleDelegate(std::shared_ptr<Models::Sample> sample);
			delegate void ManagedMultipleSampleDelegate(std::vector<std::shared_ptr<Models::Sample>> samples);
			typedef void(__stdcall* ZDelegate) (std::shared_ptr<Models::Sample>);
			typedef void(__stdcall* ZMultipleDelegate) (std::vector<std::shared_ptr<Models::Sample>>);

			ModelRunner::ModelRunner(ZSampleDelegate^ zFunction, System::Collections::Generic::List<Stochast^>^ stochasts, CorrelationMatrix^ correlationMatrix, ProgressIndicator^ progressIndicator)
			{
				this->zFunction = zFunction;

				this->Stochasts->AddRange(stochasts);

				std::vector<std::shared_ptr<Statistics::Stochast> > native_stochasts;

				for (int i = 0; i < stochasts->Count; i++)
				{
					native_stochasts.push_back(stochasts[i]->GetStochast());
				}

				std::shared_ptr<UConverter> uConverter = std::make_shared<UConverter>(native_stochasts, correlationMatrix->GetCorrelationMatrix());

				std::shared_ptr<Models::ZModel> zModel = getZModel();

				std::shared_ptr<Models::ProgressIndicator> progress = progressIndicator != nullptr ? progressIndicator->GetProgressIndicator() : nullptr;

				this->shared = new SharedPointerProvider(new Models::ModelRunner(zModel, uConverter, progress));
			}

			Models::ZLambda ModelRunner::getZLambda()
			{
				ManagedSampleDelegate^ fp = gcnew ManagedSampleDelegate(this, &ModelRunner::invokeSample);
				System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
				handles->Add(handle);

				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
				Models::ZLambda functionPointer = static_cast<ZDelegate>(callbackPtr.ToPointer());

				return functionPointer;
			}


			Models::ZMultipleLambda ModelRunner::getZMultipleDelegate()
			{
				ManagedMultipleSampleDelegate^ fp = gcnew ManagedMultipleSampleDelegate(this, &ModelRunner::invokeMultipleSamples);
				System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
				handles->Add(handle);

				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
				Models::ZMultipleLambda functionPointer = static_cast<ZMultipleDelegate>(callbackPtr.ToPointer());

				return functionPointer;
			}

			std::shared_ptr<Models::ZModel> ModelRunner::getZModel()
			{
				Models::ZLambda zLambda = getZLambda();
				Models::ZMultipleLambda zMultipleLambda = getZMultipleDelegate();

				std::shared_ptr<Models::ZModel> zModel = std::make_shared<Models::ZModel>(zLambda, zMultipleLambda);

				return zModel;
			}

			void ModelRunner::invokeSample(std::shared_ptr<Models::Sample> sample)
			{
				Sample^ sampleWrapper = gcnew Wrappers::Sample(sample);

				this->CalcZValue(sampleWrapper);
			}

			void ModelRunner::invokeMultipleSamples(std::vector<std::shared_ptr<Models::Sample>> samples)
			{
				System::Collections::Generic::List<Sample^>^ sampleWrappers = gcnew System::Collections::Generic::List<Sample^>();

				for (int i = 0; i < samples.size(); i++)
				{
					sampleWrappers->Add(gcnew Sample(samples[i]));
				}

				this->CalcZValues(sampleWrappers);
			}

			void ModelRunner::CalcZValues(System::Collections::Generic::IList<Sample^>^ samples)
			{
				this->CalcSamples(samples);
			}

			void ModelRunner::CalcZValue(Sample^ sample)
			{
				this->CalcSample(sample);
			}

			void ModelRunner::CalcSamples(System::Collections::Generic::IList<Sample^>^ samples)
			{
				for (int i = 0; i < samples->Count; i++)
				{
					CalcSample(samples[i]);
				}
			}

			void ModelRunner::CalcSample(Sample^ sample)
			{
				this->zFunction->Invoke(sample);
			}

			Sample^ ModelRunner::GetXSample(Sample^ sample)
			{
				std::shared_ptr<Models::Sample> xSample = shared->object->getXSample(sample->GetSample());

				return gcnew Sample(xSample);
			}

			Sample^ ModelRunner::GetOnlyVaryingSample(Sample^ sample)
			{
				std::shared_ptr<Models::Sample> onlyVaryingSample = shared->object->getOnlyVaryingSample(sample->GetSample());

				return gcnew Sample(onlyVaryingSample);
			}

		}
	}
}

