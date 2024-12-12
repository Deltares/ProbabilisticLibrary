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
#include "BaseStochast.h"
#include "../../Deltares.Probabilistic/Statistics/DistributionType.h"
#include "../../Deltares.Probabilistic/Statistics/Stochast.h"
#include "../../Deltares.Probabilistic/Statistics/StandardNormal.h"
#include "../Utils/SharedPointerProvider.h"
#include "../Utils/NativeSupport.h"
#include "../Utils/CallBackList.h"
#include "DiscreteValue.h"
#include "HistogramValue.h"
#include "FragilityValue.h"
#include "ContributingStochast.h"
#include "VariableStochastValueSet.h"
#include "DistributionType.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            public enum class ConstantParameterType { Deviation, VariationCoefficient };
            public delegate double ManagedUXDelegate(double u);

            public ref class Stochast : public BaseStochast
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Statistics::Stochast>* shared = nullptr;

                bool synchronizing = false;

                void Initialize()
                {
                    shared->object->ValueSet = this->ValueSet->GetValue();

                    histogramValues->SetCallBack(gcnew ListCallBack<HistogramValue^>(this, &Stochast::SynchronizeHistogramValues));
                    discreteValues->SetCallBack(gcnew ListCallBack<DiscreteValue^>(this, &Stochast::SynchronizeDiscreteValues));
                    fragilityValues->SetCallBack(gcnew ListCallBack<FragilityValue^>(this, &Stochast::SynchronizeFragilityValues));
                    contributingStochasts->SetCallBack(gcnew ListCallBack<ContributingStochast^>(this, &Stochast::SynchronizeContributingStochasts));
                }

                void SynchronizeHistogramValues(ListOperationType listOperationType, HistogramValue^ histogramValue);
                void SynchronizeDiscreteValues(ListOperationType listOperationType, DiscreteValue^ discreteValue);
                void SynchronizeFragilityValues(ListOperationType listOperationType, FragilityValue^ discreteValue);
                void SynchronizeContributingStochasts(ListOperationType listOperationType, ContributingStochast^ contributingStochast);

                CallBackList<HistogramValue^>^ histogramValues = gcnew CallBackList<HistogramValue^>();
                CallBackList<DiscreteValue^>^ discreteValues = gcnew CallBackList<DiscreteValue^>();
                CallBackList<FragilityValue^>^ fragilityValues = gcnew CallBackList<FragilityValue^>();
                CallBackList<ContributingStochast^>^ contributingStochasts = gcnew CallBackList<ContributingStochast^>();

                Stochast^ source = nullptr;
                VariableStochastValueSet^ valueSet = gcnew VariableStochastValueSet();

                static Deltares::Statistics::ConstantParameterType getNativeConstantParameterType(ConstantParameterType constantParameterType);
                static Statistics::Wrappers::ConstantParameterType getConstantParameterType(Statistics::ConstantParameterType constantParameterType);

                void updateStochast();
                void updateLists();

                bool AreHistogramValuesMatching();
                bool AreFragilityValuesMatching();
                bool AreDiscreteValuesMatching();
                bool AreContributingStochastsMatching();

                System::Collections::Generic::List<ManagedUXDelegate^>^ handles = gcnew System::Collections::Generic::List<ManagedUXDelegate^>();
            protected:
                virtual void setNativeObject(std::shared_ptr<Statistics::Stochast> nativeStochast)
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(nativeStochast);
                }
            public:
                Stochast()
                {
                    this->setNativeObject(std::make_shared<Statistics::Stochast>());
                    this->Initialize();
                }

                Stochast(std::shared_ptr<Statistics::Stochast> nativeStochast)
                {
                    this->setNativeObject(nativeStochast);
                    this->Initialize();
                    this->updateLists();
                }

                Stochast(DistributionType distributionType, array<double>^ values)
                {
                    const Statistics::DistributionType nativeDistributionType = DistributionTypeConverter::getNativeDistributionType(distributionType);
                    std::vector<double> nValues = NativeSupport::toNative(values);

                    this->setNativeObject(std::make_shared<Statistics::Stochast>(nativeDistributionType, nValues));
                    this->Initialize();
                }

                ~Stochast() { this->!Stochast(); }
                !Stochast() { delete shared; }

                property System::String^ Name
                {
                    System::String ^ get() { return NativeSupport::toManaged(shared->object->name); }
                    void set(System::String ^ value) { shared->object->name = NativeSupport::toNative(value); }
                }

                    virtual property Wrappers::DistributionType DistributionType
                {
                    Wrappers::DistributionType get() { return DistributionTypeConverter::getManagedDistributionType(shared->object->getDistributionType()); }
                    void set(Wrappers::DistributionType value) { shared->object->setDistributionType(DistributionTypeConverter::getNativeDistributionType(value)); }
                }

                    /**
                     * \brief Sets the distribution type and maintains the mean and deviation for some distributions
                     * \param distributionType
                     */
                    void SetDistributionType(Wrappers::DistributionType distributionType)
                {
                    shared->object->setDistributionType(DistributionTypeConverter::getNativeDistributionType(distributionType));
                }

                /**
                 * \brief Sets a call back function, which calculates the transformation from u to x
                 * \param uxDelegate
                 */
                void SetExternalDistribution(ManagedUXDelegate^ uxDelegate);

                virtual bool IsValid()
                {
                    return shared->object->isValid();
                }

                virtual bool IsVarying()
                {
                    return shared->object->isVarying();
                }

                virtual bool IsQualitative()
                {
                    return shared->object->isQualitative();
                }

                virtual property bool IsVariableStochast
                {
                    bool get() { return shared->object->IsVariableStochast; }
                    void set(bool value) { shared->object->IsVariableStochast = value; }
                }

                    Stochast^ GetVariableStochast(double x)
                {
                    return gcnew Stochast(shared->object->getVariableStochast(x));
                }

                virtual property double Mean
                {
                    double get() { return shared->object->getMean(); }
                    void set(double value) { shared->object->setMean(value); }
                }

                    virtual property double Deviation
                {
                    double get() { return shared->object->getDeviation(); }
                    void set(double value)
                    {
                        shared->object->constantParameterType = this->getNativeConstantParameterType(Deltares::Statistics::Wrappers::ConstantParameterType::Deviation);
                        shared->object->setDeviation(value);
                    }
                }

                    virtual property double Variation
                {
                    double get() { return shared->object->getVariation(); }
                    void set(double value)
                    {
                        shared->object->constantParameterType = this->getNativeConstantParameterType(Deltares::Statistics::Wrappers::ConstantParameterType::VariationCoefficient);
                        shared->object->setVariation(value);
                    }
                }

                    virtual property Deltares::Statistics::Wrappers::ConstantParameterType ConstantParameterType
                {
                    Deltares::Statistics::Wrappers::ConstantParameterType get() { return this->getConstantParameterType(shared->object->constantParameterType); }
                    void set(Deltares::Statistics::Wrappers::ConstantParameterType value) { shared->object->constantParameterType = this->getNativeConstantParameterType(Deltares::Statistics::Wrappers::ConstantParameterType::VariationCoefficient); }
                }

                    virtual property double Location
                {
                    double get() { return shared->object->getProperties()->Location; }
                    void set(double value) { shared->object->getProperties()->Location = value; }
                }

                    virtual property double Scale
                {
                    double get() { return shared->object->getProperties()->Scale; }
                    void set(double value) { shared->object->getProperties()->Scale = value; }
                }

                    virtual property double Shift
                {
                    double get() { return shared->object->getProperties()->Shift; }
                    void set(double value) { shared->object->getProperties()->Shift = value; }
                }

                    void SetShift(double shift)
                {
                    shared->object->setShift(shift);
                }

                virtual property double ShiftB
                {
                    double get() { return shared->object->getProperties()->ShiftB; }
                    void set(double value) { shared->object->getProperties()->ShiftB = value; }
                }

                    virtual property double Shape
                {
                    double get() { return shared->object->getProperties()->Shape; }
                    void set(double value) { shared->object->getProperties()->Shape = value; }
                }

                    virtual property double ShapeB
                {
                    double get() { return shared->object->getProperties()->ShapeB; }
                    void set(double value) { shared->object->getProperties()->ShapeB = value; }
                }

                    virtual property double Minimum
                {
                    double get() { return shared->object->getProperties()->Minimum; }
                    void set(double value) { shared->object->getProperties()->Minimum = value; }
                }

                    virtual property double Maximum
                {
                    double get() { return shared->object->getProperties()->Maximum; }
                    void set(double value) { shared->object->getProperties()->Maximum = value; }
                }

                    property int Observations
                {
                    int get() { return shared->object->getProperties()->Observations; }
                    void set(int value) { shared->object->getProperties()->Observations = value; }
                }

                    virtual bool CanTruncate()
                {
                    return shared->object->canTruncate();
                }

                virtual property bool Truncated
                {
                    bool get() { return shared->object->isTruncated(); }
                    void set(bool value) { shared->object->setTruncated(value); }
                }

                    virtual bool CanInvert()
                {
                    return shared->object->canInvert();
                }

                virtual property bool Inverted
                {
                    bool get() { return shared->object->isInverted(); }
                    void set(bool value) { shared->object->setInverted(value); }
                }

                    property System::Collections::Generic::IList<DiscreteValue^>^ DiscreteValues
                {
                    System::Collections::Generic::IList<DiscreteValue^> ^ get() { return discreteValues; }
                }

                    property System::Collections::Generic::IList<HistogramValue^>^ HistogramValues
                {
                    System::Collections::Generic::IList<HistogramValue^> ^ get() { return histogramValues; }
                }

                    property System::Collections::Generic::IList<FragilityValue^>^ FragilityValues
                {
                    System::Collections::Generic::IList<FragilityValue^> ^ get() { return fragilityValues; }
                }

                    property System::Collections::Generic::IList<ContributingStochast^>^ ContributingStochasts
                {
                    System::Collections::Generic::IList<ContributingStochast^> ^ get() { return contributingStochasts; }
                }

                    virtual bool HasParameter(DistributionPropertyType property)
                {
                    return shared->object->hasParameter(DistributionTypeConverter::getNativeDistributionPropertyType(property));
                }

                virtual double GetPDF(double x)
                {
                    return shared->object->getPDF(x);
                }

                virtual double GetCDF(double x)
                {
                    return shared->object->getCDF(x);
                }

                virtual double GetXFromU(double u)
                {
                    return shared->object->getXFromU(u);
                }

                virtual double GetXFromUAndSource(double xSource, double u)
                {
                    shared->object->initializeConditionalValues();
                    return shared->object->getXFromUAndSource(xSource, u);
                }

                virtual double GetXFromP(double p)
                {
                    double u = Statistics::StandardNormal::getUFromP(p);
                    return shared->object->getXFromU(u);
                }

                virtual double GetUFromX(double x)
                {
                    return shared->object->getUFromX(x);
                }

                virtual void SetXAtU(double x, double u, Deltares::Statistics::Wrappers::ConstantParameterType constantType)
                {
                    shared->object->setXAtU(x, u, this->getNativeConstantParameterType(constantType));
                }

                virtual property double DesignQuantile
                {
                    double get() { return shared->object->designQuantile; }
                    void set(double value) { shared->object->designQuantile = value; }
                }

                virtual property double DesignFactor
                {
                    double get() { return shared->object->designFactor; }
                    void set(double value) { shared->object->designFactor = value; }
                }

                virtual property double DesignValue
                {
                    double get() { return shared->object->getDesignValue(); }
                    void set(double value) { shared->object->setDesignValue(value); }
                }

                property bool IsArray
                {
                    bool get() { return shared->object->modelParameter->isArray; }
                    void set(bool value) { shared->object->modelParameter->isArray = value; }
                }

                property int ArraySize
                {
                    int get() { return shared->object->modelParameter->arraySize; }
                    void set(int value) { shared->object->modelParameter->arraySize = value; }
                }

                virtual void InitializeForRun()
                {
                    updateStochast();

                    shared->object->initializeForRun();
                }

                virtual bool CanFit()
                {
                    return shared->object->canFit();
                }

                virtual void Fit(array<double>^ values)
                {
                    std::vector<double> nativeValues = NativeSupport::toNative(values);

                    shared->object->fit(nativeValues);

                    updateLists();
                }

                virtual void FitWeighted(array<double>^ values, array<double>^ weights)
                {
                    std::vector<double> nativeValues = NativeSupport::toNative(values);
                    std::vector<double> nativeWeights = NativeSupport::toNative(weights);

                    shared->object->fitWeighted(nativeValues, nativeWeights);

                    updateLists();
                }

                virtual double GetGoodnessOfFit(array<double>^ values)
                {
                    std::vector<double> nativeValues = NativeSupport::toNative(values);

                    return shared->object->getKSTest(nativeValues);
                }

                virtual void CopyFrom(Stochast^ source)
                {
                    shared->object->copyFrom(source->GetStochast());
                    this->updateLists();
                }

                virtual array<double>^ GetSpecialXValues()
                {
                    return NativeSupport::toManaged(shared->object->getSpecialXValues());
                }

                property VariableStochastValueSet^ ValueSet
                {
                    VariableStochastValueSet ^ get() { return valueSet; }
                    void set(VariableStochastValueSet ^ value) { this->valueSet = value; }
                }

                property Stochast^ VariableSource
                {
                    Stochast ^ get() { return this->source; }
                    void set(Stochast ^ value) { this->source = value; }
                }

                System::String^ ToString() override
                {
                    return this->Name;
                }

                std::shared_ptr<Statistics::Stochast> GetStochast() override
                {
                    updateStochast();

                    if (this->IsVariableStochast)
                    {
                        shared->object->ValueSet = this->ValueSet->GetValue();

                        if (this->VariableSource != nullptr)
                        {
                            shared->object->VariableSource = this->VariableSource->GetStochast();
                        }
                    }

                    return shared->object;
                }
            };
        }
    }
}

