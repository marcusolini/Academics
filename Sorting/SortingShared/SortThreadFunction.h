// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#ifndef SORT_THREAD_FUNCTION_H
#define SORT_THREAD_FUNCTION_H

#ifdef _WIN32
     #include <Windows.h>
#else

#endif // _WIN32

#include <stdio.h>
#include <thread>
#include <atomic>
#include <vector>
#include <sstream>
#include <mutex>

#include "SortTest.h"

#include "../../Error_Checks/ERROR_CHECKS.H"

// SORT THREAD FUNCTION

static void SortThreadFunction(CSortTest* iTest)
{
     int nStatus = 0;

     std::wstringstream ss;
     uint64_t id = 0;

     std::vector<int> vArray;
     int* tempArray = nullptr;
     int* iTempArray = nullptr;
     std::size_t tempArraySize = 0;

     std::size_t nNumberOfSorts = 0;
     std::chrono::duration<double> duration;
     bool bSortCorrect = false;

     try
     {
          std::this_thread::yield();

          iTest->SetState(CSortTest::ESTATE_TYPE::STARTED);

          ss << std::this_thread::get_id();
          id = std::stoull(ss.str());
          iTest->SetThreadId(id);

          std::this_thread::sleep_for(std::chrono::milliseconds{ 750 });  // For Progress Bar Show

          iTest->SetState(CSortTest::ESTATE_TYPE::RUNNING);

          // Convet vector to array.
          vArray = iTest->GetArray();
          tempArraySize = vArray.size();

          tempArray = new int[tempArraySize];
          iTempArray = tempArray;

          for (auto& iArray : vArray)
          {
               *iTempArray = iArray;
               iTempArray += 1;
          }

          switch (iTest->GetSortType())
          {
          case CSortTest::ESORT_TYPE::QUICK_SORT:
               CHECK_SUCCEEDED_LOG_THROW((CSorting<int, std::size_t>::Sort(ESortingTypes::QuickSort, tempArray, tempArraySize, &nNumberOfSorts, &duration)));
               break;
          case CSortTest::ESORT_TYPE::MERGE_SORT:
               CHECK_SUCCEEDED_LOG_THROW((CSorting<int, std::size_t>::Sort(ESortingTypes::MergeSort, tempArray, tempArraySize, &nNumberOfSorts, &duration)));
               break;
          case CSortTest::ESORT_TYPE::BUBBLE_SORT:
               CHECK_SUCCEEDED_LOG_THROW((CSorting<int, std::size_t>::Sort(ESortingTypes::BubbleSort, tempArray, tempArraySize, &nNumberOfSorts, &duration)));
               break;
          default:
               break;
          }

          vArray.clear();

          for (std::size_t nIndex = 0; nIndex < tempArraySize; nIndex++)
          {
               vArray.push_back(tempArray[nIndex]);
          }

          iTest->SetSortedArray(vArray);
          iTest->SetNumberOfSorts(nNumberOfSorts);
          iTest->SetDuration(duration);

          CHECK_SUCCEEDED_LOG_THROW((CSorting<int, std::size_t>::VerifySort(tempArray, tempArraySize, bSortCorrect)));

          CHECK_BOOL_TRUE_LOG_THROW(bSortCorrect);

          iTest->SetState(CSortTest::ESTATE_TYPE::SUCCESS);
     }
     catch (long& check_catch_lresult)
     {
          nStatus = check_catch_lresult;
          iTest->SetError(nStatus);
          iTest->SetState(CSortTest::ESTATE_TYPE::FAILED);
     }

     iTest->SetIsComplete();
}

#endif // SORT_THREAD_FUNCTION_H

