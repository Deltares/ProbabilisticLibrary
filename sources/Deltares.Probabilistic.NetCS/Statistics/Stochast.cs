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
using System.Collections.Generic;
using System.Linq;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Statistics
{
    public class Stochast
    {
        private int id = 0;
        private string name = string.Empty;
        private bool nameAssigned = false;

        private CallBackList<HistogramValue> histogramValues = null;
        private CallBackList<DiscreteValue> discreteValues = null;
        private CallBackList<FragilityValue> fragilityValues = null;
        private CallBackList<ContributingStochast> contributingStochasts = null;
        private CallBackList<ConditionalValue> conditionalValues = null;
        private Stochast conditionalSource = null;

        public Stochast()
        {
            this.id = Interface.Create("stochast");
            ObjectFactory.Register(this, id);
        }

        public Stochast(int id)
        {
            this.id = id;
            ObjectFactory.Register(this, id);
        }

        ~Stochast()
        {
            ObjectFactory.Destroy(id);
        }

        public int GetId()
        {
            return id;
        }

        internal void SetId(int id)
        {
            this.id = id;
        }

        public string Name
        {
            get
            {
                if (!nameAssigned)
                {
                    name = Interface.GetStringValue(id, "name");
                    nameAssigned = true;
                }

                return name;
            }
            set
            {
                Interface.SetStringValue(id, "name", value);
                name = value;
                nameAssigned = true;
            }
        }

        public DistributionType DistributionType
        {
            get { return DistributionTypeConverter.ConvertFromString(Interface.GetStringValue(id, "distribution")); }
            set { Interface.SetStringValue(id, "distribution", DistributionTypeConverter.ConvertToString(value)); }
        }

        public ConstantParameterType ConstantParameterType
        {
            get { return ConstantParameterTypeConverter.ConvertFromString(Interface.GetStringValue(id, "constant_parameter")); }
            set { Interface.SetStringValue(id, "constant_parameter", ConstantParameterTypeConverter.ConvertToString(value)); }
        }

        public bool Inverted
        {
            get { return Interface.GetBoolValue(id, "inverted"); }
            set { Interface.SetBoolValue(id, "inverted", value); }
        }

        public bool Truncated
        {
            get { return Interface.GetBoolValue(id, "truncated"); }
            set { Interface.SetBoolValue(id, "truncated", value); }
        }

        public double Mean
        {
            get { return Interface.GetValue(id, "mean"); }
            set { Interface.SetValue(id, "mean", value); }
        }

        public double Deviation
        {
            get { return Interface.GetValue(id, "deviation"); }
            set { Interface.SetValue(id, "deviation", value); }
        }

        public double Variation
        {
            get { return Interface.GetValue(id, "variation"); }
            set { Interface.SetValue(id, "variation", value); }
        }

        public double Location
        {
            get { return Interface.GetValue(id, "location"); }
            set { Interface.SetValue(id, "location", value); }
        }

        public double Scale
        {
            get { return Interface.GetValue(id, "scale"); }
            set { Interface.SetValue(id, "scale", value); }
        }

        public double Shift
        {
            get { return Interface.GetValue(id, "shift"); }
            set { Interface.SetValue(id, "set_shift", value); }
        }

        public void SetShift(double value)
        {
            Interface.SetValue(id, "shift", value);
        }

        public double ShiftB
        {
            get { return Interface.GetValue(id, "shift_b"); }
            set { Interface.SetValue(id, "shift_b", value); }
        }

        public double Minimum
        {
            get { return Interface.GetValue(id, "minimum"); }
            set { Interface.SetValue(id, "minimum", value); }
        }

        public double Maximum
        {
            get { return Interface.GetValue(id, "maximum"); }
            set { Interface.SetValue(id, "maximum", value); }
        }

        public double Shape
        {
            get { return Interface.GetValue(id, "shape"); }
            set { Interface.SetValue(id, "shape", value); }
        }

        public double ShapeB
        {
            get { return Interface.GetValue(id, "shape_b"); }
            set { Interface.SetValue(id, "shape_b", value); }
        }

        public int Observations
        {
            get { return Interface.GetIntValue(id, "observations"); }
            set { Interface.SetIntValue(id, "observations", value); }
        }

        public IList<HistogramValue> HistogramValues
        {
            get
            {
                if (histogramValues == null)
                {
                    histogramValues = new CallBackList<HistogramValue>(HistogramValuesChanged);

                    int[] histogramIds = Interface.GetArrayIdValue(id, "histogram_values");
                    foreach (int histogramId in histogramIds)
                    {
                        histogramValues.AddWithoutCallBack(new HistogramValue(histogramId));
                    }
                }

                return histogramValues;
            }
        }

        private void HistogramValuesChanged(ListOperationType listOperation, HistogramValue item)
        {
            Interface.SetArrayIntValue(id, "histogram_values", this.histogramValues.Select(p => p.GetId()).ToArray());
        }

        public IList<DiscreteValue> DiscreteValues
        {
            get
            {
                if (discreteValues == null)
                {
                    discreteValues = new CallBackList<DiscreteValue>(DiscreteValuesChanged);

                    int[] discreteIds = Interface.GetArrayIdValue(id, "discrete_values");
                    foreach (int discreteId in discreteIds)
                    {
                        discreteValues.AddWithoutCallBack(new DiscreteValue(discreteId));
                    }
                }

                return discreteValues;
            }
        }

        private void DiscreteValuesChanged(ListOperationType listOperation, DiscreteValue item)
        {
            Interface.SetArrayIntValue(id, "discrete_values", this.discreteValues.Select(p => p.GetId()).ToArray());
        }

        public IList<FragilityValue> FragilityValues
        {
            get
            {
                if (fragilityValues == null)
                {
                    fragilityValues = new CallBackList<FragilityValue>(FragilityValuesChanged);

                    int[] fragilityIds = Interface.GetArrayIdValue(id, "fragility_values");
                    foreach (int fragilityId in fragilityIds)
                    {
                        fragilityValues.AddWithoutCallBack(new FragilityValue(fragilityId));
                    }
                }

                return fragilityValues;
            }
        }

        private void FragilityValuesChanged(ListOperationType listOperation, FragilityValue item)
        {
            if (listOperation == ListOperationType.Add)
            {
                Interface.SetIntValue(id, "fragility_values", item.GetId());
            }
            else
            {
                Interface.SetArrayIntValue(id, "fragility_values", this.fragilityValues.Select(p => p.GetId()).ToArray());
            }
        }

        public IList<ContributingStochast> ContributingStochasts
        {
            get
            {
                if (contributingStochasts == null)
                {
                    contributingStochasts = new CallBackList<ContributingStochast>(ContributingStochastsChanged);

                    int[] stochastIds = Interface.GetArrayIdValue(id, "contributing_stochasts");
                    foreach (int stochastId in stochastIds)
                    {
                        contributingStochasts.AddWithoutCallBack(new ContributingStochast(stochastId));
                    }
                }

                return contributingStochasts;
            }
        }

        private void ContributingStochastsChanged(ListOperationType listOperation, ContributingStochast item)
        {
            Interface.SetArrayIntValue(id, "contributing_stochasts", this.contributingStochasts.Select(s => s.GetId()).ToArray());
        }

        public double DesignQuantile
        {
            get { return Interface.GetValue(id, "design_quantile"); }
            set { Interface.SetValue(id, "design_quantile", value); }
        }

        public double DesignFactor
        {
            get { return Interface.GetValue(id, "design_factor"); }
            set { Interface.SetValue(id, "design_factor", value); }
        }

        public double DesignValue
        {
            get { return Interface.GetValue(id, "design_value"); }
            set { Interface.SetValue(id, "design_value", value); }
        }

        public bool IsArray
        {
            get { return Interface.GetBoolValue(id, "is_array"); }
            set { Interface.SetBoolValue(id, "is_array", value); }
        }

        public int ArraySize
        {
            get { return Interface.GetIntValue(id, "array_size"); }
            set { Interface.SetIntValue(id, "array_size", value); }
        }

        public double GetQuantile(double quantile)
        {
            return Interface.GetArgValue(id, "quantile", quantile);
        }

        public double GetXFromU(double u)
        {
            return Interface.GetArgValue(id, "x_from_u", u);
        }

        public virtual double GetUFromX(double x)
        {
            return Interface.GetArgValue(id, "u_from_x", x);
        }

        public double GetXFromP(double p)
        {
            return Interface.GetArgValue(id, "x_from_p", p);
        }

        public double GetPFromX(double p)
        {
            return Interface.GetArgValue(id, "p_from_x", p);
        }

        public double GetPDF(double x)
        {
            return Interface.GetArgValue(id, "pdf", x);
        }

        public double GetCDF(double x)
        {
            return Interface.GetArgValue(id, "cdf", x);
        }

        public double GetUFromXAndSource(double xSource, double x)
        {
            Interface.SetArrayValue(id, "x_and_source", [x, xSource]);
            Interface.Execute(id, "initialize_conditional_values");
            return Interface.GetValue(id, "u_from_x_and_source");
        }

        public double GetXFromUAndSource(double u, double x)
        {
            Interface.SetArrayValue(id, "u_and_x", [u, x]);
            Interface.Execute(id, "initialize_conditional_values");
            return Interface.GetValue(id, "x_from_u_and_source");
        }

        public void SetXAtU(double x, double u, ConstantParameterType constantParameterType)
        {
            Interface.SetArrayValue(id, "u_and_x", [u, x]);
            if (constantParameterType == ConstantParameterType.Deviation)
            {
                Interface.Execute(id, "set_x_at_u_dev");
            }
            else if (constantParameterType == ConstantParameterType.VariationCoefficient)
            {
                Interface.Execute(id, "set_x_at_u_var");
            }
        }

        public bool IsVarying()
        {
            return Interface.GetBoolValue(id, "is_varying");
        }

        public bool IsConditional
        {
            get { return Interface.GetBoolValue(id, "conditional"); }
            set { Interface.SetBoolValue(id, "conditional", value); }
        }

        public Stochast ConditionalSource
        {
            get
            {
                if (conditionalSource == null)
                {
                    int sourceId = Interface.GetIdValue(id, "conditional_source");
                    if (sourceId > 0)
                    {
                        conditionalSource = ObjectFactory.GetObject<Stochast>(sourceId);
                    }
                }

                return conditionalSource;
            }
            set
            {
                if (value != null)
                {
                    Interface.SetIntValue(id, "conditional_source", value.GetId());
                }

                conditionalSource = value;
            }
        }

        public IList<ConditionalValue> ConditionalValues
        {
            get
            {
                if (conditionalValues == null)
                {
                    conditionalValues = new CallBackList<ConditionalValue>(ConditionalValuesChanged);

                    int[] conditionalValueIds = Interface.GetArrayIdValue(id, "conditional_values");
                    foreach (int conditionalValueId in conditionalValueIds)
                    {
                        conditionalValues.AddWithoutCallBack(new ConditionalValue(conditionalValueId));
                    }
                }

                return conditionalValues;
            }
        }

        private void ConditionalValuesChanged(ListOperationType listOperation, ConditionalValue item)
        {
            Interface.SetArrayIntValue(id, "conditional_values", this.conditionalValues.Select(s => s.GetId()).ToArray());
        }

        public Stochast GetConditionalStochast(double x)
        {
            Interface.SetValue(id, "conditional_x", x);
            int stochastId = Interface.GetIdValue(id, "conditional_x");

            return new Stochast(stochastId);
        }

        public bool CanFit()
        {
            return Interface.GetBoolValue(id, "can_fit");
        }

        public void Fit(double[] values, double shift = double.NaN)
        {
            Interface.SetIntValue(id, "prior", 0);
            Interface.SetValue(id, "shift_for_fit", shift);
            Interface.SetArrayValue(id, "data", values);

            Interface.Execute(id, "fit");

            this.histogramValues = null;
            this.discreteValues = null;
            this.fragilityValues = null;
        }

        public bool CanFitPrior()
        {
            return Interface.GetBoolValue(id, "can_fit_prior");
        }

        public void FitPrior(double[] values, Stochast prior, double shift = double.NaN)
        {
            Interface.SetIntValue(id, "prior", prior?.GetId() ?? 0);
            Interface.SetValue(id, "shift_for_fit", shift);
            Interface.SetArrayValue(id, "data", values);

            Interface.Execute(id, "fit_prior");

            this.histogramValues = null;
            this.discreteValues = null;
            this.fragilityValues = null;
        }

        public void FitWeighted(double[] values, double[] weights)
        {
            Interface.SetIntValue(id, "prior", 0);
            Interface.SetValue(id, "shift_for_fit", 0);
            Interface.SetArrayValue(id, "data", values);
            Interface.SetArrayValue(id, "weights", values);

            Interface.Execute(id, "fit_weighted");

            this.histogramValues = null;
            this.discreteValues = null;
            this.fragilityValues = null;
        }

        public double GetKSTest(double[] values)
        {
            Interface.SetArrayValue(id, "data", values);
            return Interface.GetValue(id, "ks_test");
        }

        public double[] GetSpecialXValues()
        {
            return Interface.GetArrayValue(id, "special_values");
        }

        public void CopyFrom(Stochast source)
        {
            if (source != null)
            {
                Interface.SetIntValue(id, "copy_from", source.GetId());

                this.histogramValues = null;
                this.discreteValues = null;
                this.fragilityValues = null;
                this.conditionalValues = null;
                this.contributingStochasts = null;
            }
        }

        public void InitializeForRun()
        {
            Interface.Execute(id, "initialize_for_run");
        }

        public bool CanTruncate()
        {
            return Interface.GetBoolValue(id, "can_truncate");
        }

        public bool CanInvert()
        {
            return Interface.GetBoolValue(id, "can_invert");
        }

        public bool HasParameter(DistributionPropertyType property)
        {
            switch (property)
            {
                case DistributionPropertyType.Location: return Interface.GetBoolValue(id, "is_used_location");
                case DistributionPropertyType.Scale: return Interface.GetBoolValue(id, "is_used_scale");
                case DistributionPropertyType.Minimum: return Interface.GetBoolValue(id, "is_used_minimum");
                case DistributionPropertyType.Maximum: return Interface.GetBoolValue(id, "is_used_maximum");
                case DistributionPropertyType.Shift: return Interface.GetBoolValue(id, "is_used_shift");
                case DistributionPropertyType.ShiftB: return Interface.GetBoolValue(id, "is_used_shift_b");
                case DistributionPropertyType.Shape: return Interface.GetBoolValue(id, "is_used_shape");
                case DistributionPropertyType.ShapeB: return Interface.GetBoolValue(id, "is_used_shape_b");
                case DistributionPropertyType.Observations: return Interface.GetBoolValue(id, "is_used_observations");
                default: throw new System.NotSupportedException(property.ToString());
            }
        }

        public bool IsQualitative()
        {
            return Interface.GetBoolValue(id, "is_qualitative");
        }

        public bool IsValid()
        {
            return Interface.GetBoolValue(id, "is_valid");
        }
    }
}
