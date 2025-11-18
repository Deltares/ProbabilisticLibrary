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
#include <ostream>
#include <set>

namespace Deltares
{
    namespace Models
    {
        UConverter::UConverter(std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> stochasts, std::shared_ptr<Statistics::BaseCorrelation> stochastCorrelationMatrix)
        {
            this->stochasts.clear();

            std::unordered_map<int, int> mapping;

            int k = 0;
            bool hasArrayStochasts = false;

            for (size_t i = 0; i < stochasts.size(); i++)
            {
                if (stochasts[i]->modelParameter->isArray)
                {
                    for (int j = 0; j < stochasts[i]->modelParameter->arraySize; j++)
                    {
                        hasArrayStochasts = true;
                        if (static_cast<int>(stochasts[i]->ArrayVariables.size()) > j)
                        {
                            stochasts[i]->ArrayVariables[j]->name = stochasts[i]->getIndexedStochastName(j);
                            this->stochasts.push_back(std::make_shared<ComputationalStochast>(stochasts[i], stochasts[i]->ArrayVariables[j], j));
                        }
                        else
                        {
                            this->stochasts.push_back(std::make_shared<ComputationalStochast>(stochasts[i], stochasts[i], j));
                        }
                        mapping[k++] = static_cast<int>(i);
                        k++;
                    }
                }
                else
                {
                    this->stochasts.push_back(std::make_shared<ComputationalStochast>(stochasts[i], stochasts[i]));
                    mapping[k++] = static_cast<int>(i);
                }
            }

            if (!hasArrayStochasts)
            {
                this->correlationMatrix = stochastCorrelationMatrix;
            }
            else if (stochastCorrelationMatrix->IsIdentity())
            {
                this->correlationMatrix = std::make_shared<Statistics::CorrelationMatrix>();
                this->correlationMatrix->init(static_cast<int>(this->stochasts.size()));
            }
            else
            {
                this->correlationMatrix = std::make_shared<Statistics::CorrelationMatrix>();
                this->correlationMatrix->init(static_cast<int>(this->stochasts.size()));

                for (int i = 0; i < static_cast<int>(this->stochasts.size()); i++)
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
            this->pureVaryingStochastIndex.clear();
            this->variableStochastIndex.clear();

            this->hasQualitiveStochasts = false;
            this->hasVariableStochasts = false;
            this->sampleValuesChanged = false;

            std::set<std::shared_ptr<Statistics::Stochast>> initializedStochasts;

            for (std::shared_ptr<ComputationalStochast> stochast : this->stochasts)
            {
                if (!initializedStochasts.contains(stochast->definition))
                {
                    stochast->definition->initializeForRun();
                    initializedStochasts.insert(stochast->definition);
                }
            }

            for (size_t i = 0; i < this->stochasts.size(); i++)
            {
                initializeStochastForRun(i);
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
                varyingCorrelationMatrix->init(static_cast<int>(varyingStochasts.size()));
                varyingCorrelationMatrix->filter(correlationMatrix, varyingStochastIndex);
            }

            varyingCorrelationMatrix->CholeskyDecomposition();
            varyingCorrelationMatrix->InverseCholeskyDecomposition();
        }

        void UConverter::checkArraysMatch(std::shared_ptr<ComputationalStochast> stochast, std::shared_ptr<ComputationalStochast> otherStochast) const
        {
            bool valid = !otherStochast->source->modelParameter->isArray ||
                (stochast->definition->modelParameter->isArray &&
                    stochast->definition->modelParameter->arraySize == otherStochast->source->modelParameter->arraySize);

            if (!valid)
            {
                throw Reliability::probLibException("When using array variable stochasts, the size of the arrays must match");
            }
        }

        void UConverter::initializeStochastForRun(size_t index)
        {
            std::shared_ptr<ComputationalStochast> stochast = this->stochasts[index];

            if (stochast->definition->isVarying() && !isFullyCorrelated(static_cast<int>(index), this->varyingStochastIndex))
            {
                this->varyingStochastIndex.push_back(static_cast<int>(index));
                this->pureVaryingStochastIndex.push_back(static_cast<int>(index));
                this->varyingStochasts.push_back(stochast);
                this->hasQualitiveStochasts |= stochast->definition->isQualitative();
            }
            else
            {
                int type = stochast->definition->isVarying() ? -1 : -2;
                this->varyingStochastIndex.push_back(type);
            }

            variableStochastIndex.push_back(-1);

            if (stochast->definition->isVariable())
            {
                this->hasVariableStochasts = true;
                initializeVariableStochastForRun(stochast, index);
            }
        }

        void UConverter::initializeVariableStochastForRun(std::shared_ptr<ComputationalStochast> stochast, size_t index)
        {
            bool variableStochastIndexFound = false;

            std::shared_ptr<Statistics::Stochast> requiredSource = stochast->definition->getVariableSource();

            for (size_t j = 0; j < this->stochasts.size(); j++)
            {
                std::shared_ptr<ComputationalStochast> otherStochast = stochasts[j];

                if (otherStochast->source == requiredSource)
                {
                    checkArraysMatch(stochast, otherStochast);

                    if (!otherStochast->source->modelParameter->isArray || stochast->index == otherStochast->index)
                    {
                        variableStochastIndex[index] = static_cast<int>(j);
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
            std::map<std::shared_ptr<Statistics::Stochast>, std::vector<std::shared_ptr<Deltares::Reliability::StochastSettings>>> stochastSettingsMap;
            std::map<std::shared_ptr<Statistics::Stochast>, size_t> stochastSettingsCount;

            // stochasts are not unique, therefore register all settings and use them one by one
            for (size_t i = 0; i < settings->stochastSettings.size(); i++)
            {
                std::shared_ptr<Statistics::Stochast> stochast = settings->stochastSettings[i]->stochast;

                if (!stochastSettingsMap.contains(stochast))
                {
                    stochastSettingsMap[stochast] = std::vector<std::shared_ptr<Deltares::Reliability::StochastSettings>>();
                    stochastSettingsCount[stochast] = 0;
                }
                stochastSettingsMap[stochast].push_back(settings->stochastSettings[i]);
            }

            settings->stochastSettings.clear();
            settings->VaryingStochastSettings.clear();
            int j = 0; // varying stochast counter

            std::vector<double> startValues;

            for (size_t i = 0; i < stochasts.size(); i++)
            {
                std::shared_ptr<Statistics::Stochast> stochast = stochasts[i]->definition;

                if (stochastSettingsMap.contains(stochast))
                {
                    std::shared_ptr<Deltares::Reliability::StochastSettings> stochastSettings = stochastSettingsMap[stochast][stochastSettingsCount[stochast]];
                    settings->stochastSettings.push_back(stochastSettings->clone());

                    if (stochastSettingsCount[stochast] < stochastSettingsMap[stochast].size() - 1)
                    {
                        stochastSettingsCount[stochast]++;
                    }
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
                auto corrM = std::dynamic_pointer_cast<Statistics::CorrelationMatrix> (varyingCorrelationMatrix);
                if (corrM == nullptr)
                {
                    throw Reliability::probLibException("not implemented yet");
                }
                const std::vector<double> uncorrelatedStartValues = corrM->InverseCholesky(startValues);

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
            auto corrM = std::dynamic_pointer_cast<Statistics::CorrelationMatrix> (varyingCorrelationMatrix);
            if (corrM == nullptr)
            {
                throw Reliability::probLibException("not implemented yet");
            }
            return corrM->Cholesky(sample->Values);
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

            auto corrM = std::dynamic_pointer_cast<Statistics::CorrelationMatrix> (varyingCorrelationMatrix);
            if (corrM == nullptr)
            {
                throw Reliability::probLibException("not implemented yet");
            }
            std::vector<double> uCorrelated = corrM->Cholesky(unexpandedUValues);

            std::vector<double> expandedUValues = getExpandedValues(uCorrelated);

            return expandedUValues;
        }

        std::vector<double> UConverter::getXValues(std::shared_ptr<Sample> sample)
        {
            std::vector<double> expandedUValues = getExpandedUValues(sample);

            auto xValues = std::vector<double>(this->stochasts.size());

            for (size_t i = 0; i < this->stochasts.size(); i++)
            {
                if (!this->hasVariableStochasts || !stochasts[i]->definition->isVariable())
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

        std::vector<double> UConverter::getValuesFromType(Statistics::RunValuesType type) const
        {
            auto xValues = std::vector<double>(this->stochasts.size());

            for (size_t i = 0; i < this->stochasts.size(); i++)
            {
                if (!this->hasVariableStochasts || !stochasts[i]->definition->isVariable())
                {
                    xValues[i] = this->stochasts[i]->definition->getXFromType(type);
                }
            }

            if (this->hasVariableStochasts)
            {
                for (int stochastIndex : variableStochastList)
                {
                    int sourceIndex = variableStochastIndex[stochastIndex];

                    double xSource = xValues[sourceIndex];
                    xValues[stochastIndex] = stochasts[stochastIndex]->definition->getXFromTypeAndSource(xSource, type);
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
                    const int stochastIndex = variableStochastList[i];
                    const double xStochast = xValues[stochastIndex];

                    const int sourceIndex = variableStochastIndex[stochastIndex];

                    if (originalValues[sourceIndex] != xValues[sourceIndex])
                    {
                        const double xOriginalSource = originalValues[sourceIndex];
                        const double uStochast = stochasts[stochastIndex]->definition->getUFromXAndSource(xOriginalSource, xStochast);

                        const double xSource = xValues[sourceIndex];
                        xValues[stochastIndex] = stochasts[stochastIndex]->definition->getXFromUAndSource(xSource, uStochast);

                        this->sampleValuesChanged = true;
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

        std::shared_ptr<Sample> UConverter::getSampleFromStochastPoint(std::shared_ptr<Models::StochastPoint> stochastPoint)
        {
            std::shared_ptr<Sample> sample = std::make_shared<Sample>(this->getVaryingStochastCount());

            // only select the varying stochasts
            for (size_t i = 0; i < pureVaryingStochastIndex.size(); i++)
            {
                sample->Values[i] = stochastPoint->Alphas[pureVaryingStochastIndex[i]]->U;
            }

            return sample;
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
                if (!this->stochasts[i]->definition->isVariable())
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

            int count = sample->getSize();

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

                if (this->varyingStochasts.size() < this->stochasts.size() && uValues.size() == this->stochasts.size())
                {
                    std::vector<double> uValuesNew = std::vector<double>(this->varyingStochasts.size());
                    for (size_t i = 0; i < this->varyingStochasts.size(); i++)
                    {
                        uValuesNew[i] = uValues[pureVaryingStochastIndex[i]];
                    }

                    uValues = uValuesNew;
                    count = static_cast<int>(uValuesNew.size());
                }

                auto corrM = std::dynamic_pointer_cast<Statistics::CorrelationMatrix> (varyingCorrelationMatrix);
                if (corrM == nullptr)
                {
                    throw Reliability::probLibException("not implemented yet");
                }
                auto uCorrelated = corrM->Cholesky(uValues);

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
                    else if (!this->hasVariableStochasts || !stochasts[i]->definition->isVariable())
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

            realization->updateInfluenceFactors();

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

        Sensitivity::SensitivityResult UConverter::getSensitivityResult()
        {
            Sensitivity::SensitivityResult result;

            for (int index = 0; index < this->varyingStochasts.size(); index++)
            {
                std::shared_ptr<Sensitivity::SensitivityValue> value = std::make_shared<Sensitivity::SensitivityValue>();

                value->stochast = this->varyingStochasts[index]->definition;
                value->arrayIndex = this->varyingStochasts[index]->index;

                result.values.push_back(value);
            }

            return result;
        }

        void  UConverter::registerSample(std::shared_ptr<Uncertainty::CorrelationMatrixBuilder> correlationMatrixBuilder, std::shared_ptr<Sample> sample)
        {
            std::vector<double> uValues = this->getExpandedUValues(sample);

            for (int i = 0; i < this->getVaryingStochastCount(); i++)
            {
                correlationMatrixBuilder->registerStochastValue(this->varyingStochasts[i]->definition, uValues[varyingStochastIndex[i]]);
            }
        }
    }
}


