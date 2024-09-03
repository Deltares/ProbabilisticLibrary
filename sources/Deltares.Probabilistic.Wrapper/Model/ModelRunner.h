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
#include "../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../Deltares.Probabilistic/Model/ModelRunner.h"
#include "../../Deltares.Probabilistic/Model/Sample.h"
#include "../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Statistics/Stochast.h"
#include "../Utils/SharedPointerProvider.h"
#include "../Utils/TagRepository.h"
#include "ProgressIndicator.h"
#include "RunSettings.h"
#include "ModelSample.h"
#include "Sample.h"
#include "Evaluation.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            using namespace Deltares::Reliability;
            using namespace Deltares::Statistics::Wrappers;

            public delegate void ZSampleDelegate(ModelSample^);
            public delegate bool ShouldExitDelegate(bool finalCall);
            public delegate void RemoveTaskDelegate(int iterationIndex);

            public ref class ModelRunner
            {
            private:
                SharedPointerProvider<Models::ModelRunner>* shared = nullptr;

                std::shared_ptr<Models::ZModel> getZModel();
                ZLambda getZLambda();
                ZMultipleLambda getZMultipleDelegate();

                ZSampleDelegate^ zFunction = nullptr;

                void CalcZValues(System::Collections::Generic::IList<ModelSample^>^ samples);
                void CalcZValue(ModelSample^ sample);

                void invokeSample(std::shared_ptr<Models::ModelSample> sample);
                void invokeMultipleSamples(std::vector<std::shared_ptr<Models::ModelSample>> samples);

                Deltares::Utils::Wrappers::TagRepository^ tagRepository = gcnew TagRepository();

                System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>^ handles = gcnew System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>();

            public:
                /**
                 * \brief Constructor
                 * \remark Call Release() when done to prevent a memory leak
                 */
                ModelRunner(ZSampleDelegate^ zFunction, System::Collections::Generic::List<Stochast^>^ stochasts, CorrelationMatrix^ correlationMatrix, ProgressIndicator^ progressIndicator);
                ~ModelRunner() { this->!ModelRunner(); }
                !ModelRunner()
                {
                    delete shared;
                }

                /**
                 * \brief Releases the allocated handles
                 * \remark This method cannot be called in the destructor, because the handles to be freed prevent this object from being garbage collected
                 */
                void ReleaseHandles()
                {
                    for (int i = 0; i < handles->Count; i++)
                    {
                        handles[i].Free();
                    }

                    handles->Clear();
                    shared->object->releaseCallBacks();
                }

                System::Collections::Generic::List<Stochast^>^ Stochasts = gcnew System::Collections::Generic::List<Stochast^>();

                virtual void CalcSamples(System::Collections::Generic::IList<ModelSample^>^ samples);
                virtual void CalcSample(ModelSample^ sample);

                void SetShouldExitDelegate(ShouldExitDelegate^ shouldExitDelegate);

                void SetRemoveTaskDelegate(RemoveTaskDelegate^ removeTaskDelegate);

                virtual property int VaryingStochastCount
                {
                    int get() { return shared->object->getVaryingStochastCount(); }
                }

                bool IsVaryingStochast(int index)
                {
                    return shared->object->isVaryingStochast(index);
                }

                /**
                 * \brief Clears all evaluations and messages
                 */
                virtual void Clear()
                {
                    shared->object->clear();
                }

                /**
                 * \brief Calculates a z-value for a reliability method in .net
                 * \param sample
                 * \return
                 */
                virtual double GetZValue(Sample^ sample)
                {
                    return shared->object->getZValue(sample->GetSample());
                }

                /**
                 * \brief Calculates multiple z-values for a reliability method in .net
                 * \param samples
                 * \return
                 */
                virtual array<double>^ GetZValues(System::Collections::Generic::List<Sample^>^ samples)
                {
                    std::vector<std::shared_ptr<Models::Sample>> nativeSamples(samples->Count);

                    for (int i = 0; i < samples->Count; i++)
                    {
                        nativeSamples[i] = samples[i]->GetSample();
                    }

                    std::vector<double> zValues = shared->object->getZValues(nativeSamples);

                    return NativeSupport::toManaged(zValues);
                }

                RunSettings^ Settings = gcnew RunSettings();

                virtual void InitializeForRun()
                {
                    shared->object->initializeForRun();
                }

                ModelSample^ GetModelSample(Sample^ sample);

                array<double>^ GetOnlyVaryingValues(array<double>^ values);

                int SetTag(System::Object^ object)
                {
                    return tagRepository->RegisterTag(object);
                }

                void AssignTag(Wrappers::Evaluation^ evaluation)
                {
                    evaluation->AssignTag(this->tagRepository);
                }

                std::shared_ptr<Models::ModelRunner> GetModelRunner()
                {
                    shared->object->Settings = this->Settings->GetSettings();
                    return shared->object;
                }
            };
        }
    }
}




