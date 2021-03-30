// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//

#include "loggerlib.h"

#ifdef _WIN32
    #include <windows.h>
#endif

#include <fstream>
#include <iostream>
#include <unordered_map>

#include "LockGuard.h"     // contains conditional <mutex>, <pthread.h> and <windows.h>

// FORWARD DECLARATIONS

class CLoggerLib;
     

// DECLARATIONS

class CLoggerLib : public ILoggerLib
{
public:
    
    static long CreateInstance(CLoggerLib** ppCLoggerLib);
    static CLoggerLib* AcquireInstance();

    long ReleaseInstance();

    long Configure(ILoggerLib::Severity eSeverity);
    
    bool Enabled(ILoggerLib::Severity eSeverity);

    long Writer(const TSTRING& sMessage);
    long Writer(const TOSTRINGSTREAM& sMessage);

private:

    static CLoggerLib*          g_pCLoggerLib;
    static bool                 g_bLoggerEnabled;
    static int                  g_nInstances;

    CLoggerLib();
    virtual ~CLoggerLib() {};
    CLoggerLib(const CLoggerLib&) {};
    CLoggerLib& operator=(const CLoggerLib&) {};

    MUTEX_TYPE      m_mutex; 
    MUTEX_TYPE      m_write_mutex; 

    ILoggerLib::Severity    m_eSeverity;

    static MUTEX_TYPE       g_instance_mutex;
    static CStaticMutexInit<MUTEX_TYPE>  g_initStaticInstanceMutex;
};



// PRIVATE GLOBALS

MUTEX_TYPE      CLoggerLib::g_instance_mutex;
CStaticMutexInit<MUTEX_TYPE> CLoggerLib::g_initStaticInstanceMutex(CLoggerLib::g_instance_mutex);

CLoggerLib*  CLoggerLib::g_pCLoggerLib = NULL;
bool CLoggerLib::g_bLoggerEnabled = false;
int CLoggerLib::g_nInstances = 0;


// DEFINITIONS

namespace SeverityString
{
    const TSTRING sNone = _TEXT("None");
    const TSTRING sDebug = _TEXT("Debug");
    const TSTRING sInformational = _TEXT("Informational");
    const TSTRING sWarning = _TEXT("Warning");
    const TSTRING sError = _TEXT("Error");
};


// ILoggerLib

/*static*/ long ILoggerLib::CreateInstance(ILoggerLib** ppILoggerLib)
{
    long  lStatus = ILoggerLib::Success;
    CLoggerLib* pCLoggerLib = NULL;

    if (NULL == ppILoggerLib) return ILoggerLib::InvalidParameter;

    lStatus = CLoggerLib::CreateInstance(&pCLoggerLib);
    *ppILoggerLib = pCLoggerLib;

    return lStatus;
}

/*static*/ ILoggerLib* ILoggerLib::AcquireInstance()
{
    long  lStatus = ILoggerLib::Success;
    CLoggerLib* pCLoggerLib = NULL;

    pCLoggerLib = CLoggerLib::AcquireInstance();

    return pCLoggerLib;
}

/*static*/ long ILoggerLib::MakeErrorCode(const long code)
{
    return (((long)code) * -1);
}
/*static*/ bool ILoggerLib::IsSuccess(const long code)
{
    return (((long)code) >= 0);
}
/*static*/ bool ILoggerLib::IsError(const long code)
{
    return (((long)code) < 0);
}



// CLoggerLib

/*static*/ long CLoggerLib::CreateInstance(CLoggerLib** ppCLoggerLib)
{
    CLockGuard<MUTEX_TYPE> lock(g_instance_mutex);

    long  lStatus = ILoggerLib::Success;

    if (NULL == ppCLoggerLib) return ILoggerLib::InvalidParameter;

    if (NULL == CLoggerLib::g_pCLoggerLib)
    {
        CLoggerLib::g_pCLoggerLib = new CLoggerLib();
        *ppCLoggerLib = CLoggerLib::g_pCLoggerLib;
        CLoggerLib::g_nInstances = 1;
    }
    else
    {
        *ppCLoggerLib = CLoggerLib::g_pCLoggerLib;
        CLoggerLib::g_nInstances++;
    }

    return lStatus;
}

/*static*/ CLoggerLib* CLoggerLib::AcquireInstance()
{
    CLockGuard<MUTEX_TYPE> lock(g_instance_mutex);

    long  lStatus = ILoggerLib::Success;

    if (NULL != CLoggerLib::g_pCLoggerLib)
    {
        CLoggerLib::g_nInstances++;
    }

    return CLoggerLib::g_pCLoggerLib;
}

long CLoggerLib::ReleaseInstance()
{
    CLockGuard<MUTEX_TYPE> lock(g_instance_mutex);


    long  lStatus = ILoggerLib::Success;

    try
    {
        if (NULL != CLoggerLib::g_pCLoggerLib)
        {
            CLoggerLib::g_nInstances--;

            if (0 >= CLoggerLib::g_nInstances)
            {
                delete CLoggerLib::g_pCLoggerLib;
                CLoggerLib::g_pCLoggerLib = NULL;
            }
        }
    }
    catch (long&  lcatch_return)
    {
        lStatus = lcatch_return;
    }
    catch (...)
    {
        lStatus = ILoggerLib::ConfigurationError;
    }

    return lStatus;
}

CLoggerLib::CLoggerLib() : m_eSeverity(ILoggerLib::Error)
{
    CLockGuard<MUTEX_TYPE>::InitializeLock(m_mutex); 
    CLockGuard<MUTEX_TYPE>::InitializeLock(m_write_mutex); 
}



long CLoggerLib::Configure(ILoggerLib::Severity eSeverity)
{
    CLockGuard<MUTEX_TYPE> lock(m_mutex);

    long  lStatus = ILoggerLib::Success;

    try
    {
        m_eSeverity = eSeverity;
    }
    catch (long&  lcatch_return)
    {
        lStatus = lcatch_return;
    }
    catch (...)
    {
        lStatus = ILoggerLib::ConfigurationError;
    }

    return lStatus;
}

bool CLoggerLib::Enabled(ILoggerLib::Severity eSeverity)
{
    long  lStatus = ILoggerLib::Success;

    ILoggerLib::Severity eCurrentServerity = ILoggerLib::Debug;
    bool bEnabled = false;

    try
    {
        if (ILoggerLib::None != eCurrentServerity)
        {
            if (eSeverity >= eCurrentServerity)
            {
                bEnabled = true;
            }
        }
    }
    catch (long&  lcatch_return)
    {
        lStatus = lcatch_return;
    }
    catch (...)
    {
        lStatus = ILoggerLib::LoggingError;
    }

    return bEnabled;
}

long CLoggerLib::Writer(const TSTRING& sMessage)
{
    CLockGuard<MUTEX_TYPE> lock(m_write_mutex);

    long  lStatus = ILoggerLib::Success;

    try
    {
        TCERR << sMessage << std::endl;
    }
    catch (long&  lcatch_return)
    {
        lStatus = lcatch_return;
    }
    catch (...)
    {
        lStatus = ILoggerLib::LoggingError;
    }

    return lStatus;
}

long CLoggerLib::Writer(const TOSTRINGSTREAM& sMessage)
{
    CLockGuard<MUTEX_TYPE> lock(m_write_mutex);

    long  lStatus = ILoggerLib::Success;

    try
    {
        TCERR << sMessage.str() << std::endl;
    }
    catch (long&  lcatch_return)
    {
        lStatus = lcatch_return;
    }
    catch (...)
    {
        lStatus = ILoggerLib::LoggingError;
    }

    return lStatus;
}



/*static*/ ILoggerLib::Severity ILoggerLib::SeverityFromString(const TSTRING& sSeverity)
{
    ILoggerLib::Severity eSeverity = ILoggerLib::None;

    if (SeverityString::sDebug == sSeverity)
    {
        eSeverity = ILoggerLib::Debug;
    }
    else if (SeverityString::sInformational == sSeverity)
    {
        eSeverity = ILoggerLib::Informational;
    }
    else if (SeverityString::sWarning == sSeverity)
    {
        eSeverity = ILoggerLib::Warning;
    }
    else if (SeverityString::sError == sSeverity)
    {
        eSeverity = ILoggerLib::Error;
    }
    else
    {
        eSeverity = ILoggerLib::None;
    }

    return eSeverity;
}

/*static*/ TSTRING ILoggerLib::SeverityToString(ILoggerLib::Severity eSeverity)
{
    TSTRING sServerity = SeverityString::sNone;

    switch (eSeverity)
    {
    case ILoggerLib::Debug:
        sServerity = SeverityString::sDebug;
        break;
    case ILoggerLib::Informational:
        sServerity = SeverityString::sInformational;
        break;
    case ILoggerLib::Warning:
        sServerity = SeverityString::sWarning;
        break;
    case ILoggerLib::Error:
        sServerity = SeverityString::sError;
        break;
    case ILoggerLib::None:
    default:
        sServerity = SeverityString::sNone;
        break;
    }

    return sServerity;
}
