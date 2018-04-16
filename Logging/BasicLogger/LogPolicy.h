// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

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