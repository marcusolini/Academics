// Copyright(c) 2018 marcusolini@outlook.com
// References
// Doctor Dobbs - A lightweight logger
// A Thread-Safe Logger

#pragma once


#include "LogPolicy.h"

class CConsoleLogPolicy : public ILogPolicy
{
public:
     CConsoleLogPolicy() {}
     ~CConsoleLogPolicy() {};

     void Open(const std::string& name);
     void Close();
     void Write(const std::string& msg);
};


void CConsoleLogPolicy::Open(const std::string& name)
{
}

void CConsoleLogPolicy::Close()
{
}

void CConsoleLogPolicy::Write(const std::string& msg)
{
     std::wcout << msg.c_str() << std::endl;
}


