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
#include "../../Error_Checks/ERROR_CHECKS.H"

/*
 * 
 */
int main(int argc, char** argv) {
    
     int nExitStatus = 0;
     int nStatus = 0;
     
     std::size_t leakNewCalls = 1;
     std::size_t leakNewBytes = 10000;

     std::size_t leakMallocCalls = 100000;
     std::size_t leakMallocBytes = 20000;

     std::size_t leakCallocCalls = 1000000;
     std::size_t leakCallocBytes = 40000;

     std::size_t leakHandleCalls = 1000;

     //system("pause");

     try
     {
          CHECK_SUCCEEDED_LOG_THROW(CLeakLib::LeakNewMemory(leakNewCalls, leakNewBytes));
         
          CHECK_SUCCEEDED_LOG_THROW(CLeakLib::LeakMallocMemory(leakMallocCalls, leakMallocBytes));

          CHECK_SUCCEEDED_LOG_THROW(CLeakLib::LeakCallocMemory(leakCallocCalls, leakCallocBytes));
          
          #ifdef _WIN32
          CHECK_SUCCEEDED_LOG_THROW(CLeakLib::LeakHandle(leakHandleCalls));
          #endif // _WIN32
     }
     catch (long& check_catch_lresult)
     {
         nExitStatus = check_catch_lresult;
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
