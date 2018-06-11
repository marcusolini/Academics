// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#pragma once


#ifdef _WIN32
     //#include "stdafx.h"

#include <stdio.h>

     #ifdef LEAKLIB_EXPORTS  
          #define CLASS_DECLSPEC    __declspec(dllexport)  
     #else  
          #define CLASS_DECLSPEC    __declspec(dllimport)  
     #endif  // LEAKLIB_EXPORTS

     #define CALL __stdcall
#else
    #define CLASS_DECLSPEC
#endif // _WIN32

#include <cstddef>

extern "C"
{
class CLeakLib
{
public:
     CLASS_DECLSPEC static int CALL LeakNewMemory(const std::size_t numberOfCalls, const std::size_t bytesEachCall);
     CLASS_DECLSPEC static int CALL LeakMallocMemory(const std::size_t numberOfCalls, const std::size_t bytesEachCall);
     CLASS_DECLSPEC static int CALL LeakCallocMemory(const std::size_t numberOfCalls, const std::size_t bytesEachCall);
     
     #ifdef _WIN32
     CLASS_DECLSPEC static int CALL LeakHandle(const std::size_t numberOfCalls);
     #endif // _WIN32
};
}