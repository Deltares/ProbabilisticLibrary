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

#include <cmath>
#include <functional>
#include <vector>
#include <memory>

namespace Deltares::Models
{
    /**
     * \brief Combination of values defined in u-space in a reliability algorithm
     */
    class Sample
    {
    private:
        int size = 0;
        bool extended = false;

    public:
        Sample() = default;

        Sample(size_t size, bool extended = false)
        {
            this->size = static_cast<int>(size);
            this->extended = extended;
            for (size_t i = 0; i < size; i++)
            {
                Values.push_back(0.0);
            }
        }

        Sample(int size, bool extended = false)
        {
            this->size = size;
            this->extended = extended;
            for (int i = 0; i < size; i++)
            {
                Values.push_back(0.0);
            }
        }

        Sample(std::vector<double> values, bool extended = false)
        {
            this->size = static_cast<int>(values.size());
            this->Values = values;
            this->extended = extended;
        }

        /**
         * \brief Resets all contents of the sample to its default values
         */
        void clear()
        {
            for (size_t i = 0; i < Values.size(); i++)
            {
                Values[i] = 0;
            }

            IterationIndex = -1;
            threadId = 0;
            Weight = std::nan("");
            AllowProxy = true;
            IsRestartRequired = false;
            Z = std::nan("");
        }

        std::vector<double> Values;

        int IterationIndex = -1;
        int threadId = 0;
        double Weight = std::nan("");
        bool AllowProxy = true;
        bool IsRestartRequired = false;
        double Z = nan("");

        /**
         * \brief Indicates whether the last value is an artificial value indicating the probability. This value does not correspond with an input variable of a model.
         */
        bool IsExtended() const { return extended; }

        int getSize() const;

        double getBeta() const;
        double getDistance(Sample& other) const;
        double getDistance2(Sample& other) const;
        void setInitialValues(double beta);
        Sample clone() const;
        Sample getNormalizedSample() const { return getSampleAtBeta(1.0); }
        Sample getSampleAtBeta(double beta) const;
        Sample getMultipliedSample(double factor) const;

        /**
         * \brief Gets a sample with one additional u-value
         * \param extendedUValue The u value which will be appended to the list of values
         * \return Extended sample
         */
        Sample getExtendedSample(double extendedUValue) const;

        /**
         * \brief Gets a sample where the last value from the list of values is removed
         * \return Reduced sample
         */
        Sample getReducedSample() const;

        void correctSmallValues(double tolerance = 1E-10);
        bool areValuesEqual(Sample& other);

        /**
         * \brief Performs an operation on a sample resulting in a numeric value for a collection of samples
         * \param samples Collection of samples
         * \param function Operation on a sample
         * \return Resulting numeric values
         */
        static std::vector<double> select(std::vector<Sample>& samples, std::function<double(Sample)> function)
        {
            std::vector<double> result(samples.size());

            for (size_t i = 0; i < samples.size(); i++)
            {
                result[i] = function(samples[i]);
            }

            return result;
        }

        /**
         * \brief Performs an operation on a sample resulting in a numeric value for a collection of samples
         * \param samples Collection of samples
         * \param function Operation on a sample
         * \return Resulting numeric values
         */
        static std::vector<double> select(std::vector<Sample*>& samples, std::function<double(Sample*)> function)
        {
            std::vector<double> result(samples.size());

            for (size_t i = 0; i < samples.size(); i++)
            {
                result[i] = function(samples[i]);
            }

            return result;
        }
    };

    class SampleStorage
    {
    private:
        std::vector<Sample> samples;

    public:
        SampleStorage() = default;

        SampleStorage(size_t size)
        {
            samples.reserve(size);
        }

        void reserve(size_t size)
        {
            samples.reserve(size);
        }

        Sample* keep(Sample& sample)
        {
            samples.push_back(sample);
            return &samples.back();
        }

        void clear()
        {
            samples.clear();
        }
    };
}

