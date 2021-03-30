// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//

#ifndef ERROR_CHECKS_H
#define ERROR_CHECKS_H

#include <iostream>
#include <cstddef>
#include <cstdio>
#include <cwctype>
#include <stdarg.h>

#include "TSTRING.h"

#include "LOGGER_MACROS.H"

#ifdef _WIN32
     //#pragma warning(disable : 4996) // for _vsnwprintf
     #include <winerror.h>
     #include <errno.h>
     #ifndef SUCCEEDED
          #define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
     #endif
#else
    #ifndef HRESULT
        typedef long HRESULT;
    #endif

    #ifndef HRESULT_FROM_WIN32
        #define HRESULT_FROM_WIN32(x) ((((HRESULT)(x)) <= 0) ? ((HRESULT)(x)) : ((HRESULT)((x) * (-1))))
    #endif

    #ifndef SUCCEEDED
        #define SUCCEEDED(x) (((HRESULT)(x)) >= 0)
    #endif

    #ifndef GetLastError
        #define GetLastError() (HRESULT_FROM_WIN32(EINVAL))
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
typedef void (*FPCHECK_LOGGER_FUNCTION)(const TCHAR* pszMsgFormat, ...);

static FPCHECK_LOGGER_FUNCTION FPCHECK_LOGGER = NULL;

// Override the default logger function
static void CHECK_SET_LOGGER_FUCTION(FPCHECK_LOGGER_FUNCTION fpLogger)
{ 
     if ( nullptr != fpLogger) 
     { 
          FPCHECK_LOGGER = fpLogger;
     } 
}

// Logging
#define CHECK_LOG_ERROR        1
#define CHECK_NOLOG_ERROR      2

// Logging Propogation
#define CHECK_THROW_ERROR      3
#define CHECK_NOTHROW_ERROR    4


// LOGGING ROUTINES

#ifdef _WIN32
#define CHECK_LOG_MESSAGE(pwszMsgFormat, ...) \
{ \
    TSTRING sCheckMessageBuffer; \
    LOG_MESSAGE_ARGS(sCheckMessageBuffer, pwszMsgFormat, ## __VA_ARGS__); \
    if (NULL != FPCHECK_LOGGER) \
    { \
        FPCHECK_LOGGER( _TEXT("%s - [%S]:[%d] \n"), sCheckMessageBuffer.c_str(), __FUNCTION__, __LINE__); \
    } \
    else \
    { \
        LOG_CHAR(ILoggerLib::Severity::Error, _TEXT("%s"), sCheckMessageBuffer.c_str()); \
    } \
} \

#else
#define CHECK_LOG_MESSAGE(pwszMsgFormat, ...) \
{ \
    TSTRING sCheckMessageBuffer; \
    LOG_MESSAGE_ARGS(sCheckMessageBuffer, pwszMsgFormat, ## __VA_ARGS__); \
    if (NULL != FPCHECK_LOGGER) \
    { \
        FPCHECK_LOGGER( _TEXT("%s - [%s]:[%d] \n"), sCheckMessageBuffer.c_str(), __FUNCTION__, __LINE__); \
    } \
    else \
    { \
        LOG_CHAR(ILoggerLib::Severity::Informational, _TEXT("%s"), sCheckMessageBuffer.c_str()); \
    } \
} \

#endif // _WIN32


#ifdef _WIN32
#define CHECK_LOG_ERROR_MESSAGE(check_log_severity, pwszMsgFormat, ...) \
{ \
    TSTRING sCheckMessageBuffer; \
    LOG_MESSAGE_ARGS(sCheckMessageBuffer, pwszMsgFormat, ## __VA_ARGS__); \
    if (NULL != FPCHECK_LOGGER) \
    { \
        FPCHECK_LOGGER( _TEXT("%s - [%S]:[%d] \n"), sCheckMessageBuffer.c_str(), __FUNCTION__, __LINE__); \
    } \
    else \
    { \
        LOG_CHAR(check_log_severity, _TEXT("%s"), sCheckMessageBuffer.c_str()); \
    } \
} \

#else
#define CHECK_LOG_ERROR_MESSAGE(check_log_severity, pwszMsgFormat, ...) \
{ \
    TSTRING sCheckMessageBuffer; \
    LOG_MESSAGE_ARGS(sCheckMessageBuffer, pwszMsgFormat, ## __VA_ARGS__); \
    if (NULL != FPCHECK_LOGGER) \
    { \
        FPCHECK_LOGGER( _TEXT("%s - [%s]:[%d] \n"), sCheckMessageBuffer.c_str(), __FUNCTION__, __LINE__); \
    } \
    else \
    { \
        LOG_CHAR(check_log_severity, _TEXT("%s"), sCheckMessageBuffer.c_str()); \
    } \
} \

#endif // _WIN32

// STATUS CHECK MACROS

#define CHECK_SUCCEEDED_LOG_THROW( error_check_call )  CHECK_SUCCEEDED_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_SUCCEEDED_LOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_SUCCEEDED_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_SUCCEEDED_NOLOG_THROW( error_check_call )  CHECK_SUCCEEDED_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_SUCCEEDED_NOLOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_SUCCEEDED_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_SUCCEEDED_LOG_NOTHROW( error_check_call )  CHECK_SUCCEEDED_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_SUCCEEDED_NOLOG_NOTHROW( error_check_call )  CHECK_SUCCEEDED_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_SUCCEEDED_HANDLER( error_check_call, error_check_log, error_check_throw, error_check_throw_alt_error ) \
{ \
     long lresult_error_check_result = (error_check_call); \
     lresult_error_check_result = (lresult_error_check_result); \
     if ( !SUCCEEDED(lresult_error_check_result) ) \
     { \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          CHECK_LOG_ERROR_MESSAGE(ILoggerLib::Severity::Error, _TEXT("[OPERATION:%s] - [STATUS:0x%08X]"), #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
        { \
          if ( 0 != error_check_throw_alt_error ) \
            throw error_check_throw_alt_error; \
          else \
            throw lresult_error_check_result; \
        } \
     } \
} \


#define CHECK_BOOL_TRUE_LOG_THROW( error_check_call )  CHECK_BOOL_TRUE_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_BOOL_TRUE_LOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_BOOL_TRUE_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_BOOL_TRUE_NOLOG_THROW( error_check_call )  CHECK_BOOL_TRUE_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_BOOL_TRUE_NOLOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_BOOL_TRUE_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_BOOL_TRUE_LOG_NOTHROW( error_check_call )  CHECK_BOOL_TRUE_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_BOOL_TRUE_NOLOG_NOTHROW( error_check_call )  CHECK_BOOL_TRUE_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_BOOL_TRUE_HANDLER( error_check_call, error_check_log, error_check_throw, error_check_throw_alt_error ) \
{ \
     bool bool_error_check_result = (error_check_call);  \
     if ( true != bool_error_check_result ) \
     { \
        long lresult_error_check_result = HRESULT_FROM_WIN32(EINVAL); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          CHECK_LOG_ERROR_MESSAGE(ILoggerLib::Severity::Error, _TEXT("[OPERATION:%s] - [STATUS:0x%08X]"), #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
        { \
          if ( 0 != error_check_throw_alt_error ) \
            throw error_check_throw_alt_error; \
          else \
            throw lresult_error_check_result; \
        } \
     } \
} \

#define CHECK_BOOL_FALSE_LOG_THROW( error_check_call )  CHECK_BOOL_FALSE_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_BOOL_FALSE_LOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_BOOL_FALSE_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_BOOL_FALSE_NOLOG_THROW( error_check_call )  CHECK_BOOL_FALSE_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_BOOL_FALSE_NOLOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_BOOL_FALSE_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_BOOL_FALSE_LOG_NOTHROW( error_check_call )  CHECK_BOOL_FALSE_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_BOOL_FALSE_NOLOG_NOTHROW( error_check_call )  CHECK_BOOL_FALSE_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_BOOL_FALSE_HANDLER( error_check_call, error_check_log, error_check_throw, error_check_throw_alt_error ) \
{ \
     bool bool_error_check_result = (error_check_call);  \
     if ( false != bool_error_check_result ) \
     { \
        long lresult_error_check_result = HRESULT_FROM_WIN32(EINVAL); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          CHECK_LOG_ERROR_MESSAGE(ILoggerLib::Severity::Error, _TEXT("[OPERATION:%s] - [STATUS:0x%08X]"), #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
        { \
          if ( 0 != error_check_throw_alt_error ) \
            throw error_check_throw_alt_error; \
          else \
            throw lresult_error_check_result; \
        } \
     } \
} \


#define CHECK_BOOL_TRUE_LAST_ERROR_LOG_THROW( error_check_call )  CHECK_BOOL_TRUE_LAST_ERROR_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_BOOL_TRUE_LAST_ERROR_LOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_BOOL_TRUE_LAST_ERROR_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_BOOL_TRUE_LAST_ERROR_NOLOG_THROW( error_check_call )  CHECK_BOOL_TRUE_LAST_ERROR_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_BOOL_TRUE_LAST_ERROR_NOLOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_BOOL_TRUE_LAST_ERROR_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_BOOL_TRUE_LAST_ERROR_LOG_NOTHROW( error_check_call )  CHECK_BOOL_TRUE_LAST_ERROR_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_BOOL_TRUE_LAST_ERROR_NOLOG_NOTHROW( error_check_call )  CHECK_BOOL_TRUE_LAST_ERROR_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_BOOL_TRUE_LAST_ERROR_HANDLER( error_check_call, error_check_log, error_check_throw, error_check_throw_alt_error ) \
{ \
     bool bool_error_check_result = (error_check_call);  \
     if ( true != bool_error_check_result ) \
     { \
        long lresult_error_check_result = GetLastError(); \
        lresult_error_check_result = HRESULT_FROM_WIN32(lresult_error_check_result); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          CHECK_LOG_ERROR_MESSAGE(ILoggerLib::Severity::Error, _TEXT("[OPERATION:%s] - [STATUS:0x%08X]"), #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
        { \
          if ( 0 != error_check_throw_alt_error ) \
            throw error_check_throw_alt_error; \
          else \
            throw lresult_error_check_result; \
        } \
     } \
} \

#define CHECK_NEW_ALLOC_LOG_THROW( error_check_call )  CHECK_NEW_ALLOC_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_NEW_ALLOC_LOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_NEW_ALLOC_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_NEW_ALLOC_NOLOG_THROW( error_check_call )  CHECK_NEW_ALLOC_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_NEW_ALLOC_NOLOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_NEW_ALLOC_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_NEW_ALLOC_LOG_NOTHROW( error_check_call )  CHECK_NEW_ALLOC_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_NEW_ALLOC_NOLOG_NOTHROW( error_check_call )  CHECK_NEW_ALLOC_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_NEW_ALLOC_HANDLER( error_check_call, error_check_log, error_check_throw, error_check_throw_alt_error ) \
{ \
     if ( (nullptr == (error_check_call) ) ) \
     { \
        long lresult_error_check_result = HRESULT_FROM_WIN32(ENOMEM); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          CHECK_LOG_ERROR_MESSAGE(ILoggerLib::Severity::Error, _TEXT("[OPERATION:%s] - [STATUS:0x%08X]"), #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
        { \
          if ( 0 != error_check_throw_alt_error ) \
            throw error_check_throw_alt_error; \
          else \
            throw lresult_error_check_result; \
        } \
     } \
} \

#define CHECK_NOT_NULL_LOG_THROW( error_check_call )  CHECK_NOT_NULL_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_NOT_NULL_LOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_NOT_NULL_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_NOT_NULL_NOLOG_THROW( error_check_call )  CHECK_NOT_NULL_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_NOT_NULL_NOLOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_NOT_NULL_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_NOT_NULL_LOG_NOTHROW( error_check_call )  CHECK_NOT_NULL_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_NOT_NULL_NOLOG_NOTHROW( error_check_call )  CHECK_NOT_NULL_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_NOT_NULL_HANDLER( error_check_call, error_check_log, error_check_throw, error_check_throw_alt_error ) \
{ \
     if ( nullptr == (error_check_call) ) \
     { \
        long lresult_error_check_result = HRESULT_FROM_WIN32(EINVAL); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          CHECK_LOG_ERROR_MESSAGE(ILoggerLib::Severity::Error, _TEXT("[OPERATION:%s] - [STATUS:0x%08X]"), #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
        { \
          if ( 0 != error_check_throw_alt_error ) \
            throw error_check_throw_alt_error; \
          else \
            throw lresult_error_check_result; \
        } \
     } \
} \

#define CHECK_NOT_NULL_LAST_ERROR_LOG_THROW( error_check_call )  CHECK_NOT_NULL_LAST_ERROR_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_NOT_NULL_LAST_ERROR_LOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_NOT_NULL_LAST_ERROR_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_NOT_NULL_LAST_ERROR_NOLOG_THROW( error_check_call )  CHECK_NOT_NULL_LAST_ERROR_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_NOT_NULL_LAST_ERROR_NOLOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_NOT_NULL_LAST_ERROR_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_NOT_NULL_LAST_ERROR_LOG_NOTHROW( error_check_call )  CHECK_NOT_NULL_LAST_ERROR_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_NOT_NULL_LAST_ERROR_NOLOG_NOTHROW( error_check_call )  CHECK_NOT_NULL_LAST_ERROR_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_NOT_NULL_LAST_ERROR_HANDLER( error_check_call, error_check_log, error_check_throw, error_check_throw_alt_error ) \
{ \
     if ( nullptr == (error_check_call) ) \
     { \
        long lresult_error_check_result = GetLastError(); \
        lresult_error_check_result = HRESULT_FROM_WIN32(lresult_error_check_result); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          CHECK_LOG_ERROR_MESSAGE(ILoggerLib::Severity::Error, _TEXT("[OPERATION:%s] - [STATUS:0x%08X]"), #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
        { \
          if ( 0 != error_check_throw_alt_error ) \
            throw error_check_throw_alt_error; \
          else \
            throw lresult_error_check_result; \
        } \
     } \
} \

#define CHECK_NULL_LOG_THROW( error_check_call )  CHECK_NULL_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_NULL_LOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_NULL_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_NULL_NOLOG_THROW( error_check_call )  CHECK_NULL_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_NULL_NOLOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_NULL_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_NULL_LOG_NOTHROW( error_check_call )  CHECK_NULL_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_NULL_NOLOG_NOTHROW( error_check_call )  CHECK_NULL_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_NULL_HANDLER( error_check_call, error_check_log, error_check_throw, error_check_throw_alt_error ) \
{ \
     if ( nullptr != (error_check_call) ) \
     { \
        long lresult_error_check_result = HRESULT_FROM_WIN32(EINVAL); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          CHECK_LOG_ERROR_MESSAGE(ILoggerLib::Severity::Error, _TEXT("[OPERATION:%s] - [STATUS:0x%08X]"), #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
        { \
          if ( 0 != error_check_throw_alt_error ) \
            throw error_check_throw_alt_error; \
          else \
            throw lresult_error_check_result; \
        } \
     } \
} \

#define CHECK_NOT_ZERO_LOG_THROW( error_check_call )  CHECK_NOT_ZERO_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_NOT_ZERO_LOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_NOT_ZERO_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_NOT_ZERO_NOLOG_THROW( error_check_call )  CHECK_NOT_ZERO_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, 0 )
#define CHECK_NOT_ZERO_NOLOG_THROW_ALT( error_check_call, error_check_alt_error )  CHECK_NOT_ZERO_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR, error_check_alt_error )
#define CHECK_NOT_ZERO_LOG_NOTHROW( error_check_call )  CHECK_NOT_ZERO_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_NOT_ZERO_NOLOG_NOTHROW( error_check_call )  CHECK_NOT_ZERO_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR, 0 )
#define CHECK_NOT_ZERO_HANDLER( error_check_call, error_check_log, error_check_throw, error_check_throw_alt_error ) \
{ \
     if ( 0 == (error_check_call) ) \
     { \
        long lresult_error_check_result = HRESULT_FROM_WIN32(EINVAL); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          CHECK_LOG_ERROR_MESSAGE(ILoggerLib::Severity::Error, _TEXT("[OPERATION:%s] - [STATUS:0x%08X]"), #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
        { \
          if ( 0 != error_check_throw_alt_error ) \
            throw error_check_throw_alt_error; \
          else \
            throw lresult_error_check_result; \
        } \
     } \
} \


#define CHECK_THROWN_LOG_THROW( error_check_call )  CHECK_THROWN_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_THROWN_NOLOG_THROW( error_check_call )  CHECK_THROWN_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_THROWN_LOG_NOTHROW( error_check_call )  CHECK_THROWN_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_THROWN_NOLOG_NOTHROW( error_check_call )  CHECK_THROWN_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_THROWN_HANDLER( error_check_call, error_check_log, error_check_throw ) \
{ \
    long lresult_error_check_result = 0; \
    try \
    { (error_check_call); } \
    catch(const long& lcatch_return) \
    { lresult_error_check_result = lcatch_return; } \
    catch(const std::invalid_argument&) \
    { lresult_error_check_result = EINVAL; } \
    catch(...) \
    { lresult_error_check_result = EINVAL; } \
    if (0 != lresult_error_check_result) \
    { \
        if ( CHECK_LOG_ERROR == error_check_log ) \
            CHECK_LOG_ERROR_MESSAGE(ILoggerLib::Severity::Error, _TEXT("[THROWN OPERATION:%s] - [STATUS:0x%08X]"), #error_check_call,  lresult_error_check_result); \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
          throw lresult_error_check_result; \
    } \
} \

#endif //ERROR_CHECKS_H
