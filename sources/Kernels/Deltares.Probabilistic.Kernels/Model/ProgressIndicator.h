#pragma once
#include <string>

namespace Deltares
{
	namespace Models
	{
		enum ProgressType { Global, Detailed };

		typedef void(__stdcall* ProgressDelegate) (double);
		typedef void(__stdcall* DetailedProgressDelegate) (int, int, double, double);
		typedef void(__stdcall* TextualProgressDelegate) (ProgressType, std::string);

		class ProgressIndicator
		{
		private:
			ProgressDelegate progressDelegate;
			DetailedProgressDelegate detailedProgressDelegate;
			TextualProgressDelegate textualProgressDelegate;

		public:
			ProgressIndicator(ProgressDelegate progressDelegate, DetailedProgressDelegate detailedProgressDelegate = nullptr, TextualProgressDelegate textualProgressDelegate = nullptr)
			{
				this->progressDelegate = progressDelegate;
				this->detailedProgressDelegate = detailedProgressDelegate;
				this->textualProgressDelegate = textualProgressDelegate;
			}
			void doProgress(double progress)
			{
				if (progressDelegate != nullptr) progressDelegate(progress);
			}
			void doDetailedProgress(int step, int loop, double reliability, double convergence)
			{
				if (detailedProgressDelegate != nullptr) detailedProgressDelegate(step, loop, reliability, convergence);
			}
			void doTextualProgress(ProgressType progressType, std::string text)
			{
				if (textualProgressDelegate != nullptr) textualProgressDelegate(progressType, text);
			}
		};
	}
}

