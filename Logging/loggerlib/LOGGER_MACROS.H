// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//

#ifndef LOGGER_MACROS_H
#define LOGGER_MACROS_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif 

#include "loggerlib.h"

#include "Common/TSTRING.h"

// HELPER FUNCTIONS AND MACROS
static void LOG_MESSAGE_ARGS(TSTRING& sMessage, const TCHAR* pszMsgFormat, ...);
static void LOG_MESSAGE_HEADER(TSTRING& sHeader, ILoggerLib::Severity eSeverity);

#ifdef _UNICODE

#define LOG_FOOTER( log_footer_sFooter ) \
    { \
        int log_footer_nFooterBufOut = 0; \
        const int log_footer_nFooterBuf = 8192; \
        TCHAR log_footer_sFooterBuf [log_footer_nFooterBuf] = {0}; \
        log_footer_nFooterBufOut = TSNPRINTF_S(log_footer_sFooterBuf, log_footer_nFooterBuf, _TEXT(" - [%S]:[%d]"), __FUNCTION__, __LINE__); \
        log_footer_sFooter.assign( log_footer_sFooterBuf, log_footer_nFooterBufOut); \
    } \

#else
#define LOG_FOOTER( log_footer_sFooter ) \
    { \
        int log_footer_nFooterBufOut = 0; \
        const int log_footer_nFooterBuf = 8192; \
        TCHAR log_footer_sFooterBuf [log_footer_nFooterBuf] = {0}; \
        log_footer_nFooterBufOut = TSNPRINTF_S(log_footer_sFooterBuf, log_footer_nFooterBuf, _TEXT(" - [%s]:[%d]"), __FUNCTION__, __LINE__); \
        log_footer_sFooter.assign( log_footer_sFooterBuf, log_footer_nFooterBufOut); \
    } \

#endif

// USER LOGGING MACROS
// [TID] [YYYY-MM-DD HH:MM:SS] [SEVERITY] - USER-DEFINED MESSAGE - [FUNCTION]:[LINE]

#define LOG_CHAR( log_char_severity, log_char_pwszMsgFormat, ... ) \
{ \
    LoggerPtr<ILoggerLib> pILoggerPtr(ILoggerLib::AcquireInstance()); \
    if (pILoggerPtr != NULL) \
    { \
        if (true == pILoggerPtr->Enabled(log_char_severity)) \
        { \
            TSTRING log_char_sHeadFoot; \
            TSTRING log_char_sChar; \
            TSTRING log_char_sCharTemp; \
            LOG_MESSAGE_HEADER(log_char_sHeadFoot, log_char_severity); \
            log_char_sChar = log_char_sHeadFoot; \
            LOG_MESSAGE_ARGS(log_char_sCharTemp, log_char_pwszMsgFormat, ## __VA_ARGS__); \
            log_char_sChar += log_char_sCharTemp; \
            LOG_FOOTER( log_char_sHeadFoot); \
            log_char_sChar += log_char_sHeadFoot; \
            pILoggerPtr->Writer(log_char_sChar); \
        } \
    } \
} \

#define LOG_STRING( log_string_severity, log_string_string ) \
{ \
    LoggerPtr<ILoggerLib> pILoggerPtr(ILoggerLib::AcquireInstance()); \
    if (pILoggerPtr != NULL) \
    { \
        if (true == pILoggerPtr->Enabled(log_string_severity)) \
        { \
            TSTRING log_string_sHeadFoot; \
            TSTRING log_string_sString; \
            LOG_MESSAGE_HEADER(log_string_sHeadFoot, log_string_severity); \
            log_string_sString = log_string_sHeadFoot; \
            log_string_sString += log_string_string; \
            LOG_FOOTER( log_string_sHeadFoot); \
            log_string_sString += log_string_sHeadFoot; \
            pILoggerPtr->Writer(log_string_sString); \
        } \
    } \
} \

#define LOG_STREAM( log_stream_severity, log_stream_stream ) \
{ \
    LoggerPtr<ILoggerLib> pILoggerPtr(ILoggerLib::AcquireInstance()); \
    if (pILoggerPtr != NULL) \
    { \
        if (true == pILoggerPtr->Enabled(log_stream_severity)) \
        { \
            TSTRING log_stream_sHeadFoot; \
            TOSTRINGSTREAM log_stream_ssStream; \
            LOG_MESSAGE_HEADER(log_stream_sHeadFoot, log_stream_severity); \
            log_stream_ssStream << log_stream_sHeadFoot; \
            log_stream_ssStream << log_stream_stream.str(); \
            LOG_FOOTER( log_stream_sHeadFoot); \
            log_stream_ssStream << log_stream_sHeadFoot; \
            pILoggerPtr->Writer(log_stream_ssStream); \
        } \
    } \
} \


// HELPER MACROS

void LOG_MESSAGE_ARGS(TSTRING& sMessage, const TCHAR* pszMsgFormat, ...)
{
    const int   nCheckBuffer = 8192;
    TCHAR     szCheckBuffer[nCheckBuffer] = { 0 };

    if (NULL != pszMsgFormat)
    {
        sMessage.erase();

        va_list marker;
        va_start(marker, pszMsgFormat);
        TVSNPRINTF(szCheckBuffer, nCheckBuffer - 1, pszMsgFormat, marker);

        if (NULL != szCheckBuffer)
        {
            sMessage = szCheckBuffer;

            TSTRING::size_type pos = TSTRING::npos;
            TSTRING newLine = _TEXT("\n");
            TSTRING carrageReturn = _TEXT("\r");

            pos = sMessage.find(newLine);
            if (TSTRING::npos != pos) sMessage.replace(pos, newLine.length(), _TEXT(""));

            pos = sMessage.find(carrageReturn);
            if (TSTRING::npos != pos) sMessage.replace(pos, carrageReturn.length(), _TEXT(""));
        }

        va_end(marker);
    }
}

static void LOG_MESSAGE_HEADER(TSTRING& sHeader, ILoggerLib::Severity eSeverity)
{
    TOSTRINGSTREAM  ssHeader;
    struct tm           tm = { 0 };
    time_t              now = { 0 };

    struct tm*          ptm = &tm;

    sHeader = _TEXT("[TID] [YYYY-MM-DD HH:MM:SS] [SEVERITY] - ");

    now = time(0);

#ifdef _WIN32
    errno_t  err = localtime_s(&tm, &now);
    if (0 == err)
#else
    ptm = localtime(&now);
    if (NULL != ptm)
#endif
    {
        ssHeader << _TEXT("[") << CThreadGuard<THREAD_TYPE>::GetThisThreadId() << _TEXT("] ");
        ssHeader << std::setfill(_TEXT('0')) << _TEXT("[") << std::setw(4) << (1900 + ptm->tm_year) << _TEXT("-") << std::setw(2) << (1 + ptm->tm_mon) << _TEXT("-") << std::setw(2) << ptm->tm_mday << _TEXT(" ") << std::setw(2) << ptm->tm_hour << _TEXT(":") << std::setw(2) << ptm->tm_min << _TEXT(":") << std::setw(2) << ptm->tm_sec << _TEXT("] ");
        ssHeader << _TEXT("[") << ILoggerLib::SeverityToString(eSeverity) << _TEXT("] - ");
    }

    sHeader = ssHeader.str();
}

#endif // LOGGER_MACROS_H

