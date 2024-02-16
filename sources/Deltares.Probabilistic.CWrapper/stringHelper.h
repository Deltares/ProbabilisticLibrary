#pragma once
#include <stddef.h>
#include <string>

const size_t ERRORMSGLENGTH = 256;
struct tError
{
    char errorMessage[ERRORMSGLENGTH];
    int errorCode;
};

void fillErrorMessage(tError & error, const std::string s);
