#pragma once

#include <iostream>


#define CHECK_LOG        1
#define CHECK_NOLOG      2
#define CHECK_THROW      3
#define CHECK_NOTHROW    4

#define CHECK_SUCCEEDED_LOG_THROW( check_call )  CHECK_SUCCEEDED_HANDLER( check_call, CHECK_LOG, CHECK_THROW )
#define CHECK_SUCCEEDED_NOLOG_THROW( check_call )  CHECK_SUCCEEDED_HANDLER( check_call, CHECK_NOLOG, CHECK_THROW )
#define CHECK_SUCCEEDED_LOG_NOTHROW( check_call )  CHECK_SUCCEEDED_HANDLER( check_call, CHECK_LOG, CHECK_NOTHROW )
#define CHECK_SUCCEEDED_NOLOG_NOTHROW( check_call )  CHECK_SUCCEEDED_HANDLER( check_call, CHECK_NOLOG, CHECK_NOTHROW )
#define CHECK_SUCCEEDED_HANDLER( check_call, check_log, check_throw ) \
{ \
     HRESULT h_check_result = HRESULT_FROM_WIN32(check_call);  \
     if ( !SUCCEEDED(h_check_result) ) \
     { \
        if ( CHECK_LOG == check_log ) \
          std::wcout << TEXT("OPERATION:") << #check_call << TEXT(" STATUS:") << std::hex << std::showbase << h_check_result << std::dec << TEXT(" LOCATION:") << __FUNCTION__ << TEXT(":") << __LINE__ << std::endl; \
        if ( CHECK_THROW == check_throw ) \
          throw h_check_result; \
     } \
} \


#define CHECK_BOOL_TRUE_LOG_THROW( check_call )  CHECK_BOOL_TRUE_HANDLER( check_call, CHECK_LOG, CHECK_THROW )
#define CHECK_BOOL_TRUE_NOLOG_THROW( check_call )  CHECK_BOOL_TRUE_HANDLER( check_call, CHECK_NOLOG, CHECK_THROW )
#define CHECK_BOOL_TRUE_LOG_NOTHROW( check_call )  CHECK_BOOL_TRUE_HANDLER( check_call, CHECK_LOG, CHECK_NOTHROW )
#define CHECK_BOOL_TRUE_NOLOG_NOTHROW( check_call )  CHECK_BOOL_TRUE_HANDLER( check_call, CHECK_NOLOG, CHECK_NOTHROW )
#define CHECK_BOOL_TRUE_HANDLER( check_call, check_log, check_throw ) \
{ \
     bool b_check_result = (check_call);  \
     if ( true != b_check_result ) \
     { \
        HRESULT h_check_result = E_FAIL; \
        if ( CHECK_LOG == check_log ) \
          std::wcout << TEXT("OPERATION:") << #check_call << TEXT(" STATUS:") << std::hex << std::showbase << h_check_result << std::dec << TEXT(" LOCATION:") << __FUNCTION__ << TEXT(":") << __LINE__ << std::endl; \
        if ( CHECK_THROW == check_throw ) \
          throw h_check_result; \
     } \
} \
