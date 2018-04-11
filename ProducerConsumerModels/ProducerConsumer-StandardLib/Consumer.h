// Copyright(c) 2018 marcusolini@outlook.com

#pragma once

#include "ProdConQueue.h"

#include "Log.h"

template <class DATA> class CConsumer
{
public:
     CConsumer(CProdConQueue<DATA>& queue, size_t quantity, const DWORD removeDelay = 0);
     virtual ~CConsumer();

     void operator()();

     DATA Remove();
     size_t Quantity();

     //CConsumer(const CConsumer&) = delete;
     //CConsumer& operator=(const CConsumer&) = delete;

private:
     CProdConQueue<DATA>& m_queue;
     size_t m_quantity = 0;
     DWORD m_removeDelay = 0;  // milliseconds
};


template <class DATA>
CConsumer<DATA>::CConsumer(CProdConQueue<DATA>& queue, const size_t quantity, const DWORD removeDelay)
     : m_queue(queue), m_quantity(quantity), m_removeDelay(removeDelay)
{
     LOG_START_TRACE();
}

template <class DATA>
CConsumer<DATA>::~CConsumer()
{
     LOG_START_TRACE();
}

template <class DATA>
DATA CConsumer<DATA>::Remove()
{
     LOG_START_TRACE();

     if (m_removeDelay)
     {
          Sleep(m_removeDelay);
     }

     return m_queue.Remove();
}

template <class DATA>
size_t CConsumer<DATA>::Quantity()
{
     LOG_START_TRACE();

     return m_quantity;
}

template <class DATA>
void CConsumer<DATA>::operator()()
{
     LOG_START_TRACE();

     try
     {

     //for (auto& count : m_quantity)
     for (int count = 0; count < m_quantity; count++)
     {
          POD_CON_DATA data = Remove();
          LOG("Removed Data: ", data);
     }

     }
     catch (std::exception& e)
     {
          LOG_ERROR("exception: ", e.what());
          throw;
     }
}

