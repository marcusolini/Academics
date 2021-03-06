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
#include "ThreadGuard.h"

#include <exception>
#include <iostream>
#include <thread>
#include <vector>

//#include "CreateLogger.h"
//#include "Log.h"

// TODO: Create Data Class
//using POD_CON_DATA = int;


int main()
{
     int nStatus = EXIT_SUCCESS;

     uint64_t dwStatus = 0;

     size_t queueSize = 10;

     size_t producerOneQuantity = 55;
     //size_t producerTwoQuantity = 30;

     size_t consumerOneQuantity = 20;
     size_t consumerTwoQuantity = 25;
     size_t consumerThreeQuantity = 10;

     std::vector<std::thread> threads;

     //std::shared_ptr< CLogger < CConsoleLogPolicy > > consoleLogger = CCreateLogger::CreateConsoleLogger("ConsoleLogger", ESeverity::Informational);

     try
     {

     CProdConQueue<POD_CON_DATA> prodConQueue(queueSize);

     CProducer<POD_CON_DATA> producerOne(prodConQueue, producerOneQuantity, 0, 0);
     //CProducer<POD_CON_DATA> producerTwo(prodConQueue, producerTwoQuantity, 0, 0);

     CConsumer<POD_CON_DATA> consumerOne(prodConQueue, consumerOneQuantity, 0);
     CConsumer<POD_CON_DATA> consumerTwo(prodConQueue, consumerTwoQuantity, 0);
     CConsumer<POD_CON_DATA> consumerThree(prodConQueue, consumerThreeQuantity, 0);

     threads.push_back(std::thread(consumerOne));
     threads.push_back(std::thread(consumerTwo));
     threads.push_back(std::thread(consumerThree));

     threads.push_back(std::thread(producerOne));

     CThreadGuard tOne(threads[0]);
     CThreadGuard tTwo(threads[1]);
     CThreadGuard tThree(threads[2]);
     CThreadGuard tFour(threads[3]);

     //for (auto& thread : threads)
     //{
     //     thread.join();
     //}

     }
     catch (...)
     {
          ////LOG_ERROR("exception: ", e.what());
          nStatus = EXIT_FAILURE;
     }

     //CCreateLogger::DeleteConsoleLogger(consoleLogger);

     return nStatus;
}