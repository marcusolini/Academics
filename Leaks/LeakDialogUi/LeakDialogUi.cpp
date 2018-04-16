//Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.


#include <Windows.h>
#include <Windowsx.h>
#include <CommCtrl.h>
#include <tchar.h>
#include "resource.h"

#include <thread>
#include <atomic>
#include <vector>
#include <sstream>

#include "LeakLib.h"
#pragma comment(lib, "LeakLib.lib")


#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")


#pragma comment(lib, "ComCtl32.lib")


#define WM_USER_THREAD_COMPLETE (WM_USER + 100)
#define WM_USER_THREAD_STATE (WM_USER + 101)


enum class ERESOURCE_ALLOCATION_TYPES : int;
enum class ERESOURCE_ALLOCATION_COMPLETED_STATE : int;


class ResourceLeakTest;
std::vector<ResourceLeakTest> gTests;

// MAIN DIALOG
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow);
INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onInitMainDialog(const HWND hDlg);
void onMainOk(const HWND hDlg);
void onMainCancel(const HWND hDlg);
void onMainClose(const HWND hDlg);
void onCheckBox(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onGetData(const HWND hDlg, const ERESOURCE_ALLOCATION_TYPES eType, bool& bChecked, size_t& nIterations, size_t& nBytesPerIteration);

// PROGRESS DIALOG
INT_PTR CALLBACK ProgressDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onProgressInit(const HWND hDlg);
void onProgressPauseContinue(const HWND hDlg);
void onProgressThreadState(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onProgressThreadComplete(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onProgressClose(const HWND hDlg, const WPARAM wParam);

// THREADS
std::atomic<bool> gbThreadContinue = true;
std::atomic<bool> gbThreadPause = false;
void AllocationThreadFunc(const HWND hDlg, const ERESOURCE_ALLOCATION_TYPES eAllocationType, const size_t nIterations, const size_t nBytesPerIteration);


enum class ERESOURCE_ALLOCATION_TYPES : int
{
     UNDEFINED = 0,
     NEW_OPERATOR = IDC_NEW_LEAK_CHECK,
     MALLOC_FUNCTION = IDC_MALLOC_LEAK_CHECK,
     CALLOC_FUNCTION = IDC_CALLOC_LEAK_CHECK,
     HANDLE_FUNCTION = IDC_HANDLE_LEAK_CHECK,
};

enum class ERESOURCE_ALLOCATION_COMPLETED_STATE : int
{
     UNDEFINED = 0,
     STARTED,
     RUNNING,
     PAUSED,
     INTERRUPTED,
     SUCCESS,
     FAILED
};

class ResourceLeakTest
{
public:
     ResourceLeakTest(const ERESOURCE_ALLOCATION_TYPES eResourceAllocationType, const size_t nIterations, const size_t nBytesPerIteration)
          : m_eResourceAllocationType(eResourceAllocationType), m_nIterations(nIterations), m_nBytesPerIteration(nBytesPerIteration){}
     virtual ~ResourceLeakTest() {};

     ERESOURCE_ALLOCATION_TYPES GetResourceAllocationType() { return m_eResourceAllocationType; };
     size_t GetInterations() { return m_nIterations; }
     size_t GetBytesPerIteration() { return m_nBytesPerIteration; }
     void SetIsComplete() { m_bComplete = true; }
     bool IsComplete() { return m_bComplete; }
     void SetState(const ERESOURCE_ALLOCATION_COMPLETED_STATE state) { m_state = state; }
     ERESOURCE_ALLOCATION_COMPLETED_STATE GetState() { return m_state; }
     void SetThreadId(uint64_t id) { m_threadId = id; }
     uint64_t GetThreadId() { return m_threadId; }
private:
     ERESOURCE_ALLOCATION_TYPES m_eResourceAllocationType = ERESOURCE_ALLOCATION_TYPES::UNDEFINED;
     size_t m_nIterations = 0;
     size_t m_nBytesPerIteration = 0;
     bool m_bComplete = false;
     ERESOURCE_ALLOCATION_COMPLETED_STATE m_state = ERESOURCE_ALLOCATION_COMPLETED_STATE::UNDEFINED;
     uint64_t m_threadId = 0;
};


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
     HWND hDlg{};
     MSG msg{};

     InitCommonControls();
     hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_LEAK_TEST_DIALOG), nullptr, MainDialogProc);
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
          case IDC_NEW_LEAK_CHECK:
          case IDC_MALLOC_LEAK_CHECK:
          case IDC_CALLOC_LEAK_CHECK:
          case IDC_HANDLE_LEAK_CHECK:
          {
               onCheckBox(hDlg, wParam, lParam);
               return TRUE;
               break;
          }

          case IDCANCEL:
               onMainCancel(hDlg);
               return TRUE;
               break;
          case IDOK:
               onMainOk(hDlg);
               return TRUE;
               break;

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
     SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDCANCEL, (LPARAM)IDCANCEL);
}

void onMainOk(const HWND hDlg)
{
     LRESULT checked = BST_INDETERMINATE;

     wchar_t szTemp[1024] = {};
     WORD wTemp = 0;

     gbThreadContinue = true;

     bool bChecked = false;
     size_t nIterations = 0;
     size_t nBytesPerIteration = 0;

     bool bLeakTesting = false;

     onGetData(hDlg, ERESOURCE_ALLOCATION_TYPES::NEW_OPERATOR, bChecked, nIterations, nBytesPerIteration);

     if (true == bChecked)
     {
          if (0 != nIterations && 0 != nBytesPerIteration)
          {
               bLeakTesting = true;

               ResourceLeakTest resourceLeakTest(ERESOURCE_ALLOCATION_TYPES::NEW_OPERATOR, nIterations, nBytesPerIteration);
               gTests.push_back(resourceLeakTest);
          }
          else
          {
               int msgBoxID = MessageBox(hDlg, TEXT("New Operator Invalid Data Entered"), TEXT("Invalid Data"), (MB_ICONERROR | MB_OK));
          }
     }

     onGetData(hDlg, ERESOURCE_ALLOCATION_TYPES::MALLOC_FUNCTION, bChecked, nIterations, nBytesPerIteration);

     if (true == bChecked)
     {
          if (0 != nIterations && 0 != nBytesPerIteration)
          {
               bLeakTesting = true;

               ResourceLeakTest resourceLeakTest(ERESOURCE_ALLOCATION_TYPES::MALLOC_FUNCTION, nIterations, nBytesPerIteration);
               gTests.push_back(resourceLeakTest);
          }
          else
          {
               int msgBoxID = MessageBox(hDlg, TEXT("Malloc Function Invalid Data Entered"), TEXT("Invalid Data"), (MB_ICONERROR | MB_OK));
          }
     }

     onGetData(hDlg, ERESOURCE_ALLOCATION_TYPES::CALLOC_FUNCTION, bChecked, nIterations, nBytesPerIteration);

     if (true == bChecked)
     {
          if (0 != nIterations && 0 != nBytesPerIteration)
          {
               bLeakTesting = true;

               ResourceLeakTest resourceLeakTest(ERESOURCE_ALLOCATION_TYPES::CALLOC_FUNCTION, nIterations, nBytesPerIteration);
               gTests.push_back(resourceLeakTest);
          }
          else
          {
               int msgBoxID = MessageBox(hDlg, TEXT("Calloc Function Invalid Data Entered"), TEXT("Invalid Data"), (MB_ICONERROR | MB_OK));
          }

     }

     onGetData(hDlg, ERESOURCE_ALLOCATION_TYPES::HANDLE_FUNCTION, bChecked, nIterations, nBytesPerIteration);

     if (true == bChecked)
     {
          if (0 != nIterations )
          {
               bLeakTesting = true;

               ResourceLeakTest resourceLeakTest(ERESOURCE_ALLOCATION_TYPES::HANDLE_FUNCTION, nIterations, nBytesPerIteration);
               gTests.push_back(resourceLeakTest);
          }
          else
          {
               int msgBoxID = MessageBox(hDlg, TEXT("Handle Function Invalid Data Entered"), TEXT("Invalid Data"), (MB_ICONERROR | MB_OK));

          }
     }

     if (bLeakTesting)
     {
          INT_PTR pProgressDialog = 0;

          pProgressDialog = DialogBox(nullptr, MAKEINTRESOURCE(IDD_RUNNING_DIALOG), hDlg, ProgressDialogProc);
     }
     else
     {
          int msgBoxID = MessageBox(hDlg, TEXT("No Tests to Run"), TEXT("Testing"), (MB_ICONERROR | MB_OK));
     }

     gTests.clear();

}

void onMainCancel(const HWND hDlg)
{
     SendMessage(hDlg, WM_CLOSE, 0, 0);
}

void onMainClose(const HWND hDlg)
{
     int msgBoxID = IDNO;

     msgBoxID = MessageBox(hDlg, TEXT("Close Resource Leak Test?"), TEXT("Close"), (MB_ICONQUESTION | MB_YESNO));

     if (IDYES == msgBoxID)
     {
          DestroyWindow(hDlg);
     }
}

void onCheckBox(const HWND hDlg, const WPARAM wParam, const LPARAM lParam)
{
     HWND hCheck{};
     int IdIternations = 0;
     int IdBytesPerIterations = 0;
     LRESULT checked = 0;

     switch (LOWORD(wParam))
     {
     case IDC_NEW_LEAK_CHECK:
     {
          IdIternations = IDC_NEW_LEAK_ITERATIONS;
          IdBytesPerIterations = IDC_NEW_LEAK_BYTES_PER_INTERATION;
          break;
     }
     case IDC_MALLOC_LEAK_CHECK:
     {
          IdIternations = IDC_MALLOC_LEAK_ITERATIONS;
          IdBytesPerIterations = IDC_MALLOC_LEAK_BYTES_PER_ITERATIONS;
          break;
     }
     case IDC_CALLOC_LEAK_CHECK:
     {
          IdIternations = IDC_CALLOC_LEAK_ITERATIONS;
          IdBytesPerIterations = IDC_CALLOC_LEAK_BYTES_PER_ITERATIONS;
          break;
     }
     case IDC_HANDLE_LEAK_CHECK:
     {
          IdIternations = IDC_HANDLE_LEAK_ITERATIONS;
          IdBytesPerIterations = IDC_HANDLE_LEAK_BYTES_PER_ITERATIONS;
          break;
     }
     } // switch

     checked = SendDlgItemMessage(hDlg, LOWORD(wParam), BM_GETCHECK, (WPARAM)0, (LPARAM)0);
     if (BST_CHECKED == checked)
     {
          hCheck = GetDlgItem(hDlg, IdIternations);
          Edit_Enable(hCheck, TRUE);

          hCheck = GetDlgItem(hDlg, IdBytesPerIterations);
          Edit_Enable(hCheck, TRUE);
     }
     else
     {
          hCheck = GetDlgItem(hDlg, IdIternations);
          Edit_Enable(hCheck, FALSE);

          hCheck = GetDlgItem(hDlg, IdBytesPerIterations);
          Edit_Enable(hCheck, FALSE);
     }
}

void onGetData(const HWND hDlg, const ERESOURCE_ALLOCATION_TYPES eType, bool& bChecked, size_t& nIterations, size_t& nBytesPerIteration)
{
     LRESULT checked = BST_INDETERMINATE;
     int IdIternations = 0;
     int IdBytesPerIterations = 0;

     const int nSize = 1024;
     wchar_t szTemp[nSize] = {};
     WORD wTemp = 0;

     bChecked = false;
     nIterations = 0;
     nBytesPerIteration = 0;

     switch (eType)
     {
     case ERESOURCE_ALLOCATION_TYPES::NEW_OPERATOR:
     {
          IdIternations = IDC_NEW_LEAK_ITERATIONS;
          IdBytesPerIterations = IDC_NEW_LEAK_BYTES_PER_INTERATION;
          break;
     }
     case ERESOURCE_ALLOCATION_TYPES::MALLOC_FUNCTION:
     {
          IdIternations = IDC_MALLOC_LEAK_ITERATIONS;
          IdBytesPerIterations = IDC_MALLOC_LEAK_BYTES_PER_ITERATIONS;
          break;
     }
     case ERESOURCE_ALLOCATION_TYPES::CALLOC_FUNCTION:
     {
          IdIternations = IDC_CALLOC_LEAK_ITERATIONS;
          IdBytesPerIterations = IDC_CALLOC_LEAK_BYTES_PER_ITERATIONS;
          break;
     }
     case ERESOURCE_ALLOCATION_TYPES::HANDLE_FUNCTION:
     {
          IdIternations = IDC_HANDLE_LEAK_ITERATIONS;
          IdBytesPerIterations = IDC_HANDLE_LEAK_BYTES_PER_ITERATIONS;
          break;
     }
     } // switch

     checked = SendDlgItemMessage(hDlg, (int)eType, BM_GETCHECK, (WPARAM)0, (LPARAM)0);

     if (BST_CHECKED == checked)
     {
          bChecked = true;

          wTemp = (WORD)SendDlgItemMessage(hDlg, IdIternations, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
          *((LPWORD)szTemp) = wTemp;
          SendDlgItemMessage(hDlg, IdIternations, EM_GETLINE, (WPARAM)0, (LPARAM)szTemp);
          nIterations = _wtoi64(szTemp);

          std::memset(szTemp, 0, nSize);
          wTemp = 0;

          wTemp = (WORD)SendDlgItemMessage(hDlg, IdBytesPerIterations, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
          *((LPWORD)szTemp) = wTemp;
          SendDlgItemMessage(hDlg, IdBytesPerIterations, EM_GETLINE, (WPARAM)0, (LPARAM)szTemp);
          nBytesPerIteration = _wtoi64(szTemp);
     }
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
          case IDCANCEL:
               onProgressClose(hDlg, wParam);
               return TRUE;
               break;
          case IDC_PAUSE:
               onProgressPauseContinue(hDlg);
               return TRUE;
          }
          break;
     }
     case WM_USER_THREAD_STATE:
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

     } // switch

     return FALSE;
}

void onProgressInit(const HWND hDlg)
{
     SendDlgItemMessage(hDlg, IDC_PROGRESS, PBM_SETMARQUEE, (WPARAM)1, (LPARAM)0);
     SendDlgItemMessage(hDlg, IDC_PROGRESS_TEXT, WM_SETTEXT, (WPARAM)0, (LPARAM)L"Tests Running...");

     //SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDC_PAUSE, (LPARAM)IDC_PAUSE);

     for (auto& test : gTests)
     {
          std::thread thread = std::thread(AllocationThreadFunc, hDlg, test.GetResourceAllocationType(), test.GetInterations(), test.GetBytesPerIteration());

          std::wstringstream ss;
          ss << thread.get_id();
          uint64_t id = std::stoull(ss.str());

          test.SetThreadId(id);
          test.SetState( ERESOURCE_ALLOCATION_COMPLETED_STATE::STARTED);

          thread.detach(); // Thread will be communicated to with atomic flags. Thread will communicate with SendMessage. Will not be joined.
     }
}

void onProgressPauseContinue(const HWND hDlg)
{
     std::wstring sPauseState;
     wchar_t szTemp[1024] = {};
     WORD wTemp = 0;

     GetDlgItemText(hDlg, IDC_PAUSE, szTemp, 1024);

     sPauseState = szTemp;

     if (0 == sPauseState.compare(L"Pause"))
     {
          gbThreadPause = true;
          SendDlgItemMessage(hDlg, IDC_PROGRESS_TEXT, WM_SETTEXT, (WPARAM)0, (LPARAM)L"Tests Paused");
          SendDlgItemMessage(hDlg, IDC_PAUSE, WM_SETTEXT, (WPARAM)0, (LPARAM)L"Continue");
          SendDlgItemMessage(hDlg, IDC_PROGRESS, PBM_SETSTATE, (WPARAM)PBST_PAUSED, (LPARAM)0);

     }
     else
     {
          gbThreadPause = false;
          SendDlgItemMessage(hDlg, IDC_PAUSE, WM_SETTEXT, (WPARAM)0, (LPARAM)L"Pause");
          SendDlgItemMessage(hDlg, IDC_PROGRESS_TEXT, WM_SETTEXT, (WPARAM)0, (LPARAM)L"Tests Running...");
          SendDlgItemMessage(hDlg, IDC_PROGRESS, PBM_SETSTATE, (WPARAM)PBST_NORMAL, (LPARAM)0);
     }
}

void onProgressThreadState(const HWND hDlg, const WPARAM wParam, const LPARAM lParam)
{
     uint64_t threadId = (uint64_t)wParam;
     ERESOURCE_ALLOCATION_COMPLETED_STATE state = (ERESOURCE_ALLOCATION_COMPLETED_STATE)lParam;

     for (auto& test : gTests)
     {
          if (test.GetThreadId() == threadId)
          {
               test.SetState(state);
          }
     }
}

void onProgressThreadComplete(const HWND hDlg, const WPARAM wParam, const LPARAM lParam)
{
     uint64_t threadId = (uint64_t)wParam;
     ERESOURCE_ALLOCATION_COMPLETED_STATE state = (ERESOURCE_ALLOCATION_COMPLETED_STATE)lParam;
     bool bAllThreadComplete = true;

     for (auto& test : gTests)
     {
          if (test.GetThreadId() == threadId)
          {
               test.SetIsComplete();
               test.SetState(state);
          }
     }

     // Determine if all threads are complete
     for (auto& test : gTests)
     {
          if ( false == test.IsComplete())
          {
               bAllThreadComplete = false;
               break;
          }
     }

     if (true == bAllThreadComplete)
     {
          SendDlgItemMessage(hDlg, IDC_PROGRESS, PBM_SETSTATE, (WPARAM)PBST_PAUSED, (LPARAM)0);
          MessageBox(hDlg, TEXT("All Tests Complete"), TEXT("Complete"), (MB_ICONINFORMATION | MB_OK));
          EndDialog(hDlg, wParam);
     }
}


void onProgressClose(const HWND hDlg, const WPARAM wParam)
{
     int msgBoxID = IDNO;

     msgBoxID = MessageBox(hDlg, TEXT("Stop Leak Test?"), TEXT("Stop"), (MB_ICONQUESTION | MB_YESNO));

     if (IDYES == msgBoxID)
     {
          gbThreadContinue = false;
          std::this_thread::sleep_for(std::chrono::milliseconds{ 250 });
          EndDialog(hDlg, wParam);
     }
}


void AllocationThreadFunc(const HWND hDlg, const ERESOURCE_ALLOCATION_TYPES eAllocationType, const size_t nIterations, const size_t nBytesPerIteration)
{
     DWORD dwStatus = ERROR_SUCCESS;
     ERESOURCE_ALLOCATION_COMPLETED_STATE state = ERESOURCE_ALLOCATION_COMPLETED_STATE::UNDEFINED;
     std::wstringstream ss;
     uint64_t id = 0;

     ss << std::this_thread::get_id();
     id = std::stoull(ss.str());

     state = ERESOURCE_ALLOCATION_COMPLETED_STATE::RUNNING;

     SendMessage(hDlg, WM_USER_THREAD_STATE, (WPARAM)id, (LPARAM)state);

     std::this_thread::yield();

     for (size_t nIteration = 0; (nIteration < nIterations) && gbThreadContinue && SUCCEEDED(dwStatus); nIteration++)
     {
          state = ERESOURCE_ALLOCATION_COMPLETED_STATE::RUNNING;

          switch (eAllocationType)
          {
          case ERESOURCE_ALLOCATION_TYPES::NEW_OPERATOR:
               dwStatus = CLeakLib::LeakNewMemory(1, nBytesPerIteration);
               if (FAILED(dwStatus)) MessageBox(hDlg, TEXT("New Operator Allocation Failure Occurred"), TEXT("Allocation Failure"), (MB_ICONERROR | MB_OK));
               break;
          case ERESOURCE_ALLOCATION_TYPES::MALLOC_FUNCTION:
               dwStatus = CLeakLib::LeakMallocMemory(1, nBytesPerIteration);
               if (FAILED(dwStatus)) MessageBox(hDlg, TEXT("Malloc Function Allocation Failure Occurred"), TEXT("Allocation Failure"), (MB_ICONERROR | MB_OK));
               break;
          case ERESOURCE_ALLOCATION_TYPES::CALLOC_FUNCTION:
               dwStatus = CLeakLib::LeakCallocMemory(1, nBytesPerIteration);
               if (FAILED(dwStatus)) MessageBox(hDlg, TEXT("Calloc Function Allocation Failure Occurred"), TEXT("Allocation Failure"), (MB_ICONERROR | MB_OK));
               break;
          case ERESOURCE_ALLOCATION_TYPES::HANDLE_FUNCTION:
               dwStatus = CLeakLib::LeakHandle(1);
               if (FAILED(dwStatus)) MessageBox(hDlg, TEXT("Handle Function Allocation Failure Occurred"), TEXT("Allocation Failure"), (MB_ICONERROR | MB_OK));
               break;
          default:
               MessageBox(hDlg, TEXT("Invalid Allocation Type"), TEXT("Invalid Entry"), (MB_ICONERROR | MB_OK));
               dwStatus = E_INVALIDARG;
               break;
          }

          while (gbThreadPause && gbThreadContinue)
          {
               SendMessage(hDlg, WM_USER_THREAD_STATE, (WPARAM)id, (LPARAM)ERESOURCE_ALLOCATION_COMPLETED_STATE::PAUSED);
               std::this_thread::sleep_for(std::chrono::milliseconds{ 300 });
               SendMessage(hDlg, WM_USER_THREAD_STATE, (WPARAM)id, (LPARAM)ERESOURCE_ALLOCATION_COMPLETED_STATE::RUNNING);
          }
     }

     if (SUCCEEDED(dwStatus))
     {
          state = ERESOURCE_ALLOCATION_COMPLETED_STATE::SUCCESS;
     }
     else
     {
          state = ERESOURCE_ALLOCATION_COMPLETED_STATE::FAILED;
     }

     if (false == gbThreadContinue)
     {
          state = ERESOURCE_ALLOCATION_COMPLETED_STATE::INTERRUPTED;
     }


     SendMessage(hDlg, WM_USER_THREAD_COMPLETE, (WPARAM)id, (LPARAM)state);
}