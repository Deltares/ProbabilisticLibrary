using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Statistics
{
    public class Stochast : IDisposable
    {
        private int id = 0;

        private CallBackList<HistogramValue> histogramValues = null;
        private CallBackList<DiscreteValue> discreteValues = null;
        private CallBackList<FragilityValue> fragilityValues = null;
        private CallBackList<ContributingStochast> contributingStochasts = null;
        private CallBackList<ConditionalValue> conditionalValues = null;
        private Stochast conditionalSource = null;
        private bool synchronizing = false;

        public Stochast()
        {
            this.id = Interface.Create("stochast");
        }

        internal Stochast(int id)
        {
            this.id = id;
        }

        public void Dispose()
        {
            Interface.Destroy(id);
        }

        internal int GetId()
        {
            return id;
        }

        public string Name
        {
            get { return Interface.GetStringValue(id, "name"); }
            set { Interface.SetStringValue(id, "name", value); }
        }

        public DistributionType DistributionType
        {
            get { return DistributionTypeConverter.ConvertFromString(Interface.GetStringValue(id, "distribution")); }
            set { Interface.SetStringValue(id, "distribution", DistributionTypeConverter.ConvertToString(value)); }
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
            set { Interface.SetValue(id, "shift", value); }
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
                    synchronizing = true;
                    histogramValues = new CallBackList<HistogramValue>(HistogramValuesChanged);

                    int[] histogramIds = Interface.GetArrayIdValue(id, "histogram_values");
                    foreach (int histogramId in histogramIds)
                    {
                        histogramValues.Add(new HistogramValue(histogramId));
                    }

                    synchronizing = true;
                }

                return histogramValues;
            }
        }

        private void HistogramValuesChanged(ListOperationType listOperation, HistogramValue item)
        {
            if (!synchronizing)
            {
                Interface.SetArrayIntValue(id, "histogram_values", this.histogramValues.Select(p => p.GetId()).ToArray());
            }
        }

        public IList<DiscreteValue> DiscreteValues
        {
            get
            {
                if (discreteValues == null)
                {
                    synchronizing = true;
                    discreteValues = new CallBackList<DiscreteValue>(DiscreteValuesChanged);

                    int[] discreteIds = Interface.GetArrayIdValue(id, "discrete_values");
                    foreach (int discreteId in discreteIds)
                    {
                        discreteValues.Add(new DiscreteValue(discreteId));
                    }

                    synchronizing = true;
                }

                return discreteValues;
            }
        }

        private void DiscreteValuesChanged(ListOperationType listOperation, DiscreteValue item)
        {
            if (!synchronizing)
            {
                Interface.SetArrayIntValue(id, "discrete_values", this.discreteValues.Select(p => p.GetId()).ToArray());
            }
        }

        public IList<FragilityValue> FragilityValues
        {
            get
            {
                if (fragilityValues == null)
                {
                    synchronizing = true;
                    fragilityValues = new CallBackList<FragilityValue>(FragilityValuesChanged);

                    int[] fragilityIds = Interface.GetArrayIdValue(id, "fragility_values");
                    foreach (int fragilityId in fragilityIds)
                    {
                        fragilityValues.Add(new FragilityValue(fragilityId));
                    }

                    synchronizing = true;
                }

                return fragilityValues;
            }
        }

        private void FragilityValuesChanged(ListOperationType listOperation, FragilityValue item)
        {
            if (!synchronizing)
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
                    synchronizing = true;
                    contributingStochasts = new CallBackList<ContributingStochast>(ContributingStochastsChanged);

                    int[] stochastIds = Interface.GetArrayIdValue(id, "contributing_stochasts");
                    foreach (int stochastId in stochastIds)
                    {
                        contributingStochasts.Add(new ContributingStochast(stochastId));
                    }

                    synchronizing = false;
                }

                return contributingStochasts;
            }
        }

        private void ContributingStochastsChanged(ListOperationType listOperation, ContributingStochast item)
        {
            if (!synchronizing)
            {
                Interface.SetArrayIntValue(id, "contributing_stochasts", this.contributingStochasts.Select(s => s.GetId()).ToArray());
            }
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

        public double GetXFromP(double p)
        {
            double u = StandardNormal.GetUFromP(p);
            return GetXFromU(u);
        }

        public double GetUFromX(double x)
        {
            return Interface.GetArgValue(id, "u_from_x", x);
        }

        public double GetPDF(double x)
        {
            return Interface.GetArgValue(id, "pdf", x);
        }

        public double GetCDF(double x)
        {
            return Interface.GetArgValue(id, "cdf", x);
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
                        conditionalSource = new Stochast(sourceId);
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
                    synchronizing = true;
                    conditionalValues = new CallBackList<ConditionalValue>(ConditionalValuesChanged);

                    int[] conditionalValueIds = Interface.GetArrayIdValue(id, "conditional_values");
                    foreach (int conditionalValueId in conditionalValueIds)
                    {
                        conditionalValues.Add(new ConditionalValue(conditionalValueId));
                    }

                    synchronizing = true;
                }

                return conditionalValues;
            }
        }

        private void ConditionalValuesChanged(ListOperationType listOperation, ConditionalValue item)
        {
            if (!synchronizing)
            {
                Interface.SetArrayIntValue(id, "conditional_values", this.conditionalValues.Select(s => s.GetId()).ToArray());
            }
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

            Interface.Execute(id, "fit");

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
    }
}
