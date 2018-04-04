// Copyright(c) 2018 Mark Nicolini

#pragma once

#include <windows.h>
#include <deque>
#include <exception>
#include <iostream>


// DECLARATIONS

template <class DATA> class CProdConQueue
{
public:
     CProdConQueue(size_t size);
     virtual ~CProdConQueue();
     void Add(DATA& data);
     DATA& Remove();
private:
     std::deque<DATA> m_queue;
     size_t m_size = 0;

     DWORD  m_semWaitinMinutes = 2 * 60 * 1000;  // 2 minute wait.
     int    m_retries = 3;
     HANDLE m_semHandle = INVALID_HANDLE_VALUE;
};


// DEFINIITIONS

template <class DATA>
CProdConQueue<DATA>::CProdConQueue(size_t size) : m_size(size)
{
     std::wcout << __FUNCTION__ << ":" << __LINE__ << std::endl;

     m_semHandle = CreateSemaphore(nullptr, 0, (LONG)size - 1, nullptr);

     if (nullptr == m_semHandle)
     {
          std::wcout << L"Error creating Semaphore: " << GetLastError() << std::endl;
          throw std::runtime_error("Semaphore Creation Failure");
     }
}

template <class DATA>
CProdConQueue<DATA>::~CProdConQueue()
{
     std::wcout << __FUNCTION__ << ":" << __LINE__ << std::endl;

     if (INVALID_HANDLE_VALUE != m_semHandle)
     {
          bool bStatus = CloseHandle(m_semHandle);

          if (false == bStatus)
          {
               std::wcout << L"Error closing Semaphore: " << GetLastError() << std::endl;
               //throw std::runtime_error("Semaphore Deletion Failure");
          }
     }
}

template <class DATA>
void CProdConQueue<DATA>::Add(DATA& data)
{
     std::wcout << __FUNCTION__ << ":" << __LINE__ << std::endl;

     bool bStatus = false;

     m_queue.push_back(data);

     while (false == bStatus)
     {    // If the size has been hit, then release will fail, which is set to size-1 
          bStatus = ReleaseSemaphore(m_semHandle, 1, nullptr);
     };
}

template <class DATA>
DATA& CProdConQueue<DATA>::Remove()
{
     std::wcout << __FUNCTION__ << ":" << __LINE__ << std::endl;

     DWORD dwStatus = ERROR_SUCCESS;
     bool  bStatus = false;
     DATA data;

     for (int retried = 0; (retried < m_retries) && (false == bStatus); m_retries++)
     {
          dwStatus = WaitForSingleObject(m_semHandle, m_semWaitinMinutes);

          switch (dwStatus)
          {
          case WAIT_OBJECT_0:
               data = m_queue.front();
               m_queue.pop_front();
               bStatus = true;
               break;

          case WAIT_TIMEOUT:
               std::wcout << L"Semaphore timeout" << std::endl;
               break;

          case WAIT_ABANDONED_0:
               std::wcout << L"Semaphore abandoned" << std::endl;
               break;

          case WAIT_FAILED:
               std::wcout << L"Semaphore failed: " << GetLastError() << std::endl;
               break;

          default:
               std::wcout << L"Semaphore error: " << GetLastError() << std::endl;
          }
     }

     if (false == bStatus)
     {
          std::wcout << L"Error removing data" << std::endl;
          throw std::runtime_error("Removal Failure");
     }

     return data;
}