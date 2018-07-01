// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#ifndef SORT_TEST_H
#define SORT_TEST_H

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

#include "../../Error_Checks/ERROR_CHECKS.H"
#include "../SortingShared/SortingLib.h"

// DECLARATIONS

class CSortTest;


// DEFINITIONS

class CSortTest
{
public:

     enum class ESORT_TYPE : int
     {
          UNDEFINED = 0, QUICK_SORT, MERGE_SORT, BUBBLE_SORT
     };

     enum class ESTATE_TYPE
     {
          UNDEFINED = 0, NOT_RUN, STARTED, RUNNING, PAUSED, INTERRUPTED, SUCCESS, FAILED
     };

     CSortTest(const ESORT_TYPE eSortType, const std::vector<int> vArray) : m_eSortType(eSortType), m_vArray(vArray) {}

     virtual ~CSortTest() {}

     ESORT_TYPE GetSortType() { return m_eSortType; }

     std::vector<int> GetArray() { return m_vArray; }
     std::vector<int> GetSortedArray() { return m_vSortedArray; }
     void SetSortedArray(std::vector<int>& vSortedArray) { m_vSortedArray = vSortedArray; }

     void SetIsComplete() { m_bComplete = true; }
     bool IsComplete() { return m_bComplete; }

     void SetState(IN const ESTATE_TYPE eState) { m_eState = eState; }
     ESTATE_TYPE GetState() { return m_eState; }

     void SetNumberOfSorts(IN const std::size_t nNumberOfSorts) { m_nNumberOfSorts = nNumberOfSorts; }
     std::size_t GetNumberOfSorts() { return m_nNumberOfSorts; }

     void SetDuration(IN const std::chrono::duration<double> duration) { m_duration = duration; }
     std::chrono::duration<double> GetDuration() { return m_duration; }

     void SetError(IN const long nError) { m_nError = nError; }
     long GetError() { return m_nError; }

     void SetThreadId(IN uint64_t id) { m_threadId = id; }
     uint64_t GetThreadId() { return m_threadId; }

     static std::wstring ConvertVectorToString(IN const std::vector<int>& vArray)
     {
          std::wstring sArray;

          for (auto& iArray : vArray)
          {
               sArray += std::to_wstring(iArray);
               sArray += L" ";
          }

          return sArray;
     }


     static long ValidateAndConvertFromStringToVector(IN const std::wstring& sArray, OUT std::vector<int>& vArray)
     {
          long nStatus = 0;

          //int intTest = 0;
          std::wstring sParsed;
          std::wstring sSpace = L" ";

          try
          {
               for (auto iParse : sArray)
               {
                    std::wstring c = { iParse };

                    if (sSpace.compare(c))
                    {
                         /*intTest =*/ std::stoi(c); // should throw if not an integer.
                         sParsed += iParse;
                    }
                    else
                    {
                         if (sParsed.size())
                         {
                              vArray.push_back(std::stoi(sParsed));
                              sParsed.erase();
                         }
                    }
               }

               // Store the last parsed element.
               if (sParsed.size())
               {
                    vArray.push_back(std::stoi(sParsed)); // should throw if not an integer.
                    sParsed.erase();
               }
          }
          catch (std::invalid_argument)
          {
               nStatus = EINVAL;
          }
          catch (std::out_of_range)
          {
               nStatus = EINVAL;
          }
          catch (long& check_catch_lresult)
          {
               nStatus = check_catch_lresult;
          }

          return nStatus;
     }


     static std::wstring ConvertSortTypeToString(IN const CSortTest::ESORT_TYPE eSortType)
     {
          switch (eSortType)
          {
          case CSortTest::ESORT_TYPE::UNDEFINED: return L"Undefined"; break;
          case CSortTest::ESORT_TYPE::QUICK_SORT: return L"Quick Sort"; break;
          case CSortTest::ESORT_TYPE::MERGE_SORT: return L"Merge Sort"; break;
          case CSortTest::ESORT_TYPE::BUBBLE_SORT: return L"Bubble Sort"; break;
          default: return L"Undefined"; break;
          }
     }

     static std::wstring ConvertSortStateToString(IN const CSortTest::ESTATE_TYPE eSortState)
     {
          switch (eSortState)
          {
          case CSortTest::ESTATE_TYPE::UNDEFINED: return L"Undefined"; break;
          case CSortTest::ESTATE_TYPE::NOT_RUN: return L"Not Run"; break;
          case CSortTest::ESTATE_TYPE::STARTED: return L"Started"; break;
          case CSortTest::ESTATE_TYPE::RUNNING: return L"Running"; break;
          case CSortTest::ESTATE_TYPE::PAUSED: return L"Paused"; break;
          case CSortTest::ESTATE_TYPE::INTERRUPTED: return L"Interrupted"; break;
          case CSortTest::ESTATE_TYPE::SUCCESS: return L"Success"; break;
          case CSortTest::ESTATE_TYPE::FAILED: return L"Failed"; break;
          default: return L"Undefined"; break;
          }
     }


private:

     ESORT_TYPE m_eSortType = ESORT_TYPE::UNDEFINED;
     std::vector<int> m_vArray;
     std::vector<int> m_vSortedArray;
     bool m_bComplete = false;
     ESTATE_TYPE m_eState = ESTATE_TYPE::UNDEFINED;
     std::size_t m_nNumberOfSorts = 0;
     std::chrono::duration<double> m_duration;

     long m_nError = 0;
     uint64_t m_threadId = 0;
};



#endif // SORT_TEST_H
