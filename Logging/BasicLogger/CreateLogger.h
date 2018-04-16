// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

// References
// Doctor Dobbs - A lightweight logger
// A Thread-Safe Logger

#pragma once

#include "Logger.h"
#include "ConsoleLogPolicy.h"
#include "FileLogPolicy.h"

std::shared_ptr< CLogger < CConsoleLogPolicy > > g_consoleLoggerInstance;
std::shared_ptr< CLogger < CFileLogPolicy > > g_fileLoggerInstance;

std::once_flag initConsoleLoggerFlag;
std::once_flag initFileLoggerFlag;

class CCreateLogger
{
public:
     static std::shared_ptr< CLogger < CConsoleLogPolicy > >& CreateConsoleLogger(const std::string& name = "ConsoleLogger", ESeverity level = ESeverity::Trace)
     {
          std::call_once(initConsoleLoggerFlag, InitConsoleLogger, name, level);
          return g_consoleLoggerInstance;
     }

     static std::shared_ptr< CLogger < CFileLogPolicy > >& CreateFileLogger(const std::string& name = "C:\\FileLogger.log", ESeverity level = ESeverity::Trace)
     {
          std::call_once(initFileLoggerFlag, InitFileLogger, name, level);
          return g_fileLoggerInstance;
     }

     static void DeleteConsoleLogger(std::shared_ptr< CLogger < CConsoleLogPolicy > >& consoleLoggerInstance)
     {
          consoleLoggerInstance.reset();
     }

     static void DeleteFileLogger(std::shared_ptr< CLogger < CFileLogPolicy >>& fileLoggerInstance)
     {
          fileLoggerInstance.reset();
     }

private:
     static void InitConsoleLogger(const std::string& name, ESeverity level)
     {
          g_consoleLoggerInstance = std::make_shared < CLogger < CConsoleLogPolicy >>(name, level);
     }

     static void InitFileLogger(const std::string& name, ESeverity level)
     {
          g_fileLoggerInstance = std::make_shared < CLogger < CFileLogPolicy >>(name, level);
     }
};
