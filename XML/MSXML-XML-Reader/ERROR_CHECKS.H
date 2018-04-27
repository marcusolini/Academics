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
        if ( check_log ) \
          std::wcout << TEXT("OPERATION:") << #check_call << TEXT(" STATUS:") << std::hex << std::showbase << h_check_result << std::dec << TEXT(" LOCATION:") << __FUNCTION__ << TEXT(":") << __LINE__ << std::endl; \
        if ( check_throw ) \
          throw h_check_result; \
     } \
} \
