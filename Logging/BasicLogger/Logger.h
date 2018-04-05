#pragma once

#include "LogPolicy.h"

#include <mutex>
#include <sstream>
#include <time.h>


enum class ESeverity
{
     Informational = 1,
     Warning,
     Error
};



template< class LogPolicy > class CLogger
{
public:

     CLogger(const std::string& name);
     virtual ~CLogger();

     template< ESeverity severity, class...Args >
     void Print(Args...args);
     
private:
     unsigned m_lineNumber = 0;

     void GenerateTime();
     void GenerateHeader();

     std::ostringstream m_logStream;
     std::ostringstream m_headerStream;
     std::string m_timeString;

     ILogPolicy* m_pPolicy = nullptr;;

     std::mutex m_writeMutex;

     void PrintImpl();
     template<class First, class...Rest> void PrintImpl(First parm1, Rest...parm);
};


template< class LogPolicy >
CLogger< LogPolicy >::CLogger(const std::string& name)
{
     if (nullptr == m_pPolicy)
     {
          m_pPolicy = new LogPolicy();
     }

     if (nullptr != m_pPolicy)
     {
          m_pPolicy->Open(name);
          Print< ESeverity::Informational >("LOGGING STARTED >>>>>>");
     }
}

template< class LogPolicy >
CLogger< LogPolicy >::~CLogger()
{
     if (nullptr != m_pPolicy)
     {
          Print< ESeverity::Informational >("<<<<< LOGGING ENDED");
          m_pPolicy->Close();
          delete m_pPolicy;
          m_pPolicy = nullptr;
     }
}


template< class LogPolicy >
template< ESeverity severity, class...Args >
void CLogger< LogPolicy >::Print(Args...args)
{
     std::lock_guard<std::mutex> lock(m_writeMutex);

     m_logStream.str("");

     GenerateTime();
     GenerateHeader();

     switch (severity)
     {
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


template< class LogPolicy >
void CLogger< LogPolicy >::PrintImpl()
{
     m_pPolicy->Write(m_headerStream.str() + m_logStream.str() );
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