/* 
* File:   Singleton.h
*/

#ifndef SINGLETON_H
#define SINGLETON_H

// INCLUDES

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>

    #ifdef SINGLETON_EXPORTS  
        #define CLASS_DECLSPEC    __declspec(dllexport)  
    #else  
        #define CLASS_DECLSPEC    __declspec(dllimport)  
    #endif  // SINGLETON_EXPORTS

    #define CALL __stdcall
#else
    #define CLASS_DECLSPEC
    #define CALL
#endif // _WIN32

//#include <stdarg.h>
//#include <vector>
//#include <mutex>
//#include <thread>
//#include <iostream>
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

#include "Commons/LockGuard.h" // contains conditional <mutex>, <pthread.h> and <windows.h>
//#include "Commons/ThreadGuard.h"
#include "Commons/TSTRING.h"


// FORWARD DECLARATIONS

class ISingleton;


// DECLARATIONS

extern "C"
{
    class ISingleton
    {
    public:
        CLASS_DECLSPEC static long CreateInstance(ISingleton** ppInstance);
        CLASS_DECLSPEC static ISingleton* AcquireInstance();

        virtual long ReleaseInstance() = 0;

        // ADD INTERFACE METHODS (PURE VIRTUAL)

    };
}


// SMART POINTER

template <class T> class SingletonPtr
{
public:
    SingletonPtr(T* p) : m_p(p) {}
    ~SingletonPtr() { (m_p != NULL) ? m_p->ReleaseInstance() : 0; }

    T* operator-> () { return m_p; }

    bool operator==(T* p) const { return m_p == p; }
    bool operator!=(T* p) const { return m_p != p; }
private:
    T* m_p;
};


#endif //SINGLETON_H
