#pragma once

#include "Logger.h"
#include "ConsoleLogPolicy.h"
#include "FileLogPolicy.h"

extern CLogger < CConsoleLogPolicy > *pConsoleLoggerInstance;
extern CLogger < CFileLogPolicy > *pFileLoggerInstance;


#define LOG(...) \
if (pConsoleLoggerInstance) pConsoleLoggerInstance->print< ESeverity::Informational >(__VA_ARGS__); \
if (pFileLoggerInstance) pFileLoggerInstance->print< ESeverity::Informational >(__VA_ARGS__) \

#define LOG_WARNING(...) \
if (pConsoleLoggerInstance) pConsoleLoggerInstance->print< ESeverity::Warning >(__VA_ARGS__); \
if (pFileLoggerInstance) pFileLoggerInstance->print< ESeverity::Warning >(__VA_ARGS__) \

#define LOG_ERROR(...) \
if (pConsoleLoggerInstance) pConsoleLoggerInstance->print< ESeverity::Error >(__VA_ARGS__); \
if (pFileLoggerInstance) pFileLoggerInstance->print< ESeverity::Error >(__VA_ARGS__) \