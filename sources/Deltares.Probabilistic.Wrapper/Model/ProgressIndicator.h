#pragma once

#include "../../Deltares.Probabilistic/Model/ProgressIndicator.h"
#include "../Utils/SharedPointerProvider.h"
#include "../Utils/NativeSupport.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			public enum ProgressTextType {Global, Detailed};

			public delegate void ProgressDelegate(double);
			public delegate void DetailedProgressDelegate(int, int, double, double);
			public delegate void TextualProgressDelegate(ProgressTextType, System::String^ text);

			public ref class ProgressIndicator
			{

			public:
				ProgressIndicator(ProgressDelegate^ progressDelegate, DetailedProgressDelegate^ detailedProgressDelegate, TextualProgressDelegate^ textualProgressDelegate);
				~ProgressIndicator() { this->!ProgressIndicator(); }
				!ProgressIndicator()
				{
					for (int i = 0; i < handles->Count; i++)
					{
						handles[i].Free();
					}
					delete shared;
				}

				void DoProgress(double progress);
				void DoDetailedProgress(int step, int loop, double reliability, double convergence);
				void DoTextualProgress(Models::ProgressType progressType, std::string text);

				void Reset()
				{
					shared->object->reset();
				}

				void Complete()
				{
					shared->object->complete();
				}

				void Initialize(double factor, double offset)
				{
					shared->object->initialize(factor, offset);
				}

				void SetTask(System::String^ task)
				{
					shared->object->setTask(Utils::Wrappers::NativeSupport::toNative(task));
				}

				std::shared_ptr<Models::ProgressIndicator> GetProgressIndicator()
				{
					return shared->object;
				}

			private:
				ProgressDelegate^ progressDelegate;
				DetailedProgressDelegate^ detailedProgressDelegate;
				TextualProgressDelegate^ textualProgressDelegate;

				Utils::Wrappers::SharedPointerProvider<Deltares::Models::ProgressIndicator>* shared = nullptr;

				Models::ProgressLambda getProgressLambda();
				Models::DetailedProgressLambda getDetailedProgressLambda();
				Models::TextualProgressLambda getTextualProgressLambda();

				ProgressTextType GetProgressType(Models::ProgressType progressType);

				System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>^ handles = gcnew System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>();
			};
		}
	}
}



