#pragma once

#include <mutex>
#include <condition_variable>

class Semaphore
{
public:
     Semaphore(const size_t maxCount) : m_maxCount(maxCount) { }
     Semaphore() { }
     void SetCount(const size_t maxCount) { m_maxCount = maxCount; }

     bool Wait()
     {
          bool bStatus = false;

          std::unique_lock<std::mutex> lock(m_mutex);

          while (0 == m_count)
          {
               m_conditionVariable.wait(lock);
          }

          m_count--;
          bStatus = true;

          return bStatus;
     }

     bool Release()
     {
          bool bStatus = false;

          std::unique_lock<std::mutex> lock(m_mutex);

          if (m_count < m_maxCount)
          {
               bStatus = true;
               m_count++;
               m_conditionVariable.notify_one();
          }
          else
          {
               bStatus = false;
          }

          return bStatus;
     }

private:
     size_t m_maxCount = 0;
     size_t m_count = 0;

     std::mutex m_mutex;

     std::condition_variable m_conditionVariable;
};
