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

class CSortTest;

// DECLARATIONS

void ThreadFunc(const HWND hDlg, CSortTest * iTest);

// MAIN DIALOG
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onInitMainDialog(const HWND hDlg);
void onMainRunSortTestsButton(const HWND hDlg);
void onMainCancel(const HWND hDlg);
void onMainClose(const HWND hDlg);
void onChange(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
long GetInput(IN const HWND hDlg, OUT std::vector<int>& vArray, IN OPTIONAL bool* pbDataPresent = nullptr);
long ParseInput(IN const std::wstring& sToParse, OUT std::vector<int>& vArray);

// PROGRESS DIALOG
INT_PTR CALLBACK ProgressDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onProgressInit(const HWND hDlg);
void onProgressPauseContinue(const HWND hDlg);
void onProgressThreadState(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onProgressThreadComplete(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onProgressClose(const HWND hDlg, const WPARAM wParam);

std::wstring LoadStringFromResourceId(const UINT id);

class CSortTest
{
public:

     enum class ESORT_TYPE : int
     { UNDEFINED = 0, QUICK_SORT, MERGE_SORT, BUBBLE_SORT };

     enum class ESTATE_TYPE
     { UNDEFINED = 0, STARTED, RUNNING, PAUSED, INTERRUPTED, SUCCESS, FAILED };

     CSortTest(const ESORT_TYPE eSortType, const std::vector<int> vArray) : m_eSortType(eSortType), m_vArray(vArray) {}

     virtual ~CSortTest() {}

     ESORT_TYPE GetResourceAllocationType() { return m_eSortType; }
     void SetIsComplete() { std::lock_guard<std::recursive_mutex> lock(CSortTest::g_mutex); m_bComplete = true; }
     bool IsComplete() { std::lock_guard<std::recursive_mutex> lock(CSortTest::g_mutex); return m_bComplete; }
     void SetState(const ESTATE_TYPE state) { std::lock_guard<std::recursive_mutex> lock(CSortTest::g_mutex); m_state = state; }
     ESTATE_TYPE GetState() { std::lock_guard<std::recursive_mutex> lock(CSortTest::g_mutex);  return m_state; }
     void SetThreadId(uint64_t id) { std::lock_guard<std::recursive_mutex> lock(CSortTest::g_mutex);  m_threadId = id; }
     uint64_t GetThreadId() { std::lock_guard<std::recursive_mutex> lock(CSortTest::g_mutex);  return m_threadId; }

private:
     static std::recursive_mutex g_mutex;
     
     std::vector<int> m_vArray;
     ESORT_TYPE m_eSortType = ESORT_TYPE::UNDEFINED;
     bool m_bComplete = false;
     ESTATE_TYPE m_state = ESTATE_TYPE::UNDEFINED;
     uint64_t m_threadId = 0;
};



#endif // SORTING_TEST_UI_H