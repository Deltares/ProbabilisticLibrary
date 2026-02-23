using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Model
{
    public delegate void ZSampleDelegate(ModelSample sample);


    public class ModelProject : IDisposable
    {
        private int id = 0;

        private CallBackList<Stochast> stochasts = null;
        private CallBackList<ModelParameter> inputParameters = null;
        private CallBackList<ModelParameter> outputParameters = null;
        private ZSampleDelegate zFunction = null;
        private CorrelationMatrix correlationMatrix = null;
        private TagRepository tagRepository = new TagRepository();
        private ProgressIndicator progressIndicator = null;

        public ModelProject()
        {
            this.id = Interface.Create("model_project");
            Interface.SetModelSampleCallback(id, "model", ModelCallback);
        }

        internal ModelProject(int id)
        {
            if (id > 0)
            {
                this.id = id;
                Interface.SetModelSampleCallback(id, "model", ModelCallback);
            }
        }

        public void Dispose()
        {
            Interface.Destroy(id);
        }

        internal int GetId()
        {
            return id;
        }

        internal void SetId(int id)
        {
            this.id = id;
            Interface.SetModelSampleCallback(id, "model", ModelCallback);
        }

        private void ModelCallback(ref ModelSampleStruct sample)
        {
            ModelSample modelSample = new ModelSample(sample);

            zFunction.Invoke(modelSample);

            modelSample.PrepareForReturn(ref sample);
        }


        public ZSampleDelegate ZFunction
        {
            get { return zFunction; }
            set { zFunction = value; }
        }

        public ProgressIndicator ProgressIndicator
        {
            get { return progressIndicator; }
            set
            {
                Interface.SetIntValue(id, "progress_indicator", value != null ? value.GetId() : 0);
                progressIndicator = value;
            }
        }

        public IList<ModelParameter> InputParameters
        {
            get
            {
                if (inputParameters == null)
                {
                    inputParameters = new CallBackList<ModelParameter>(InputParametersChanged);

                    int[] parameterIds = Interface.GetArrayIdValue(id, "input_parameters");
                    foreach (int parameterId in parameterIds)
                    {
                        inputParameters.AddWithoutCallBack(new ModelParameter(parameterId));
                    }
                }

                return inputParameters;
            }
        }

        private void InputParametersChanged(ListOperationType listOperation, ModelParameter item)
        {
            Interface.SetArrayIntValue(id, "input_parameters", this.inputParameters.Select(p => p.GetId()).ToArray());
        }

        public IList<ModelParameter> OutputParameters
        {
            get
            {
                if (outputParameters == null)
                {
                    outputParameters = new CallBackList<ModelParameter>(OutputParametersChanged);

                    int[] parameterIds = Interface.GetArrayIdValue(id, "output_parameters");
                    foreach (int parameterId in parameterIds)
                    {
                        outputParameters.AddWithoutCallBack(new ModelParameter(parameterId));
                    }
                }

                return outputParameters;
            }
        }

        private void OutputParametersChanged(ListOperationType listOperation, ModelParameter item)
        {
            Interface.SetArrayIntValue(id, "output_parameters", this.outputParameters.Select(p => p.GetId()).ToArray());
        }


        public IList<Stochast> Stochasts
        {
            get
            {
                if (stochasts == null)
                {
                    stochasts = new CallBackList<Stochast>(StochastValuesChanged);

                    int[] stochastIds = Interface.GetArrayIdValue(id, "variables");
                    foreach (int stochastId in stochastIds)
                    {
                        stochasts.AddWithoutCallBack(new Stochast(stochastId));
                    }
                }

                return stochasts;
            }
        }

        private void StochastValuesChanged(ListOperationType listOperation, Stochast item)
        {
            Interface.SetArrayIntValue(id, "variables", this.stochasts.Select(p => p.GetId()).ToArray());
        }

        public CorrelationMatrix CorrelationMatrix
        {
            get
            {
                if (correlationMatrix == null)
                {
                    int matrixId = Interface.GetIdValue(id, "correlation_matrix");
                    if (matrixId == 0)
                    {
                        correlationMatrix = new CorrelationMatrix();
                        Interface.SetIntValue(id, "correlation_matrix", correlationMatrix.GetId());
                    }
                    else
                    {
                        correlationMatrix = new CorrelationMatrix(matrixId);
                    }
                }

                return correlationMatrix;
            }
            set
            {
                Interface.SetIntValue(id, "correlation_matrix", value.GetId());
                correlationMatrix = value;
            }
        }

        public TagRepository TagRepository
        {
            get { return tagRepository; }
            set { tagRepository = value; }
        }
    }
}
