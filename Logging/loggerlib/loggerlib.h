// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//

// NOTES
//  #pragma comment(lib, "loggerlib.lib")
//  #include "loggerlib/loggerlib.h"
//
//  String or Stream Logging
//  Error standard output logging
//  Non-buffering
//  Single Process

// TODOS
//  Add teardown
//  Add use case scenarios
//  Add documentation
//  Consider buffered logging thread
//  Consider logging to file.

#ifndef LOGGERLIB_H
#define LOGGERLIB_H

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif 

#include <stdarg.h>
#include <string>
#include <ctime> 
#include <sstream>
#include <iomanip>
#include <time.h>
#include <cstdio>

#include "LockGuard.h" // contains conditional <mutex>, <pthread.h> and <windows.h>
#include "ThreadGuard.h"
#include "TSTRING.h"

// FORWARD DECLARATIONS

class ILoggerLib;


// DECLARATIONS

class ILoggerLib
{
public:

    enum Severity
    { None = 0, Debug, Informational, Warning, Error };

    static long CreateInstance(ILoggerLib** ppILoggerLib);
    static ILoggerLib* AcquireInstance();

    virtual long ReleaseInstance() = 0;

    virtual long Configure(ILoggerLib::Severity eSeverity) = 0;

    virtual bool Enabled(ILoggerLib::Severity eSeverity) = 0;

    virtual long Writer(const TSTRING& sMessage) = 0;
    virtual long Writer(const TOSTRINGSTREAM& sMessage) = 0;

    static ILoggerLib::Severity SeverityFromString(const TSTRING& sSeverity);
    static TSTRING SeverityToString(ILoggerLib::Severity eSeverity);

    static bool IsSuccess(const long code);
    static bool IsError(const long code);
    static long MakeErrorCode(const long code);

    static const long Success = 0;
    static const long InvalidParameter = 100 * -1;
    static const long InstanceError = 101 * -1;
    static const long ConfigurationError = 102 * -1;
    static const long SeverityError = 103 * -1;
    static const long LoggingError = 104 * -1;

};



template <class T> class LoggerPtr
{
public:
    LoggerPtr(T* p) : m_p(p) {}
    ~LoggerPtr() { (m_p != NULL) ? m_p->ReleaseInstance() : 0; }

        T* operator-> () { return m_p; }

    bool operator==(T* p) const { return m_p == p; }
    bool operator!=(T* p) const { return m_p != p; }
private:
    T* m_p;
};
#endif //LOGGERLIB_H


