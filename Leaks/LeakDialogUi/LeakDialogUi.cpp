#include <Windows.h>
#include <Windowsx.h>
#include <CommCtrl.h>
#include <tchar.h>
#include "resource.h"

#include <thread>
#include <atomic>
#include <vector>

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

// MAIN DIALOG
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow);
INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onInitMainDialog(const HWND hDlg);
void onMainOk(const HWND hDlg);
void onMainCancel(const HWND hDlg);
void onMainClose(const HWND hDlg);
void onCheckBox(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onGetData(const HWND hDlg, const int Id, bool& bChecked, size_t& nIterations, size_t& nBytesPerIteration);

// PROGRESS DIALOG
INT_PTR CALLBACK ProgressDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onInitProgressnDialog(const HWND hDlg);
void onProgressClose(const HWND hDlg, const WPARAM wParam);

// THREADS
std::vector<std::thread> gThreadVector;
std::atomic<bool> gbThreadContinue = true;
void NewOperatorThreadFunc(const HWND hDlg, const size_t nIterations, const size_t nBytesPerIteration);
void MallocFunctionThreadFunc(const HWND hDlg, const size_t nIterations, const size_t nBytesPerIteration);


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
          TranslateMessage(&msg);
          DispatchMessage(&msg);
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
          }
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
     }

     return FALSE;
}

void onInitMainDialog(const HWND hDlg)
{
     //SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDCANCEL, (LPARAM)IDCANCEL);
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

     onGetData(hDlg, IDC_NEW_LEAK_CHECK, bChecked, nIterations, nBytesPerIteration);

     if (true == bChecked)
     {
          if (0 != nIterations && 0 != nBytesPerIteration)
          {
               bLeakTesting = true;
               gThreadVector.push_back(std::thread(NewOperatorThreadFunc, hDlg, nIterations, nBytesPerIteration));
          }
          else
          {
               int msgBoxID = MessageBox(hDlg, TEXT("New Operator Invalid Data Entered"), TEXT("Invalid Data"), (MB_ICONERROR | MB_OK));

          }
     }

     onGetData(hDlg, IDC_MALLOC_LEAK_CHECK, bChecked, nIterations, nBytesPerIteration);

     if (true == bChecked)
     {
          if (0 != nIterations && 0 != nBytesPerIteration)
          {
               bLeakTesting = true;
               gThreadVector.push_back(std::thread(MallocFunctionThreadFunc, hDlg, nIterations, nBytesPerIteration));
          }
          else
          {
               int msgBoxID = MessageBox(hDlg, TEXT("Malloc Function Invalid Data Entered"), TEXT("Invalid Data"), (MB_ICONERROR | MB_OK));

          }
     }

     if (bLeakTesting)
     {
          INT_PTR pProgressDialog = 0;

          pProgressDialog = DialogBox(nullptr, MAKEINTRESOURCE(IDD_RUNNING_DIALOG), hDlg, ProgressDialogProc);

          gbThreadContinue = false;
     }
     else
     {
          int msgBoxID = MessageBox(hDlg, TEXT("No Tests to Run"), TEXT("Testing"), (MB_ICONERROR | MB_OK));
     }

     for (auto& thread : gThreadVector)
     {
          thread.join();
     }

     gThreadVector.clear();
}

void onMainCancel(const HWND hDlg)
{
     SendMessage(hDlg, WM_CLOSE, 0, 0);
}

void onMainClose(const HWND hDlg)
{
     int msgBoxID = IDNO;

     msgBoxID = MessageBox(hDlg, TEXT("Close Leak Test?"), TEXT("Close"), (MB_ICONQUESTION | MB_YESNO));

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
     }

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

void onGetData(const HWND hDlg, const int Id, bool& bChecked, size_t& nIterations, size_t& nBytesPerIteration)
{
     LRESULT checked = BST_INDETERMINATE;
     int IdIternations = 0;
     int IdBytesPerIterations = 0;

     wchar_t szTemp[1024] = {};
     WORD wTemp = 0;

     bChecked = false;
     nIterations = 0;
     nBytesPerIteration = 0;

     switch (Id)
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

     }

     checked = SendDlgItemMessage(hDlg, Id, BM_GETCHECK, (WPARAM)0, (LPARAM)0);

     if (BST_CHECKED == checked)
     {
          bChecked = true;

          wTemp = (WORD)SendDlgItemMessage(hDlg, IdIternations, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
          *((LPWORD)szTemp) = wTemp;
          SendDlgItemMessage(hDlg, IdIternations, EM_GETLINE, (WPARAM)0, (LPARAM)szTemp);
          nIterations = _wtoi64(szTemp);

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
          onInitProgressnDialog(hDlg);
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
          }
          break;
     }
     }

     return FALSE;
}

void onInitProgressnDialog(const HWND hDlg)
{
     SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_SETMARQUEE, (WPARAM)1, (LPARAM)0);
}


void onProgressClose(const HWND hDlg, const WPARAM wParam)
{
     int msgBoxID = IDNO;

     msgBoxID = MessageBox(hDlg, TEXT("Stop Leak Test?"), TEXT("Stop"), (MB_ICONQUESTION | MB_YESNO));

     if (IDYES == msgBoxID)
     {
          EndDialog(hDlg, wParam);
     }
}


void NewOperatorThreadFunc(const HWND hDlg, const size_t nIterations, const size_t nBytesPerIteration)
{
     DWORD dwStatus = ERROR_SUCCESS;

     for (size_t nIteration = 0; (nIteration < nIterations) && gbThreadContinue && SUCCEEDED(dwStatus); nIteration++)
     {
          dwStatus = CLeakLib::LeakNewMemory(1, nBytesPerIteration);
     }

     if (FAILED(dwStatus))
     {
          int msgBoxID = MessageBox(hDlg, TEXT("New Operator Failure"), TEXT("Memory Failure"), (MB_ICONERROR | MB_OK));
     }
}

void MallocFunctionThreadFunc(const HWND hDlg, const size_t nIterations, const size_t nBytesPerIteration)
{
     DWORD dwStatus = ERROR_SUCCESS;

     for (size_t nIteration = 0; (nIteration < nIterations) && gbThreadContinue && SUCCEEDED(dwStatus); nIteration++)
     {
          dwStatus = CLeakLib::LeakMallocMemory(1, nBytesPerIteration);
     }

     if (FAILED(dwStatus))
     {
          int msgBoxID = MessageBox(hDlg, TEXT("Malloc Function Failure"), TEXT("Memory Failure"), (MB_ICONERROR | MB_OK));
     }
}
