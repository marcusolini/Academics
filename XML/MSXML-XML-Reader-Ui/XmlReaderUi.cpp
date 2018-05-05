// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.


#include <Windows.h>
#include <Windowsx.h>
#include <CommCtrl.h>
#include <tchar.h>
#include "resource.h"


#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")


#pragma comment(lib, "MSXML-Library.lib")
#include "..\\MSXML-Library\\IMsXmlLib.h"


// GLOBAL - DECLARATIONS
std::wstring LoadStringFromResourceId(const UINT id);
IMsXmlLib* pIMsXmlLib = nullptr;


// MAIN XML DIALOG - DECLARATIONS
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow);
INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onMainInitDialog(const HWND hDlg);
void onMainOpenFile(const HWND hDlg);
void onMainLoadXML(const HWND hDlg);
void onMainOpenXML(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onMainOk(const HWND hDlg);
void onMainCancel(const HWND hDlg);
void onMainClose(const HWND hDlg);


// OPEN XML DIALOG - DECLARATIONS
INT_PTR CALLBACK OpenXmlDialogProc(const HWND hDlg, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
void onOpenXmlDialogInit(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onOpenXmlDialogClose(const HWND hDlg, const WPARAM wParam);


// GLOBAL - DEFINITIONS

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


// MAIN XML DIALOG - DEFINITIONS

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
          case IDCLOSE:
               onMainCancel(hDlg);
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
               onMainOpenXML(hDlg, 0, IDC_OpenXmlButton);
               return TRUE;
               break;

          case IDC_OpenParsedXmlButton:
               onMainOpenXML(hDlg, 0, IDC_OpenParsedXmlButton);
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

     SendMessage(hDlg, WM_SETTEXT, TRUE, (LPARAM)(LoadStringFromResourceId(IDS_XML_MENU)).c_str());

     SendDlgItemMessage(hDlg, IDC_XmlFilenameStatic, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_XML_FILE_NAME)).c_str());
     SendDlgItemMessage(hDlg, IDC_LoadXmlButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_LOAD_XML)).c_str());
     SendDlgItemMessage(hDlg, IDC_OpenXmlButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_OPEN_XML)).c_str());
     SendDlgItemMessage(hDlg, IDC_OpenParsedXmlButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_OPEN_PARSED_XML)).c_str());
     SendDlgItemMessage(hDlg, IDC_Status, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_STATUS)).c_str());
     SendDlgItemMessage(hDlg, IDC_StatusText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_STATUS_SELECT_XML)).c_str());
     SendDlgItemMessage(hDlg, IDCLOSE, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_CLOSE_MENU)).c_str());

     HRESULT hr = IMsXmlLib::CreateInstance(&pIMsXmlLib);
     if (FAILED(hr))
     {
          MessageBox(hDlg, (LoadStringFromResourceId(IDS_UNABLE_TO_LOAD_XML_LIB)).c_str(), (LoadStringFromResourceId(IDS_FATAL_ERROR)).c_str(), (MB_ICONEXCLAMATION | MB_OK));
          
          HWND hTemp{};

          hTemp = GetDlgItem(hDlg, IDC_OpenFileButton);
          Edit_Enable(hTemp, FALSE);

          hTemp = GetDlgItem(hDlg, IDC_XmlFilename);
          Edit_Enable(hTemp, FALSE);

          hTemp = GetDlgItem(hDlg, IDC_OpenXmlButton);
          Edit_Enable(hTemp, FALSE);

          hTemp = GetDlgItem(hDlg, IDC_OpenParsedXmlButton);
          Edit_Enable(hTemp, FALSE);

          SendDlgItemMessage(hDlg, IDC_StatusText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_UNABLE_TO_LOAD_XML_LIB)).c_str());
     }
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
     HWND hTemp{};

     std::wstring sLoadedState;
     wchar_t szTemp[1024] = {};
     WORD wTemp = 0;

     GetDlgItemText(hDlg, IDC_LoadXmlButton, szTemp, 1024);

     sLoadedState = szTemp;

     if (0 == sLoadedState.compare((LoadStringFromResourceId(IDS_UNLOAD_XML).c_str())))
     {
          SendDlgItemMessage(hDlg, IDC_LoadXmlButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_LOAD_XML)).c_str());
          SendDlgItemMessage(hDlg, IDC_StatusText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_STATUS_XML_SELECTED)).c_str());

          hTemp = GetDlgItem(hDlg, IDC_OpenFileButton);
          Edit_Enable(hTemp, TRUE);

          hTemp = GetDlgItem(hDlg, IDC_XmlFilename);
          Edit_Enable(hTemp, TRUE);

          hTemp = GetDlgItem(hDlg, IDC_OpenXmlButton);
          Edit_Enable(hTemp, FALSE);

          hTemp = GetDlgItem(hDlg, IDC_OpenParsedXmlButton);
          Edit_Enable(hTemp, FALSE);

     }
     else
     {
          wchar_t szTemp[MAX_PATH] = {};
          WORD wTemp = 0;
          std::wstring sXmlFilename;

          wTemp = (WORD)SendDlgItemMessage(hDlg, IDC_XmlFilename, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
          *((LPWORD)szTemp) = wTemp;
          SendDlgItemMessage(hDlg, IDC_XmlFilename, EM_GETLINE, (WPARAM)0, (LPARAM)szTemp);

          sXmlFilename = szTemp;

          if (pIMsXmlLib)
          {
               HRESULT hr = pIMsXmlLib->LoadXML(sXmlFilename);

               if (SUCCEEDED(hr))
               {
                    SendDlgItemMessage(hDlg, IDC_LoadXmlButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_UNLOAD_XML)).c_str());
                    SendDlgItemMessage(hDlg, IDC_StatusText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_OPEN_XML)).c_str());

                    hTemp = GetDlgItem(hDlg, IDC_OpenFileButton);
                    Edit_Enable(hTemp, FALSE);

                    hTemp = GetDlgItem(hDlg, IDC_XmlFilename);
                    Edit_Enable(hTemp, FALSE);

                    hTemp = GetDlgItem(hDlg, IDC_OpenXmlButton);
                    Edit_Enable(hTemp, TRUE);

                    hTemp = GetDlgItem(hDlg, IDC_OpenParsedXmlButton);
                    Edit_Enable(hTemp, TRUE);

                    SendDlgItemMessage(hDlg, IDC_StatusText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_STATUS_XML_LOADED)).c_str());
               }
               else
               {
                    MessageBox(hDlg, (LoadStringFromResourceId(IDS_UNABLE_TO_LOAD_XML_FILE)).c_str(), (LoadStringFromResourceId(IDS_ERROR)).c_str(), (MB_ICONEXCLAMATION | MB_OK));
               }
          }
          else
          {
               MessageBox(hDlg, (LoadStringFromResourceId(IDS_UNABLE_TO_LOAD_XML_LIB)).c_str(), (LoadStringFromResourceId(IDS_FATAL_ERROR)).c_str(), (MB_ICONEXCLAMATION | MB_OK));
          }
     }
}



void onMainOpenXML(const HWND hDlg, const WPARAM wParam, const LPARAM lParam)
{
     INT_PTR OpenXmlDialog = 0;

     HWND hTemp{};

     hTemp = GetDlgItem(hDlg, IDC_LoadXmlButton);
     Edit_Enable(hTemp, FALSE);

     hTemp = GetDlgItem(hDlg, IDC_OpenXmlButton);
     Edit_Enable(hTemp, FALSE);

     hTemp = GetDlgItem(hDlg, IDC_OpenParsedXmlButton);
     Edit_Enable(hTemp, FALSE);

     hTemp = GetDlgItem(hDlg, IDCLOSE);
     Edit_Enable(hTemp, FALSE);

     SendDlgItemMessage(hDlg, IDC_LoadXmlButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_UNLOAD_XML)).c_str());
     SendDlgItemMessage(hDlg, IDC_StatusText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_STATUS_XML_OPENNED)).c_str());

     OpenXmlDialog = DialogBoxParam(nullptr, MAKEINTRESOURCE(IDD_XmlOutputDialog), hDlg, OpenXmlDialogProc, lParam);

     SendDlgItemMessage(hDlg, IDC_LoadXmlButton, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_UNLOAD_XML)).c_str());
     SendDlgItemMessage(hDlg, IDC_StatusText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_STATUS_XML_LOADED)).c_str());
     
     hTemp = GetDlgItem(hDlg, IDC_LoadXmlButton);
     Edit_Enable(hTemp, TRUE);

     hTemp = GetDlgItem(hDlg, IDC_OpenXmlButton);
     Edit_Enable(hTemp, TRUE);

     hTemp = GetDlgItem(hDlg, IDC_OpenParsedXmlButton);
     Edit_Enable(hTemp, TRUE);

     hTemp = GetDlgItem(hDlg, IDCLOSE);
     Edit_Enable(hTemp, TRUE);
}

void onMainCancel(const HWND hDlg)
{
     SendMessage(hDlg, WM_CLOSE, 0, 0);
}

void onMainClose(const HWND hDlg)
{
     int msgBoxID = IDNO;

     msgBoxID = MessageBox(hDlg, (LoadStringFromResourceId(IDS_CLOSE)).c_str(), (LoadStringFromResourceId(IDS_CLOSE_MENU)).c_str(), (MB_ICONQUESTION | MB_YESNO));

     if (IDYES == msgBoxID)
     {
          HRESULT hr = IMsXmlLib::DeleteInstance(pIMsXmlLib);
          if (FAILED(hr))
          {
               // TODO MESSAGEBOX
          }

          DestroyWindow(hDlg);
     }
}


// OPEN XML DIALOG - DEFINITIONS

INT_PTR CALLBACK OpenXmlDialogProc(const HWND hDlg, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
     switch (uMsg)
     {
     case WM_INITDIALOG:
          onOpenXmlDialogInit(hDlg, wParam, lParam);
          return TRUE;
          break;

     case WM_COMMAND:
     {
          switch (LOWORD(wParam))
          {
          case IDCLOSE:
               onOpenXmlDialogClose(hDlg, wParam);
               return TRUE;
               break;
          }
          break;
     }

     } // switch

     return FALSE;
}

void onOpenXmlDialogInit(const HWND hDlg, const WPARAM wParam, const LPARAM lParam)
{
     SendMessage(hDlg, WM_SETTEXT, TRUE, (LPARAM)(LoadStringFromResourceId(IDS_XML_DATA)).c_str());

     SendDlgItemMessage(hDlg, IDC_StatusXmlFileText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_STATUS_XML_FILE)).c_str());
     SendDlgItemMessage(hDlg, IDCLOSE, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_CLOSE_MENU)).c_str());

     if (pIMsXmlLib)
     {
          HRESULT hr = ERROR_SUCCESS;

          HWND hMainDialog{};
          hMainDialog = GetParent(hDlg);

          wchar_t szTemp[MAX_PATH] = {};
          WORD wTemp = 0;
          std::wstring sXmlFilename;

          wTemp = (WORD)SendDlgItemMessage(hMainDialog, IDC_XmlFilename, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
          *((LPWORD)szTemp) = wTemp;
          SendDlgItemMessage(hMainDialog, IDC_XmlFilename, EM_GETLINE, (WPARAM)0, (LPARAM)szTemp);
          sXmlFilename = szTemp;

          switch (lParam)
          {
          case IDC_OpenXmlButton:
          {
               std::wstring sXmlData;
               hr = pIMsXmlLib->GetXML(sXmlData);

               if (SUCCEEDED(hr))
               {

                    SendDlgItemMessage(hDlg, IDC_XmlOutputText, WM_SETTEXT, (WPARAM)0, (LPARAM)(sXmlData.c_str()));
                    SendDlgItemMessage(hDlg, IDC_StatusXmlFileName, WM_SETTEXT, (WPARAM)0, (LPARAM)(sXmlFilename.c_str()));
               }
               else
               {
                    MessageBox(hDlg, (LoadStringFromResourceId(IDS_UNABLE_TO_GET_XML_DATA)).c_str(), (LoadStringFromResourceId(IDS_ERROR)).c_str(), (MB_ICONEXCLAMATION | MB_OK));
               }
               break;
          }
          case IDC_OpenParsedXmlButton:
          {
               XML_NODE_MAP xmlNodeMap;
               hr = pIMsXmlLib->ParseXML(xmlNodeMap);

               if (SUCCEEDED(hr))
               {
                    SendDlgItemMessage(hDlg, IDC_StatusXmlFileName, WM_SETTEXT, (WPARAM)0, (LPARAM)(sXmlFilename.c_str()));
               }
               else
               {
                    MessageBox(hDlg, (LoadStringFromResourceId(IDS_UNABLE_TO_GET_XML_DATA)).c_str(), (LoadStringFromResourceId(IDS_ERROR)).c_str(), (MB_ICONEXCLAMATION | MB_OK));
               }

               break;
          }
          } // switch

     }
     else
     {
          MessageBox(hDlg, (LoadStringFromResourceId(IDS_UNABLE_TO_LOAD_XML_LIB)).c_str(), (LoadStringFromResourceId(IDS_FATAL_ERROR)).c_str(), (MB_ICONEXCLAMATION | MB_OK));
     }
}

void onOpenXmlDialogClose(const HWND hDlg, const WPARAM wParam)
{
     EndDialog(hDlg, wParam);
}
