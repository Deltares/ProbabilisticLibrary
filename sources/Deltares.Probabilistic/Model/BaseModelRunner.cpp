// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "BaseModelRunner.h"
#include "ModelSample.h"
#include "../Math/NumericSupport.h"

#include <cmath>
#include <format>

namespace Deltares::Models
{

    void BaseModelRunner::initializeForRun()
    {
        this->zModel->setMaxProcesses(this->Settings->MaxParallelProcesses);
        this->zModel->setHandleInvalidType(this->Settings->handleInvalidType);
        this->zModel->setAllowRepository(this->Settings->AllowRepository);
        this->zModel->setUseZFromSample(this->Settings->UseZFromSample);
        this->zModel->resetModelRuns();

        this->zModel->initializeForRun();

        if (!this->Settings->ReuseCalculations)
        {
            this->zModel->clearRepository();
        }

        if (this->locker == nullptr)
        {
            this->locker = new Utils::Locker();
        }
    }

    void BaseModelRunner::clear()
    {
        clearLists();
    }

    void BaseModelRunner::clearLists()
    {
        this->evaluations.clear();
        this->messages.clear();
    }

    /**
     * \brief Indicates whether the sample repository is allowed
     * \param allowRepository Indication
     */
    void BaseModelRunner::setAllowRepository(bool allowRepository) const
    {
        this->zModel->setAllowRepository(allowRepository);
    }

    Evaluation BaseModelRunner::getEvaluationFromSample(ModelSample& sample) const
    {
        Evaluation evaluation = Evaluation();

        evaluation.Z = sample.Z;
        evaluation.Beta = sample.Beta;
        evaluation.Iteration = sample.IterationIndex;
        evaluation.Weight = sample.Weight;
        evaluation.usedProxy = sample.UsedProxy;
        evaluation.InputValues = sample.Values;
        evaluation.OutputValues = sample.OutputValues;
        evaluation.Tag = sample.Tag;

        return evaluation;
    }

    int BaseModelRunner::getOutputParametersSize() const
    {
        return this->zModel->outputParameters.size();
    }

    /**
     * \brief Registers an evaluation for a calculated sample
     * \param sample Calculated sample
     */
    void BaseModelRunner::registerEvaluation(ModelSample& sample)
    {
        if (this->Settings->SaveEvaluations)
        {
            std::shared_ptr<Evaluation> evaluation = std::make_shared<Evaluation>(getEvaluationFromSample(sample));

            if (this->Settings->MaxParallelProcesses > 1)
            {
                locker->lock();
                this->evaluations.push_back(evaluation);
                locker->unlock();
            }
            else
            {
                this->evaluations.push_back(evaluation);
            }
        }
    }

    bool BaseModelRunner::canProgress() const
    {
        return progressIndicator != nullptr;
    }


    void BaseModelRunner::reportProgress(int step, int maxSteps, double reliability, double convergence) const
    {
        if (this->progressIndicator != nullptr)
        {
            const double progress = Numeric::NumericSupport::Divide(step, maxSteps);
            this->progressIndicator->doProgress(progress);

            auto text = std::format("{}/{}", step, maxSteps);

            if (!std::isnan(reliability))
            {
                text += std::format(", Reliability = {:.3f}", reliability);
            }

            if (!std::isnan(convergence))
            {
                text += std::format(", Convergence = {:.3f}", convergence);
            }

            this->progressIndicator->doTextualProgress(ProgressType::Detailed, text);
        }
    }

    void BaseModelRunner::reportMessage(Logging::MessageType type, std::string text)
    {
        if (Settings->SaveMessages && this->messages.size() < static_cast<size_t>(this->Settings->MaxMessages) && type >= this->Settings->LowestMessageType)
        {
            this->messages.push_back(std::make_shared<Logging::Message>(type, text));
        }
    }

    void BaseModelRunner::reportDetailedProgress(int step, int loop, double reliability, double convergence) const
    {
        if (progressIndicator != nullptr)
        {
            this->progressIndicator->doDetailedProgress(step, loop, reliability, convergence);
        }
    }

    void BaseModelRunner::doTextualProgress(ProgressType type, const std::string& text) const
    {
        if (this->progressIndicator != nullptr)
        {
            this->progressIndicator->doTextualProgress(type, text);
        }
    }

    int BaseModelRunner::getModelRuns() const
    {
        int runs = this->zModel->getModelRuns();
        this->zModel->resetModelRuns();
        return runs;
    }

    void BaseModelRunner::invoke(ModelSample& sample) const
    {
        this->zModel->invoke(sample);
    }

    void BaseModelRunner::invoke(std::vector<ModelSample*>& samples) const
    {
        this->zModel->invoke(samples);
    }

    /**
     * \brief Assigns the collected evaluations and messages to given collections
     * \param evaluations The evaluations to assign the collected evaluations to
     * \param messages The messages to assign the collected messages to
     */
    void BaseModelRunner::CollectMessages(std::vector<std::shared_ptr<Evaluation>>& evaluations, std::vector<std::shared_ptr<Logging::Message>>& messages) const
    {
        for (const auto& evaluation : this->evaluations)
        {
            evaluations.push_back(evaluation);
        }

        for (const auto& message : this->messages)
        {
            messages.push_back(message);
        }
    }
}



