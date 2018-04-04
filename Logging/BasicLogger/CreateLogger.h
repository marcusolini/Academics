#pragma once

#include "Logger.h"
#include "ConsoleLogPolicy.h"

CLogger < CConsoleLogPolicy > *pConsoleLoggerInstance = 0;
CLogger < CFileLogPolicy > *pFileLoggerInstance = 0;

std::mutex gCreateLoggerMutex;

class CCreateLogger
{
public:
     static void CreateConsoleLogger(const std::string& name = "ConsoleLogger")
     {
          std::lock_guard<std::mutex> lock(gCreateLoggerMutex);

          if (nullptr == pConsoleLoggerInstance)
          {
               pConsoleLoggerInstance = new CLogger < CConsoleLogPolicy >(name);
          }
     }

     static void CreateFileLogger(const std::string& name = "C:\\ConsoleLogger.log")
     {
          std::lock_guard<std::mutex> lock(gCreateLoggerMutex);

          if (nullptr == pFileLoggerInstance)
          {
               pFileLoggerInstance = new CLogger < CFileLogPolicy >(name);
          }
     }

     static void DeleteConsoleLogger()
     {
          std::lock_guard<std::mutex> lock(gCreateLoggerMutex);

          if (nullptr != pConsoleLoggerInstance)
          {
               delete pConsoleLoggerInstance;
               pConsoleLoggerInstance = nullptr;
          }
     }

     static void DeleteFileLogger()
     {
          std::lock_guard<std::mutex> lock(gCreateLoggerMutex);

          if (nullptr != pFileLoggerInstance)
          {
               delete pFileLoggerInstance;
               pFileLoggerInstance = nullptr;
          }
     }

};
