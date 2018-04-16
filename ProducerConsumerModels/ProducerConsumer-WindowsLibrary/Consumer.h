// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#pragma once

#include "ProdConQueue.h"

template <class DATA> class CConsumer
{
public:
     CConsumer(CProdConQueue<DATA>& queue, size_t quantity, const DWORD removeDelay = 0);
     virtual ~CConsumer();

     DATA Remove();
     size_t Quantity();

     CConsumer(const CConsumer&) = delete;
     CConsumer& operator=(const CConsumer&) = delete;

private:
     CProdConQueue<DATA>& m_queue;
     size_t m_quantity = 0;
     DWORD m_removeDelay = 0;  // milliseconds
};


template <class DATA>
CConsumer<DATA>::CConsumer(CProdConQueue<DATA>& queue, const size_t quantity, const DWORD removeDelay)
     : m_queue(queue), m_quantity(quantity), m_removeDelay(removeDelay)
{
     std::wcout << __FUNCTION__ << ":" << __LINE__ << std::endl;
}

template <class DATA>
CConsumer<DATA>::~CConsumer()
{
     std::wcout << __FUNCTION__ << ":" << __LINE__ << std::endl;
}

template <class DATA>
DATA CConsumer<DATA>::Remove()
{
     std::wcout << __FUNCTION__ << ":" << __LINE__ << std::endl;

     if (m_removeDelay)
     {
          Sleep(m_removeDelay);
     }

     return m_queue.Remove();
}

template <class DATA>
size_t CConsumer<DATA>::Quantity()
{
     std::wcout << __FUNCTION__ << ":" << __LINE__ << std::endl;

     return m_quantity;
}