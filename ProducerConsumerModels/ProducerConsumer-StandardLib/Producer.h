// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#pragma once

#include "ProdConQueue.h"

//#include "Log.h"

template <class DATA> class CProducer
{
public:
     CProducer(CProdConQueue<DATA>& queue, const std::size_t quantity, const uint64_t createDelay = 0, const uint64_t addDelay = 0);
     virtual ~CProducer();

     void operator()();

     DATA& Create();
     void Add(DATA& data);
     std::size_t Quantity();

     //CProducer(const CProducer&) = delete;
     //CProducer& operator=(const CProducer&) = delete;

private:
     CProdConQueue<DATA>& m_queue;
     std::size_t m_quantity = 0;
     uint64_t m_createDelay = 0;  // milliseconds
     uint64_t m_addDelay = 0;  // milliseconds
     DATA m_data = 0;
};


template <class DATA>
CProducer<DATA>::CProducer(CProdConQueue<DATA>& queue, const std::size_t quantity, const uint64_t createDelay, const uint64_t addDelay)
     : m_queue(queue), m_quantity(quantity), m_createDelay(createDelay), m_addDelay(addDelay)
{
     //LOG_START_TRACE();
}

template <class DATA>
CProducer<DATA>::~CProducer()
{
     //LOG_START_TRACE();
}


template <class DATA>
DATA& CProducer<DATA>::Create()
{
     //LOG_START_TRACE();

     if (m_createDelay)
     {
          std::this_thread::sleep_for(std::chrono::milliseconds{ m_createDelay });
     }

     m_data += 1;

     return m_data;
}


template <class DATA>
void CProducer<DATA>::Add(DATA& data)
{
     //LOG_START_TRACE();

     if (m_addDelay)
     {
          std::this_thread::sleep_for(std::chrono::milliseconds{ m_addDelay });
     }

     m_queue.Add(data);
}

template <class DATA>
std::size_t CProducer<DATA>::Quantity()
{
     //LOG_START_TRACE();

     return m_quantity;
}


template <class DATA>
void CProducer<DATA>::operator()()
{
     //LOG_START_TRACE();

     try
     {

     for (std::size_t count = 0; count < m_quantity; count++)
     {
          POD_CON_DATA data = Create();
          Add(data);
          //LOG("Added Data: ", data);
     }

     }
     catch (...)
     {
          //LOG_ERROR("exception: ", e.what());
          throw;
     }
}

