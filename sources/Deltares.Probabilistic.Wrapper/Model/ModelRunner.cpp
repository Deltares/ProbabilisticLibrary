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
#include "ModelRunner.h"

#include "ProgressIndicator.h"
#include "ModelSample.h"
#include "../../Deltares.Probabilistic/Model/Sample.h"
#include "../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares
{
    namespace Models
    {
        namespace Wrappers
        {
            using namespace Deltares::Models;

            delegate void ManagedSampleDelegate(std::shared_ptr<Models::ModelSample> sample);
            delegate void ManagedMultipleSampleDelegate(std::vector<std::shared_ptr<Models::ModelSample>> samples);
            typedef void(__stdcall* ZDelegate) (std::shared_ptr<Models::ModelSample>);
            typedef void(__stdcall* ZMultipleDelegate) (std::vector<std::shared_ptr<Models::ModelSample>>);
            typedef bool(__stdcall* ShouldExitNativeDelegate) (bool finalCall);
            typedef void(__stdcall* RemoveTaskNativeDelegate) (int iterationIndex);

            ModelRunner::ModelRunner(ZSampleDelegate^ zFunction, System::Collections::Generic::List<Stochast^>^ stochasts, CorrelationMatrix^ correlationMatrix, ProgressIndicator^ progressIndicator)
            {
                this->zFunction = zFunction;

                this->Stochasts->AddRange(stochasts);

                std::vector<std::shared_ptr<Statistics::Stochast> > nativeStochasts;

                for (int i = 0; i < stochasts->Count; i++)
                {
                    nativeStochasts.push_back(stochasts[i]->GetStochast());
                }

                const std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(nativeStochasts, correlationMatrix->GetCorrelationMatrix());

                const std::shared_ptr<Models::ZModel> zModel = getZModel();

                const std::shared_ptr<Models::ProgressIndicator> progress = progressIndicator != nullptr ? progressIndicator->GetProgressIndicator() : nullptr;

                this->shared = new SharedPointerProvider(new Models::ModelRunner(zModel, uConverter, progress));
            }

            Models::ZLambda ModelRunner::getZLambda()
            {
                ManagedSampleDelegate^ fp = gcnew ManagedSampleDelegate(this, &ModelRunner::invokeSample);
                System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
                handles->Add(handle);

                System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
                Models::ZLambda functionPointer = static_cast<ZDelegate>(callbackPtr.ToPointer());

                return functionPointer;
            }


            Models::ZMultipleLambda ModelRunner::getZMultipleDelegate()
            {
                ManagedMultipleSampleDelegate^ fp = gcnew ManagedMultipleSampleDelegate(this, &ModelRunner::invokeMultipleSamples);
                System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
                handles->Add(handle);

                System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
                ZMultipleLambda functionPointer = static_cast<ZMultipleDelegate>(callbackPtr.ToPointer());

                return functionPointer;
            }

            std::shared_ptr<ZModel> ModelRunner::getZModel()
            {
                ZLambda zLambda = getZLambda();
                ZMultipleLambda zMultipleLambda = getZMultipleDelegate();

                std::shared_ptr<ZModel> zModel = std::make_shared<ZModel>(zLambda, zMultipleLambda);

                return zModel;
            }

            void ModelRunner::invokeSample(std::shared_ptr<Models::ModelSample> sample)
            {
                ModelSample^ sampleWrapper = gcnew ModelSample(sample);
                this->CalcZValue(sampleWrapper);
            }

            /**
             * \brief Callback method for calculating samples in .net invoked by a native algorithm
             * \param samples 
             */
            void ModelRunner::invokeMultipleSamples(std::vector<std::shared_ptr<Models::ModelSample>> samples)
            {
                System::Collections::Generic::List<ModelSample^>^ sampleWrappers = gcnew System::Collections::Generic::List<Wrappers::ModelSample^>();

                for (int i = 0; i < samples.size(); i++)
                {
                    sampleWrappers->Add(gcnew ModelSample(samples[i]));
                }

                this->CalcZValues(sampleWrappers);
            }

            void ModelRunner::SetShouldExitDelegate(ShouldExitDelegate^ shouldExitDelegate)
            {
                System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(shouldExitDelegate);
                handles->Add(handle);

                System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(shouldExitDelegate);
                Models::ShouldExitLambda functionPointer = static_cast<ShouldExitNativeDelegate>(callbackPtr.ToPointer());

                shared->object->setShouldExitFunction(functionPointer);
            }

            void ModelRunner::SetRemoveTaskDelegate(RemoveTaskDelegate^ removeTaskDelegate)
            {
                System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(removeTaskDelegate);
                handles->Add(handle);

                System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(removeTaskDelegate);
                Models::RemoveTaskLambda functionPointer = static_cast<RemoveTaskNativeDelegate>(callbackPtr.ToPointer());

                shared->object->setRemoveTaskFunction(functionPointer);
            }

            void ModelRunner::CalcZValues(System::Collections::Generic::IList<ModelSample^>^ samples)
            {
                this->CalcSamples(samples);
            }

            void ModelRunner::CalcZValue(ModelSample^ sample)
            {
                this->CalcSample(sample);
            }

            void ModelRunner::CalcSamples(System::Collections::Generic::IList<ModelSample^>^ samples)
            {
                for (int i = 0; i < samples->Count; i++)
                {
                    CalcSample(samples[i]);
                }
            }

            void ModelRunner::CalcSample(ModelSample^ sample)
            {
                this->zFunction->Invoke(sample);
            }

            ModelSample^ ModelRunner::GetModelSample(Sample^ sample)
            {
                std::shared_ptr<Models::ModelSample> xSample = shared->object->getModelSample(sample->GetSample());

                return gcnew ModelSample(xSample);
            }

            array<double>^ ModelRunner::GetOnlyVaryingValues(array<double>^ values)
            {
                return NativeSupport::toManaged(shared->object->getOnlyVaryingValues(NativeSupport::toNative(values)));
            }
        }
    }
}

