// Copyright(c) 2018 Mark Nicolini

#pragma once

#include "ProdConQueue.h"

template <class DATA> class CProducer
{
public:
     CProducer(CProdConQueue<DATA>& queue, const size_t quantity, const DWORD createDelay = 0, const DWORD addDelay = 0);
     virtual ~CProducer();

     DATA& Create();
     void Add(DATA& data);
     size_t Quantity();

     CProducer(const CProducer&) = delete;
     CProducer& operator=(const CProducer&) = delete;

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
     std::wcout << __FUNCTION__ << ":" << __LINE__ << std::endl;
}

template <class DATA>
CProducer<DATA>::~CProducer()
{
     std::wcout << __FUNCTION__ << ":" << __LINE__ << std::endl;
}


template <class DATA>
DATA& CProducer<DATA>::Create()
{
     std::wcout << __FUNCTION__ << ":" << __LINE__ << std::endl;

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
     std::wcout << __FUNCTION__ << ":" << __LINE__ << std::endl;

     if (m_addDelay)
     {
          Sleep(m_addDelay);
     }

     m_queue.Add(data);
}

template <class DATA>
size_t CProducer<DATA>::Quantity()
{
     std::wcout << __FUNCTION__ << ":" << __LINE__ << std::endl;

     return m_quantity;
}