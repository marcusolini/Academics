#pragma once

#include "Logger.h"
#include "ConsoleLogPolicy.h"
#include "FileLogPolicy.h"

extern std::shared_ptr< CLogger < CConsoleLogPolicy > > g_consoleLoggerInstance;
extern std::shared_ptr< CLogger < CFileLogPolicy > > g_fileLoggerInstance;


#define LOG(...) \
if (g_consoleLoggerInstance) g_consoleLoggerInstance->Print< ESeverity::Informational >(" " __FUNCTION__, ":", __LINE__, " ~ ", __VA_ARGS__); \
if (g_fileLoggerInstance) g_fileLoggerInstance->Print< ESeverity::Informational >(" " __FUNCTION__, ":", __LINE__, " ~ ", __VA_ARGS__) \

#define LOG_WARNING(...) \
if (g_consoleLoggerInstance) g_consoleLoggerInstance->Print< ESeverity::Warning >(" " __FUNCTION__, ":", __LINE__, " ~ ", __VA_ARGS__); \
if (g_fileLoggerInstance) g_fileLoggerInstance->Print< ESeverity::Warning >(" " __FUNCTION__, ":", __LINE__, " ~ ", __VA_ARGS__) \

#define LOG_ERROR(...) \
if (g_consoleLoggerInstance) g_consoleLoggerInstance->print< ESeverity::Error >(" " __FUNCTION__, ":", __LINE__, " ~ ", __VA_ARGS__); \
if (g_fileLoggerInstance) g_fileLoggerInstance->Print< ESeverity::Error >(" " __FUNCTION__, ":", __LINE__, " ~ ", __VA_ARGS__) \