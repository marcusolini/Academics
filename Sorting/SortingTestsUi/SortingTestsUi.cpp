// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#include "SortingTestsUi.h"
#include "../../Error_Checks/ERROR_CHECKS.H"

#include "../SortingTemplateLib/SortingLib.h"

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")


// GLOBALS

static std::vector<CSortTest> g_tests;
static std::recursive_mutex g_mutex;


// DEFINITIONS

std::wstring LoadStringFromResourceId(const UINT id)
{
     const std::size_t nString = 2048;
     wchar_t szString[nString] = {};
     if (LoadString(GetModuleHandle(nullptr), id, szString, nString - 1))
     {
          return szString;
     }
     else
     {
          if (LoadString(GetModuleHandle(nullptr), IDS_RESOURCE_STRING_MISSING, szString, nString - 1))
          {
               return szString;
          }
          else
          {
               return L"Resource String Missing";
          }
     }
}



// MAIN DIALOG

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
     HWND hDlg{};
     MSG msg{};

     InitCommonControls();
     hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN_SORT_DIALOG), nullptr, MainDialogProc);
     ShowWindow(hDlg, nCmdShow);
     UpdateWindow(hDlg);

     while (GetMessage(&msg, 0, 0, 0))
     {
          if (!IsDialogMessage(hDlg, &msg))
          {
               TranslateMessage(&msg);
               DispatchMessage(&msg);
          }
     }

     return (int)msg.wParam;
}

INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
     long nStatus = 0;

     switch (uMsg)
     {
     case WM_INITDIALOG:
          onMainInitDialog(hDlg);
          return TRUE;
          break;

     case WM_COMMAND:
     {
          switch (LOWORD(wParam))
          {
          case IDCANCEL:
               onMainCancel(hDlg);
               return TRUE;
               break;
          case ID_RunSortTestsButton:
               onMainRunSortTestsButton(hDlg);
               return TRUE;
               break;
          case IDC_QuickSortCheckbox:
          case IDC_MergeSortCheckbox:
          case IDC_BubbleSortCheckbox:
               switch (HIWORD(wParam))
               {
               case BN_CLICKED:
                    onMainChange(hDlg, uMsg, wParam, lParam);
                    return TRUE;
                    break;
               } // switch
               return TRUE;
               break;
          case IDC_SortArrayEdit:
               switch (HIWORD(wParam))
               {
               case EN_CHANGE:
                    onMainChange(hDlg, uMsg, wParam, lParam);
                    return TRUE;
                    break;
               } // switch
          } // switch

          break;
     }

     case WM_CLOSE:
          onMainClose(hDlg);
          return TRUE;
          break;
     case WM_DESTROY:
          PostQuitMessage(0);
          return TRUE;
          break;

     } // switch

     return FALSE;
}

void onMainInitDialog(const HWND hDlg)
{
     SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDCANCEL, (LPARAM)0);

     SendMessage(hDlg, WM_SETTEXT, TRUE, (LPARAM)(LoadStringFromResourceId(IDS_MAIN_SORT_MENU)).c_str());

     SendDlgItemMessage(hDlg, IDCANCEL, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_CLOSE)).c_str());
     SendDlgItemMessage(hDlg, IDC_SortArrayStatic, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SORT_ARRAY)).c_str());
     SendDlgItemMessage(hDlg, IDC_SortAlgorithmsGroup, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SORT_ALGORITHMS_GROUP)).c_str());
     SendDlgItemMessage(hDlg, IDC_QuickSortCheckbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_QUICK_SORT)).c_str());
     SendDlgItemMessage(hDlg, IDC_MergeSortCheckbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_MERGE_SORT)).c_str());
     SendDlgItemMessage(hDlg, IDC_BubbleSortCheckbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_BUBBLE_SORT)).c_str());
     SendDlgItemMessage(hDlg, ID_RunSortTestsButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_RUN_SORT_TESTS)).c_str());

     HWND hTemp{};

     hTemp = GetDlgItem(hDlg, IDC_QuickSortCheckbox);
     Edit_Enable(hTemp, FALSE);

     hTemp = GetDlgItem(hDlg, IDC_MergeSortCheckbox);
     Edit_Enable(hTemp, FALSE);

     hTemp = GetDlgItem(hDlg, IDC_BubbleSortCheckbox);
     Edit_Enable(hTemp, FALSE);

     hTemp = GetDlgItem(hDlg, ID_RunSortTestsButton);
     Edit_Enable(hTemp, FALSE);
}

void onMainChange(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
     long nStatus = 0;

     bool bDataPresent = false;
     std::vector<int> vArray;
     HWND hTemp{};
     LRESULT checked = 0;

     try
     {
          CHECK_SUCCEEDED_LOG_THROW(GetInput(hDlg, vArray, &bDataPresent));
     }
     catch (long& check_catch_lresult)
     {
          nStatus = check_catch_lresult;
          if (true == bDataPresent)
          {
               MessageBox(hDlg, (LoadStringFromResourceId(IDS_INVALID_DATA_ENTERED)).c_str(), (LoadStringFromResourceId(IDS_INVALID_DATA)).c_str(), (MB_ICONERROR | MB_OK));
          }
          vArray.clear();  // to cause diasabling of run tests button.
     }

     if (vArray.empty())
     {
          hTemp = GetDlgItem(hDlg, IDC_QuickSortCheckbox);
          Edit_Enable(hTemp, FALSE);

          hTemp = GetDlgItem(hDlg, IDC_MergeSortCheckbox);
          Edit_Enable(hTemp, FALSE);

          hTemp = GetDlgItem(hDlg, IDC_BubbleSortCheckbox);
          Edit_Enable(hTemp, FALSE);

          hTemp = GetDlgItem(hDlg, ID_RunSortTestsButton);
          Edit_Enable(hTemp, FALSE);
     }
     else
     {
          hTemp = GetDlgItem(hDlg, IDC_QuickSortCheckbox);
          Edit_Enable(hTemp, TRUE);

          hTemp = GetDlgItem(hDlg, IDC_MergeSortCheckbox);
          Edit_Enable(hTemp, TRUE);

          hTemp = GetDlgItem(hDlg, IDC_BubbleSortCheckbox);
          Edit_Enable(hTemp, TRUE);

          checked |= SendDlgItemMessage(hDlg, LOWORD(IDC_QuickSortCheckbox), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
          checked |= SendDlgItemMessage(hDlg, LOWORD(IDC_MergeSortCheckbox), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
          checked |= SendDlgItemMessage(hDlg, LOWORD(IDC_BubbleSortCheckbox), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
          if (BST_CHECKED == checked)
          {
               hTemp = GetDlgItem(hDlg, ID_RunSortTestsButton);
               Edit_Enable(hTemp, TRUE);
          }
          else
          {
               hTemp = GetDlgItem(hDlg, ID_RunSortTestsButton);
               Edit_Enable(hTemp, FALSE);
          }
     }
}

void onMainRunSortTestsButton(const HWND hDlg)
{
     long nStatus = 0;
     INT_PTR pProgressDialog = 0;
     INT_PTR pSortResultsDialog = 0;
     LRESULT checked = 0;

     std::vector<int> vArray;

     try
     {
          CHECK_SUCCEEDED_LOG_THROW( GetInput(hDlg, vArray) );

          checked |= SendDlgItemMessage(hDlg, LOWORD(IDC_QuickSortCheckbox), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
          checked |= SendDlgItemMessage(hDlg, LOWORD(IDC_MergeSortCheckbox), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
          checked |= SendDlgItemMessage(hDlg, LOWORD(IDC_BubbleSortCheckbox), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
          if (BST_CHECKED == checked)
          {
               checked = SendDlgItemMessage(hDlg, LOWORD(IDC_QuickSortCheckbox), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
               if (BST_CHECKED == checked)
               {
                    CSortTest sortTest(CSortTest::ESORT_TYPE::QUICK_SORT, vArray);
                    g_tests.push_back(sortTest);
               }

               checked = SendDlgItemMessage(hDlg, LOWORD(IDC_MergeSortCheckbox), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
               if (BST_CHECKED == checked)
               {
                    CSortTest sortTest(CSortTest::ESORT_TYPE::MERGE_SORT, vArray);
                    g_tests.push_back(sortTest);
               }

               checked = SendDlgItemMessage(hDlg, LOWORD(IDC_BubbleSortCheckbox), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
               if (BST_CHECKED == checked)
               {
                    CSortTest sortTest(CSortTest::ESORT_TYPE::BUBBLE_SORT, vArray);
                    g_tests.push_back(sortTest);
               }

               pProgressDialog = DialogBox(nullptr, MAKEINTRESOURCE(IDD_ProgressDialog), hDlg, ProgressDialogProc);

               pSortResultsDialog = DialogBox(nullptr, MAKEINTRESOURCE(IDD_SortResultsDialog), hDlg, SortResultsDialogProc);
          }
          else
          {
               CHECK_SUCCEEDED_LOG_THROW(EINVAL);
          }
     }
     catch (long& check_catch_lresult)
     {
          nStatus = check_catch_lresult;
          MessageBox(hDlg, (LoadStringFromResourceId(IDS_INVALID_DATA_ENTERED)).c_str(), (LoadStringFromResourceId(IDS_INVALID_DATA)).c_str(), (MB_ICONERROR | MB_OK));
     }

     g_tests.clear();
}

void onMainCancel(const HWND hDlg)
{
     SendMessage(hDlg, WM_CLOSE, 0, 0);
}

void onMainClose(const HWND hDlg)
{
     int msgBoxID = IDNO;

     msgBoxID = MessageBox(hDlg, (LoadStringFromResourceId(IDS_CLOSE_TEST)).c_str(), (LoadStringFromResourceId(IDS_CLOSE)).c_str(), (MB_ICONQUESTION | MB_YESNO));

     if (IDYES == msgBoxID)
     {
          DestroyWindow(hDlg);
     }
}

long GetInput(IN const HWND hDlg, OUT std::vector<int>& vArray, IN OPTIONAL bool* pbDataPresent /*= nullptr*/)
{
     long nStatus = 0;

     const int nSize = 1024;
     wchar_t szTemp[nSize] = { 0 };
     WORD wTemp = 0;

     try
     {
          wTemp = (WORD)SendDlgItemMessage(hDlg, IDC_SortArrayEdit, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
          *((LPWORD)szTemp) = wTemp;
          SendDlgItemMessage(hDlg, IDC_SortArrayEdit, EM_GETLINE, (WPARAM)0, (LPARAM)szTemp);

          std::wstring sToParse;

          if ((0 != wTemp) && (0 != szTemp[0]))
          {
               if (nullptr != pbDataPresent)
               {
                    *pbDataPresent = true;
               }
               sToParse = szTemp;
          }

          CHECK_SUCCEEDED_LOG_THROW(ParseInput(sToParse, vArray));

          CHECK_NOT_ZERO_LOG_THROW(vArray.empty());

     }
     catch (long& check_catch_lresult)
     {
          nStatus = check_catch_lresult;
     }

     return nStatus;
}

long ParseInput(IN const std::wstring& sToParse, OUT std::vector<int>& vArray)
{
     long nStatus = 0;

     std::wstring sParsed;
     std::wstring sSpace = L" ";

     try
     {
          for (auto iParse : sToParse)
          {
               std::wstring c = { iParse };

               if (sSpace.compare(c))
               {
                    int intTest = std::stoi(c); // should throw if not an integer.
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



// PROGRESS DIALOG

INT_PTR CALLBACK ProgressDialogProc(const HWND hDlg, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
     switch (uMsg)
     {
     case WM_INITDIALOG:
          onProgressInit(hDlg);
          return TRUE;
          break;

     case WM_COMMAND:
     {
          //switch (LOWORD(wParam))
         // {
         // }
          break;
     }

     case WM_ALL_THREADS_COMPLETE:
          EndDialog(hDlg, wParam);
          return TRUE;
          break;

     case WM_USER_THREAD_COMPLETE:
     {
          onProgressThreadComplete(hDlg, wParam, lParam);
          return TRUE;
          break;
     }
     } // switch

     return FALSE;
}

void onProgressInit(const HWND hDlg)
{
     SendMessage(hDlg, WM_SETTEXT, TRUE, (LPARAM)(LoadStringFromResourceId(IDS_PROGRESS_DIALOG)).c_str());
     SendDlgItemMessage(hDlg, IDC_ProgressBar, PBM_SETMARQUEE, (WPARAM)1, (LPARAM)0);
     SendDlgItemMessage(hDlg, ID_ProgressStopButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_PROGRESS_STOP)).c_str());

     {std::lock_guard<std::recursive_mutex> lock(g_mutex);
     for (auto& iTest : g_tests)
     {
          std::thread thread = std::thread(ThreadFunc, hDlg, &iTest);
          thread.detach(); // Thread will be communicated to with atomic flags. Thread will communicate with SendMessage. Will not be joined.
     }
     } // {std::lock_guard<std::recursive_mutex> lock(g_mutex);
}

void onProgressThreadComplete(const HWND hDlg, const WPARAM wParam, const LPARAM lParam)
{
     long nStatus = 0;
     bool bAllThreadsComplete = true;

     // Determine if all threads are complete
     {std::lock_guard<std::recursive_mutex> lock(g_mutex);
     for (auto& iTest : g_tests)
     {
          if (false == iTest.IsComplete())
          {
               bAllThreadsComplete = false;
               break;
          }
     }
     } //std::lock_guard<std::recursive_mutex> lock(g_mutex);

     if (true == bAllThreadsComplete)
     {
          SendMessage(hDlg, WM_ALL_THREADS_COMPLETE, (WPARAM)0, (LPARAM)0);
     }
}

void onProgressClose(const HWND hDlg, const WPARAM wParam)
{
     int msgBoxID = IDNO;

     msgBoxID = MessageBox(hDlg, (LoadStringFromResourceId(IDS_STOP_QUESTION)).c_str(), (LoadStringFromResourceId(IDS_PROGRESS_STOP)).c_str(), (MB_ICONQUESTION | MB_YESNO));

     if (IDYES == msgBoxID)
     {
          EndDialog(hDlg, wParam);
     }
}



// RESULTS DIALOG

INT_PTR CALLBACK SortResultsDialogProc(const HWND hDlg, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
     switch (uMsg)
     {
     case WM_INITDIALOG:
          onSortResultsInit(hDlg);
          return TRUE;
          break;

     case WM_COMMAND:
     {
          switch (LOWORD(wParam))
          {
          case IDCLOSE:
               onSortResultsClose(hDlg, wParam);
               return TRUE;
               break;
          } // switch
          break;
     }

     } // switch

     return FALSE;
}

void onSortResultsInit(const HWND hDlg)
{
     SendMessage(hDlg, WM_SETTEXT, TRUE, (LPARAM)(LoadStringFromResourceId(IDS_SORTING_RESULTS)).c_str());

     SendDlgItemMessage(hDlg, IDCLOSE, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_CLOSE)).c_str());

     SendDlgItemMessage(hDlg, IDC_StatusStatic, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SORT_STATUS)).c_str());
     SendDlgItemMessage(hDlg, IDC_StatusStatic2, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SORT_STATUS)).c_str());
     SendDlgItemMessage(hDlg, IDC_StatusStatic3, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SORT_STATUS)).c_str());

     SendDlgItemMessage(hDlg, IDC_NumberOfSortsStatic, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_NUMBER_OF_SORTS)).c_str());
     SendDlgItemMessage(hDlg, IDC_NumberOfSortsStatic2, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_NUMBER_OF_SORTS)).c_str());
     SendDlgItemMessage(hDlg, IDC_NumberOfSortsStatic3, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_NUMBER_OF_SORTS)).c_str());

     SendDlgItemMessage(hDlg, IDC_SortDurationStatic, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SORT_DURATION)).c_str());
     SendDlgItemMessage(hDlg, IDC_SortDurationStatic2, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SORT_DURATION)).c_str());
     SendDlgItemMessage(hDlg, IDC_SortDurationStatic3, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SORT_DURATION)).c_str());

     SendDlgItemMessage(hDlg, IDC_QuickSortGroup, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_QUICK_SORT)).c_str());
     SendDlgItemMessage(hDlg, IDC_MergeSortGroup, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_MERGE_SORT)).c_str());
     SendDlgItemMessage(hDlg, IDC_BubbleSortGroup, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_BUBBLE_SORT)).c_str());

     SendDlgItemMessage(hDlg, IDC_QickSortStatusEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SORT_NOT_RUN)).c_str());
     SendDlgItemMessage(hDlg, IDC_MergeSortStatusEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SORT_NOT_RUN)).c_str());
     SendDlgItemMessage(hDlg, IDC_BubbleSortStatusEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SORT_NOT_RUN)).c_str());

     {std::lock_guard<std::recursive_mutex> lock(g_mutex);
     for (auto& iTest : g_tests)
     {
          switch (iTest.GetSortType())
          {
          case CSortTest::ESORT_TYPE::QUICK_SORT:
          {
               SendDlgItemMessage(hDlg, IDC_QickSortStatusEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(CSortTest::ConvertSortStateToString(iTest.GetState())).c_str());
               SendDlgItemMessage(hDlg, IDC_SortedQuickSortArrayEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(CSortTest::ConvertVectorToString(iTest.GetSortedArray()).c_str()));
               SendDlgItemMessage(hDlg, IDC_QuickSortNumberOfSortsEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(std::to_wstring(iTest.GetNumberOfSorts()).c_str()));

               auto nTotalMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(iTest.GetDuration());
               SendDlgItemMessage(hDlg, IDC_QuickSortDurationEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(std::to_wstring(nTotalMicroseconds.count()).c_str()));
               SendDlgItemMessage(hDlg, IDC_QuickSortUnitsEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_MICROSECONDS)).c_str());
               break;
          }
          case CSortTest::ESORT_TYPE::MERGE_SORT:
          {
               SendDlgItemMessage(hDlg, IDC_MergeSortStatusEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(CSortTest::ConvertSortStateToString(iTest.GetState())).c_str());
               SendDlgItemMessage(hDlg, IDC_SortedMergeSortArrayEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(CSortTest::ConvertVectorToString(iTest.GetSortedArray()).c_str()));
               SendDlgItemMessage(hDlg, IDC_MergeSortNumberOfSortsEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(std::to_wstring(iTest.GetNumberOfSorts()).c_str()));

               auto nTotalMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(iTest.GetDuration());
               SendDlgItemMessage(hDlg, IDC_MergeSortDurationEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(std::to_wstring(nTotalMicroseconds.count()).c_str()));
               SendDlgItemMessage(hDlg, IDC_MergeSortUnitsEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_MICROSECONDS)).c_str());
               break;
          }
          case CSortTest::ESORT_TYPE::BUBBLE_SORT:
          {
               SendDlgItemMessage(hDlg, IDC_BubbleSortStatusEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(CSortTest::ConvertSortStateToString(iTest.GetState())).c_str());
               SendDlgItemMessage(hDlg, IDC_SortedBubbleSortArrayEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(CSortTest::ConvertVectorToString(iTest.GetSortedArray()).c_str()));
               SendDlgItemMessage(hDlg, IDC_BubbleSortNumberOfSortsEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(std::to_wstring(iTest.GetNumberOfSorts()).c_str()));

               auto nTotalMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(iTest.GetDuration());
               SendDlgItemMessage(hDlg, IDC_BubbleSortDurationEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(std::to_wstring(nTotalMicroseconds.count()).c_str()));
               SendDlgItemMessage(hDlg, IDC_BubbleSortUnitsEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_MICROSECONDS)).c_str());
               break;
          }
          default:
               break;
          }

     }
     } // {std::lock_guard<std::recursive_mutex> lock(g_mutex);
}


void onSortResultsClose(const HWND hDlg, const WPARAM wParam)
{
     int msgBoxID = IDNO;

     msgBoxID = MessageBox(hDlg, (LoadStringFromResourceId(IDS_CLOSE_TEST)).c_str(), (LoadStringFromResourceId(IDS_CLOSE)).c_str(), (MB_ICONQUESTION | MB_YESNO));

     if (IDYES == msgBoxID)
     {
          EndDialog(hDlg, wParam);
     }
}



// THREAD FUNCTION

void ThreadFunc(const HWND hDlg, CSortTest* iTest)
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
          {std::lock_guard<std::recursive_mutex> lock(g_mutex);} 

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

     {std::lock_guard<std::recursive_mutex> lock(g_mutex);
     iTest->SetIsComplete();
     } //std::lock_guard<std::recursive_mutex> lock(g_mutex);

     SendMessage(hDlg, WM_USER_THREAD_COMPLETE, (WPARAM)iTest->GetThreadId(), (LPARAM)iTest->GetState());

}
