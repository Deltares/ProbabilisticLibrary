// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
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
        };
    }
}

