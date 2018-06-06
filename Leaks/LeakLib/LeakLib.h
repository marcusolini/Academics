// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#pragma once

#ifdef _WIN32
     //#include "stdafx.h"

     #ifdef LEAKLIB_EXPORTS  
          #define CLASS_DECLSPEC    __declspec(dllexport)  
     #else  
          #define CLASS_DECLSPEC    __declspec(dllimport)  
     #endif  // LEAKLIB_EXPORTS
#else
     #define CLASS_DECLSPEC
#endif // _WIN32

class CLeakLib
{
public:
     CLASS_DECLSPEC static int LeakNewMemory(const size_t numberOfCalls, const size_t bytesEachCall);
     CLASS_DECLSPEC static int LeakMallocMemory(const size_t numberOfCalls, const size_t bytesEachCall);
     CLASS_DECLSPEC static int LeakCallocMemory(const size_t numberOfCalls, const size_t bytesEachCall);
     
     #ifdef _WIN32
     CLASS_DECLSPEC static int LeakHandle(const size_t numberOfCalls);
     #endif // _WIN32
};