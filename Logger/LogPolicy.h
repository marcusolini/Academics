#pragma once

#include <iostream>
#include <memory>
//#include <fstream.h>

class log_policy_interface
{
public:
     virtual void     open_ostream(const std::string& name) = 0;
     virtual void     close_ostream() = 0;
     virtual void     write(const std::string& msg) = 0;

 //    virtual ~log_policy_interface() noexcept = default;
};


class console_log_policy : public log_policy_interface
{
public:
     console_log_policy() {}
     ~console_log_policy() {};

     void open_ostream(const std::string& name) ;
     void close_ostream() ;
     void write(const std::string& msg) ;
};


void console_log_policy::open_ostream(const std::string& name)
{
}

void console_log_policy::close_ostream()
{
}

void console_log_policy::write(const std::string& msg)
{
     std::wcout << msg.c_str() << std::endl;
}



/*
class file_log_policy : public log_policy_interface
{
     std::unique_ptr< std::ofstream > out_stream;
public:
     file_log_policy() : out_stream(new std::ofstream) {}
     void open_ostream(const std::string& name);
     void close_ostream();
     void write(const std::string& msg);
     ~file_log_policy();
};


void file_log_policy::open_ostream(const std::string& name)
{
     out_stream->open(name.c_str(), std::ios_base::binary | std::ios_base::out);
     if (!out_stream->is_open())
     {
          throw(std::runtime_error("LOGGER: Unable to open an output stream"));
     }
}

void file_log_policy::close_ostream()
{
     if (out_stream)
     {
          out_stream->close();
     }
}

void file_log_policy::write(const std::string& msg)
{
     (*out_stream) << msg << std::endl;
}

file_log_policy::~file_log_policy()
{
     if (out_stream)
     {
          close_ostream();
     }
}
*/