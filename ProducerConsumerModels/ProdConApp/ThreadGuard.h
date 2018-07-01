// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#pragma once

#include <thread>

class CThreadGuard
{
public:
     explicit CThreadGuard(std::thread& thread) : m_thread(thread) { }

     ~CThreadGuard()
     { 
          if (m_thread.joinable())
          {
               m_thread.join();
          }
     }

     CThreadGuard(CThreadGuard const&) = delete;
     CThreadGuard& operator=(CThreadGuard const&) = delete;
private:

     std::thread& m_thread;
};