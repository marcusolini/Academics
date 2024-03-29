// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#ifndef LEAK_LIB_H
#define LEAK_LIB_H

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
    #define CALL
#endif // _WIN32

#include <cstddef>

extern "C"
{
class CLeakLib
{
public:
     CLASS_DECLSPEC static long CALL LeakNewMemory(const std::size_t numberOfCalls, const std::size_t bytesEachCall);
     CLASS_DECLSPEC static long CALL LeakMallocMemory(const std::size_t numberOfCalls, const std::size_t bytesEachCall);
     CLASS_DECLSPEC static long CALL LeakCallocMemory(const std::size_t numberOfCalls, const std::size_t bytesEachCall);
     CLASS_DECLSPEC static long CALL LeakHandle(const std::size_t numberOfCalls);
};
}

#endif // LEAK_LIB_H