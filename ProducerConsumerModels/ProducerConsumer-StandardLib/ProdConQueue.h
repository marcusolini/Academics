// Copyright(c) 2018 Mark Nicolini

#pragma once

#include <windows.h>
#include <deque>
#include <exception>
#include <iostream>
#include <chrono>

#include "Log.h"
#include "Semaphore.h"

// DECLARATIONS

template <class DATA> class CProdConQueue
{
public:
     CProdConQueue(size_t size);
     virtual ~CProdConQueue();
     void Add(DATA& data);
     DATA Remove();
private:
     std::deque<DATA> m_queue;
     size_t m_size = 0;

     Semaphore m_semaphore;
};


// DEFINIITIONS

template <class DATA>
CProdConQueue<DATA>::CProdConQueue(size_t size) : m_size(size)
{
     LOG_START_TRACE();

     m_semaphore.SetCount(size-1);
}

template <class DATA>
CProdConQueue<DATA>::~CProdConQueue()
{
     LOG_START_TRACE();
}

template <class DATA>
void CProdConQueue<DATA>::Add(DATA& data)
{
     LOG_START_TRACE();

     bool bStatus = false;

     m_queue.push_back(data);

     while (false == bStatus)
     {    // If the size has been hit, then release will fail, which is set to size-1 
          bStatus = m_semaphore.Release();
          if (false == bStatus) std::this_thread::sleep_for(std::chrono::milliseconds{ 50 });
     };
}

template <class DATA>
DATA CProdConQueue<DATA>::Remove()
{
     LOG_START_TRACE();

     bool  bStatus = false;
     DATA data;

     bStatus = m_semaphore.Wait();

     if (true == bStatus)
     {
          data = m_queue.front();
          m_queue.pop_front();
     }
     else
     {
          LOG_ERROR("Error removing data");
          throw std::runtime_error("Removal Failure");
     }

     return data;
}