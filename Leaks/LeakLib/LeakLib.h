// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#pragma once

#include "stdafx.h"

#ifdef LEAKLIB_EXPORTS  
#define CLASS_DECLSPEC    __declspec(dllexport)  
#else  
#define CLASS_DECLSPEC    __declspec(dllimport)  
#endif  

class CLeakLib
{
public:
     CLASS_DECLSPEC static DWORD LeakNewMemory(const size_t numberOfCalls, const size_t bytesEachCall);
     CLASS_DECLSPEC static DWORD LeakMallocMemory(const size_t numberOfCalls, const size_t bytesEachCall);
     CLASS_DECLSPEC static DWORD LeakCallocMemory(const size_t numberOfCalls, const size_t bytesEachCall);
     CLASS_DECLSPEC static DWORD LeakHandle(const size_t numberOfCalls);
};