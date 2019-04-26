/* 
* File:   SingletonClient.cpp
*/


// INCLUDES

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif // _WIN32

//#include <stdarg.h>
//#include <vector>
//#include <mutex>
//#include <thread>
#include <iostream>
//#include <iomanip>
//#include <sstream>
//#include <cstdlib>
//#include <cstddef>
//#include <cstdio>
//#include <cstdlib>
//#include <cwchar>
//#include <cerrno>
//#include <cstring>
//#include <cmath>
//#include <time.h>

//#include "Commons/LockGuard.h" // contains conditional <mutex>, <pthread.h> and <windows.h>
//#include "Commons/ThreadGuard.h"
#include "Commons/TSTRING.h"

#include "Singleton.h"
#include "SingletonStatus.h"


// STRUCTURED EXCEPTION HANDLING  -  Compiler Option: Yes with SEH Exceptions (/EHa)
#ifdef _WIN32
    #include <eh.h>
    void my_trans_func( unsigned int u, EXCEPTION_POINTERS* pExp )  { throw std::bad_exception(); }  
    void my_terminate_handler() { TCOUT << _TEXT("Application Terminating") << std::endl; }
#endif // _WIN32


// MAIN

int main()
{
    long lStatus = SingletonStatus::SUCCESS;
    ISingleton* pCreatedInstance = NULL;
    ISingleton* pAquiredInstance = NULL;

    // STRUCTURED EXCEPTION HANDLING  -  Compiler Option: Yes with SEH Exceptions (/EHa)
	// IDE EXECUTION - Disable Exception Settings - Win32 Exceptions - Access Violation
    #ifdef _WIN32
        _set_se_translator(my_trans_func);  
        set_terminate(my_terminate_handler);
    #endif // _WIN32

    try {

    // 0 INSTANCES

    pAquiredInstance = ISingleton::AcquireInstance();
    if (NULL != pAquiredInstance)
    {
        TCOUT << _TEXT("Acquire Instance Error") << std::endl;
        TCOUT << _TEXT("TESTS STATUS: FAILURE");
        return lStatus;
    }

    // 0 INSTANCES

    lStatus = ISingleton::CreateInstance(&pCreatedInstance);
    if (IS_ERROR(lStatus) || (NULL == pCreatedInstance))
    {
        TCOUT << _TEXT("Create Instance Error") << std::endl;
        TCOUT << _TEXT("TESTS STATUS: FAILURE");
        return lStatus;
    }

    // 1 INSTANCES

    lStatus = pCreatedInstance->ReleaseInstance();
    if (IS_ERROR(lStatus))
    {
        TCOUT << _TEXT("Release Created Instance Error") << std::endl;
        TCOUT << _TEXT("TESTS STATUS: FAILURE");
        return lStatus;
    }

    // 0 INSTANCES

    try{
    lStatus = pCreatedInstance->ReleaseInstance();
    if (IS_SUCCESS(lStatus))
    {
        TCOUT << _TEXT("Release Created Instance Error") << std::endl;
        TCOUT << _TEXT("TESTS STATUS: FAILURE");
        return lStatus;
    }
    } catch(...) { /*Exception is success*/}

    // 0 INSTANCES

    pAquiredInstance = ISingleton::AcquireInstance();
    if (NULL != pAquiredInstance)
    {
        TCOUT << _TEXT("Acquire Instance Error") << std::endl;
        TCOUT << _TEXT("TESTS STATUS: FAILURE");
        return lStatus;
    }

    // 0 INSTANCES

    lStatus = ISingleton::CreateInstance(&pCreatedInstance);
    if (IS_ERROR(lStatus) || (NULL == pCreatedInstance))
    {
        TCOUT << _TEXT("Create Instance Error") << std::endl;
        TCOUT << _TEXT("TESTS STATUS: FAILURE");
        return lStatus;
    }

    // 1 INSTANCES

    pAquiredInstance = ISingleton::AcquireInstance();
    if (NULL == pAquiredInstance)
    {
        TCOUT << _TEXT("Acquire Instance Error") << std::endl;
        TCOUT << _TEXT("TESTS STATUS: FAILURE");
        return lStatus;
    }

    // 2 INSTANCES

    lStatus = pAquiredInstance->ReleaseInstance();
    if (IS_ERROR(lStatus))
    {
        TCOUT << _TEXT("Release Aquired Instance Error") << std::endl;
        TCOUT << _TEXT("TESTS STATUS: FAILURE");
        return lStatus;
    }

    // 1 INSTANCES

    pAquiredInstance = ISingleton::AcquireInstance();
    if (NULL == pAquiredInstance)
    {
        TCOUT << _TEXT("Acquire Instance Error") << std::endl;
        TCOUT << _TEXT("TESTS STATUS: FAILURE");
        return lStatus;
    }

    // 2 INSTANCES

    lStatus = pAquiredInstance->ReleaseInstance();
    if (IS_ERROR(lStatus))
    {
        TCOUT << _TEXT("Release Aquired Instance Error") << std::endl;
        TCOUT << _TEXT("TESTS STATUS: FAILURE");
        return lStatus;
    }

    // 1 INSTANCES

    lStatus = pCreatedInstance->ReleaseInstance();
    if (IS_ERROR(lStatus))
    {
        TCOUT << _TEXT("Release Created Instance Error") << std::endl;
        TCOUT << _TEXT("TESTS STATUS: FAILURE");
        return lStatus;
    }

    // 0 INSTANCES
    try {
    lStatus = pCreatedInstance->ReleaseInstance();
    if (IS_SUCCESS(lStatus))
    {
        TCOUT << _TEXT("Release Created Instance Error") << std::endl;
        TCOUT << _TEXT("TESTS STATUS: FAILURE");
        return lStatus;
    }
    } catch(...) { /*Exception is success*/}


    TCOUT << _TEXT("TESTS STATUS: SUCCEEDED")  << std::endl;


    }
    catch (long&  lcatch_return)
    {
        TCOUT << _TEXT("TEST STATUS: EXCEPTION: ") << lcatch_return << std::endl;
        lStatus = ERROR;
    }
    catch (std::exception &e)
    {
        TCOUT << _TEXT("TEST STATUS: EXCEPTION: ") << e.what()  << std::endl;
        lStatus = ERROR;
    }
    catch (...)
    {
        TCOUT << _TEXT("TEST STATUS: EXCEPTION")  << std::endl;
        lStatus = ERROR;
    }

    return lStatus;
}
