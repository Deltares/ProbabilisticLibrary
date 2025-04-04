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
#include <gtest/gtest.h>
#include "projectBuilder.h"
#include <iostream>


using namespace Deltares::Reliability;
using namespace Deltares::Statistics;
using namespace Deltares::Models;
using namespace Deltares::Sensitivity;

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            std::shared_ptr<ModelRunner> projectBuilder::BuildProject()
            {
                std::shared_ptr<ZModel> z(new ZModel(ZModel([this](std::shared_ptr<ModelSample> v) { return zfunc(v); })));
                auto stochast = std::vector<std::shared_ptr<Stochast>>();
                auto dist = DistributionType::Normal;
                std::vector<double> params{ 0.0, 1.0 };
                std::shared_ptr<Stochast> s(new Stochast(dist, params));
                stochast.push_back(s);
                stochast.push_back(s);
                std::shared_ptr<CorrelationMatrix> corr(new CorrelationMatrix());
                std::shared_ptr<UConverter> uConverter(new UConverter(stochast, corr));
                uConverter->initializeForRun();
                std::shared_ptr<ModelRunner> m(new ModelRunner(z, uConverter));
                return m;
            }

            std::shared_ptr<ModelRunner> projectBuilder::BuildLinearProject()
            {
                std::shared_ptr<ZModel> z(new ZModel(ZModel([this](std::shared_ptr<ModelSample> v) { return linear(v); })));
                auto stochast = std::vector<std::shared_ptr<Stochast>>();
                auto dist = DistributionType::Uniform;
                std::vector<double> params{ -1.0, 1.0 };
                std::shared_ptr<Stochast> s(new Stochast(dist, params));
                stochast.push_back(s);
                stochast.push_back(s);
                std::shared_ptr<CorrelationMatrix> corr(new CorrelationMatrix());
                std::shared_ptr<UConverter> uConverter(new UConverter(stochast, corr));
                uConverter->initializeForRun();
                std::shared_ptr<ModelRunner> m(new ModelRunner(z, uConverter));
                return m;
            }

            std::shared_ptr<ModelRunner> projectBuilder::BuildLinearOutputProject()
            {
                std::shared_ptr<ZModel> z(new ZModel(ZModel([this](std::shared_ptr<ModelSample> v) { return linearMultiple(v); })));
                auto stochast = std::vector<std::shared_ptr<Stochast>>();
                auto dist = DistributionType::Uniform;
                std::vector<double> params{ -1.0, 1.0 };
                std::shared_ptr<Stochast> s(new Stochast(dist, params));
                stochast.push_back(s);
                stochast.push_back(s);
                std::shared_ptr<CorrelationMatrix> corr(new CorrelationMatrix());
                std::shared_ptr<UConverter> uConverter(new UConverter(stochast, corr));
                uConverter->initializeForRun();
                std::shared_ptr<ModelRunner> m(new ModelRunner(z, uConverter));
                return m;
            }

            std::shared_ptr<ModelRunner> projectBuilder::BuildLinearArrayProject()
            {
                std::shared_ptr<ZModel> z(new ZModel(ZModel([this](std::shared_ptr<ModelSample> v) { return linear(v); })));
                auto stochast = std::vector<std::shared_ptr<Stochast>>();
                auto dist = DistributionType::Uniform;
                std::vector<double> params{ -1.0, 1.0 };
                std::shared_ptr<Stochast> s(new Stochast(dist, params));
                s->modelParameter->isArray = true;
                s->modelParameter->arraySize = 5;
                stochast.push_back(s);
                stochast.push_back(s);
                std::shared_ptr<CorrelationMatrix> corr(new CorrelationMatrix());
                std::shared_ptr<UConverter> uConverter(new UConverter(stochast, corr));
                uConverter->initializeForRun();
                std::shared_ptr<ModelRunner> m(new ModelRunner(z, uConverter));
                return m;
            }

            std::shared_ptr<ModelRunner> projectBuilder::BuildLinearVaryingArrayProject()
            {
                std::shared_ptr<ZModel> z(new ZModel(ZModel([this](std::shared_ptr<ModelSample> v) { return linear(v); })));
                auto stochasts = std::vector<std::shared_ptr<Stochast>>();
                std::shared_ptr<Stochast> s = std::make_shared<Stochast>();
                s->modelParameter->isArray = true;
                s->modelParameter->arraySize = 5;
                s->name = "s";
                for (int i = 0; i < s->modelParameter->arraySize; i++)
                {
                    std::shared_ptr<Stochast> s1(new Stochast());
                    s1->setDistributionType(DistributionType::Uniform);
                    s1->getProperties()->Minimum = i - 3;
                    s1->getProperties()->Maximum = i - 1;
                    s->ArrayVariables.push_back(s1);
                }
                stochasts.push_back(s);
                stochasts.push_back(s);
                std::shared_ptr<CorrelationMatrix> corr(new CorrelationMatrix());
                std::shared_ptr<UConverter> uConverter(new UConverter(stochasts, corr));
                uConverter->initializeForRun();
                std::shared_ptr<ModelRunner> m(new ModelRunner(z, uConverter));
                return m;
            }

            std::shared_ptr<ModelRunner> projectBuilder::BuildQuadraticProject()
            {
                std::shared_ptr<ZModel> z = std::make_shared<ZModel>(ZModel([](std::shared_ptr<ModelSample> v) { return quadratic(v); }));
                auto stochast = std::vector<std::shared_ptr<Stochast>>();
                auto dist = DistributionType::Uniform;
                std::vector<double> params{ -1.0, 1.0 };
                std::shared_ptr<Stochast> s(new Stochast(dist, params));
                s->modelParameter->isArray = false;
                s->modelParameter->arraySize = 1;
                stochast.push_back(s);
                stochast.push_back(s);
                std::shared_ptr<CorrelationMatrix> corr(new CorrelationMatrix());
                std::shared_ptr<UConverter> uConverter(new UConverter(stochast, corr));
                uConverter->initializeForRun();
                std::shared_ptr<ModelRunner> m(new ModelRunner(z, uConverter));
                return m;
            }

            std::shared_ptr<ModelRunner> projectBuilder::BuildProjectWithDeterminist(double valueDeterminist)
            {
                std::shared_ptr<ZModel> z = std::make_shared<ZModel>(ZModel([this](std::shared_ptr<ModelSample> v) { return zfuncWithDeterminist(v); }));
                auto stochast = std::vector<std::shared_ptr<Stochast>>();
                auto dist = DistributionType::Normal;
                std::vector<double> params{ 0.0, 1.0 };
                std::shared_ptr<Stochast> s = std::make_shared<Stochast>(dist, params);
                std::shared_ptr<Stochast> determinist(new Stochast(DistributionType::Deterministic, { valueDeterminist }));
                stochast.push_back(s);
                stochast.push_back(determinist);
                stochast.push_back(s);
                std::shared_ptr<CorrelationMatrix> corr = std::make_shared<CorrelationMatrix>();
                std::shared_ptr<UConverter> uConverter = std::make_shared<UConverter>(stochast, corr);
                uConverter->initializeForRun();
                std::shared_ptr<ModelRunner> m = std::make_shared<ModelRunner>(z, uConverter);
                return m;
            }

            std::shared_ptr<FragilityCurve> projectBuilder::BuildFragilityCurve()
            {
                std::shared_ptr<FragilityCurve> fragilityCurve = std::make_shared<FragilityCurve>();

                std::shared_ptr<FragilityValue> value1 = std::make_shared<FragilityValue>();
                value1->X = 0;
                value1->Reliability = 4.2;

                fragilityCurve->getProperties()->FragilityValues.push_back(value1);

                std::shared_ptr<FragilityValue> value2 = std::make_shared<FragilityValue>();
                value2->X = 10;
                value2->Reliability = 2.6;

                fragilityCurve->getProperties()->FragilityValues.push_back(value2);

                return fragilityCurve;
            }

            void projectBuilder::sum(std::shared_ptr<ModelSample> sample)
            {
                sample->Z = 0.0;
                for (double value : sample->Values)
                {
                    sample->Z += value;
                }
            }

            void projectBuilder::linear(std::shared_ptr<ModelSample> sample)
            {
                sample->Z = 1.8;
                for (double value : sample->Values)
                {
                    sample->Z -= value;
                }
            }

            void projectBuilder::linearMultiple(std::shared_ptr<ModelSample> sample)
            {
                sample->Z = 1.8;
                for (double value : sample->Values)
                {
                    sample->Z -= value;
                }

                // register twice in output values
                sample->OutputValues.push_back(sample->Z);
                sample->OutputValues.push_back(sample->Z);
            }

            void projectBuilder::quadratic(std::shared_ptr<ModelSample> sample)
            {
                sample->Z = 1.0;
                for (double value : sample->Values)
                {
                    sample->Z -= value * value;
                }
            }

            void projectBuilder::zfunc(std::shared_ptr<Deltares::Models::ModelSample> sample) const
            {
                sample->Z = 3.0 + sample->Values[1] - 1.25 * sample->Values[0];
                if (logZtoScreen)
                {
                    std::cout << "u, z = " << sample->Values[0] << " , " << sample->Values[1] << " , " << sample->Z << std::endl;
                }
            }

            void projectBuilder::zfuncWithDeterminist(std::shared_ptr<Deltares::Models::ModelSample> sample) const
            {
                sample->Z = sample->Values[1] + sample->Values[0] - 1.25 * sample->Values[2];
                if (logZtoScreen)
                {
                    std::cout << "u, z = " << sample->Values[0] << " , " << sample->Values[1] << " , " << sample->Z << std::endl;
                }
            }

            std::shared_ptr<SensitivityProject> projectBuilder::getSensitivityProject(std::shared_ptr<ReliabilityProject> project)
            {
                std::shared_ptr<SensitivityProject> sensitivityProject = std::make_shared<SensitivityProject>();

                for (std::shared_ptr<Stochast> stochast : project->stochasts)
                {
                    sensitivityProject->stochasts.push_back(stochast);
                }

                sensitivityProject->correlationMatrix = project->correlationMatrix;
                sensitivityProject->model = project->model;

                return sensitivityProject;
            }

            std::shared_ptr<RunProject> projectBuilder::getRunProject(std::shared_ptr<ReliabilityProject> project)
            {
                std::shared_ptr<RunProject> runProject = std::make_shared<RunProject>();

                for (std::shared_ptr<Stochast> stochast : project->stochasts)
                {
                    runProject->stochasts.push_back(stochast);
                }

                runProject->correlationMatrix = project->correlationMatrix;
                runProject->model = project->model;

                return runProject;
            }

            std::shared_ptr<ReliabilityProject> projectBuilder::getAddOneProject()
            {
                std::shared_ptr<ReliabilityProject> project = std::make_shared<ReliabilityProject>();

                project->stochasts.push_back(getDeterministicStochast(1));
                project->stochasts.push_back(getUniformStochast(-1));

                project->correlationMatrix = std::make_shared<CorrelationMatrix>();
                project->correlationMatrix->init(project->stochasts);

                project->model = std::make_shared<ZModel>(projectBuilder::sum);

                return project;
            }

            std::shared_ptr<ReliabilityProject> projectBuilder::getLinearProject()
            {
                std::shared_ptr<ReliabilityProject> project = std::make_shared<ReliabilityProject>();

                project->stochasts.push_back(getUniformStochast(-1));
                project->stochasts.push_back(getUniformStochast(-1));

                project->correlationMatrix = std::make_shared<CorrelationMatrix>();
                project->correlationMatrix->init(project->stochasts);

                project->model = std::make_shared<ZModel>(projectBuilder::linear);

                return project;
            }

            std::shared_ptr<ReliabilityProject> projectBuilder::getLinearOutputProject()
            {
                std::shared_ptr<ReliabilityProject> project = std::make_shared<ReliabilityProject>();

                project->stochasts.push_back(getUniformStochast(-1));
                project->stochasts.push_back(getUniformStochast(-1));

                project->correlationMatrix = std::make_shared<CorrelationMatrix>();
                project->correlationMatrix->init(project->stochasts);

                project->model = std::make_shared<ZModel>(projectBuilder::linearMultiple);
                project->model->outputParameters.push_back(std::make_shared<ModelInputParameter>("Result1"));
                project->model->outputParameters.push_back(std::make_shared<ModelInputParameter>("Result2"));

                return project;
            }
            
            std::shared_ptr<ReliabilityProject> projectBuilder::getTriangularLinearProject()
            {
                std::shared_ptr<ReliabilityProject> project = std::make_shared<ReliabilityProject>();

                project->stochasts.push_back(getTriangularStochast(0,0, 1));
                project->stochasts.push_back(getTriangularStochast(0, 0, 1));

                project->correlationMatrix = std::make_shared<CorrelationMatrix>();
                project->correlationMatrix->init(project->stochasts);

                project->model = std::make_shared<ZModel>(projectBuilder::linear);

                return project;
            }

            std::shared_ptr<Stochast> projectBuilder::getDeterministicStochast(double mean)
            {
                std::vector<double> values = {mean};
                return std::make_shared<Stochast>(DistributionType::Deterministic, values);
            }

            std::shared_ptr<Stochast>  projectBuilder::getNormalStochast(double mean, double stddev)
            {
                std::vector<double> values = { mean, stddev };
                return std::make_shared<Stochast>(DistributionType::Normal, values);
            }

            std::shared_ptr<Stochast>  projectBuilder::getUniformStochast(double min, double max)
            {
                std::vector<double> values = { min, max };
                return std::make_shared<Stochast>(DistributionType::Uniform, values);
            }

            std::shared_ptr<Stochast>  projectBuilder::getTriangularStochast(double min, double top, double max)
            {
                std::vector<double> values = { min, top, max };
                return std::make_shared<Stochast>(DistributionType::Triangular, values);
            }

            std::shared_ptr<Stochast> projectBuilder::getLogNormalStochast(double mean, double stddev, double shift)
            {
                std::vector<double> values = { mean, stddev, shift };
                return std::make_shared<Stochast>(DistributionType::LogNormal, values);

            }

            std::shared_ptr<Stochast> projectBuilder::getGumbelStochast(double mean, double stddev)
            {
                std::vector<double> values = { mean, stddev };
                return std::make_shared<Stochast>(DistributionType::Gumbel, values);
            }
        }
    }
}
