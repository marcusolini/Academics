// Copyright(c) 2018 Mark Nicolini

#pragma once

#include "ProdConQueue.h"

#include "Log.h"

template <class DATA> class CProducer
{
public:
     CProducer(CProdConQueue<DATA>& queue, const size_t quantity, const DWORD createDelay = 0, const DWORD addDelay = 0);
     virtual ~CProducer();

     void operator()();

     DATA& Create();
     void Add(DATA& data);
     size_t Quantity();

     //CProducer(const CProducer&) = delete;
     //CProducer& operator=(const CProducer&) = delete;

private:
     CProdConQueue<DATA>& m_queue;
     size_t m_quantity = 0;
     DWORD m_createDelay = 0;  // milliseconds
     DWORD m_addDelay = 0;  // milliseconds
     DATA m_data = 0;
};


template <class DATA>
CProducer<DATA>::CProducer(CProdConQueue<DATA>& queue, const size_t quantity, const DWORD createDelay, const DWORD addDelay)
     : m_queue(queue), m_quantity(quantity), m_createDelay(createDelay), m_addDelay(addDelay)
{
     LOG_START_TRACE();
}

template <class DATA>
CProducer<DATA>::~CProducer()
{
     LOG_START_TRACE();
}


template <class DATA>
DATA& CProducer<DATA>::Create()
{
     LOG_START_TRACE();

     if (m_createDelay)
     {
          Sleep(m_createDelay);
     }

     m_data += 1;

     return m_data;
}


template <class DATA>
void CProducer<DATA>::Add(DATA& data)
{
     LOG_START_TRACE();

     if (m_addDelay)
     {
          Sleep(m_addDelay);
     }

     m_queue.Add(data);
}

template <class DATA>
size_t CProducer<DATA>::Quantity()
{
     LOG_START_TRACE();

     return m_quantity;
}


template <class DATA>
void CProducer<DATA>::operator()()
{
     LOG_START_TRACE();

     try
     {

     //for (auto& i : m_quantity)
     for (int count = 0; count < m_quantity; count++)
     {
          POD_CON_DATA data = Create();
          Add(data);
          LOG("Added Data: ", data);
//          std::wcout << L"Added Data: " << data << L" " << __FUNCTION__ << L":" << __LINE__ << std::endl;
     }

     }
     catch (std::exception& e)
     {
          std::wcerr << L"exception: " << e.what() << std::endl;
          throw;
     }
}

