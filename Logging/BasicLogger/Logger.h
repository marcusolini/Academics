// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

// References
// Doctor Dobbs - A lightweight logger
// A Thread-Safe Logger

#pragma once

// TODO
// Add logfile size limits
// Add new incremental logfile creation (log-00000, log-00001)
// Add circular buffer to maintain messages and prune. For automatic logging.
// Add automatic logging of all circular buffer when error occurs.
// Consider consoldating differnt logging policies to use one buffer.

#include "LogPolicy.h"

#include <mutex>
#include <sstream>
#include <time.h>
#include <atomic>
#include <deque> 
#include <chrono>


enum class ESeverity
{
     Trace = 0,
     Informational,
     Warning,
     Error
};



template< class LogPolicy > class CLogger
{
public:

     CLogger(const std::string& name, ESeverity level = ESeverity::Trace);
     virtual ~CLogger();

     template< ESeverity severity, class...Args >
     void Print(Args...args);
     
private:
     ESeverity m_level = ESeverity::Trace;

     unsigned m_lineNumber = 0;

     void GenerateTime();
     void GenerateHeader();

     std::ostringstream m_logStream;
     std::ostringstream m_headerStream;
     std::string m_timeString;

     ILogPolicy* m_pPolicy = nullptr;;

     std::mutex m_writeMutex;
     std::thread m_loggingThread;
     std::atomic_bool m_bLoggingThreadContinue;
     std::deque < std::string> m_loggingQueue;
     std::timed_mutex m_LoggingQueueTimedMutex;

     void PrintImpl();
     template<class First, class...Rest> void PrintImpl(First parm1, Rest...parm);

     static void LoggingThread(CLogger< LogPolicy >* pLogger);
};


template< class LogPolicy >
CLogger< LogPolicy >::CLogger(const std::string& name, ESeverity level)
{
     m_level = level;
     m_bLoggingThreadContinue = true;

     if (nullptr == m_pPolicy)
     {
          m_pPolicy = new LogPolicy();
     }

     if (nullptr != m_pPolicy)
     {
          m_pPolicy->Open(name);

          ESeverity origLevel = m_level;
          m_level = ESeverity::Trace;
          Print< ESeverity::Trace >("LOGGING STARTED >>>>>>");
          m_level = origLevel;
     }

      m_loggingThread = std::thread(CLogger::LoggingThread, this);

}

template< class LogPolicy >
CLogger< LogPolicy >::~CLogger()
{
     if (nullptr != m_pPolicy)
     {
          ESeverity origLevel = m_level;
          m_level = ESeverity::Trace;
          Print< ESeverity::Trace >("<<<<< LOGGING ENDED");
          m_level = origLevel;

          m_bLoggingThreadContinue = false;
          m_loggingThread.join();

          m_pPolicy->Close();
          delete m_pPolicy;
          m_pPolicy = nullptr;
     }
}


template< class LogPolicy >
/*static*/ void CLogger< LogPolicy >::LoggingThread(CLogger< LogPolicy >* pLogger)
{
     std::unique_lock<std::timed_mutex> lock(pLogger->m_LoggingQueueTimedMutex, std::defer_lock);

     while (true == pLogger->m_bLoggingThreadContinue)
     {
          if (true == lock.try_lock_for(std::chrono::milliseconds{ 50 }))
          {
               if (pLogger->m_loggingQueue.size())
               {
                    pLogger->m_pPolicy->Write( pLogger->m_loggingQueue.front() );
                    pLogger->m_loggingQueue.pop_front();
               }
               lock.unlock();
          }
          std::this_thread::sleep_for(std::chrono::milliseconds{ 50 });
     }

     // Write remaning buffers
     lock.lock();
     for (auto& buffer : pLogger->m_loggingQueue)
     {
          pLogger->m_pPolicy->Write(buffer);
     }
     pLogger->m_loggingQueue.clear();
     lock.unlock();

}


template< class LogPolicy >
template< ESeverity severity, class...Args >
void CLogger< LogPolicy >::Print(Args...args)
{
     std::lock_guard<std::mutex> lock(m_writeMutex);

     if (severity >= m_level)
     {
          m_logStream.str("");

          GenerateTime();
          GenerateHeader();

          switch (severity)
          {
          case ESeverity::Trace:
               m_logStream << "<TRACE> :";
               break;
          case ESeverity::Informational:
               m_logStream << "<INFO> :";
               break;
          case ESeverity::Warning:
               m_logStream << "<WARNING> :";
               break;
          case ESeverity::Error:
               m_logStream << "<ERROR> :";
               break;
          };
          PrintImpl(args...);
     }
}


template< class LogPolicy >
void CLogger< LogPolicy >::PrintImpl()
{
     std::lock_guard<std::timed_mutex> lock(m_LoggingQueueTimedMutex);
     m_loggingQueue.push_back(m_headerStream.str() + m_logStream.str());
}

template< class LogPolicy >
template<class First, class...Rest >
void CLogger< LogPolicy >::PrintImpl(First parm1, Rest...parm)
{
     m_logStream << parm1;
     PrintImpl(parm...);
}

template< class LogPolicy >
void CLogger< LogPolicy >::GenerateTime()
{
     m_timeString.erase();

     std::string timeString;
     time_t rawTime = { 0 };

     time(&rawTime);

     const int size = 1024;
     timeString.resize(size);

     ctime_s((char*)timeString.data(), size, &rawTime);
     timeString = timeString.c_str();

     m_timeString = timeString.substr(0, timeString.size() - 1);
}


template< class LogPolicy >
void CLogger< LogPolicy >::GenerateHeader()
{
     m_headerStream.str("");

     m_headerStream.fill('0');
     m_headerStream.width(7);
     m_headerStream << std::this_thread::get_id();
     m_headerStream << " ~ ";
     m_headerStream.width(7);
     m_headerStream << m_lineNumber++ << " < " << m_timeString << " - ";
     m_headerStream.fill('0');
     m_headerStream.width(7);
     m_headerStream << clock() << " > ~ ";
}