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
#define WM_ALL_THREADS_COMPLETE (WM_USER + 101)

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

std::wstring ConvertSortTypeToString(IN const CSortTest::ESORT_TYPE eSortType);
std::wstring ConvertSortStateToString(IN const CSortTest::ESTATE_TYPE eSortState);

std::wstring LoadStringFromResourceId(const UINT id);

#endif // SORTING_TEST_UI_H