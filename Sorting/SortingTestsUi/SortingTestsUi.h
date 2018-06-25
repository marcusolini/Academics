// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#ifndef SORTING_TEST_UI_H
#define SORTING_TEST_UI_H

#include <Windows.h>
#include <Windowsx.h>
#include <CommCtrl.h>
#include <tchar.h>
#include "resource.h"

#include <stdio.h>
#include <thread>
#include <atomic>
#include <vector>
#include <sstream>
#include <mutex>

// DECLARATIONS

#define WM_USER_THREAD_COMPLETE (WM_USER + 100)

class CSortTest;

void ThreadFunc(const HWND hDlg, CSortTest * iTest);

// MAIN DIALOG
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onMainInitDialog(const HWND hDlg);
void onMainRunSortTestsButton(const HWND hDlg);
void onMainCancel(const HWND hDlg);
void onMainClose(const HWND hDlg);
void onMainChange(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
long GetInput(IN const HWND hDlg, OUT std::vector<int>& vArray, IN OPTIONAL bool* pbDataPresent = nullptr);
long ParseInput(IN const std::wstring& sToParse, OUT std::vector<int>& vArray);

// PROGRESS DIALOG
INT_PTR CALLBACK ProgressDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onProgressInit(const HWND hDlg);
void onProgressThreadComplete(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onProgressClose(const HWND hDlg, const WPARAM wParam);

// RESULTS DIALOG
INT_PTR CALLBACK SortResultsDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onSortResultsInit(const HWND hDlg);
void onSortResultsClose(const HWND hDlg, const WPARAM wParam);

std::wstring LoadStringFromResourceId(const UINT id);


// DEFINITIONS

class CSortTest
{
public:

     enum class ESORT_TYPE : int
     { UNDEFINED = 0, QUICK_SORT, MERGE_SORT, BUBBLE_SORT };

     enum class ESTATE_TYPE
     { UNDEFINED = 0, STARTED, RUNNING, PAUSED, INTERRUPTED, SUCCESS, FAILED };

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
     std::chrono::duration<double> GetDuration() { return m_duration;  }

     void SetError(IN const long nError) { m_nError = nError; }
     long GetError() { return m_nError; }
     
     void SetThreadId(IN uint64_t id) { m_threadId = id; }
     uint64_t GetThreadId() { return m_threadId; }

     static std::wstring ConvertSortTypeToString(IN const ESORT_TYPE eSortType)
     {
          switch (eSortType)
          {
          case ESORT_TYPE::UNDEFINED: return LoadStringFromResourceId(IDS_UNDEFINED); break;
          case ESORT_TYPE::QUICK_SORT: return LoadStringFromResourceId(IDS_QUICK_SORT); break;
          case ESORT_TYPE::MERGE_SORT: return LoadStringFromResourceId(IDS_MERGE_SORT); break;
          case ESORT_TYPE::BUBBLE_SORT: return LoadStringFromResourceId(IDS_BUBBLE_SORT); break;
          default: return LoadStringFromResourceId(IDS_UNDEFINED); break;
          }
     }

     static std::wstring ConvertSortStateToString(IN const ESTATE_TYPE eSortState)
     {
          switch (eSortState)
          {
          case ESTATE_TYPE::UNDEFINED: return LoadStringFromResourceId(IDS_UNDEFINED); break;
          case ESTATE_TYPE::STARTED: return LoadStringFromResourceId(IDS_SORT_STARTED); break;
          case ESTATE_TYPE::RUNNING: return LoadStringFromResourceId(IDS_SORT_RUNNING); break;
          case ESTATE_TYPE::PAUSED: return LoadStringFromResourceId(IDS_SORT_PAUSED); break;
          case ESTATE_TYPE::INTERRUPTED: return LoadStringFromResourceId(IDS_SORT_NOT_RUN); break;
          case ESTATE_TYPE::SUCCESS: return LoadStringFromResourceId(IDS_SORT_SUCCESS); break;
          case ESTATE_TYPE::FAILED: return LoadStringFromResourceId(IDS_SORT_FAILED); break;
          default: return LoadStringFromResourceId(IDS_UNDEFINED); break;
          }
     }

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

     static std::vector<int> ConvertFromStringToVector(IN const std::wstring& sArray)
     {
          std::vector<int> vArray;

          for (auto& iArray : sArray)
          {
               vArray.push_back(iArray);
          }

          return vArray;
     }

private:
     
     std::vector<int> m_vArray;
     std::vector<int> m_vSortedArray;
     ESORT_TYPE m_eSortType = ESORT_TYPE::UNDEFINED;
     bool m_bComplete = false;
     ESTATE_TYPE m_eState = ESTATE_TYPE::UNDEFINED;
     std::size_t m_nNumberOfSorts = 0;
     std::chrono::duration<double> m_duration;

     long m_nError = 0;
     uint64_t m_threadId = 0;
};



#endif // SORTING_TEST_UI_H