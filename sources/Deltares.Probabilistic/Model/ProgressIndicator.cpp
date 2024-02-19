#include "ProgressIndicator.h"

namespace Deltares
{
    namespace Models
    {
        void ProgressIndicator::reset()
        {
            this->doProgress(0);
            this->doTextualProgress(ProgressType::Detailed, "");
            this->task = "";
        }

        void ProgressIndicator::initialize(double factor, double offset)
        {
            progressFactor = factor;
            progressOffset = offset;
        }

        /// <summary>
        /// Sets the progress to completed
        /// </summary>
        void ProgressIndicator::complete()
        {
            this->doProgress(1);
            this->doTextualProgress(ProgressType::Detailed, "");
        }

        void ProgressIndicator::increaseOffset()
        {
            this->progressOffset += progressFactor;
        }

        void ProgressIndicator::setTask(std::string task)
        {
            this->task = task;
        }

        ProgressIndicator* ProgressIndicator::getSubIndicator(std::string subTask)
        {
            return new ProgressIndicator(this->progressLambda, this->detailedProgressLambda, this->textualProgressLambda, task + subTask);
        }
    }
}
