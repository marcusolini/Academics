// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

// ProducerConsumer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ProdConQueue.h"
#include "Producer.h"
#include "Consumer.h"

#include <exception>
#include <iostream>

// TODO: Create Data Class
using POD_CON_DATA = int;

DWORD WINAPI ProducerThread(LPVOID lpParam);
DWORD WINAPI ConsumerThread(LPVOID lpParam);


int wmain()
{
     int nStatus = EXIT_SUCCESS;

     DWORD dwStatus = ERROR_SUCCESS;

     size_t queueSize = 10;

     const int nProducerThreads = 1;
     size_t producerOneQuantity = 55;
     //size_t producerTwoQuantity = 30;

     const int nConsumerThreads = 3;
     size_t consumerOneQuantity = 20;
     size_t consumerTwoQuantity = 25;
     size_t consumerThreeQuantity = 10;

     const int nThreads = nProducerThreads + nConsumerThreads;
     HANDLE hThreads[nThreads] = {};
     DWORD dwThreadIds[nThreads] = {};

     try
     {

     CProdConQueue<POD_CON_DATA> prodConQueue(queueSize);

     CProducer<POD_CON_DATA> producerOne(prodConQueue, producerOneQuantity, 0, 0);
     //CProducer<POD_CON_DATA> producerTwo(prodConQueue, producerTwoQuantity, 0, 0);

     CConsumer<POD_CON_DATA> consumerOne(prodConQueue, consumerOneQuantity, 0);
     CConsumer<POD_CON_DATA> consumerTwo(prodConQueue, consumerTwoQuantity, 0);
     CConsumer<POD_CON_DATA> consumerThree(prodConQueue, consumerThreeQuantity, 0);

     int num = 0;
     hThreads[num] = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ProducerThread, &producerOne, 0, &dwThreadIds[num]);
     num += 1;
     //hThreads[num] = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ProducerThread, &producerTwo, 0, &dwThreadIds[num]);
     //num += 1;
     hThreads[num] = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ConsumerThread, &consumerOne, 0, &dwThreadIds[num]);
     num += 1;
     hThreads[num] = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ConsumerThread, &consumerTwo, 0, &dwThreadIds[num]);
     num += 1;
     hThreads[num] = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ConsumerThread, &consumerThree, 0, &dwThreadIds[num]);

     WaitForMultipleObjects(nThreads, hThreads, TRUE, INFINITE);

     for (auto& handle : hThreads)
     {
          CloseHandle(handle);
     }

     }
     catch (std::exception& e)
     {
          std::wcerr << L"exception: " << e.what() << std::endl;
          nStatus = EXIT_FAILURE;
     }

     return nStatus;
}


DWORD WINAPI ProducerThread(LPVOID lpParam)
{
     DWORD dwStatus = EXIT_SUCCESS;

     CProducer<POD_CON_DATA>* pProducer = (CProducer<POD_CON_DATA>*)lpParam;

     try
     {

     size_t quantity = pProducer->Quantity();

     for (size_t count = 0; count < quantity; count++)
     {
          POD_CON_DATA data = pProducer->Create();
          pProducer->Add(data);
          std::wcout << L"Added Data: " << data << L" " << __FUNCTION__ << L":" << __LINE__ << std::endl;
     }

     }
     catch (std::exception& e)
     {
          std::wcerr << L"exception: " << e.what() << std::endl;
          dwStatus = EXIT_FAILURE;
     }

     return dwStatus;
}


DWORD WINAPI ConsumerThread(LPVOID lpParam)
{
     DWORD dwStatus = EXIT_SUCCESS;

     CConsumer<POD_CON_DATA>* pConsumer = (CConsumer<POD_CON_DATA>*)lpParam;

     try
     {

          size_t quantity = pConsumer->Quantity();

          for (size_t count = 0; count < quantity; count++)
          {
               POD_CON_DATA data = pConsumer->Remove();
               std::wcout << L"Removed Data: " << data << L" " << __FUNCTION__ << L":" << __LINE__ << std::endl;
          }

     }
     catch (std::exception& e)
     {
          std::wcerr << L"exception: " << e.what() << std::endl;
          dwStatus = EXIT_FAILURE;
     }

     return dwStatus;
}
