/**
* © Copyright 2018, Cole Engineering Services, Inc.   All Rights Reserved.
*/

// Thread Guard for Windows and Linux:
//   std::thread (C++14) - Windows Visual Studio and Netbeans
//   pthread_t (C++98) - Linux Netbeans
//   HANDLE (C++98) - Windows Visual Studio

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

#ifndef THREAD_GUARD_H
#define THREAD_GUARD_H

// TODO: Add Error Handling

#if __cplusplus >= 201103L
     #include <thread>
     typedef std::thread THREAD_TYPE;
#else
     #ifdef _WIN32
          #include <windows.h>
          typedef HANDLE THREAD_TYPE;
     #else
          #include <pthread.h>
          typedef pthread_t THREAD_TYPE;
     #endif
#endif


#include <iostream>
          
// DECLARATIONS

template < class TTHREAD > class CThreadGuard
{
public:
     /*explicit*/ CThreadGuard(TTHREAD& thread);
     ~CThreadGuard();

     static unsigned long long     GetThisThreadId();      
     static long                   ExecuteThread(TTHREAD& thread, void* pvStartRoutine, void* pvParameter); // TODO
     static void                   UnInitializeThread(TTHREAD& thread);                  // TODO

private:
     CThreadGuard(CThreadGuard const&); // = delete;
     CThreadGuard& operator=(CThreadGuard const&); // = delete;

     TTHREAD& m_thread;
};


// DEFINITIONS

template < class TTHREAD >
CThreadGuard<TTHREAD>::CThreadGuard(TTHREAD& thread) : m_thread(thread) { }


template < class TTHREAD >
CThreadGuard<TTHREAD>::~CThreadGuard()
{
#if __cplusplus >= 201103L
     if (m_thread.joinable())
     {
         m_thread.join();
     }
#else
     #ifdef _WIN32
         DWORD dwStatus = WaitForSingleObject(m_thread, INFINITE);
     #else
         pthread_join(m_thread, NULL);     
     #endif
#endif
}


template < class TTHREAD >
unsigned long long  CThreadGuard<TTHREAD>::GetThisThreadId()
{
     unsigned long long ullThreadId = 0;

#if __cplusplus >= 201103L

     std::thread::id threadId = std::this_thread::get_id();

     #ifdef _WIN32
          std::stringstream ss;
          ss << threadId;
          ullThreadId = std::stoull(ss.str());
     #else
          //std::stringstream ss;
          //ss << threadId;   // Does not compile for windows netbeans.
          //ullThreadId = std::stoull(ss.str());
          pthread_t pid = pthread_self();
          ullThreadId = (unsigned long long)(pid);
     #endif
#else
     #ifdef _WIN32
          ullThreadId = GetCurrentThreadId();
     #else
          pthread_t pid = pthread_self();
          ullThreadId = (unsigned long long)(pid);
     #endif
#endif
     return ullThreadId;
}

template < class TTHREAD >
long CThreadGuard<TTHREAD>::ExecuteThread(TTHREAD& thread, void* pvStartRoutine, void* pvParameter)
{
     long lStatus = 0;

#if __cplusplus >= 201103L
     TTHREAD localThread = THREAD_TYPE(pvStartRoutine, pvParameter);  // Does not compile.  Add Try-Catch
#else
     #ifdef _WIN32
          thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)pvStartRoutine, pvParameter, 0, NULL );

          if (NULL == thread)
          {
               lStatus = GetLastError();
          }
     #else
          lStatus = pthread_create(&thread, NULL, (void* (*)(void*)) (void*)(((pvStartRoutine))), pvParameter);
     #endif
#endif
     return lStatus;
}

template < class TTHREAD >
void CThreadGuard<TTHREAD>::UnInitializeThread(TTHREAD& thread)
{
#if __cplusplus >= 201103L

#else
     #ifdef _WIN32

     #else

     #endif
#endif
}

#endif // THREAD_GUARD_H