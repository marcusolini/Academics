// Copyright(c) 2018 marcusolini@outlook.com
// References
// Doctor Dobbs - A lightweight logger
// A Thread-Safe Logger

#pragma once

#include <iostream>

class ILogPolicy
{
public:
     virtual void Open(const std::string& name) = 0;
     virtual void Close() = 0;
     virtual void Write(const std::string& msg) = 0;

     //    virtual ~ILogPolicy() noexcept = default;
};