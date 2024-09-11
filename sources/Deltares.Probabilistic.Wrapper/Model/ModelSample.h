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
#include "../Utils/SharedPointerProvider.h"
#include "../../Deltares.Probabilistic/Model/ModelSample.h"


namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            using namespace Deltares::Models;
            using namespace Deltares::Utils::Wrappers;

            /**
             * \brief Sample with values defined in x-space
             */
            public ref class ModelSample
            {
            private:
                array<double>^ values = nullptr;
                SharedPointerProvider<Models::ModelSample>* shared = nullptr;

            public:
                ModelSample(std::shared_ptr<Models::ModelSample> sample)
                {
                    shared = new SharedPointerProvider(sample);
                    this->values = NativeSupport::toManaged(sample->Values);
                }

                ModelSample(array<double>^ values)
                {
                    std::vector<double> nativeValues = NativeSupport::toNative(values);
                    shared = new SharedPointerProvider(new Models::ModelSample(nativeValues));
                    this->values = NativeSupport::toManaged(shared->object->Values);
                }

                ~ModelSample() { this->!ModelSample(); }
                !ModelSample()
                {
                    delete shared;
                }

                void SetNativeModelSample(const std::shared_ptr<Models::ModelSample> nativeModelSample)
                {
                    shared = new SharedPointerProvider(nativeModelSample);

                    if (this->values->Length == shared->object->Values.size())
                    {
                        for (int i = 0; i < this->values->Length; i++)
                        {
                            this->values[i] = shared->object->Values[i];
                        }
                    }
                    else
                    {
                        this->values = NativeSupport::toManaged(shared->object->Values);
                    }
                }

                property array<double>^ Values
                {
                    array<double>^ get() { return values; }
                }

                property double Beta
                {
                    double get() { return shared->object->Beta; }
                }

                property double Weight
                {
                    double get() { return shared->object->Weight; }
                }

                property int Iteration
                {
                    int get() { return shared->object->IterationIndex; }
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

                bool AreValuesEqual(ModelSample^ other)
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

                /// <summary>
                /// Reference to an object to be attached to the sample
                /// </summary>
                property int Tag
                {
                    int get()
                    {
                        return shared->object->Tag;
                    }
                    void set(int value)
                    {
                        shared->object->Tag = value;
                    }
                }

                /// <summary>
                /// Gets the native underlying object
                /// </summary>
                std::shared_ptr<Models::ModelSample> GetModelSample()
                {
                    return shared->object;
                }
            };
        }
    }
}


