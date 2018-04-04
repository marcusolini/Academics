#pragma once

#include "Logger.h"
#include "ConsoleLogPolicy.h"
#include "FileLogPolicy.h"

extern CLogger < CConsoleLogPolicy > *pConsoleLoggerInstance;
extern CLogger < CFileLogPolicy > *pFileLoggerInstance;


#define LOG(DATA) \
if (pConsoleLoggerInstance) pConsoleLoggerInstance->print< ESeverity::Informational >(DATA); \
if (pFileLoggerInstance) pFileLoggerInstance->print< ESeverity::Informational >(DATA) \

#define LOG_WARNING(DATA) \
if (pConsoleLoggerInstance) pConsoleLoggerInstance->print< ESeverity::Warning >(DATA); \
if (pFileLoggerInstance) pFileLoggerInstance->print< ESeverity::Warning >(DATA) \

#define LOG_ERROR(DATA) \
if (pConsoleLoggerInstance) pConsoleLoggerInstance->print< ESeverity::Error >(DATA); \
if (pFileLoggerInstance) pFileLoggerInstance->print< ESeverity::Error >(DATA) \