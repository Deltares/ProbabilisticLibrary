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

            public ref class Sample
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Models::Sample>* shared = nullptr;
                array<double>^ values = nullptr;
                bool dirty = false;
            public:
                Sample(std::shared_ptr<Models::Sample> sample)
                {
                    this->shared = new Utils::Wrappers::SharedPointerProvider(sample);
                    this->SpaceType = Wrappers::SpaceType::U;
                }

                Sample(array<double>^ values)
                {
                    std::vector<double> nativeValues = Utils::Wrappers::NativeSupport::toNative(values);

                    Models::Sample* sample = new Models::Sample(nativeValues);

                    shared = new Utils::Wrappers::SharedPointerProvider(sample);
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
                            values = Utils::Wrappers::NativeSupport::toManaged(shared->object->Values);
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

                // TODO: Next methods and properties should be removed proxies have been converted

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

                Sample^ Clone()
                {
                    UpdateValues();
                    return gcnew Sample(shared->object->clone());
                }
            };
        }
    }
}


