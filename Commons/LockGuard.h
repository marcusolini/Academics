// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//

// Mutex Guard for Windows and Linux:
//   std::recursive_mutex (C++11)
//   pthread_mutex_t (C++98)
//   HANDLE - Windows Only (C++98)
//
// Compiling:
//   Windows - Visual Studio 2017 Version 15.7.4
//        For C++14:
//             Compiler Option: "/Zc:__cplusplus" to honor C++14
//             Compiler Language: "ISO C++14 Standard (/std:c++14)"
//        For C++98:
//             Compiler Option: "/Zc:__cplusplus-" for C++98 (unconfigured default if VS v15.7.4 is not used)
//             Compiler Language: Can leave blank because if compiler option is set to to above or not set, then will default to 199711L, which is C++98.
//        Details: https://blogs.msdn.microsoft.com/vcblog/2018/04/09/msvc-now-correctly-reports-__cplusplus/
//   Linux & Windows - NetBeans Version 8.2 (Build 201609300101)
//        For C++14:
//             Compiler Option: "C++14"
//        For C++98:
//             Compiler Option: "C++98"

#ifndef LOCK_GUARD_H
#define LOCK_GUARD_H

// TODO: Add Error Handling

#if __cplusplus >= 201103L
     #include <mutex>
     typedef std::recursive_mutex MUTEX_TYPE;
#else
     #ifdef _WIN32
          #include <windows.h>
          typedef HANDLE MUTEX_TYPE;
     #else
          #include <pthread.h>
          typedef pthread_mutex_t MUTEX_TYPE;
     #endif
#endif



// DECLARATIONS

template <class TMUTEX> class CStaticMutexInit
{
public:
   CStaticMutexInit(TMUTEX& mutex);
};
 

template < class TMUTEX > class CLockGuard
{
public:
     /*explicit*/ CLockGuard(TMUTEX& mutex);
     ~CLockGuard();

     static void InitializeLock(TMUTEX& mutex);
     static void UnInitializeLock(TMUTEX& mutex);

private:
     CLockGuard(CLockGuard const&); // = delete;
     CLockGuard& operator=(CLockGuard const&); // = delete;

     TMUTEX& m_mutex;
};



// DEFINITIONS
template <class TMUTEX>
CStaticMutexInit<TMUTEX>::CStaticMutexInit(TMUTEX& mutex)
{ 
    CLockGuard<TMUTEX>::InitializeLock(mutex);
}
 

template < class TMUTEX >
CLockGuard<TMUTEX>::CLockGuard(TMUTEX& mutex) : m_mutex(mutex)
{
#if __cplusplus >= 201103L         
     m_mutex.lock();
#else
     #ifdef _WIN32
          DWORD dwStatus = WaitForSingleObject(m_mutex, INFINITE);
     #else
          int nStatus = pthread_mutex_lock(&m_mutex);
     #endif
#endif
}

template < class TMUTEX >
CLockGuard<TMUTEX>::~CLockGuard()
{
#if __cplusplus >= 201103L         
     m_mutex.unlock();
#else
     #ifdef _WIN32
          bool bStatus = ReleaseMutex(m_mutex);
     #else
          int nStatus = pthread_mutex_unlock(&m_mutex);
     #endif
#endif
}
template < class TMUTEX >
/*static*/ void CLockGuard<TMUTEX>::InitializeLock(TMUTEX& mutex)
{
#if __cplusplus >= 201103L         
#else
     #ifdef _WIN32
          mutex = CreateMutex(NULL, FALSE, NULL);
     #else
          //mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
          pthread_mutexattr_t mta;
          pthread_mutexattr_init(&mta);
          pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE);
          pthread_mutex_init(&mutex, &mta);
     #endif
#endif
}

template < class TMUTEX >
/*static*/ void CLockGuard<TMUTEX>::UnInitializeLock(TMUTEX& mutex)
{
#if __cplusplus >= 201103L         
#else
     #ifdef _WIN32
          bool bStatus = CloseHandle(mutex);
     #else
          int nStatus = pthread_mutex_destroy(&mutex);
     #endif
#endif
}

#endif // LOCK_GUARD_H
