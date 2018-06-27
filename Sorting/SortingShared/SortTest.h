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

#ifndef QT_GUI_LIB
     void ThreadFunc(const HWND hDlg, CSortTest* iTest);
     #ifdef _WIN32
          #define WM_USER_THREAD_COMPLETE (WM_USER + 100)
          #define WM_ALL_THREADS_COMPLETE (WM_USER + 101)
     #endif // _WIN32
#else
     #include "../sortingdialogqtui.h"
     void ThreadFunc(sortingdialog* pSortingdialog, CSortTest* iTest);


#endif // QT_GUI_LIB


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
          UNDEFINED = 0, STARTED, RUNNING, PAUSED, INTERRUPTED, SUCCESS, FAILED
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
     size_t GetNumberOfSorts() { return m_nNumberOfSorts; }

     void SetDuration(IN const std::chrono::duration<double> duration) { m_duration = duration; }
     std::chrono::duration<double> GetDuration() { return m_duration; }

     void SetError(IN const long nError) { m_nError = nError; }
     long GetError() { return m_nError; }

     void SetThreadId(IN uint64_t id) { m_threadId = id; }
     uint64_t GetThreadId() { return m_threadId; }

     static std::recursive_mutex g_mutex;
     static std::vector<CSortTest> g_sortTests;

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


// DEFINITIONS

// GLOBALS

/*static*/ std::recursive_mutex CSortTest::g_mutex;
/*static*/ std::vector<CSortTest> CSortTest::g_sortTests;

// THREAD FUNCTION

#ifndef QT_GUI_LIB
    void ThreadFunc(const HWND hDlg, CSortTest* iTest)
#else
    void ThreadFunc(sortingdialog* pSortingdialog, CSortTest* iTest)
#endif // QT_GUI_LIB
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

                                                                          // Synchronize thread runnings
          {std::lock_guard<std::recursive_mutex> lock(CSortTest::g_mutex); }

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

     {std::lock_guard<std::recursive_mutex> lock(CSortTest::g_mutex);
     iTest->SetIsComplete();
     } //std::lock_guard<std::recursive_mutex> lock(g_mutex);

#ifndef QT_GUI_LIB
     SendMessage(hDlg, WM_USER_THREAD_COMPLETE, (WPARAM)iTest->GetThreadId(), (LPARAM)iTest->GetState());
#else
       // TODO
#endif // QT_GUI_LIB



}

#endif // SORT_TEST_H
