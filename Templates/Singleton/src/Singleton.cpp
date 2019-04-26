/*
* File:   Singleton.cpp
*/


// INCLUDES

#include "Singleton.h"
#include "SingletonStatus.h"


// FORWARD DECLARATIONS

class CSingleton;


// DECLARATIONS

class CSingleton : public ISingleton
{
public:

    static long CreateInstance(CSingleton** ppInstance);
    static CSingleton* AcquireInstance();

    long ReleaseInstance();

    // ADD OVERRIDDEN INTERFACE METHOD DECLARATIONS

private:

    static CSingleton*          g_pInstance;
    static int                  g_nInstances;

    CSingleton();
    virtual ~CSingleton() {};
    CSingleton(const CSingleton&) {};
    CSingleton& operator=(const CSingleton&) {};

    MUTEX_TYPE      m_mutex; 

    static MUTEX_TYPE                       g_instance_mutex;
    static CStaticMutexInit<MUTEX_TYPE>     g_initStaticInstanceMutex;
};



// PRIVATE GLOBALS

MUTEX_TYPE                      CSingleton::g_instance_mutex;
CStaticMutexInit<MUTEX_TYPE>    CSingleton::g_initStaticInstanceMutex(CSingleton::g_instance_mutex);

CSingleton*                     CSingleton::g_pInstance = NULL;
int                             CSingleton::g_nInstances = 0;


// DEFINITIONS


// ISingleton

/*static*/ long ISingleton::CreateInstance(ISingleton** ppInstance)
{
    long  lStatus = SingletonStatus::SUCCESS;
    CSingleton* pCInstance = NULL;

    try
    {
        if (NULL == ppInstance) return SingletonStatus::E_INVALID_PARAMETER;

        lStatus = CSingleton::CreateInstance(&pCInstance);
        *ppInstance = pCInstance;
    }
    catch (long&  lcatch_return)
    {
        lStatus = lcatch_return;
    }
    catch (...)
    {
        lStatus = SingletonStatus::E_INSTANCE_EXCEPTION;
    }

    return lStatus;
}

/*static*/ ISingleton* ISingleton::AcquireInstance()
{
    long  lStatus = SingletonStatus::SUCCESS;
    CSingleton* pCInstance = NULL;

    try
    {
        pCInstance = CSingleton::AcquireInstance();
    }
    catch (long&  lcatch_return)
    {
        lStatus = lcatch_return;
    }
    catch (...)
    {
        lStatus = SingletonStatus::E_INSTANCE_EXCEPTION;
    }

    return pCInstance;
}


// CSingleton

/*static*/ long CSingleton::CreateInstance(CSingleton** ppInstance)
{
    long  lStatus = SingletonStatus::SUCCESS;

    try
    {
        CLockGuard<MUTEX_TYPE> lock(g_instance_mutex);

        if (NULL == ppInstance) return SingletonStatus::E_INVALID_PARAMETER;

        if (NULL == CSingleton::g_pInstance)
        {
            CSingleton::g_pInstance = new CSingleton();
            *ppInstance = CSingleton::g_pInstance;
            CSingleton::g_nInstances = 1;
        }
        else
        {
            *ppInstance = CSingleton::g_pInstance;
            CSingleton::g_nInstances++;
        }
    }
    catch (long&  lcatch_return)
    {
        lStatus = lcatch_return;
    }
    catch (...)
    {
        lStatus = SingletonStatus::E_INSTANCE_EXCEPTION;
    }

    return lStatus;
}

/*static*/ CSingleton* CSingleton::AcquireInstance()
{
    long  lStatus = SingletonStatus::SUCCESS;

    try
    {
        CLockGuard<MUTEX_TYPE> lock(g_instance_mutex);

        if (NULL != CSingleton::g_pInstance)
        {
            CSingleton::g_nInstances++;
        }
    }
    catch (long&  lcatch_return)
    {
        lStatus = lcatch_return;
    }
    catch (...)
    {
        lStatus = SingletonStatus::E_INSTANCE_EXCEPTION;
    }

    return CSingleton::g_pInstance;
}

long CSingleton::ReleaseInstance()
{
    long  lStatus = SingletonStatus::SUCCESS;

    try
    {
        CLockGuard<MUTEX_TYPE> lock(g_instance_mutex);

        if (NULL != CSingleton::g_pInstance)
        {
            CSingleton::g_nInstances--;

            if (0 >= CSingleton::g_nInstances)
            {
                delete CSingleton::g_pInstance;
                CSingleton::g_pInstance = NULL;
            }
        }
        else
        {
            lStatus = SingletonStatus::E_INSTANCE_DOES_NOT_EXIST;
        }
    }
    catch (long&  lcatch_return)
    {
        lStatus = lcatch_return;
    }
    catch (...)
    {
        lStatus = SingletonStatus::E_INSTANCE_EXCEPTION;
    }

    return lStatus;
}

CSingleton::CSingleton()
{
    long  lStatus = SingletonStatus::SUCCESS;

    try
    {
        CLockGuard<MUTEX_TYPE>::InitializeLock(m_mutex); 
    }
    catch (long&  lcatch_return)
    {
        throw lcatch_return;
    }
    catch (...)
    {
        throw SingletonStatus::E_INSTANCE_EXCEPTION;
    }
}
