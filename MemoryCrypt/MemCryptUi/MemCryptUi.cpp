//Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.


#include <Windows.h>
#include <Windowsx.h>
#include <CommCtrl.h>
#include <tchar.h>
#include <string>
#include "resource.h"

#include "../inc/MemoryCrypt.h"
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
std::wstring sData;

// MAIN DIALOG
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow);
INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onInitMainDialog(const HWND hDlg);
void onMainEncrypt(const HWND hDlg);
void onMainCancel(const HWND hDlg);
void onMainClose(const HWND hDlg);


std::wstring LoadStringFromResourceId(const UINT id)
{
     const size_t nString = 2048;
     wchar_t szString[nString] = {};

     std::wstring sString;

     if (LoadString(GetModuleHandle(nullptr), id, szString, nString - 1))
     {
          sString = szString;
     }
     else
     {
          if (LoadString(GetModuleHandle(nullptr), IDS_RESOURCE_STRING_MISSING, szString, nString - 1))
          {
               sString = szString;
          }
          else
          {
               sString = TEXT("Resource String Missing");
          }
     }

     return sString;
}


// MAIN DIALOG

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
     HWND hDlg{};
     MSG msg{};

     InitCommonControls();
     hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MemEncryptDialog), nullptr, MainDialogProc);
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
          case IDC_EncryptBtn:
               onMainEncrypt(hDlg);
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

     SendMessage(hDlg, WM_SETTEXT, TRUE, (LPARAM)(LoadStringFromResourceId(IDS_MEMORY_ENCRYPTION_DIALOG)).c_str());
     SendDlgItemMessage(hDlg, IDC_EncryptBtn, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_ENCRYPT)).c_str());
     SendDlgItemMessage(hDlg, IDCANCEL, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_CLOSE)).c_str());
}

void onMainEncrypt(const HWND hDlg)
{
     HWND hTemp{};

     std::wstring sTemp;
     wchar_t szTemp[1024] = {};
     WORD wTemp = 0;

     GetDlgItemText(hDlg, IDC_EncryptBtn, szTemp, 1024);
     sTemp = szTemp ? szTemp:TEXT("");

     if (0 == sTemp.compare((LoadStringFromResourceId(IDS_ENCRYPT).c_str())))
     {
          sTemp.clear();
          memset(szTemp, 0, 1024);
          wTemp = 0;

          GetDlgItemText(hDlg, IDC_DataEdit, szTemp, 1024);
          sData = szTemp ? szTemp : TEXT("");

          if (0 == sData.size())
          {
               MessageBox(hDlg, (LoadStringFromResourceId(IDS_ENTER_VALID_DATA)).c_str(), (LoadStringFromResourceId(IDS_INVALID_DATA)).c_str(), (MB_ICONEXCLAMATION | MB_OK));
          }
          else
          {
               HRESULT hrResult = ERROR_SUCCESS;

               try
               {
                    CHECK_SUCCEEDED_LOG_THROW(MemoryCrypt::Encrypt(sData));

                    SendDlgItemMessage(hDlg, IDC_EncryptBtn, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_DECRYPT)).c_str());
                    SendDlgItemMessage(hDlg, IDC_DataEdit, WM_SETTEXT, (WPARAM)0, (LPARAM)(sData.c_str()));

                    hTemp = GetDlgItem(hDlg, IDC_DataEdit);
                    Edit_Enable(hTemp, FALSE);
               }
               catch (HRESULT& check_catch_hresult)
               {
                    hrResult = check_catch_hresult;
                    MessageBox(hDlg, (LoadStringFromResourceId(IDS_UNABLE_TO_ENCRYPT)).c_str(), (LoadStringFromResourceId(IDS_ENCRYPTION_FAILURE)).c_str(), (MB_ICONEXCLAMATION | MB_OK));
               }
          }
     }
     else
     {
          HRESULT hrResult = ERROR_SUCCESS;

          try
          {
               CHECK_SUCCEEDED_LOG_THROW(MemoryCrypt::Decrypt(sData));
          }
          catch (HRESULT& check_catch_hresult)
          {
               hrResult = check_catch_hresult;
               MessageBox(hDlg, (LoadStringFromResourceId(IDS_UNABLE_TO_DECRYPT)).c_str(), (LoadStringFromResourceId(IDS_ENCRYPTION_FAILURE)).c_str(), (MB_ICONEXCLAMATION | MB_OK));
          }

          SendDlgItemMessage(hDlg, IDC_EncryptBtn, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_ENCRYPT)).c_str());
          //SendDlgItemMessage(hDlg, IDC_DataEdit, WM_SETTEXT, (WPARAM)0, (LPARAM)(sData.c_str()));

          hTemp = GetDlgItem(hDlg, IDC_DataEdit);
          Edit_Enable(hTemp, TRUE);
     }
}

void onMainCancel(const HWND hDlg)
{
     SendMessage(hDlg, WM_CLOSE, 0, 0);
}

void onMainClose(const HWND hDlg)
{
     int msgBoxID = IDNO;

     msgBoxID = MessageBox(hDlg, (LoadStringFromResourceId(IDS_CLOSE_QUESTION)).c_str(), (LoadStringFromResourceId(IDS_CLOSE)).c_str(), (MB_ICONQUESTION | MB_YESNO));

     if (IDYES == msgBoxID)
     {
          DestroyWindow(hDlg);
     }
}

