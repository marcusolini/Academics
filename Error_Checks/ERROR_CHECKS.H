// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//

#pragma once

#include <iostream>


#define CHECK_LOG_ERROR        1
#define CHECK_NOLOG_ERROR      2
#define CHECK_THROW_ERROR      3
#define CHECK_NOTHROW_ERROR    4

#define LOG_MESSAGE( pwszMessage ) \
{ \
    std::wcout << pwszMessage << TEXT(" LOCATION:") << __FUNCTION__ << TEXT(":") << __LINE__ << std::endl; \
} \


#define CHECK_SUCCEEDED_LOG_THROW( error_check_call )  CHECK_SUCCEEDED_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_SUCCEEDED_NOLOG_THROW( error_check_call )  CHECK_SUCCEEDED_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_SUCCEEDED_LOG_NOTHROW( error_check_call )  CHECK_SUCCEEDED_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_SUCCEEDED_NOLOG_NOTHROW( error_check_call )  CHECK_SUCCEEDED_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_SUCCEEDED_HANDLER( error_check_call, error_check_log, error_check_throw ) \
{ \
     HRESULT hresult_error_check_result = HRESULT_FROM_WIN32(error_check_call);  \
     if ( !SUCCEEDED(hresult_error_check_result) ) \
     { \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          std::wcout << TEXT("OPERATION:") << #error_check_call << TEXT(" STATUS:") << std::hex << std::showbase << hresult_error_check_result << std::dec << TEXT(" LOCATION:") << __FUNCTION__ << TEXT(":") << __LINE__ << std::endl; \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
          throw hresult_error_check_result; \
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
        HRESULT hresult_error_check_result = E_FAIL; \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          std::wcout << TEXT("OPERATION:") << #error_check_call << TEXT(" STATUS:") << std::hex << std::showbase << hresult_error_check_result << std::dec << TEXT(" LOCATION:") << __FUNCTION__ << TEXT(":") << __LINE__ << std::endl; \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
          throw hresult_error_check_result; \
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
        HRESULT hresult_error_check_result = HRESULT_FROM_WIN32(::GetLastError()); \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          std::wcout << TEXT("OPERATION:") << #error_check_call << TEXT(" STATUS:") << std::hex << std::showbase << hresult_error_check_result << std::dec << TEXT(" LOCATION:") << __FUNCTION__ << TEXT(":") << __LINE__ << std::endl; \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
          throw hresult_error_check_result; \
     } \
} \

#define CHECK_NEW_ALLOC_LOG_THROW( error_check_call )  CHECK_NEW_ALLOC_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_NEW_ALLOC_NOLOG_THROW( error_check_call )  CHECK_NEW_ALLOC_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_THROW_ERROR )
#define CHECK_NEW_ALLOC_LOG_NOTHROW( error_check_call )  CHECK_NEW_ALLOC_HANDLER( error_check_call, CHECK_LOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_NEW_ALLOC_NOLOG_NOTHROW( error_check_call )  CHECK_NEW_ALLOC_HANDLER( error_check_call, CHECK_NOLOG_ERROR, CHECK_NOTHROW_ERROR )
#define CHECK_NEW_ALLOC_HANDLER( error_check_call, error_check_log, error_check_throw ) \
{ \
     if ( nullptr == (error_check_call) ) \
     { \
        HRESULT hresult_error_check_result = E_OUTOFMEMORY; \
        if ( CHECK_LOG_ERROR == error_check_log ) \
          std::wcout << TEXT("OPERATION:") << #error_check_call << TEXT(" STATUS:") << std::hex << std::showbase << hresult_error_check_result << std::dec << TEXT(" LOCATION:") << __FUNCTION__ << TEXT(":") << __LINE__ << std::endl; \
        if ( CHECK_THROW_ERROR == error_check_throw ) \
          throw hresult_error_check_result; \
     } \
} \
