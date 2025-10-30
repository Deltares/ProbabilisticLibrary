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
#include <functional>
#include <limits>

#include "../StochastProperties.h"
#include "../../Math/WeightedValue.h"
#include "../../Utils/probLibException.h"
#include "../../Logging/ValidationReport.h"

namespace Deltares::Statistics
{
    /**
     * \brief Constant parameter value, used in setXAtU
     */
    enum class ConstantParameterType { Deviation, VariationCoefficient };

    /**
     * \brief Function definition of an externally defined distribution
     */
    typedef std::function<double(double) > UXLambda;

    /**
     * \brief Base class for statistical distributions, such as a normal or log normal distribution
     * \remark Usually a derived class of a distribution is instantiated
     */
    class Distribution
    {
    public:
        /**
         * \brief Initializes the stochast with a number of values
         * \param stochast Stochast to be initialized
         * \param values Values by which the stochast is initialized
         */
        virtual void initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values) {}

        /**
         * \brief Gets the x-value (physical value) for a standard normal value (u-value)
         * \param stochast Stochast from which the x-value is derived
         * \param u u-value (Standard normal value)
         * \return x-value
         */
        virtual double getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            return getXFromU(*stochast, u);
        }
        virtual double getXFromU(const StochastProperties& stochast, double u) { return 0; }

        /**
         * \brief Gets the u-value (standard normal value) for a physical value (x-value)
         * \param stochast Stochast from which the u-value is derived
         * \param x x-value (Physical value)
         * \return u-value
         */
        virtual double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) { return 0; }

        /**
         * \brief Indicates whether the stochast can lead to different x-values
         * \param stochast Stochast which is or is not varying
         * \return Indication
         */
        virtual bool isVarying(std::shared_ptr<StochastProperties> stochast) { return false; }

        /**
         * \brief Indicates whether th stochast can be truncated
         * \return Indication
         */
        virtual bool canTruncate() { return false; }

        /**
         * \brief Indicates whether the stochast can be inverted
         * \return Indication
         */
        virtual bool canInvert() { return false; }

        /**
         * \brief Indicates whether the stochast can be fitted to data, i.e. parameters in stochast can be estimated based on a given set of x-values
         * \return Indication
         */
        virtual bool canFit() { return false; }

        /**
         * \brief Indicates whether the stochast can be fitted to data with use of a prior stochast
         * \return Indication
         */
        virtual bool canFitPrior() { return false; }

        /**
         * \brief Indicates whether it is meaningful to compare x-values produced by this distribution (no: qualitative, yes: not qualitative)
         * \remark Only the qualitative distribution is qualitative. For example: A qualitative distribution returns the index number of an input file.
         * \return Indication
         */
        virtual bool isQualitative() { return false; }

        /**
         * \brief Gets a representative u-value which leads to the same x-value as a given u-value
         * \remarks This method is useful when several u-values lead to the same x-value. All u-values leading to the same-x-value lead to the same representative u-value.
         * \param stochast Stochast for which the u-value applies
         * \param u Given u-value
         * \return Representative u-value
         */
        virtual double getRepresentativeU(std::shared_ptr<StochastProperties> stochast, double u) { return u; }

        /**
         * \brief Gets the mean value for a stochast
         * \param stochast Stochast to which the mean value applies
         * \return Mean value
         */
        virtual double getMean(std::shared_ptr<StochastProperties> stochast)
        {
            return getMean(*stochast);
        }
        virtual double getMean(StochastProperties& stochast) { return 0; }

        /**
         * \brief Gets the standard deviation value for a stochast
         * \param stochast Stochast to which the standard deviation applies
         * \return Standard deviation
         */
        virtual double getDeviation(std::shared_ptr<StochastProperties> stochast) { return 0; }

        /**
         * \brief Indicates whether mean and deviation should be maintained when the distribution type of a stochast is changed to this distribution
         * \param stochast Stochast of which the distribution type is changed
         * \return Indication
         */
        virtual bool maintainMeanAndDeviation(std::shared_ptr<StochastProperties> stochast) { return true; }

        /**
         * \brief Modifies the stochast parameters so that the given mean and deviation are achieved
         * \param stochast Stochast for which the mean and deviation are set
         * \param mean Given mean value
         * \param deviation Given standard deviation
         */
        virtual void setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
            setMeanAndDeviation(*stochast, mean, deviation);
        }
        virtual void setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation) {}

        /**
         * \brief Sets the shift value and maintains mean and deviation
         * \param stochast Stochast
         * \param shift Given shift value
         * \param inverted Inverted value
         */
        virtual void setShift(std::shared_ptr<StochastProperties> stochast, const double shift, bool inverted) { stochast->Shift = shift; }

        /**
         * \brief Initializes a stochast for fast u->x conversions during probabilistic analysis
         * \param stochast Stochast to be initialized
         */
        virtual void initializeForRun(std::shared_ptr<StochastProperties> stochast)
        {
            initializeForRun(*stochast);
        }
        virtual void initializeForRun(StochastProperties& stochast) {}

        /**
         * \brief Gets the Probability Density Function (PDF) of a stochast and given x-value
         * \param stochast Stochast for which the PDF applies
         * \param x Given x-value
         * \return PDF value
         */
        virtual double getPDF(std::shared_ptr<StochastProperties> stochast, double x) { return 0; }

        /**
         * \brief Gets the Cumulative Density Function (CDF) of a stochast and given x-value
         * \param stochast Stochast for which the CDF applies
         * \param x Given x-value
         * \return CDF value
         */
        virtual double getCDF(std::shared_ptr<StochastProperties> stochast, double x) { return 0; }

        /**
         * \brief Modifies the stochast properties so that a given x-value is returned for a given u-value
         * \param stochast Stochast to be modified
         * \param x Given x-value
         * \param u Given u-value
         * \param constantType Value to be kept constant when modifying the stochast (variation coefficient or standard deviation) 
         */
        virtual void setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType) {}

        /**
         * \brief Updates parameters of a stochast, so that they fit best a number of given x-values
         * \param stochast Stochast to be updated
         * \param values Given x-values
         * \param shift Shift value, if set the shift parameter will not be fitted (available for limited distributions)
         */
        virtual void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, const double shift)
        {
            throw Reliability::probLibException("fit not supported");
        }

        /**
         * \brief Updates parameters of a stochast, so that they fit best a number of given x-values and their weights
         * \param stochast Stochast to be updated
         * \param values Given x-values
         * \param weights Given weights
         */
        virtual void fitWeighted(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values,
                                 std::vector<double>& weights) { fit(stochast, values, nan("")); }

        /**
         * \brief Updates parameters of a stochast with the use of a prior stochast, so that they fit best a number of given x-values
         * \param stochast Stochast to be updated
         * \param values Given x-values
         * \param prior Prior stochast
         * \param shift Shift value, if set the shift parameter will not be fitted (available for limited distributions)
         */
        virtual void fitPrior(const std::shared_ptr<StochastProperties>& stochast, std::vector<double>& values, const std::shared_ptr<StochastProperties>& prior, const double shift)
        {
            throw Reliability::probLibException("fit with prior not supported");
        }

        /**
         * \brief Gets the maximum allowed value for the shift parameter when given as argument for fitting
         * \param values Given x-values
         * \return Maximum allowed shift value
         */
        virtual double getMaxShiftValue(std::vector<double>& values) { return std::numeric_limits<double>::max(); }

        /**
         * \brief Indicates whether parameters of a stochast have valid values
         * \param stochast Stochast having the parameters
         * \return Indication valid parameters
         */
        bool isValid(std::shared_ptr<StochastProperties> stochast);
        bool isValid(StochastProperties& stochast);

        /**
         * \brief Validates the stochastic parameters and puts the results in a report
         * \param report The validation report containing the validation results
         * \param stochast Stochast having the parameters
         * \param subject String representing the validated object
         * \remark Only parameters which are used by the distribution are evaluated
         */
        virtual void validate(Logging::ValidationReport& report, std::shared_ptr<StochastProperties> stochast, std::string& subject);
        virtual void validate(Logging::ValidationReport& report, StochastProperties& stochast, std::string& subject);

        /**
         * \brief Gets the log likelihood of a stochast at a given x-value
         * \param stochast Stochast for which the log likelihood is calculated
         * \param x given x-value
         * \return Log likelihood
         */
        virtual double getLogLikelihood(std::shared_ptr<StochastProperties> stochast, double x);

        /**
         * \brief Gets a list of x values where the CDF-value is discontinuous
         * \param stochast Stochast in use
         * \return List of x-values
         */
        virtual std::vector<double> getDiscontinuityPoints(const StochastProperties& stochast);

        /**
         * \brief Gets interesting x-values of a stochast
         * \remark Used when plotting PDF or CDF of the stochast
         * \param stochast Stochast in use
         * \return List of x-values
         */
        virtual std::vector<double> getSpecialPoints(std::shared_ptr<StochastProperties> stochast) { return std::vector<double>(0); }

        /**
         * \brief Gets the parameters of a stochast used by the distribution
         * \return List of parameters
         */
        virtual std::vector<DistributionPropertyType> getParameters() {    return {}; }

    protected:

        virtual double getFittedMinimum(std::vector<double>& x);
    };
}

