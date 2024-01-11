#pragma once

#include "../Model/ProgressIndicator.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public enum ProgressTextType {Global, Detailed};

			public delegate void ProgressDelegate(double);
			public delegate void DetailedProgressDelegate(int, int, double, double);
			public delegate void TextualProgressDelegate(ProgressTextType, System::String^ text);

			public ref class ProgressIndicatorWrapper
			{
			private:
				ProgressDelegate^ progressDelegate;
				DetailedProgressDelegate^ detailedProgressDelegate;
				TextualProgressDelegate^ textualProgressDelegate;

				Models::ProgressIndicator* progressIndicator;
				Models::ProgressDelegate getProgressDelegate();
				Models::DetailedProgressDelegate getDetailedProgressDelegate();
				Models::TextualProgressDelegate getTextualProgressDelegate();

				void doProgress(double progress);
				void doDetailedProgress(int step, int loop, double reliability, double convergence);
				void doTextualProgress(Models::ProgressType progressType, std::string text);

				ProgressTextType getProgressType(Models::ProgressType progressType);

				System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>^ handles = gcnew System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>();

			public:
				ProgressIndicatorWrapper(ProgressDelegate^ progressDelegate, DetailedProgressDelegate^ detailedProgressDelegate, TextualProgressDelegate^ textualProgressDelegate);
				~ProgressIndicatorWrapper() { this->!ProgressIndicatorWrapper(); }
				!ProgressIndicatorWrapper()
				{
					for (int i = 0; i < handles->Count; i++)
					{
						handles[i].Free();
					}
					delete progressIndicator;
				}

				Models::ProgressIndicator* getProgressIndicator()
				{
					return progressIndicator;
				}
			};
		}
	}
}



