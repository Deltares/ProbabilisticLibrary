#pragma once
#include "ICanCalculateBeta.h"
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
                ZBetaLambda getZBetaLambda();

                ZSampleDelegate^ zFunction = nullptr;

                void CalcZValues(System::Collections::Generic::IList<ModelSample^>^ samples);
                void CalcZValue(ModelSample^ sample);

                void invokeSample(std::shared_ptr<Models::ModelSample> sample);
                void invokeMultipleSamples(std::vector<std::shared_ptr<Models::ModelSample>> samples);
                double invokeBetaSample(std::shared_ptr<Models::ModelSample> sample, double beta);

                ICanCalculateBeta^ directionModel = nullptr;

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
                void Release()
                {
                    for (int i = 0; i < handles->Count; i++)
                    {
                        handles[i].Free();
                    }

                    handles->Clear();
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

            protected:
                virtual void SetDirectionModel(ICanCalculateBeta^ directionModel);

            private:
                /**
                 * \brief List of reusable samples
                 */
                System::Collections::Generic::List<ModelSample^>^ reusableSamples = gcnew System::Collections::Generic::List<ModelSample^>();
                int reusableModelSampleIndex = 0;

                ModelSample^ GetModelSample(std::shared_ptr<Models::ModelSample> sample);

                void Reset()
                {
                    reusableModelSampleIndex = 0;
                }
            };
        }
    }
}




