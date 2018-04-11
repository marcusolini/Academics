// Copyright(c) 2018 marcusolini@outlook.com

#pragma once

#include "stdafx.h"

#ifdef _EXPORTING  
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