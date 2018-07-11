// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//

#ifndef ERROR_CHECKS_H
#define ERROR_CHECKS_H

#if __cplusplus >= 201103L
     #include <mutex>
#else
     #include <pthread.h>
#endif

#include <iostream>
#include <cstddef>
#include <cstdio>
#include <cwctype>
#include <stdarg.h>

#ifdef _WIN32
     //#pragma warning(disable : 4996) // for _vsnwprintf
     #include <winerror.h>
     #include <errno.h>
     #ifndef SUCCEEDED
          #define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
     #endif
#else
     #define GetLastError() (EINVAL)
     #define HRESULT_FROM_WIN32(x) (x)
     #ifndef SUCCEEDED
          #define SUCCEEDED(x) ((0 == x))
     #endif
#endif // _WIN32

#include <cerrno>

#ifndef IN
     #define IN
#endif

#ifndef OUT
     #define OUT
#endif

#ifndef OPTIONAL
     #define OPTIONAL
#endif

#ifndef nullptr
     #define nullptr (0)
#endif


// Logger function pointer
typedef void (*FPCHECK_LOGGER_FUNCTION)(const wchar_t* pszMsgFormat, ...);

#if __cplusplus >= 201103L
     static std::recursive_mutex g_check_logger_mutex;
#else
     static pthread_mutex_t g_check_logger_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
#endif

// Default Logger
static void CHECK_LOGGER(const wchar_t* pszMsgFormat, ...);
static FPCHECK_LOGGER_FUNCTION FPCHECK_LOGGER = CHECK_LOGGER;

// Override Default Logger
static void CHECK_SET_LOGGER_FUCTION(FPCHECK_LOGGER_FUNCTION fpLogger);

static void CHECK_PRODESS_MESSAGE_ARGS(OUT std::wstring& sMessage, IN const wchar_t* pszMsgFormat, IN ...);

// Default Logger
static void CHECK_LOGGER(const wchar_t* pszMsgFormat, ...)
{
     long lStatus = 0;

     try
     {
#if __cplusplus >= 201103L         
          std::lock_guard<std::recursive_mutex> lock(g_check_logger_mutex);
#else
          pthread_mutex_lock(&g_check_logger_mutex);
#endif

          if (nullptr != pszMsgFormat)
          {
               const int nCheckMessageBuffer = 8192;
               wchar_t   szCheckMessageBuffer[nCheckMessageBuffer] = { 0 };

               va_list marker;
               va_start(marker, pszMsgFormat);
               //_vsnwprintf_s(szCheckMessageBuffer, nCheckMessageBuffer, nCheckMessageBuffer, pszMsgFormat, marker);
               vswprintf(szCheckMessageBuffer, nCheckMessageBuffer, pszMsgFormat, marker);
               wprintf(szCheckMessageBuffer);

               va_end(marker);
          }
#if __cplusplus > 201103L               
#else
          pthread_mutex_unlock(&g_check_logger_mutex);
#endif
     }
     catch (std::bad_alloc&)
     {
          lStatus = ENOMEM;
     }
#if __cplusplus >= 201103L     
     catch (std::system_error& exception)
     {
          lStatus = (exception.code()).value();
     }
#endif
     catch (long& lcatch_return)
     {
          lStatus = lcatch_return;
     }
}

static void CHECK_PROCESS_MESSAGE_ARGS(OUT std::wstring& sMessage, IN const wchar_t* pszMsgFormat, IN ...)
{
     const int nCheckBuffer = 8192;
     wchar_t   szCheckBuffer[nCheckBuffer] = { 0 };

     if (nullptr != pszMsgFormat)
     {
          sMessage.erase();

          va_list marker;
          va_start(marker, pszMsgFormat);
          //_vsnwprintf_s(szCheckBuffer, nCheckBuffer, nCheckBuffer, pszMsgFormat, marker);
          vswprintf(szCheckBuffer, nCheckBuffer-1, pszMsgFormat, marker);

          if (nullptr != szCheckBuffer)
          {
               sMessage = szCheckBuffer;

               std::wstring::size_type pos = std::wstring::npos;
               std::wstring newLine = L"\n";
               std::wstring carrageReturn = L"\r";

               pos = sMessage.find(newLine);
               if (std::wstring::npos != pos) sMessage.replace(pos, newLine.length(), L"");

               pos = sMessage.find(carrageReturn);
               if (std::wstring::npos != pos) sMessage.replace(pos, carrageReturn.length(), L"");
          }

          va_end(marker);
     }
}

// Override the default logger function
static void CHECK_SET_LOGGER_FUCTION(FPCHECK_LOGGER_FUNCTION fpLogger)
{ 
     if ( nullptr != fpLogger) 
     { 
          FPCHECK_LOGGER = fpLogger;
     } 
}


// Logging Level
#define CHECK_LOG_LEVEL_TRACE      0
#define CHECK_LOG_LEVEL_INFO       1
#define CHECK_LOG_LEVEL_WARNING    2
#define CHECK_LOG_LEVEL_ERROR      3

// Logging
#define CHECK_LOG_ERROR        1
#define CHECK_NOLOG_ERROR      2

// Logging Propogation
#define CHECK_THROW_ERROR      3
#define CHECK_NOTHROW_ERROR    4


// LOGGING ROUTINES

#define LOG_MESSAGE(pwszMsgFormat, ...) \
{ \
     std::wstring sCheckMessageBuffer; \
     CHECK_PROCESS_MESSAGE_ARGS(sCheckMessageBuffer, pwszMsgFormat, ## __VA_ARGS__); \
     FPCHECK_LOGGER( L"%s - LOCATION:%S:%d \n", sCheckMessageBuffer.c_str(), __FUNCTION__, __LINE__); \
} \


#define CHECK_SUCCEEDED_LOG_THROW( error_check_call )  CHECK_SUCCEEDED_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_SUCCEEDED_NOLOG_THROW( error_check_call )  CHECK_SUCCEEDED_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_SUCCEEDED_LOG_NOTHROW( error_check_call )  CHECK_SUCCEEDED_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_SUCCEEDED_NOLOG_NOTHROW( error_check_call )  CHECK_SUCCEEDED_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_SUCCEEDED_HANDLER( error_check_call, error_check_log, error_check_throw ) \
{ \
     long lresult_error_check_result = (error_check_call); \
     lresult_error_check_result = HRESULT_FROM_WIN32(lresult_error_check_result); \
     if ( !SUCCEEDED(lresult_error_check_result) ) \
     { \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          LOG_MESSAGE(L"OPERATION:%S - STATUS:0x%08X", #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
          throw lresult_error_check_result; \
     } \
} \


#define CHECK_BOOL_TRUE_LOG_THROW( error_check_call )  CHECK_BOOL_TRUE_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_BOOL_TRUE_NOLOG_THROW( error_check_call )  CHECK_BOOL_TRUE_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_BOOL_TRUE_LOG_NOTHROW( error_check_call )  CHECK_BOOL_TRUE_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_BOOL_TRUE_NOLOG_NOTHROW( error_check_call )  CHECK_BOOL_TRUE_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_BOOL_TRUE_HANDLER( error_check_call, error_check_log, error_check_throw ) \
{ \
     bool bool_error_check_result = (error_check_call);  \
     if ( true != bool_error_check_result ) \
     { \
        long lresult_error_check_result = HRESULT_FROM_WIN32(EINVAL); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          LOG_MESSAGE(L"OPERATION:%S - STATUS:0x%08X", #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
          throw lresult_error_check_result; \
     } \
} \

#define CHECK_BOOL_FALSE_LOG_THROW( error_check_call )  CHECK_BOOL_FALSE_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_BOOL_FALSE_NOLOG_THROW( error_check_call )  CHECK_BOOL_FALSE_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_BOOL_FALSE_LOG_NOTHROW( error_check_call )  CHECK_BOOL_FALSE_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_BOOL_FALSE_NOLOG_NOTHROW( error_check_call )  CHECK_BOOL_FALSE_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_BOOL_FALSE_HANDLER( error_check_call, error_check_log, error_check_throw ) \
{ \
     bool bool_error_check_result = (error_check_call);  \
     if ( false != bool_error_check_result ) \
     { \
        long lresult_error_check_result = HRESULT_FROM_WIN32(EINVAL); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          LOG_MESSAGE(L"OPERATION:%S - STATUS:0x%08X", #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
          throw lresult_error_check_result; \
     } \
} \


#define CHECK_BOOL_TRUE_LAST_ERROR_LOG_THROW( error_check_call )  CHECK_BOOL_TRUE_LAST_ERROR_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_BOOL_TRUE_LAST_ERROR_NOLOG_THROW( error_check_call )  CHECK_BOOL_TRUE_LAST_ERROR_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_BOOL_TRUE_LAST_ERROR_LOG_NOTHROW( error_check_call )  CHECK_BOOL_TRUE_LAST_ERROR_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_BOOL_TRUE_LAST_ERROR_NOLOG_NOTHROW( error_check_call )  CHECK_BOOL_TRUE_LAST_ERROR_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_BOOL_TRUE_LAST_ERROR_HANDLER( error_check_call, error_check_log, error_check_throw ) \
{ \
     bool bool_error_check_result = (error_check_call);  \
     if ( true != bool_error_check_result ) \
     { \
        long lresult_error_check_result = GetLastError(); \
        lresult_error_check_result = HRESULT_FROM_WIN32(lresult_error_check_result); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          LOG_MESSAGE(L"OPERATION:%S - STATUS:0x%08X", #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
          throw lresult_error_check_result; \
     } \
} \

#define CHECK_NEW_ALLOC_LOG_THROW( error_check_call )  CHECK_NEW_ALLOC_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_NEW_ALLOC_NOLOG_THROW( error_check_call )  CHECK_NEW_ALLOC_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_NEW_ALLOC_LOG_NOTHROW( error_check_call )  CHECK_NEW_ALLOC_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_NEW_ALLOC_NOLOG_NOTHROW( error_check_call )  CHECK_NEW_ALLOC_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_NEW_ALLOC_HANDLER( error_check_call, error_check_log, error_check_throw ) \
{ \
     if ( (nullptr == (error_check_call) ) ) \
     { \
        long lresult_error_check_result = HRESULT_FROM_WIN32(ENOMEM); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          LOG_MESSAGE(L"OPERATION:%S - STATUS:0x%08X", #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
          throw lresult_error_check_result; \
     } \
} \

#define CHECK_NOT_NULL_LOG_THROW( error_check_call )  CHECK_NOT_NULL_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_NOT_NULL_NOLOG_THROW( error_check_call )  CHECK_NOT_NULL_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_NOT_NULL_LOG_NOTHROW( error_check_call )  CHECK_NOT_NULL_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_NOT_NULL_NOLOG_NOTHROW( error_check_call )  CHECK_NOT_NULL_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_NOT_NULL_HANDLER( error_check_call, error_check_log, error_check_throw ) \
{ \
     if ( nullptr == (error_check_call) ) \
     { \
        long lresult_error_check_result = HRESULT_FROM_WIN32(EINVAL); \
        lresult_error_check_result = HRESULT_FROM_WIN32(lresult_error_check_result); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          LOG_MESSAGE(L"OPERATION:%S - STATUS:0x%08X", #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
          throw lresult_error_check_result; \
     } \
} \

#define CHECK_NOT_NULL_LAST_ERROR_LOG_THROW( error_check_call )  CHECK_NOT_NULL_LAST_ERROR_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_NOT_NULL_LAST_ERROR_NOLOG_THROW( error_check_call )  CHECK_NOT_NULL_LAST_ERROR_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_NOT_NULL_LAST_ERROR_LOG_NOTHROW( error_check_call )  CHECK_NOT_NULL_LAST_ERROR_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_NOT_NULL_LAST_ERROR_NOLOG_NOTHROW( error_check_call )  CHECK_NOT_NULL_LAST_ERROR_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_NOT_NULL_LAST_ERROR_HANDLER( error_check_call, error_check_log, error_check_throw ) \
{ \
     if ( nullptr == (error_check_call) ) \
     { \
        long lresult_error_check_result = GetLastError(); \
        lresult_error_check_result = HRESULT_FROM_WIN32(lresult_error_check_result); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          LOG_MESSAGE(L"OPERATION:%S - STATUS:0x%08X", #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
          throw lresult_error_check_result; \
     } \
} \

#define CHECK_NOT_ZERO_LOG_THROW( error_check_call )  CHECK_NOT_ZERO_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_NOT_ZERO_NOLOG_THROW( error_check_call )  CHECK_NOT_ZERO_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_NOT_ZERO_LOG_NOTHROW( error_check_call )  CHECK_NOT_ZERO_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_NOT_ZERO_NOLOG_NOTHROW( error_check_call )  CHECK_NOT_ZERO_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_NOT_ZERO_HANDLER( error_check_call, error_check_log, error_check_throw ) \
{ \
     if ( 0 == (error_check_call) ) \
     { \
        long lresult_error_check_result = HRESULT_FROM_WIN32(EINVAL); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          LOG_MESSAGE(L"OPERATION:%S - STATUS:0x%08X", #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
          throw lresult_error_check_result; \
     } \
} \

#endif //ERROR_CHECKS_H
