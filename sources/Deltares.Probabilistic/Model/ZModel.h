#pragma once
#include <limits>
#include <vector>
#include <functional>

#include "ModelParameter.h"
#include "ModelSample.h"
#include "RunSettings.h"

namespace Deltares
{
    namespace Models
    {
        typedef std::function<void(std::shared_ptr<ModelSample>)> ZLambda;
        typedef std::function<void(std::vector<std::shared_ptr<ModelSample>>)> ZMultipleLambda;
        typedef std::function<double(std::shared_ptr<ModelSample>, double beta)> ZBetaLambda;

        typedef double (*ZValuesCallBack)(double* data, int size);

        typedef void (*ZEmptyCallBack)();

        class ZModel
        {
        public:
            ZModel(ZLambda zLambda, ZMultipleLambda zMultipleLambda = nullptr)
            {
                this->zLambda = zLambda;
                this->zMultipleLambda = zMultipleLambda;
            }

            ZModel(ZValuesCallBack zValuesLambda)
            {
                ZLambda calcValuesLambda = [zValuesLambda](std::shared_ptr<ModelSample> sample)
                {
                    sample->Z = (*zValuesLambda)(sample->Values.data(), (int)sample->Values.size());
                };

                this->zLambda = calcValuesLambda;
            }

            /**
             * \brief Name of the model
             */
            std::string Name = "";

            /**
             * \brief The index of the underlying model values if the model returns an array or tuple
             */
            int Index = 0;

            void setBetaLambda(ZBetaLambda zBetaLambda)
            {
                this->zBetaLambda = zBetaLambda;
            }

            void releaseCallBacks();

            void setMaxProcesses(int maxProcesses);

            void invoke(std::shared_ptr<ModelSample> sample);

            void invoke(std::vector<std::shared_ptr<ModelSample>> samples);

            double getBeta(std::shared_ptr<ModelSample> sample, double beta);

            bool canCalculateBeta()
            {
                return this->zBetaLambda != nullptr;
            }

            int getModelRuns()
            {
                return modelRuns;
            }

            void resetModelRuns()
            {
                modelRuns = 0;
            }

            /**
             * \brief Gets the name corresponding to the Index in this class
             */
            std::string getIndexedName()
            {
                if (this->Index < this->outputParameters.size())
                {
                    return this->outputParameters[this->Index]->name;
                }
                else
                {
                    return this->Name;
                }
            }

            std::vector<std::shared_ptr<ModelParameter>> inputParameters;
            std::vector<std::shared_ptr<ModelParameter>> outputParameters;

        private:
            ZLambda zLambda = nullptr;
            ZMultipleLambda zMultipleLambda = nullptr;
            ZBetaLambda zBetaLambda = nullptr;
            int maxProcesses = 1;
            int modelRuns = 0;
            bool countRunsLambda = true;
        };
    }
}

