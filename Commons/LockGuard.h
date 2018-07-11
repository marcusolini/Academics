/**
* © Copyright 2018, Cole Engineering Services, Inc.   All Rights Reserved.
*/

#ifndef LOCK_GUARD_H
#define LOCK_GUARD_H

#if __cplusplus >= 201103L
     #include <mutex>
     typedef std::recursive_mutex       MUTEX_TYPE;
#else
     #include <pthread.h>
     typedef pthread_mutex_t            MUTEX_TYPE;
#endif

template < class TMUTEX > class CLockGuard
{
public:
     CLockGuard(TMUTEX& mutex) : m_mutex(mutex)
     {
#if __cplusplus >= 201103L         
          m_mutex.lock();
#else
          pthread_mutex_lock(&m_mutex);
#endif
     }

     ~CLockGuard()
     {
#if __cplusplus >= 201103L         
          m_mutex.unlock();
#else
          pthread_mutex_unlock(&m_mutex);
#endif
     }
     static void InitializeLock(TMUTEX& mutex) 
     {
#if __cplusplus >= 201103L         
#else
          m_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
#endif
     }

private:
     CLockGuard(CLockGuard const&);
     CLockGuard& operator=(CLockGuard const&);

     TMUTEX& m_mutex;
};

#endif // LOCK_GUARD_H