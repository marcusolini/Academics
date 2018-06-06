// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

// LeakTest.cpp : Defines the entry point for the console application.
//

// https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/finding-a-user-mode-memory-leak

#ifdef _WIN32
     #include "targetver.h"
#endif // _WIN32

#include <cerrno>
#include <cstddef>
#include <cstdlib>

#include <iostream>
#include <new>

#include "../LeakLib/LeakLib.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
     int nExitStatus = 0;
     int nStatus = 0;
     
     std::size_t leakNewCalls = 1000000;
     std::size_t leakNewBytes = 10000;

     std::size_t leakMallocCalls = 100000;
     std::size_t leakMallocBytes = 20000;

     std::size_t leakCallocCalls = 1000000;
     std::size_t leakCallocBytes = 40000;

     std::size_t leakHandleCalls = 1000;

     //system("pause");

     try
     {
          nStatus = CLeakLib::LeakNewMemory(leakNewCalls, leakNewBytes);

          if (0 != nStatus)
          {
               std::wcerr << L"New Memory Failure" << std::endl;
               nExitStatus += nStatus;
          }

          nStatus = CLeakLib::LeakMallocMemory(leakMallocCalls, leakMallocBytes);

          if (0 != nStatus)
          {
               std::wcerr << L"Malloc Memory Failure" << std::endl;
               nExitStatus += nStatus;
          }

          nStatus = CLeakLib::LeakCallocMemory(leakCallocCalls, leakCallocBytes);

          if (0 != nStatus)
          {
               std::wcerr << L"Calloc Memory Failure" << std::endl;
               nExitStatus += nStatus;
          }
          
          #ifdef _WIN32
          nStatus = CLeakLib::LeakHandle(leakHandleCalls);
          
          if (0 != nStatus)
          {
               std::wcerr << L"Handle Leak Failure" << std::endl;
               nExitStatus += nStatus;
          }
          #endif // _WIN32
     }
     catch ( std::bad_alloc )
     {
          nExitStatus = ENOMEM;
     }
     catch (...)
     {
          nExitStatus = ENOMEM;
     }

     //system("pause");

     return nExitStatus;
}
