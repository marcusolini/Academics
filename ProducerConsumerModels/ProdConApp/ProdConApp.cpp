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


#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")


// GLOBALS


std::wstring LoadStringFromResourceId(const UINT id);


// MAIN DIALOG
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow);
INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onInitMainDialog(const HWND hDlg);
void onMainOk(const HWND hDlg);
void onMainCancel(const HWND hDlg);
void onMainClose(const HWND hDlg);


std::wstring LoadStringFromResourceId(const UINT id)
{
     const size_t nString = 2048;
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
               return TEXT("Resource String Missing");
          }
     }
}


// MAIN DIALOG

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
     HWND hDlg{};
     MSG msg{};

     InitCommonControls();
     hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_ProdConMenu), nullptr, MainDialogProc);
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
          case IDCANCEL:
               onMainCancel(hDlg);
               return TRUE;
               break;
          case IDOK:
               
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


void onMainCancel(const HWND hDlg)
{
     SendMessage(hDlg, WM_CLOSE, 0, 0);
}

void onMainClose(const HWND hDlg)
{
     int msgBoxID = IDNO;

     msgBoxID = MessageBox(hDlg, (LoadStringFromResourceId(IDS_CLOSE)).c_str(), (LoadStringFromResourceId(IDS_CLOSE)).c_str(), (MB_ICONQUESTION | MB_YESNO));

     if (IDYES == msgBoxID)
     {
          DestroyWindow(hDlg);
     }
}


