// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//

#ifndef SIGNALER_H
#define SIGNALER_H

#include <mutex>
#include <condition_variable>

class CSignaler
{
public:
     CSignaler() { }

     void Wait()
     {
          std::unique_lock<std::mutex> lock(m_mutex);
          m_conditionVariable.wait(lock);
     }

     void SignalAll()
     {
          std::unique_lock<std::mutex> lock(m_mutex);
          m_conditionVariable.notify_all();
     }

     void SignalOne()
     {
          std::unique_lock<std::mutex> lock(m_mutex);
          m_conditionVariable.notify_one();
     }

private:
     std::mutex               m_mutex;
     std::condition_variable  m_conditionVariable;
};

#endif // SIGNAL_H

