// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#include "SortingTestsUi.h"
#include "../../Error_Checks/ERROR_CHECKS.H"

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")

// GLOBALS
std::vector<CSortTest> gTests;
/*static*/ std::recursive_mutex CSortTest::g_mutex;


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
          onInitMainDialog(hDlg);
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
                    onChange(hDlg, uMsg, wParam, lParam);
                    return TRUE;
                    break;
               } // switch
               return TRUE;
               break;
          case IDC_SortArrayEdit:
               switch (HIWORD(wParam))
               {
               case EN_CHANGE:
                    onChange(hDlg, uMsg, wParam, lParam);
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

void onInitMainDialog(const HWND hDlg)
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

void onChange(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

     std::vector<int> vArray;

     try
     {
          CHECK_SUCCEEDED_LOG_THROW( GetInput(hDlg, vArray) );

          // Verify checkboxes

          // Create vector of tests

          CSortTest sortTest(CSortTest::ESORT_TYPE::QUICK_SORT, vArray);
          gTests.push_back(sortTest);

          pProgressDialog = DialogBox(nullptr, MAKEINTRESOURCE(IDD_ProgressDialog), hDlg, ProgressDialogProc);
     }
     catch (long& check_catch_lresult)
     {
          nStatus = check_catch_lresult;
          MessageBox(hDlg, (LoadStringFromResourceId(IDS_INVALID_DATA_ENTERED)).c_str(), (LoadStringFromResourceId(IDS_INVALID_DATA)).c_str(), (MB_ICONERROR | MB_OK));
     }
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
          switch (LOWORD(wParam))
          {
          case ID_ProgressStopButton:
               onProgressClose(hDlg, wParam);
               return TRUE;
               break;
          }
          break;
     }
/*     case WM_USER_THREAD_STATE:
     {
          onProgressThreadState(hDlg, wParam, lParam);
          return TRUE;
          break;
     }
     case WM_USER_THREAD_COMPLETE:
     {
          onProgressThreadComplete(hDlg, wParam, lParam);
          return TRUE;
          break;
     }
*/
     } // switch

     return FALSE;
}

void onProgressInit(const HWND hDlg)
{
     SendMessage(hDlg, WM_SETTEXT, TRUE, (LPARAM)(LoadStringFromResourceId(IDS_PROGRESS_DIALOG)).c_str());
     SendDlgItemMessage(hDlg, IDC_ProgressBar, PBM_SETMARQUEE, (WPARAM)1, (LPARAM)0);
     SendDlgItemMessage(hDlg, ID_ProgressStopButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_PROGRESS_STOP)).c_str());

     for (auto& iTest : gTests)
     {
          std::thread thread = std::thread(ThreadFunc, hDlg, &iTest);

          std::wstringstream ss;
          ss << thread.get_id();
          uint64_t id = std::stoull(ss.str());

          thread.detach(); // Thread will be communicated to with atomic flags. Thread will communicate with SendMessage. Will not be joined.
     }
}



void onProgressClose(const HWND hDlg, const WPARAM wParam)
{
     int msgBoxID = IDNO;

     msgBoxID = MessageBox(hDlg, (LoadStringFromResourceId(IDS_STOP_QUESTION)).c_str(), (LoadStringFromResourceId(IDS_PROGRESS_STOP)).c_str(), (MB_ICONQUESTION | MB_YESNO));

     if (IDYES == msgBoxID)
     {
          std::this_thread::sleep_for(std::chrono::milliseconds{ 250 });
          EndDialog(hDlg, wParam);
     }
}


void ThreadFunc(const HWND hDlg, CSortTest* iTest)
{
     int nStatus = 0;
     std::wstringstream ss;
     uint64_t id = 0;

     iTest->SetState(CSortTest::ESTATE_TYPE::STARTED);

     ss << std::this_thread::get_id();
     id = std::stoull(ss.str());

     iTest->SetThreadId(id);

     std::this_thread::yield();

     iTest->SetState(CSortTest::ESTATE_TYPE::RUNNING);
     
     // Convet vector to array.
     // Call Sort

     // Check Status
     //if (false == pProgressDialog->m_bThreadContinue)
     {
          iTest->SetState(CSortTest::ESTATE_TYPE::INTERRUPTED);
     }
     //else
     {
          if (0 == nStatus)
          {
               iTest->SetState(CSortTest::ESTATE_TYPE::SUCCESS);
          }
          else
          {
               iTest->SetState(CSortTest::ESTATE_TYPE::FAILED);
          }
     }

     iTest->SetIsComplete();
}
