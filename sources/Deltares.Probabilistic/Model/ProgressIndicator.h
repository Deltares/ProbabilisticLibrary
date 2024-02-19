#pragma once
#include <string>
#include <functional>

namespace Deltares
{
	namespace Models
	{
		enum ProgressType { Global, Detailed };

#ifdef _WIN32
		typedef void(__stdcall* ProgressDelegate) (double);
		typedef void(__stdcall* DetailedProgressDelegate) (int, int, double, double);
		typedef void(__stdcall* TextualProgressDelegate) (ProgressType, std::string);
#endif
		typedef std::function<void(double)> ProgressLambda;
		typedef std::function<void(int, int, double, double)> DetailedProgressLambda;
		typedef std::function<void(ProgressType, std::string)> TextualProgressLambda;

		class ProgressIndicator
		{
		private:
			ProgressLambda progressLambda;
			DetailedProgressLambda detailedProgressLambda;
			TextualProgressLambda textualProgressLambda;

		    double progressOffset = 0;
			double progressFactor = 1;
			std::string task = "";

		public:
			ProgressIndicator(ProgressLambda progressLambda, DetailedProgressLambda detailedProgressLambda = nullptr, TextualProgressLambda textualProgressLambda = nullptr, std::string task = "")
			{
				this->progressLambda = progressLambda;
				this->detailedProgressLambda = detailedProgressLambda;
				this->textualProgressLambda = textualProgressLambda;
				this->task = task;
			}

			void doProgress(double progress)
			{
				if (progressLambda != nullptr) progressLambda(progressOffset + progressFactor * progress);
			}
			void doDetailedProgress(int step, int loop, double reliability, double convergence)
			{
				if (detailedProgressLambda != nullptr) detailedProgressLambda(step, loop, reliability, convergence);
			}
			void doTextualProgress(ProgressType progressType, std::string text)
			{
				if (textualProgressLambda != nullptr) textualProgressLambda(progressType, text);
			}

			void reset();
			void initialize(double factor, double offset);
			void complete();
			void increaseOffset();
			void setTask(std::string task);

			ProgressIndicator* getSubIndicator(std::string subTask);
		};
	}
}

