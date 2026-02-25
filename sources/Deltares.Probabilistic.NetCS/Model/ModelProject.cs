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

    public class ModelProject : IStochastProvider, IDisposable
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

            modelSample.PrepareForReturn(ref sample, tagRepository);
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
                if (value != null)
                {
                    Interface.SetProgressCallbacks(id, value.DoProgress, value.DoDetailedProgress, value.DoTextualProgress);
                }

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
                        correlationMatrix = new CorrelationMatrix(matrixId, this);
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

        protected internal TagRepository TagRepository
        {
            get { return tagRepository; }
        }

        public virtual Stochast GetStochast(int stochastId)
        {
            return this.stochasts.FirstOrDefault(p => p.GetId() == stochastId);
        }
    }
}
