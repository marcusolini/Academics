// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

// LeakLib.cpp : Defines the exported functions for the DLL application.
//

#ifdef _WIN32
     #define WIN32_LEAN_AND_MEAN
     #include "targetver.h"
     #include <windows.h>
#endif // _WIN32

#include "LeakLib.h"
#include "../../Error_Checks/ERROR_CHECKS.H"

#include <malloc.h>
#include <new>
#include <string>
#include <thread>

#include <cerrno>
#include <cstddef>
#include <cstdlib>


/*static*/ int CALL CLeakLib::LeakNewMemory(const std::size_t numberOfCalls, const std::size_t bytesEachCall)
{    
     int nStatus = 0;

     char* pChars = nullptr;

     try
     {

          for (std::size_t nCalls = 0; (nCalls < numberOfCalls) && (0 == nStatus); nCalls++)
          {
               CHECK_NEW_ALLOC_LOG_THROW(pChars = new (std::nothrow) char[bytesEachCall]);
               //std::this_thread::sleep_for(std::chrono::milliseconds{ 2 });
          }

     }
     catch (long& check_catch_lresult)
     {
         nStatus = check_catch_lresult;
     }
     catch (std::bad_alloc)
     {
          nStatus = ENOMEM;
     }
     catch (...)
     {
          nStatus = ENOMEM;
     }

     return nStatus;
}

/*static*/ int CALL CLeakLib::LeakMallocMemory(const std::size_t numberOfCalls, const std::size_t bytesEachCall)
{
     int nStatus = 0;

     void* pVoid = nullptr;

     try
     {
          for (std::size_t nCalls = 0; (nCalls < numberOfCalls) && (0 == nStatus); nCalls++)
          {
               CHECK_NEW_ALLOC_LOG_THROW(pVoid = malloc(bytesEachCall));
               //std::this_thread::sleep_for(std::chrono::milliseconds{ 3 });
          }
     }
     catch (long& check_catch_lresult)
     {
          nStatus = check_catch_lresult;
     }
     catch (std::bad_alloc)
     {
          nStatus = ENOMEM;
     }
     catch (...)
     {
          nStatus = ENOMEM;
     }

     return nStatus;
}

/*static*/ int CALL CLeakLib::LeakCallocMemory(const std::size_t numberOfCalls, const std::size_t bytesEachCall)
{
     int nStatus = 0;

     void* pVoid = nullptr;

     try
     {
          for (std::size_t nCalls = 0; (nCalls < numberOfCalls) && (0 == nStatus); nCalls++)
          {
               CHECK_NEW_ALLOC_LOG_THROW(pVoid = calloc(1, bytesEachCall));
               //std::this_thread::sleep_for(std::chrono::milliseconds{ 5 });
          }
     }
     catch (long& check_catch_lresult)
     {
          nStatus = check_catch_lresult;
     }
     catch (std::bad_alloc)
     {
          nStatus = ENOMEM;
     }
     catch (...)
     {
          nStatus = ENOMEM;
     }


     return nStatus;
}

/*static*/ int CALL CLeakLib::LeakHandle(const std::size_t numberOfCalls)
{
#ifndef _WIN32
     int nStatus = ENOMEM;
#else
     int nStatus = 0;

     HANDLE hMutex = INVALID_HANDLE_VALUE;

     try
     {
          for (std::size_t nCalls = 0; (nCalls < numberOfCalls) && (0 == nStatus); nCalls++)
          {
               std::wstring sMutexName = L"Local\\LeakedMutex" + nCalls;
               hMutex = CreateMutex(nullptr, true, sMutexName.c_str());
               if (INVALID_HANDLE_VALUE == hMutex) nStatus = ENOMEM;
               //std::this_thread::sleep_for(std::chrono::milliseconds{ 7 });
          }
     }
     catch (long& check_catch_lresult)
     {
          nStatus = check_catch_lresult;
     }
     catch (std::bad_alloc)
     {
          nStatus = ENOMEM;
     }
     catch (...)
     {
          nStatus = ENOMEM;
     }
#endif // _WIN32
     return nStatus;
}



