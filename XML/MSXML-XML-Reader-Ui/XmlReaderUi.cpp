// Copyright 2018 marcusolini@outlook.com
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

#include <Shobjidl.h>

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
void onMainInitDialog(const HWND hDlg);
void onMainOpenFile(const HWND hDlg);
void onMainLoadXML(const HWND hDlg);
void onMainOpenXML(const HWND hDlg);
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
     hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_XmlReaderMenu), nullptr, MainDialogProc);
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
          case IDOK:
               return TRUE;
               break;

          case IDC_OpenFileButton:
               onMainOpenFile(hDlg);
               return TRUE;
               break;

          case IDC_LoadXmlButton:
               onMainLoadXML(hDlg);
               return TRUE;
               break;

          case IDC_OpenXmlButton:
               onMainOpenXML(hDlg);
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

void onMainInitDialog(const HWND hDlg)
{
     SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDCANCEL, (LPARAM)IDCANCEL);

     SendDlgItemMessage(hDlg, IDD_XmlReaderMenu, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_XML_MENU)).c_str());
     SendDlgItemMessage(hDlg, IDC_XmlFilenameStatic, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_XML_FILE_NAME)).c_str());
     SendDlgItemMessage(hDlg, IDC_LoadXmlButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_LOAD_XML)).c_str());
     SendDlgItemMessage(hDlg, IDC_OpenXmlButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_OPEN_XML)).c_str());
     SendDlgItemMessage(hDlg, IDC_Status, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_STATUS)).c_str());
     SendDlgItemMessage(hDlg, IDC_StatusText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_STATUS_SELECT_XML)).c_str());
}


void onMainOpenFile(const HWND hDlg)
{
     OPENFILENAME ofn = { 0 };
     wchar_t szFileName[MAX_PATH] = { 0 };

     HWND hLoadXmlButton{};

     ofn.lStructSize = sizeof(OPENFILENAME);
     ofn.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER;
     ofn.lpstrFilter = TEXT("XML Files (*.xml)\0*.xml\0");
     ofn.lpstrFile = szFileName;
     ofn.nMaxFile = MAX_PATH;

     ofn.hwndOwner = hDlg;

     if (GetOpenFileName(&ofn))
     {
          SendDlgItemMessage(hDlg, IDC_XmlFilename, WM_SETTEXT, (WPARAM)0, (LPARAM)(szFileName));

          hLoadXmlButton = GetDlgItem(hDlg, IDC_LoadXmlButton);
          Edit_Enable(hLoadXmlButton, TRUE);

          SendDlgItemMessage(hDlg, IDC_StatusText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_STATUS_XML_SELECTED)).c_str());
     }
}



void onMainLoadXML(const HWND hDlg)
{
     HWND hXMLFilename{};
     HWND hOpenFileButton{};
     HWND hOpenXmlButton{};

     std::wstring sLoadedState;
     wchar_t szTemp[1024] = {};
     WORD wTemp = 0;

     GetDlgItemText(hDlg, IDC_LoadXmlButton, szTemp, 1024);

     sLoadedState = szTemp;

     if (0 == sLoadedState.compare((LoadStringFromResourceId(IDS_UNLOAD_XML).c_str())))
     {
          SendDlgItemMessage(hDlg, IDC_LoadXmlButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_LOAD_XML)).c_str());
          SendDlgItemMessage(hDlg, IDC_StatusText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_STATUS_XML_SELECTED)).c_str());

          hOpenFileButton = GetDlgItem(hDlg, IDC_OpenFileButton);
          Edit_Enable(hOpenFileButton, TRUE);

          hXMLFilename = GetDlgItem(hDlg, IDC_XmlFilename);
          Edit_Enable(hXMLFilename, TRUE);

          hOpenXmlButton = GetDlgItem(hDlg, IDC_OpenXmlButton);
          Edit_Enable(hOpenXmlButton, FALSE);
     }
     else
     {
          SendDlgItemMessage(hDlg, IDC_LoadXmlButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_UNLOAD_XML)).c_str());
          SendDlgItemMessage(hDlg, IDC_StatusText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_OPEN_XML)).c_str());

          hOpenFileButton = GetDlgItem(hDlg, IDC_OpenFileButton);
          Edit_Enable(hOpenFileButton, FALSE);

          hXMLFilename = GetDlgItem(hDlg, IDC_XmlFilename);
          Edit_Enable(hXMLFilename, FALSE);

          hOpenXmlButton = GetDlgItem(hDlg, IDC_OpenXmlButton);
          Edit_Enable(hOpenXmlButton, TRUE);

          SendDlgItemMessage(hDlg, IDC_StatusText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_STATUS_XML_LOADED)).c_str());
     }

}



void onMainOpenXML(const HWND hDlg)
{
     HWND hOpenXmlButton{};
     hOpenXmlButton = GetDlgItem(hDlg, IDC_OpenXmlButton);
     Edit_Enable(hOpenXmlButton, FALSE);

     SendDlgItemMessage(hDlg, IDC_LoadXmlButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_UNLOAD_XML)).c_str());
     SendDlgItemMessage(hDlg, IDC_StatusText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_STATUS_XML_OPENNED)).c_str());
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


