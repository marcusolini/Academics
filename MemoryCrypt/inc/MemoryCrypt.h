// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#pragma once

#include <windows.h>
#include <stdio.h>
#include <Wincrypt.h>
#include <strsafe.h>

#include <string>

#pragma comment(lib, "crypt32.lib")

#include "../../Error_Checks/ERROR_CHECKS.H"

class MemoryCrypt
{
public:
     static HRESULT Encrypt(IN OUT std::wstring& sData);
     static HRESULT Decrypt(IN OUT std::wstring& sData);
};

HRESULT MemoryCrypt::Encrypt(IN OUT std::wstring& sData)
{
     HRESULT hrResult = ERROR_SUCCESS;
     std::size_t nDataSize = sData.size();
     std::size_t nDataBytes = 0;
     std::size_t nModBytes = 0;

     try
     {
          if (0 == nDataSize) throw E_INVALIDARG;

          nDataBytes = nDataSize * sizeof(sData[0]);

          nModBytes = nDataBytes % CRYPTPROTECTMEMORY_BLOCK_SIZE;

          if (0 != nModBytes)
          {
               nDataBytes = nDataBytes + (CRYPTPROTECTMEMORY_BLOCK_SIZE - nModBytes);
               
               nDataSize = nDataBytes / sizeof(sData[0]);

               sData.resize(nDataSize);
          }

          CHECK_BOOL_TRUE_LAST_ERROR_LOG_THROW( CryptProtectMemory((LPVOID)sData.data(), (DWORD)nDataBytes, CRYPTPROTECTMEMORY_SAME_PROCESS) );
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
     }

     return hrResult;
}

HRESULT MemoryCrypt::Decrypt(IN OUT std::wstring& sData)
{
     HRESULT hrResult = ERROR_SUCCESS;

     std::size_t nDataSize = sData.size();
     std::size_t nDataBytes = 0;

     try
     {
          if (0 == nDataSize) throw E_INVALIDARG;

          nDataBytes = nDataSize * sizeof(sData[0]);

          CHECK_BOOL_TRUE_LAST_ERROR_LOG_THROW( CryptUnprotectMemory((LPVOID)sData.data(), (DWORD)nDataBytes, CRYPTPROTECTMEMORY_SAME_PROCESS) );
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
     }

     return hrResult;
}