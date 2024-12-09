// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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
#include "../Utils/NativeSupport.h"
#include "../../Deltares.Probabilistic/Model/Sample.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            /// <summary>
            /// Indicates the meaning of <see cref="Sample.Values"/> in a <see cref="Sample"/>
            /// </summary>
            public enum class SpaceType { U, X };

            using namespace Deltares::Models;
            using namespace Deltares::Utils::Wrappers;

            public ref class Sample
            {
            public:
                Sample(std::shared_ptr<Models::Sample> sample)
                {
                    this->shared = new SharedPointerProvider(sample);
                    this->SpaceType = Wrappers::SpaceType::U;
                }

                Sample(array<double>^ values)
                {
                    std::vector<double> nativeValues = NativeSupport::toNative(values);

                    Models::Sample* sample = new Models::Sample(nativeValues);

                    shared = new SharedPointerProvider(sample);
                    this->values = values;
                }

                ~Sample() { this->!Sample(); }
                !Sample()
                {
                    delete shared;
                }

                property array<double>^ Values
                {
                    array<double>^ get()
                    {
                        if (values == nullptr)
                        {
                            values = NativeSupport::toManaged(shared->object->Values);
                        }

                        dirty = true;

                        return values;
                    }
                }

                property double Weight
                {
                    double get() { return shared->object->Weight; }
                    void set(double value) { shared->object->Weight = value; }
                }

                property int Iteration
                {
                    int get() { return shared->object->IterationIndex; }
                    void set(int value) { shared->object->IterationIndex = value; }
                }

                property bool AllowProxy
                {
                    bool get() { return shared->object->AllowProxy; }
                    void set(bool value) { shared->object->AllowProxy = value; }
                }

                property bool IsRestartRequired
                {
                    bool get() { return shared->object->IsRestartRequired; }
                    void set(bool value) { shared->object->IsRestartRequired = value; }
                }

                property double Z
                {
                    double get() { return shared->object->Z; }
                    void set(double value) { shared->object->Z = value; }
                }

                std::shared_ptr<Models::Sample> GetSample()
                {
                    if (dirty)
                    {
                        this->UpdateValues();
                    }

                    return shared->object;
                }

                // TODO: PROBL-42 next methods and properties should be removed after after c++ conversion

                Sample(SpaceType spaceType, int count) : Sample(spaceType, -1, count)
                {
                }

                Sample(SpaceType spaceType, int iteration, int count) : Sample(spaceType, iteration, gcnew array<double>(count))
                {
                }

                Sample(SpaceType spaceType, array<double>^ values) : Sample(spaceType, -1, values)
                {
                }

                Sample(SpaceType spaceType, int iteration, array<double>^ values) : Sample(values)
                {
                    this->SpaceType = spaceType;
                    this->Iteration = iteration;
                }

                /// <summary>
                /// The space type in which the <see cref="Values"/> are defined
                /// </summary>
                SpaceType SpaceType = SpaceType::U;

                double Factor = 0;

                Sample^ GetSampleAtBeta(double beta)
                {
                    UpdateValues();
                    return gcnew Sample(shared->object->getSampleAtBeta(beta));
                }

                Sample^ GetNormalizedSample()
                {
                    UpdateValues();
                    return gcnew Sample(shared->object->getNormalizedSample());
                }

                Sample^ GetMultipliedSample(double factor)
                {
                    UpdateValues();
                    return gcnew Sample(shared->object->getMultipliedSample(factor));
                }

                Sample^ Clone()
                {
                    UpdateValues();
                    return gcnew Sample(shared->object->clone());
                }

                int ScenarioIndex = -1;

                void UpdateValues()
                {
                    if (values != nullptr)
                    {
                        // synchronize values
                        for (int i = 0; i < values->Length; i++)
                        {
                            shared->object->Values[i] = values[i];
                        }
                    }

                    dirty = false;
                }

                // TODO: PROBL-42 remove after c++ conversion
                property double Beta
                {
                    double get()
                    {
                        UpdateValues();
                        return shared->object->getBeta();
                    }
                }

                /// <summary>
                /// Gets the squared distance to another sample
                /// </summary>
                /// <param name="other"></param>
                /// <returns></returns>
                double GetDistance2(Sample^ other, bool hasWeighting)
                {
                    double sum = 0;
                    for (int j = 0; j < other->Values->Length; ++j)
                    {
                        double weight = 1;
                        if (hasWeighting) weight = other->Weight;
                        double diff = (this->Values[j] - other->Values[j]) * weight;
                        sum += diff * diff;
                    }

                    return sum;
                }

                bool AreValuesEqual(Sample^ other)
                {
                    if (this == other)
                    {
                        return true;
                    }

                    if (this->Values->Length != this->Values->Length)
                    {
                        return false;
                    }

                    for (int i = 0; i < this->Values->Length; i++)
                    {
                        if (!isnan(this->Values[i]) || !isnan(other->Values[i]))
                        {
                            if (this->Values[i] != other->Values[i])
                            {
                                return false;
                            }
                        }
                    }

                    return true;
                }

            private:
                array<double>^ values = nullptr;
                System::Object^ tag = nullptr;
                SharedPointerProvider<Models::Sample>* shared = nullptr;
                bool dirty = false;
            };
        }
    }
}


