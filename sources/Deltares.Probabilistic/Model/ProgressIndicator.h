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
#pragma once
#include <string>
#include <functional>

namespace Deltares::Models
{
    enum ProgressType { Global, Detailed };

#ifdef _WIN32
    typedef void(__stdcall* ProgressDelegate) (double);
    typedef void(__stdcall* DetailedProgressDelegate) (int, int, double, double);
    typedef void(__stdcall* TextualProgressDelegate) (ProgressType, std::string);
#endif
    typedef std::function<void(double)> ProgressLambda;
    typedef std::function<void(int, int, double, double)> DetailedProgressLambda;
    typedef std::function<void(ProgressType, const char* message)> TextualProgressLambda;

    class ProgressIndicator
    {
        const ProgressLambda& progressLambda;
        const DetailedProgressLambda& detailedProgressLambda;
        const TextualProgressLambda& textualProgressLambda;

        double progressOffset = 0.0;
        double progressFactor = 1.0;
        std::string task;

    public:
        explicit ProgressIndicator(const ProgressLambda& progress_lambda,
            const DetailedProgressLambda& detailed_progress_lambda = nullptr,
            const TextualProgressLambda& textual_progress_lambda = nullptr,
            std::string _task = "") :
        progressLambda(progress_lambda), detailedProgressLambda(detailed_progress_lambda),
        textualProgressLambda(textual_progress_lambda), task(std::move(_task))
        {
        }

        void doProgress(double progress) const
        {
            if (progressLambda != nullptr) progressLambda(progressOffset + progressFactor * progress);
        }

        void doDetailedProgress(int step, int loop, double reliability, double convergence) const
        {
            if (detailedProgressLambda != nullptr) detailedProgressLambda(step, loop, reliability, convergence);
        }

        void doTextualProgress(ProgressType progressType, const std::string& text) const
        {
            if (textualProgressLambda != nullptr) textualProgressLambda(progressType, text.c_str());
        }

        void reset();
        void initialize(double factor, double offset);
        void complete() const;
        void increaseOffset();
        void setTask(const std::string& newTask);
    };
}

