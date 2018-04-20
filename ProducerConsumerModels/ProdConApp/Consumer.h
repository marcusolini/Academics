// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#pragma once

#include "ProdConQueue.h"

//#include "Log.h"

template <class DATA> class CConsumer
{
public:
     CConsumer(size_t quantity, const uint64_t removeDelay = 0);
     virtual ~CConsumer();

     void ConnectPipeline(CProdConQueue<DATA>* pipeline);
     void operator()();

     DATA Remove();
     size_t Quantity();

     //CConsumer(const CConsumer&) = delete;
     //CConsumer& operator=(const CConsumer&) = delete;

private:
     CProdConQueue<DATA>* m_pPipeline = nullptr;
     size_t m_quantity = 0;
     uint64_t m_removeDelay = 0;  // milliseconds
};


template <class DATA>
CConsumer<DATA>::CConsumer(const size_t quantity, const uint64_t removeDelay)
     : m_quantity(quantity), m_removeDelay(removeDelay)
{
     //LOG_START_TRACE();
}

template <class DATA>
CConsumer<DATA>::~CConsumer()
{
     //LOG_START_TRACE();
}

template <class DATA>
void CConsumer<DATA>::ConnectPipeline(CProdConQueue<DATA>* pipeline)
{
     if (pipeline)
     {
          m_pPipeline = pipeline;
     }
}

template <class DATA>
DATA CConsumer<DATA>::Remove()
{
     //LOG_START_TRACE();

     if (m_removeDelay)
     {
          std::this_thread::sleep_for(std::chrono::milliseconds{ m_removeDelay });
     }

     return m_pPipeline->Remove();
}

template <class DATA>
size_t CConsumer<DATA>::Quantity()
{
     //LOG_START_TRACE();

     return m_quantity;
}

template <class DATA>
void CConsumer<DATA>::operator()()
{
     //LOG_START_TRACE();

     try
     {

     //for (auto& count : m_quantity)
     for (int count = 0; count < m_quantity; count++)
     {
          PROD_CON_DATA data = Remove();
          //LOG("Removed Data: ", data);
     }

     }
     catch (...)
     {
          //LOG_ERROR("exception: ", e.what());
          throw;
     }
}

