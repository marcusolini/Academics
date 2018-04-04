#pragma once

#include "logPolicy.h"
#include <mutex>
#include <sstream>
#include <time.h>
#include <thread>
#include <vector>
#include <atomic>
#include <utility>


template< typename log_policy > class logger;
template< typename log_policy > void LoggingFunction(logger<log_policy>* logger);


enum class severity_type
{
     debug = 1,
     error,
     warning
};



template< typename log_policy > class logger
{
public:
     logger(const std::string& name);

     template< severity_type severity, typename...Args > void print(Args...args);

     ~logger();

     template< typename log_policy > friend void LoggingFunction(logger<log_policy>* logger);

     //void operator ()();

private:
     unsigned log_line_number;
     std::string get_time();
     std::string get_logline_header();
     std::stringstream log_stream;
     log_policy policy;

     std::thread log_thread;
     std::timed_mutex write_mutex;
     std::atomic_flag isRunning{ ATOMIC_FLAG_INIT };

     std::vector<std::string> logBuffer;

     void print_impl();
     template<typename First, typename...Rest> void print_impl(First parm1, Rest...parm);



//     void LoggingFunction(logger<log_policy>* logger);

};


template< typename log_policy >
logger< log_policy >::logger(const std::string& name)
{
     log_line_number = 0;
     policy.open_ostream(name);
     isRunning.test_and_set();
     log_thread = std::move(std::thread{ LoggingFunction<log_policy>, this });
     //std::thread stuff(this);

}

template< typename log_policy >
logger< log_policy >::~logger()
{
     policy.close_ostream();
}


template< typename log_policy >
template< severity_type severity, typename...Args >
void logger< log_policy >::print(Args...args)
{
     //write_mutex.lock();
     switch (severity)
     {
     case severity_type::debug:
          log_stream << "<DEBUG> :";
          break;
     case severity_type::warning:
          log_stream << "<WARNING> :";
          break;
     case severity_type::error:
          log_stream << "<ERROR> :";
          break;
     };
     print_impl(args...);
     //write_mutex.unlock();
}


template< typename log_policy >
void logger< log_policy >::print_impl()
{
     //policy.write(get_logline_header() + log_stream.str());
     std::lock_guard<std::timed_mutex> lock(write_mutex);
     logBuffer.push_back(get_logline_header() + log_stream.str());
     log_stream.str("");
}

template< typename log_policy >
template<typename First, typename...Rest >
void logger< log_policy >::print_impl(First parm1, Rest...parm)
{
     log_stream << parm1;
     print_impl(parm...);
}

template< typename log_policy >
std::string logger< log_policy >::get_time()
{
     std::string time_str;
     time_t raw_time;
     time(&raw_time);
     //time_str = ctime(&raw_time);

     const int size = 1024;
     time_str.resize(size);
     
     ctime_s((char*)time_str.data(), size, &raw_time);
     time_str = time_str.c_str();

     //without the newline character
     return time_str.substr(0, time_str.size() - 1);
}

template< typename log_policy >
std::string logger< log_policy >::get_logline_header()
{
     std::stringstream header;
     header.str("");
     header.fill('0');
     header.width(7);
     header << log_line_number++ << " < " << get_time() << " - ";
     header.fill('0');
     header.width(7);
     header << clock() << " > ~ ";
     return header.str();
}



template< typename log_policy >
void LoggingFunction(logger<log_policy>* logger)
//void logger< log_policy >::LoggingFunction()
//void logger< log_policy >::operator()()
{
     std::unique_lock<std::timed_mutex> lock(logger->write_mutex, std::defer_lock);
     do
     {
          std::this_thread::sleep_for(std::chrono::milliseconds{ 50 });
          if (logger->logBuffer.size())
          {
               if (!lock.try_lock_for(std::chrono::milliseconds{ 50 }))
               {
                    continue;
               }
               for (auto& msg : logger->logBuffer)
               {
                    logger->policy.write(msg);
               }
               logger->logBuffer.clear();
               lock.unlock();
          }
     } while (logger->isRunning.test_and_set() || logger->logBuffer.size());
     //} while (true);
}