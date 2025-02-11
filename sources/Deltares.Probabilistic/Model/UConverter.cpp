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
#include "UConverter.h"

#include <map>

#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Reliability/StochastSettings.h"
#include "../Reliability/StochastSettingsSet.h"
#include "../Utils/probLibException.h"

#include <memory>
#include <set>

namespace Deltares
{
    namespace Models
    {
        UConverter::UConverter(std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> stochasts, std::shared_ptr<Statistics::CorrelationMatrix> stochastCorrelationMatrix)
        {
            this->stochasts.clear();

            std::unordered_map<int, int> mapping;

            int k = 0;
            bool hasArrayStochasts = false;

            for (size_t i = 0; i < stochasts.size(); i++)
            {
                if (stochasts[i]->modelParameter->isArray)
                {
                    for (size_t j = 0; j < stochasts[i]->modelParameter->arraySize; j++)
                    {
                        hasArrayStochasts = true;
                        this->stochasts.push_back(std::make_shared<ComputationalStochast>(stochasts[i], j));
                        mapping[k++] = i;
                        k++;
                    }
                }
                else
                {
                    this->stochasts.push_back(std::make_shared<ComputationalStochast>(stochasts[i]));
                    mapping[k++] = i;
                }
            }

            if (!hasArrayStochasts)
            {
                this->correlationMatrix = stochastCorrelationMatrix;
            }
            else if (stochastCorrelationMatrix->IsIdentity())
            {
                this->correlationMatrix = std::make_shared<Statistics::CorrelationMatrix>();
                this->correlationMatrix->init(this->stochasts.size());
            }
            else
            {
                this->correlationMatrix = std::make_shared<Statistics::CorrelationMatrix>();
                this->correlationMatrix->init(this->stochasts.size());

                for (int i = 0; i < this->stochasts.size(); i++)
                {
                    for (int j = 0; j < i; j++)
                    {
                        int k_i = mapping[i];
                        int k_j = mapping[j];

                        if (k_i != k_j)
                        {
                            double correlationValue = stochastCorrelationMatrix->GetCorrelation(k_i, k_j);
                            this->correlationMatrix->SetCorrelation(i, j, correlationValue);
                        }
                    }
                }
            }
        }


        void UConverter::initializeForRun()
        {
            this->varyingStochasts.clear();
            this->varyingStochastIndex.clear();
            this->variableStochastIndex.clear();

            this->hasQualitiveStochasts = false;
            this->hasVariableStochasts = false;

            for (std::shared_ptr<ComputationalStochast> stochast : this->stochasts)
            {
                if (stochast->index == 0)
                {
                    stochast->definition->initializeForRun();
                }
            }

            for (size_t i = 0; i < this->stochasts.size(); i++)
            {
                if (this->stochasts[i]->definition->isVarying() && !isFullyCorrelated(i, this->varyingStochastIndex))
                {
                    this->varyingStochastIndex.push_back(i);
                    this->varyingStochasts.push_back(this->stochasts[i]);
                    this->hasQualitiveStochasts |= this->stochasts[i]->definition->isQualitative();
                }
                else
                {
                    int type = (this->stochasts[i]->definition->isVarying() ? -1 : -2);
                    this->varyingStochastIndex.push_back(type);
                }

                variableStochastIndex.push_back(-1);

                if (this->stochasts[i]->definition->IsVariableStochast)
                {
                    this->hasVariableStochasts = true;
                    bool variableStochastIndexFound = false;
                    for (size_t j = 0; j < this->stochasts.size(); j++)
                    {
                        if (stochasts[j]->definition == this->stochasts[i]->definition->VariableSource)
                        {
                            bool valid = !stochasts[j]->definition->modelParameter->isArray ||
                                         (this->stochasts[i]->definition->modelParameter->isArray &&
                                          this->stochasts[i]->definition->modelParameter->arraySize == this->stochasts[j]->definition->modelParameter->arraySize);

                            if (!valid)
                            {
                                throw Reliability::probLibException("When using array variable stochasts, the size of the arrays must match");
                            }

                            if (!stochasts[j]->definition->modelParameter->isArray ||
                                this->stochasts[i]->index == this->stochasts[j]->index)
                            {
                                variableStochastIndex[i] = j;
                                variableStochastIndexFound = true;
                                break;
                            }
                        }
                    }

                    if (!variableStochastIndexFound)
                    {
                        throw Reliability::probLibException("Variable stochast source has not been set");
                    }
                }
            }

            if (this->hasVariableStochasts)
            {
                this->variableStochastList = getVariableStochastIndex();
            }

            if (stochasts.size() == varyingStochasts.size())
            {
                varyingCorrelationMatrix = correlationMatrix;
            }
            else
            {
                varyingCorrelationMatrix = std::make_shared<Statistics::CorrelationMatrix>();
                varyingCorrelationMatrix->init(varyingStochasts.size());
                varyingCorrelationMatrix->filter(correlationMatrix, varyingStochastIndex);
            }

            varyingCorrelationMatrix->CholeskyDecomposition();
            varyingCorrelationMatrix->InverseCholeskyDecomposition();
        }

        bool UConverter::isVaryingStochast(int index)
        {
            return this->varyingStochastIndex[index] >= 0;
        }

        std::vector<double> UConverter::getVaryingValues(std::vector<double> values)
        {
            std::vector<double> varyingValues;

            for (size_t i = 0; i < this->stochasts.size(); i++)
            {
                if (varyingStochastIndex[i] >= 0)
                {
                    varyingValues.push_back(values[i]);
                }
            }

            return varyingValues;
        }


        bool UConverter::isFullyCorrelated(const int index, std::vector<int> varyingIndices)
        {
            return correlationMatrix->isFullyCorrelated(index, varyingIndices);
        }

        int UConverter::getStochastCount()
        {
            return this->stochasts.size();
        }

        int UConverter::getVaryingStochastCount()
        {
            return this->varyingStochasts.size();
        }

        void UConverter::updateStochastSettings(std::shared_ptr<Deltares::Reliability::StochastSettingsSet> settings)
        {
            std::map<std::shared_ptr<Statistics::Stochast>, std::shared_ptr<Deltares::Reliability::StochastSettings>> stochastSettingsMap;

            for (size_t i = 0; i < settings->stochastSettings.size(); i++)
            {
                stochastSettingsMap[settings->stochastSettings[i]->stochast] = settings->stochastSettings[i];
            }

            settings->stochastSettings.clear();
            settings->VaryingStochastSettings.clear();
            int j = 0; // varying stochast counter

            std::vector<double> startValues;

            for (size_t i = 0; i < stochasts.size(); i++)
            {
                if (stochastSettingsMap.contains(stochasts[i]->definition))
                {
                    settings->stochastSettings.push_back(stochastSettingsMap[stochasts[i]->definition]->clone());
                }
                else
                {
                    std::shared_ptr<Deltares::Reliability::StochastSettings> newStochastSettings = std::make_shared<Deltares::Reliability::StochastSettings>();
                    settings->stochastSettings.push_back(newStochastSettings);
                }

                if (stochasts[i]->definition->isVarying() && !isFullyCorrelated(i, this->varyingStochastIndex))
                {
                    std::shared_ptr<Deltares::Reliability::StochastSettings> varyingStochastSettings = settings->stochastSettings.back();

                    varyingStochastSettings->StochastIndex = i;
                    varyingStochastSettings->IsQualitative = varyingStochasts[j]->definition->isQualitative();
                    varyingStochastSettings->stochast = varyingStochasts[j]->definition;

                    varyingStochastSettings->initializeForRun();

                    settings->VaryingStochastSettings.push_back(varyingStochastSettings);

                    startValues.push_back(varyingStochastSettings->StartValue);

                    j++;
                }
            }

            if (settings->AreStartValuesCorrelated)
            {
                const std::vector<double> uncorrelatedStartValues = varyingCorrelationMatrix->InverseCholesky(startValues);

                for (size_t i = 0; i < settings->VaryingStochastSettings.size(); i++)
                {
                    settings->VaryingStochastSettings[i]->UncorrelatedStartValue = uncorrelatedStartValues[i];
                }
            }
            else
            {
                for (size_t i = 0; i < settings->VaryingStochastSettings.size(); i++)
                {
                    settings->VaryingStochastSettings[i]->UncorrelatedStartValue = settings->VaryingStochastSettings[i]->StartValue;
                }
            }
        }

        void UConverter::updateDependentParameter(std::vector<double>& uValues, const int i)
        {
            auto r = varyingCorrelationMatrix->findDependent(i);
            if (r.index >= 0)
            {
                uValues[i] = r.correlation * uValues[r.index];
            }
        }

        std::vector<double> UConverter::getExpandedValues(const std::vector<double>& values)
        {
            auto uValues = std::vector<double>(this->stochasts.size());
            int ii = 0;
            for (size_t i = 0; i < this->stochasts.size(); i++)
            {
                const int varyingIndex = this->varyingStochastIndex[i];
                if (varyingIndex >= 0)
                {
                    uValues[i] = values[ii];
                    ii++;
                }
            }

            for (size_t i = 0; i < this->stochasts.size(); i++)
            {
                const int varyingIndex = this->varyingStochastIndex[i];
                if (varyingIndex < 0)
                {
                    updateDependentParameter(uValues, i);
                }
            }

            return uValues;
        }

        std::vector<double> UConverter::getExpandedValues(const std::vector<double>& values, double defaultValue)
        {
            auto uValues = std::vector<double>(this->stochasts.size());
            int ii = 0;
            for (size_t i = 0; i < this->stochasts.size(); i++)
            {
                const int varyingIndex = this->varyingStochastIndex[i];
                if (varyingIndex >= 0)
                {
                    uValues[i] = values[ii];
                    ii++;
                }
                else
                {
                    uValues[i] = 0;
                }
            }

            return uValues;
        }

        std::vector<double> UConverter::getUValues(std::shared_ptr<Sample> sample)
        {
            return varyingCorrelationMatrix->Cholesky(sample->Values);
        }

        std::vector<double> UConverter::getExpandedUValues(std::shared_ptr<Sample> sample)
        {
            std::vector<double> unexpandedUValues = sample->Values;

            if (sample->Values.size() > varyingStochasts.size())
            {
                unexpandedUValues.clear();
                for (size_t i = 0; i < varyingStochasts.size(); i++)
                {
                    unexpandedUValues.push_back(sample->Values[i]);
                }
            }

            std::vector<double> uCorrelated = varyingCorrelationMatrix->Cholesky(unexpandedUValues);

            std::vector<double> expandedUValues = getExpandedValues(uCorrelated);

            return expandedUValues;
        }

        std::vector<double> UConverter::getXValues(std::shared_ptr<Sample> sample)
        {
            std::vector<double> expandedUValues = getExpandedUValues(sample);

            auto xValues = std::vector<double>(this->stochasts.size());

            for (size_t i = 0; i < this->stochasts.size(); i++)
            {
                if (!this->hasVariableStochasts || !stochasts[i]->definition->IsVariableStochast)
                {
                    xValues[i] = this->stochasts[i]->definition->getXFromU(expandedUValues[i]);
                }
            }

            if (this->hasVariableStochasts)
            {
                for (size_t i = 0; i < variableStochastList.size(); i++)
                {
                    int stochastIndex = variableStochastList[i];
                    int sourceIndex = variableStochastIndex[stochastIndex];

                    double xSource = xValues[sourceIndex];
                    xValues[stochastIndex] = stochasts[stochastIndex]->definition->getXFromUAndSource(xSource, expandedUValues[stochastIndex]);
                }
            }

            return xValues;
        }

        void UConverter::updateVariableSample(std::vector<double>& xValues, std::vector<double>& originalValues)
        {
            if (this->hasVariableStochasts)
            {
                for (size_t i = 0; i < variableStochastList.size(); i++)
                {
                    int stochastIndex = variableStochastList[i];
                    double xStochast = xValues[stochastIndex];

                    int sourceIndex = variableStochastIndex[stochastIndex];

                    if (originalValues[sourceIndex] != xValues[sourceIndex])
                    {
                        double xOriginalSource = originalValues[sourceIndex];

                        double uStochast = stochasts[stochastIndex]->definition->getUFromXAndSource(xOriginalSource, xStochast);

                        double xSource = xValues[sourceIndex];
                        xValues[stochastIndex] = stochasts[stochastIndex]->definition->getXFromUAndSource(xSource, uStochast);
                    }
                }
            }
        }

        std::shared_ptr<Sample> UConverter::getQualitativeExcludedSample(std::shared_ptr<Sample> sample)
        {
            std::shared_ptr<Sample> qualitativeExcludedSample = sample->clone();

            for (size_t i = 0; i < sample->Values.size(); i++)
            {
                if (this->varyingStochasts[i]->definition->isQualitative())
                {
                    qualitativeExcludedSample->Values[i] = 0;
                }
            }

            return qualitativeExcludedSample->getSampleAtBeta(sample->getBeta());
        }

        /**
         * \brief Gets the indices of the variable stochasts in order of assignment
         */
        std::vector<int> UConverter::getVariableStochastIndex()
        {
            std::vector<int> newVariableStochastList;

            std::set<int> assignedAlphas;
            std::vector<int> unAssignedAlphas;

            // build up initial administration which stochasts have been assigned and which not
            for (size_t i = 0; i < this->stochasts.size(); i++)
            {
                if (!this->stochasts[i]->definition->IsVariableStochast)
                {
                    assignedAlphas.insert(i);
                }
                else
                {
                    unAssignedAlphas.push_back(i);
                }
            }

            bool ready = unAssignedAlphas.empty();

            // loop until all stochasts have been assigned
            while (!ready)
            {
                ready = true;
                bool modified = false;

                for (size_t i = 0; i < unAssignedAlphas.size(); i++)
                {
                    int alphaIndex = unAssignedAlphas[i];

                    if (!assignedAlphas.contains(alphaIndex))
                    {
                        int sourceIndex = variableStochastIndex[alphaIndex];
                        if (assignedAlphas.contains(sourceIndex))
                        {
                            newVariableStochastList.push_back(alphaIndex);

                            assignedAlphas.insert(alphaIndex);
                            modified = true;
                        }
                        else
                        {
                            ready = false;
                        }
                    }
                }

                // when no progress and not everything has been assigned => error
                if (!ready && !modified)
                {
                    throw Deltares::Reliability::probLibException("circular reference");
                }
            }

            return newVariableStochastList;
        }

        std::shared_ptr<StochastPoint> UConverter::GetStochastPoint(std::shared_ptr<Sample> sample, double beta)
        {
            std::shared_ptr<StochastPoint> realization = std::make_shared<StochastPoint>();
            realization->Beta = beta;

            std::shared_ptr<Sample> betaSample = sample->getSampleAtBeta(std::fabs(beta));

            if (this->hasQualitiveStochasts)
            {
                betaSample = getQualitativeExcludedSample(betaSample);
            }

            const int count = sample->getSize();

            const double defaultAlpha = -1 / sqrt(count);

            if (count > 0)
            {
                std::vector<double> alphas = std::vector<double>(count);

                auto uValues = std::vector<double>(count);
                for (int i = 0; i < count; i++)
                {
                    if (beta == 0)
                    {
                        uValues[i] = 0;
                        alphas[i] = -sample->Values[i];
                    }
                    else
                    {
                        uValues[i] = betaSample->Values[i]; // - beta * alphas[i];
                        alphas[i] = -uValues[i] / beta;
                    }
                }

                auto uCorrelated = varyingCorrelationMatrix->Cholesky(uValues);

                auto alphaCorrelated = std::vector<double>(count);
                for (int i = 0; i < count; i++)
                {
                    if (beta == 0)
                    {
                        alphaCorrelated[i] = defaultAlpha;
                    }
                    else
                    {
                        alphaCorrelated[i] = -uCorrelated[i] / beta;
                    }
                }

                if (this->varyingStochasts.size() < this->stochasts.size())
                {
                    uValues = getExpandedValues(uValues, 0);
                    alphas = getExpandedValues(alphas, 0);
                    uCorrelated = getExpandedValues(uCorrelated);
                    alphaCorrelated = getExpandedValues(alphaCorrelated);
                }

                for (size_t i = 0; i < this->stochasts.size(); i++)
                {
                    std::shared_ptr<StochastPointAlpha> alpha = std::make_shared<StochastPointAlpha>();
                    alpha->Stochast = this->stochasts[i]->definition;
                    alpha->Index = this->stochasts[i]->index;
                    alpha->Alpha = alphas[i];
                    alpha->AlphaCorrelated = alphaCorrelated[i];
                    alpha->U = uValues[i];

                    if (this->hasQualitiveStochasts && this->stochasts[i]->definition->isQualitative())
                    {
                        const int varyingIndex = this->varyingStochastIndex[i];
                        alpha->X = stochasts[i]->definition->getXFromU(sample->Values[varyingIndex]);
                    }
                    else if (!this->hasVariableStochasts || !stochasts[i]->definition->IsVariableStochast)
                    {
                        alpha->X = stochasts[i]->definition->getXFromU(uCorrelated[i]);
                    }

                    realization->Alphas.push_back(alpha);
                }

                // assign variable stochasts
                if (this->hasVariableStochasts)
                {
                    for (size_t i = 0; i < variableStochastList.size(); i++)
                    {
                        const int stochastIndex = variableStochastList[i];
                        const int sourceIndex = variableStochastIndex[stochastIndex];

                        const double xSource = realization->Alphas[sourceIndex]->X;
                        realization->Alphas[stochastIndex]->X = stochasts[stochastIndex]->definition->getXFromUAndSource(xSource, uCorrelated[stochastIndex]);
                    }
                }
            }

            return realization;
        }

        std::shared_ptr<StochastPoint> UConverter::GetStochastPoint(double beta, std::vector<double> alphas)
        {
            std::vector<double> uValues(alphas.size());

            for (size_t i = 0; i < alphas.size(); i++)
            {
                if (beta != 0)
                {
                    uValues[i] = -beta * alphas[i];
                }
                else
                {
                    uValues[i] = -alphas[i];
                }
            }

            std::shared_ptr<Models::Sample> sample = std::make_shared<Models::Sample>(uValues);

            std::shared_ptr<StochastPoint> stochastPoint = GetStochastPoint(sample, beta);

            stochastPoint->updateInfluenceFactors();

            return stochastPoint;
        }

        void  UConverter::registerSample(std::shared_ptr<Sensitivity::CorrelationMatrixBuilder> correlationMatrixBuilder, std::shared_ptr<Sample> sample)
        {
            std::vector<double> uValues = this->getExpandedUValues(sample);

            for (int i = 0; i < this->getVaryingStochastCount(); i++)
            {
                correlationMatrixBuilder->registerStochastValue(this->varyingStochasts[i]->definition, uValues[varyingStochastIndex[i]]);
            }
        }
    }
}


