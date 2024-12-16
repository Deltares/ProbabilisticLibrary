// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include <string>
#include <memory>
#include <iostream>

#include "basicSettings.h"
#include "createDistribution.h"
#include "../Deltares.Probabilistic/Utils/probLibException.h"
#include "../Deltares.Probabilistic/Math/vector1D.h"

#include "funcWrapper.h"
#include "progressWrapper.h"
#include "createReliabilityMethod.h"

using namespace Deltares::ProbLibCore;
using namespace Deltares::Models;
using namespace Deltares::Reliability;
using namespace Deltares::Numeric;
using namespace Deltares::Statistics;

struct fdistribs
{
    int distId;
    double params[4];
};

struct tResult
{
    tError error;
    double beta;
    double alpha[maxActiveStochast];
    int stepsNeeded;
    int samplesNeeded;
    bool convergence;
};

struct tCompIds
{
    int id;
    int nrStochasts;
    int nrCorrelations;
};

void updateX(const std::vector<std::shared_ptr<StochastPointAlpha>> & alpha, const DPoptions option, tResult & r, const std::shared_ptr<DesignPoint> & newResult,
    double x[], funcWrapper fw)
{
    if (alpha.size() <= maxActiveStochast)
    {
        switch (option) {
        case DPoptions::None:
        {
            for (size_t i = 0; i < alpha.size(); i++)
            {
                x[i] = 0.0;
            }
        }
        break;
        default:
        {
            for (size_t i = 0; i < alpha.size(); i++)
            {
                x[i] = newResult->Alphas[i]->X;
            }
        }
        }
    }
}

void copyConvergence(tResult& r, const DesignPoint& designPoint, const ProbMethod methodId)
{
    if (methodId == ProbMethod::NI) return;

    const auto convergenceReport = *designPoint.convergenceReport;

    r.convergence = convergenceReport.IsConverged;
    r.stepsNeeded = convergenceReport.TotalIterations;
    r.samplesNeeded = convergenceReport.TotalDirections;
    double failFraction = convergenceReport.FailFraction;
    int failedSamples = convergenceReport.FailedSamples;
    if (methodId == ProbMethod::DSFI || methodId == ProbMethod::DSFIHR)
    {
        // number of samples for DS from first step:
        r.samplesNeeded = designPoint.ContributingDesignPoints[0]->convergenceReport->TotalDirections;
        // number of FORM iterations from second step:
        r.stepsNeeded   = designPoint.ContributingDesignPoints[1]->convergenceReport->TotalIterations;
    }
    else if (r.samplesNeeded < 0 && failFraction > 0.0)
    {
        r.samplesNeeded = (int)round(failedSamples / failFraction);
    }
}

bool shouldRunAtDesignPoint(const DPoptions dpOption)
{
    switch (dpOption)
    {
    case DPoptions::RMinZFunc:
    case DPoptions::RMinZFuncCompatible:
        return true;
    default:
        return false;
    }
}

extern "C"
void probcalcf2c(const basicSettings* method, fdistribs c[], corrStruct correlations[],
    const double(*fx)(double[], computationSettings*, tError*),
    const bool(*pc)(ProgressType, const char*),
    const tCompIds* compIds, double x[], tResult* result)
{
    try
    {
        auto nStoch = (size_t)compIds->nrStochasts;
        auto fw = funcWrapper(compIds->id, fx);
        auto nrCorrelations = compIds->nrCorrelations;

        auto stochasts = std::vector<std::shared_ptr<Deltares::Statistics::Stochast>>();
        for (size_t i = 0; i < nStoch; i++)
        {
            auto distHR = (EnumDistributions)c[i].distId;
            auto s = createDistribution::createValid(distHR, c[i].params);
            stochasts.push_back(s);
        }

        auto createRelM = createReliabilityMethod();
        auto relMethod = createRelM.selectMethod(*method, nStoch, stochasts);
        auto zModel = std::make_shared<ZModel>([&fw](std::shared_ptr<ModelSample> v) { return fw.FDelegate(v); },
                                               [&fw](std::vector<std::shared_ptr<ModelSample>> v) { return fw.FDelegateParallel(v); }
        );

        auto corr = std::make_shared<CorrelationMatrix>();
        if (nrCorrelations > 0)
        {
            corr->init((int)nStoch);
            for (int i = 0; i < nrCorrelations; i++)
            {
                corr->SetCorrelation(correlations[i].idx1, correlations[i].idx2, correlations[i].correlation);
            }
        }
        auto uConverter = std::make_shared<UConverter>(stochasts, corr);
        auto pw = progressWrapper(pc, relMethod.get());
        auto progressDelegate = ProgressLambda();
        auto detailedProgressDelegate = DetailedProgressLambda();
        auto textualProgress = TextualProgressLambda([&pw](ProgressType p, std::string s) {pw.FPgDelegate(p, s); });
        auto progress = std::make_shared<ProgressIndicator>(progressDelegate, detailedProgressDelegate, textualProgress);
        auto modelRunner = std::make_shared<ModelRunner>(zModel, uConverter, progress);
        modelRunner->Settings->MaxParallelProcesses = method->numThreads;
        modelRunner->Settings->MaxChunkSize = method->chunkSize;
        modelRunner->Settings->SaveMessages = true;
        modelRunner->Settings->RunAtDesignPoint = shouldRunAtDesignPoint(method->designPointOptions);
        modelRunner->Settings->ExtendedLoggingAtDesignPoint = true;

        modelRunner->initializeForRun();
        auto newResult = relMethod->getDesignPoint(modelRunner);

        if (modelRunner->Settings->RunAtDesignPoint)
        {
            modelRunner->runDesignPoint(newResult);
        }

        auto allMessages = newResult->Messages;
        for (const auto& s : fw.error_messages )
        {
            allMessages.push_back(std::make_shared<Message>(Error, s));
        }

        result->error.errorCode = 0;
        for(const auto& message : allMessages)
        {
            if (message->Type == Error)
            {
                result->error.errorCode = 1;
                fillErrorMessage(result->error, message->Text);
            }
            else
            {
                // TODO: connect to feedback
                std::cout << message->Text << std::endl;
            }
        }

        result->beta = newResult->Beta;
        for (size_t i = 0; i < nStoch; i++)
        {
            result->alpha[i] = newResult->Alphas[i]->Alpha;
        }

        updateX(newResult->Alphas, method->designPointOptions, *result, newResult, x, fw);

        copyConvergence(*result, *newResult, method->methodId);
    }
    catch (const std::exception& e)
    {
        std::string s = e.what();
        result->error.errorCode = -1;
        fillErrorMessage(result->error, s);
    }
}
