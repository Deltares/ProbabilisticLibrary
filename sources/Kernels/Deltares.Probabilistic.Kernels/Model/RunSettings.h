#pragma once
#include "Message.h"

class RunSettings
{
public:
    int MaxParallelProcesses = 1;
    int MaxChunkSize = 16;
    bool SaveEvaluations = false;
    bool SaveConvergence = false;
    bool SaveMessages = false;
    int MaxMessages = 1000;
    Deltares::Models::MessageType LowestMessageType = Deltares::Models::MessageType::Warning;
};

