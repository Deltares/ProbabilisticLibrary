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
#include "../Deltares.Probabilistic/Model/DefaultValueConverter.h"
#include <iostream>

#include "../Deltares.Probabilistic/Statistics/CopulaCorrelation.h"

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> projectBuilder::BuildProject() const
    {
        auto z = std::make_shared<Models::ZModel>([this](std::shared_ptr<Models::ModelSample> v) { return zfunc(v); });
        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        auto dist = Statistics::DistributionType::Normal;
        std::vector<double> params{ 0.0, 1.0 };
        std::shared_ptr<Statistics::Stochast> s = std::make_shared<Statistics::Stochast>(dist, params);
        stochast.push_back(s);
        stochast.push_back(s);
        std::shared_ptr<Statistics::CorrelationMatrix> corr = std::make_shared<Statistics::CorrelationMatrix>(true);
        std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        std::shared_ptr<Models::ModelRunner> m = std::make_shared<Models::ModelRunner>(z, uConverter);
        return m;
    }

    std::shared_ptr<Models::ModelRunner> projectBuilder::BuildLinearProject()
    {
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v) { return linear(v); });
        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        auto dist = Statistics::DistributionType::Uniform;
        std::vector<double> params{ -1.0, 1.0 };
        std::shared_ptr<Statistics::Stochast> s = std::make_shared<Statistics::Stochast>(dist, params);
        stochast.push_back(s);
        stochast.push_back(s);
        std::shared_ptr<Statistics::CorrelationMatrix> corr = std::make_shared<Statistics::CorrelationMatrix>(true);
        std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        std::shared_ptr<Models::ModelRunner> m = std::make_shared<Models::ModelRunner>(z, uConverter);
        return m;
    }

    std::shared_ptr<Models::ModelRunner> projectBuilder::BuildLinearOutputOnlyProject()
    {
        auto zModel = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> sample) { return linearOutputOnly(sample); });
        zModel->zValueConverter = std::make_shared<Deltares::Models::DefaultValueConverter>();

        auto stochasts = std::vector<std::shared_ptr<Statistics::Stochast>>();
        std::shared_ptr<Statistics::Stochast> stochast = std::make_shared<Statistics::Stochast>(Statistics::DistributionType::Uniform, std::vector<double> { -1.0, 1.0 });
        stochasts.push_back(stochast);
        stochasts.push_back(stochast);

        std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix = std::make_shared<Statistics::CorrelationMatrix>(true);
        std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(stochasts, correlationMatrix);
        uConverter->initializeForRun();

        std::shared_ptr<Models::ModelRunner> modelRunner = std::make_shared<Models::ModelRunner>(zModel, uConverter);
        return modelRunner;
    }

    std::shared_ptr<Models::ModelRunner> projectBuilder::BuildLinearOutputProject()
    {
        std::shared_ptr<Models::ZModel> z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v) { return linearMultiple(v); });
        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        auto dist = Statistics::DistributionType::Uniform;
        std::vector<double> params{ -1.0, 1.0 };
        std::shared_ptr<Statistics::Stochast> s = std::make_shared<Statistics::Stochast>(dist, params);
        stochast.push_back(s);
        stochast.push_back(s);
        std::shared_ptr<Statistics::CorrelationMatrix> corr = std::make_shared<Statistics::CorrelationMatrix>(true);
        std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        std::shared_ptr<Models::ModelRunner> m = std::make_shared<Models::ModelRunner>(z, uConverter);
        return m;
    }

    std::shared_ptr<Models::ModelRunner> projectBuilder::BuildLinearArrayProject()
    {
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v) { return linear(v); });
        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        auto dist = Statistics::DistributionType::Uniform;
        std::vector<double> params{ -1.0, 1.0 };
        std::shared_ptr<Statistics::Stochast> s = std::make_shared<Statistics::Stochast>(dist, params);
        s->modelParameter->isArray = true;
        s->modelParameter->arraySize = 5;
        stochast.push_back(s);
        stochast.push_back(s);
        std::shared_ptr<Statistics::CorrelationMatrix> corr = std::make_shared<Statistics::CorrelationMatrix>(true);
        std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        std::shared_ptr<Models::ModelRunner> m = std::make_shared<Models::ModelRunner>(z, uConverter);
        return m;
    }

    std::shared_ptr<Models::ModelRunner> projectBuilder::BuildLinearVaryingArrayProject()
    {
        std::shared_ptr<Models::ZModel> z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v) { return linear(v); });
        auto stochasts = std::vector<std::shared_ptr<Statistics::Stochast>>();
        std::shared_ptr<Statistics::Stochast> s = std::make_shared<Statistics::Stochast>();
        s->modelParameter->isArray = true;
        s->modelParameter->arraySize = 5;
        s->name = "s";
        for (int i = 0; i < s->modelParameter->arraySize; i++)
        {
            std::shared_ptr<Statistics::Stochast> s1 = std::make_shared<Statistics::Stochast>();
            s1->setDistributionType(Statistics::DistributionType::Uniform);
            s1->getProperties()->Minimum = i - 3;
            s1->getProperties()->Maximum = i - 1;
            s->ArrayVariables.push_back(s1);
        }
        stochasts.push_back(s);
        stochasts.push_back(s);
        std::shared_ptr<Statistics::CorrelationMatrix> corr = std::make_shared<Statistics::CorrelationMatrix>(true);
        std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(stochasts, corr);
        uConverter->initializeForRun();
        std::shared_ptr<Models::ModelRunner> m = std::make_shared<Models::ModelRunner>(z, uConverter);
        return m;
    }

    std::shared_ptr<Models::ModelRunner> projectBuilder::BuildQuadraticProject()
    {
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v) { return quadratic(v); });
        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        auto dist = Statistics::DistributionType::Uniform;
        std::vector<double> params{ -1.0, 1.0 };
        std::shared_ptr<Statistics::Stochast> s = std::make_shared<Statistics::Stochast>(dist, params);
        s->modelParameter->isArray = false;
        s->modelParameter->arraySize = 1;
        stochast.push_back(s);
        stochast.push_back(s);
        std::shared_ptr<Statistics::CorrelationMatrix> corr = std::make_shared<Statistics::CorrelationMatrix>(true);
        std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        std::shared_ptr<Models::ModelRunner> m = std::make_shared<Models::ModelRunner>(z, uConverter);
        return m;
    }

    std::shared_ptr<Models::ModelRunner> projectBuilder::BuildProjectWithDeterminist(double valueDeterminist) const
    {
        auto z = std::make_shared<Models::ZModel>([this](std::shared_ptr<Models::ModelSample> v)
        { return zfuncWithDeterminist(v); });
        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochast.push_back(getNormalStochast(0.0, 1.0));
        stochast.push_back(getDeterministicStochast(valueDeterminist));
        stochast.push_back(getNormalStochast(0.0, 1.0));
        auto corr = std::make_shared<Statistics::CorrelationMatrix>(true);
        auto uConverter = std::make_shared<Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        auto m = std::make_shared<Models::ModelRunner>(z, uConverter);
        return m;
    }

    std::shared_ptr<Models::ModelRunner> projectBuilder::BuildProjectWithDeterministAndCopula(double valueDeterminist) const
    {
        auto z = std::make_shared<Models::ZModel>([this](std::shared_ptr<Models::ModelSample> v)
        { return zfuncWithDeterminist(v); });
        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochast.push_back(getNormalStochast(0.0, 1.0));
        stochast.push_back(getDeterministicStochast(valueDeterminist));
        stochast.push_back(getNormalStochast(0.0, 1.0));
        auto corr = std::make_shared<Statistics::CopulaCorrelation>();
        corr->Init(3);
        corr->SetCorrelation(0, 2, 0.5, CorrelationType::Frank);
        auto uConverter = std::make_shared<Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        auto m = std::make_shared<Models::ModelRunner>(z, uConverter);
        return m;
    }

    std::shared_ptr<Models::ModelRunner> projectBuilder::BuildProjectWithPolynome() const
    {
        auto z = std::make_shared<Models::ZModel>([this](std::shared_ptr<Models::ModelSample> v)
        { return zfuncPolynome(v); });
        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochast.push_back(getDeterministicStochast(1.0));
        stochast.push_back(getDeterministicStochast(1.0));
        stochast.push_back(getLogNormalStochast(1.0, 0.5));
        stochast.push_back(getLogNormalStochast(1.0, 0.5));
        auto corr = std::make_shared<Statistics::CorrelationMatrix>(true);
        auto uConverter = std::make_shared<Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        auto m = std::make_shared<Models::ModelRunner>(z, uConverter);
        return m;
    }

    std::shared_ptr<Models::ModelRunner> projectBuilder::BuildProjectTwoBranches(bool useProxy) const
    {
        std::shared_ptr<Models::ZModel> z;
        if (useProxy)
        {
            z = std::make_shared<Models::ZModel>([this](std::shared_ptr<Models::ModelSample> v)
            { return zfuncTwoBranchesProxy(v); });
        }
        else
        {
            z = std::make_shared<Models::ZModel>([this](std::shared_ptr<Models::ModelSample> v)
            { return zfuncTwoBranches(v); });
        }

        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochast.push_back(getNormalStochast(10.0, 0.5));
        stochast.push_back(getNormalStochast(0.0, 1.0));
        stochast.push_back(getNormalStochast(4.0, 1.0));
        auto corr = std::make_shared<Statistics::CorrelationMatrix>(true);
        auto uConverter = std::make_shared<Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        const auto m = std::make_shared<Models::ModelRunner>(z, uConverter);
        return m;
    }

    std::shared_ptr<Reliability::FragilityCurve> projectBuilder::BuildFragilityCurve()
    {
        std::shared_ptr<Reliability::FragilityCurve> fragilityCurve = std::make_shared<Reliability::FragilityCurve>();

        std::shared_ptr<Statistics::FragilityValue> value1 = std::make_shared<Statistics::FragilityValue>();
        value1->X = 0;
        value1->Reliability = 4.2;

        fragilityCurve->getProperties()->FragilityValues.push_back(value1);

        std::shared_ptr<Statistics::FragilityValue> value2 = std::make_shared<Statistics::FragilityValue>();
        value2->X = 10;
        value2->Reliability = 2.6;

        fragilityCurve->getProperties()->FragilityValues.push_back(value2);

        return fragilityCurve;
    }

    void projectBuilder::sum(std::shared_ptr<Models::ModelSample> sample)
    {
        sample->Z = 0.0;
        for (double value : sample->Values)
        {
            sample->Z += value;
        }
    }

    void projectBuilder::linear(std::shared_ptr<Models::ModelSample> sample)
    {
        sample->Z = 1.8;
        for (double value : sample->Values)
        {
            sample->Z -= value;
        }
    }

    void projectBuilder::linearOutputOnly(std::shared_ptr<Models::ModelSample> sample)
    {
        double z = 1.8;
        for (double value : sample->Values)
        {
            z -= value;
        }

        sample->OutputValues.push_back(z);
    }

    void projectBuilder::linearMultiple(std::shared_ptr<Models::ModelSample> sample)
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

    void projectBuilder::quadratic(std::shared_ptr<Models::ModelSample> sample)
    {
        sample->Z = 1.0;
        for (double value : sample->Values)
        {
            sample->Z -= value * value;
        }
    }

    void projectBuilder::zfunc(std::shared_ptr<Models::ModelSample> sample) const
    {
        sample->Z = 3.0 + sample->Values[1] - 1.25 * sample->Values[0];
        if (logZtoScreen)
        {
            std::cout << "u, z = " << sample->Values[0] << " , " << sample->Values[1] << " , " << sample->Z << std::endl;
        }
    }

    void projectBuilder::zfuncWithDeterminist(std::shared_ptr<Models::ModelSample> sample) const
    {
        sample->Z = sample->Values[1] + sample->Values[0] - 1.25 * sample->Values[2];
        if (logZtoScreen)
        {
            std::cout << "u, z = " << sample->Values[0] << " , " << sample->Values[1] << " , " << sample->Z << std::endl;
        }
    }

    void projectBuilder::zfuncPolynome(std::shared_ptr<Models::ModelSample> sample) const
    {
        double x = sample->Values[0];
        double y = sample->Values[1];
        double z = sample->Values[2];
        double u = sample->Values[3];

        double p = x + 0.3 * y * y + 0.2 * z * z * z + 0.125 * u * u * u * u;

        sample->Z = 24 - p;
    }

    void projectBuilder::zfuncTwoBranches(std::shared_ptr<Models::ModelSample> sample) const
    {
        double a = sample->Values[0];
        double b = sample->Values[1];
        double c = sample->Values[2];
        sample->Z = c <= 5.0 ? a - b - c : c - b;
    }

    void projectBuilder::zfuncTwoBranchesProxy(std::shared_ptr<Models::ModelSample> sample) const
    {
        const std::vector coeff = { 215.49, -41.012, -1.1379, -2.652, 2.0742, -0.53193, 0.23794 };
        double z = coeff[0];
        for (size_t i = 0; i < sample->Values.size(); i++)
        {
            auto x = sample->Values[i];
            z += coeff[i + 1] * x + coeff[i + 4] * pow(x, 2);
        }
        sample->Z = z;
    }

    std::shared_ptr<Uncertainty::UncertaintyProject> projectBuilder::getUncertaintyProject(std::shared_ptr<Reliability::ReliabilityProject> project)
    {
        std::shared_ptr<Uncertainty::UncertaintyProject> uncertaintyProject = std::make_shared<Uncertainty::UncertaintyProject>();

        for (std::shared_ptr<Statistics::Stochast> stochast : project->stochasts)
        {
            uncertaintyProject->stochasts.push_back(stochast);
        }

        uncertaintyProject->correlation = project->correlation;
        uncertaintyProject->model = project->model;

        return uncertaintyProject;
    }

    std::shared_ptr<Sensitivity::SensitivityProject> projectBuilder::getSensitivityProject(std::shared_ptr<Reliability::ReliabilityProject> project)
    {
        std::shared_ptr<Sensitivity::SensitivityProject> sensitivityProject = std::make_shared<Sensitivity::SensitivityProject>();

        for (std::shared_ptr<Statistics::Stochast> stochast : project->stochasts)
        {
            sensitivityProject->stochasts.push_back(stochast);
        }

        sensitivityProject->correlation = project->correlation;
        sensitivityProject->model = project->model;

        return sensitivityProject;
    }

    std::shared_ptr<Models::RunProject> projectBuilder::getRunProject(std::shared_ptr<Reliability::ReliabilityProject> project)
    {
        std::shared_ptr<Models::RunProject> runProject = std::make_shared<Models::RunProject>();

        for (std::shared_ptr<Statistics::Stochast> stochast : project->stochasts)
        {
            runProject->stochasts.push_back(stochast);
        }

        runProject->correlation = project->correlation;
        runProject->model = project->model;

        return runProject;
    }

    std::shared_ptr<Reliability::ReliabilityProject> projectBuilder::getAddOneProject()
    {
        std::shared_ptr<Reliability::ReliabilityProject> project = std::make_shared<Reliability::ReliabilityProject>();

        project->stochasts.push_back(getDeterministicStochast(1));
        project->stochasts.push_back(getUniformStochast(-1));

        project->correlation = std::make_shared<Statistics::CorrelationMatrix>(true);
        project->correlation->Init(project->stochasts);

        project->model = std::make_shared<Models::ZModel>(projectBuilder::sum);

        return project;
    }

    std::shared_ptr<Reliability::ReliabilityProject> projectBuilder::getLinearProject()
    {
        std::shared_ptr<Reliability::ReliabilityProject> project = std::make_shared<Reliability::ReliabilityProject>();

        project->stochasts.push_back(getUniformStochast(-1));
        project->stochasts.push_back(getUniformStochast(-1));

        project->correlation = std::make_shared<Statistics::CorrelationMatrix>(true);
        project->correlation->Init(project->stochasts);

        project->model = std::make_shared<Models::ZModel>(linear);

        return project;
    }

    std::shared_ptr<Reliability::ReliabilityProject> projectBuilder::getLinearOutputProject()
    {
        std::shared_ptr<Reliability::ReliabilityProject> project = std::make_shared<Reliability::ReliabilityProject>();

        project->stochasts.push_back(getUniformStochast(-1));
        project->stochasts.push_back(getUniformStochast(-1));

        project->correlation = std::make_shared<Statistics::CorrelationMatrix>(true);
        project->correlation->Init(project->stochasts);

        project->model = std::make_shared<Models::ZModel>(projectBuilder::linearMultiple);
        project->model->outputParameters.push_back(std::make_shared<Models::ModelInputParameter>("Result1"));
        project->model->outputParameters.push_back(std::make_shared<Models::ModelInputParameter>("Result2"));

        return project;
    }

    std::shared_ptr<Reliability::ReliabilityProject> projectBuilder::getTriangularLinearProject()
    {
        std::shared_ptr<Reliability::ReliabilityProject> project = std::make_shared<Reliability::ReliabilityProject>();

        project->stochasts.push_back(getTriangularStochast(0, 0, 1));
        project->stochasts.push_back(getTriangularStochast(0, 0, 1));

        project->correlation = std::make_shared<Statistics::CorrelationMatrix>(true);
        project->correlation->Init(project->stochasts);

        project->model = std::make_shared<Models::ZModel>(projectBuilder::linear);

        return project;
    }

    std::shared_ptr<Reliability::ReliabilityProject> projectBuilder::getArrayVariableProject()
    {
        std::shared_ptr<Reliability::ReliabilityProject> project = std::make_shared<Reliability::ReliabilityProject>();

        std::shared_ptr<Statistics::Stochast> a = getUniformStochast(0, 1);
        std::shared_ptr<Statistics::Stochast> b = getUniformVariableStochast(a, 0, 1);

        a->modelParameter->isArray = true;
        a->modelParameter->arraySize = 5;

        b->modelParameter->isArray = true;
        b->modelParameter->arraySize = 5;

        project->stochasts.push_back(a);
        project->stochasts.push_back(b);

        project->correlation = std::make_shared<Statistics::CorrelationMatrix>(true);
        project->correlation->Init(project->stochasts);

        project->model = std::make_shared<Models::ZModel>(projectBuilder::sum);

        return project;
    }

    std::shared_ptr<Statistics::Stochast> projectBuilder::getDeterministicStochast(double mean)
    {
        std::vector<double> values = { mean };
        return std::make_shared<Statistics::Stochast>(Statistics::DistributionType::Deterministic, values);
    }

    std::shared_ptr<Statistics::Stochast>  projectBuilder::getNormalStochast(double mean, double stddev)
    {
        std::vector<double> values = { mean, stddev };
        return std::make_shared<Statistics::Stochast>(Statistics::DistributionType::Normal, values);
    }

    std::shared_ptr<Statistics::Stochast>  projectBuilder::getUniformStochast(double min, double max)
    {
        std::vector<double> values = { min, max };
        return std::make_shared<Statistics::Stochast>(Statistics::DistributionType::Uniform, values);
    }

    std::shared_ptr<Statistics::Stochast>  projectBuilder::getUniformVariableStochast(const std::shared_ptr<Statistics::Stochast>& source, double min, double max)
    {
        std::vector<double> values = { min, max };
        double diff = max - min;

        auto stochast =  std::make_shared<Statistics::Stochast>(Statistics::DistributionType::Uniform, values);

        stochast->IsVariableStochast = true;
        stochast->VariableSource = source;

        auto value1 = std::make_shared<Statistics::VariableStochastValue>();
        value1->X = 0.2;
        value1->Stochast->Minimum = min + 0.1 * diff;
        value1->Stochast->Maximum = min + 0.3 * diff;
        stochast->ValueSet->StochastValues.push_back(value1);

        auto value2 = std::make_shared<Statistics::VariableStochastValue>();
        value2->X = 0.8;
        value2->Stochast->Minimum = max - 0.3 * diff;
        value2->Stochast->Maximum = max - 0.1 * diff;
        stochast->ValueSet->StochastValues.push_back(value2);

        return stochast;
    }

    std::shared_ptr<Statistics::Stochast>  projectBuilder::getTriangularStochast(double min, double top, double max)
    {
        std::vector<double> values = { min, top, max };
        return std::make_shared<Statistics::Stochast>(Statistics::DistributionType::Triangular, values);
    }

    std::shared_ptr<Statistics::Stochast> projectBuilder::getLogNormalStochast(double mean, double stddev, double shift)
    {
        std::vector<double> values = { mean, stddev, shift };
        return std::make_shared<Statistics::Stochast>(Statistics::DistributionType::LogNormal, values);
    }

    std::shared_ptr<Statistics::Stochast> projectBuilder::getGumbelStochast(double mean, double stddev)
    {
        std::vector<double> values = { mean, stddev };
        return std::make_shared<Statistics::Stochast>(Statistics::DistributionType::Gumbel, values);
    }
}
