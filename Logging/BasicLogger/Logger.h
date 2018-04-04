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
     void print(Args...args);
     
private:
     unsigned m_lineNumber = 0;

     std::string GetTime();
     std::string GetHeader();

     std::stringstream m_logStream;

     ILogPolicy* m_pPolicy = nullptr;;

     std::mutex m_writeMutex;

     void print_impl();
     template<class First, class...Rest> void print_impl(First parm1, Rest...parm);
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
          print< ESeverity::Informational >("LOGGING STARTED >>>>>>");
     }
}

template< class LogPolicy >
CLogger< LogPolicy >::~CLogger()
{
     if (nullptr != m_pPolicy)
     {
          print< ESeverity::Informational >("<<<<< LOGGING ENDED");
          m_pPolicy->Close();
          delete m_pPolicy;
          m_pPolicy = nullptr;
     }
}


template< class LogPolicy >
template< ESeverity severity, class...Args >
void CLogger< LogPolicy >::print(Args...args)
{
     std::lock_guard<std::mutex> lock(m_writeMutex);
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
     print_impl(args...);
}


template< class LogPolicy >
void CLogger< LogPolicy >::print_impl()
{
     m_pPolicy->Write(GetHeader() + m_logStream.str());
     m_logStream.str("");
}

template< class LogPolicy >
template<class First, class...Rest >
void CLogger< LogPolicy >::print_impl(First parm1, Rest...parm)
{
     m_logStream << parm1;
     print_impl(parm...);
}

template< class LogPolicy >
std::string CLogger< LogPolicy >::GetTime()
{
     std::string time_str;
     time_t raw_time;
     time(&raw_time);

     const int size = 1024;
     time_str.resize(size);

     ctime_s((char*)time_str.data(), size, &raw_time);
     time_str = time_str.c_str();

     return time_str.substr(0, time_str.size() - 1);
}

template< class LogPolicy >
std::string CLogger< LogPolicy >::GetHeader()
{
     std::stringstream header;
     header.str("");
     header.fill('0');
     header.width(7);
     header << m_lineNumber++ << " < " << GetTime() << " - ";
     header.fill('0');
     header.width(7);
     header << clock() << " > ~ ";
     return header.str();
}