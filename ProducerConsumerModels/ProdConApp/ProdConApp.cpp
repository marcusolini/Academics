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

#include "Producer.h"
#include "Consumer.h"
#include "ProdConQueue.h"


#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")


// GLOBALS

CProducer<PROD_CON_DATA>* g_pProducer = nullptr;
CConsumer<PROD_CON_DATA>* g_pConsumer = nullptr;
CProdConQueue<PROD_CON_DATA>* g_pPipeline = nullptr;

std::wstring LoadStringFromResourceId(const UINT id);


// MAIN DIALOG
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow);
INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onMainInitDialog(const HWND hDlg);
void onMainOk(const HWND hDlg);
void onMainCancel(const HWND hDlg);
void onMainClose(const HWND hDlg);

// PRODUCER
INT_PTR CALLBACK ProdDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onProdInitDialog(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onProdConnectPipeline(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onProdStart(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);

// CONSUMER
INT_PTR CALLBACK ConDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onConInitDialog(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onConConnectPipeline(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onConStart(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);


// PIPELINE
INT_PTR CALLBACK PipeDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onPipeInitDialog(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);


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
          case IDC_CreateProducer:
          {
               HWND hCheck = GetDlgItem(hDlg, IDC_CreateProducer);
               Edit_Enable(hCheck, FALSE);
               hDlg = CreateDialog(nullptr, MAKEINTRESOURCE(IDD_ProducerDialog), nullptr, ProdDialogProc);
               ShowWindow(hDlg, true);
               return TRUE;
               break;
          }
          case IDC_CreatePipeline:
          {
               HWND hCheck = GetDlgItem(hDlg, IDC_CreatePipeline);
               Edit_Enable(hCheck, FALSE);
               hDlg = CreateDialog(nullptr, MAKEINTRESOURCE(IDD_PipelineDialog), nullptr, PipeDialogProc);
               ShowWindow(hDlg, true);
               return TRUE;
               break;
          }
          case IDC_CreateConsumer:
          {
               HWND hCheck = GetDlgItem(hDlg, IDC_CreateConsumer);
               Edit_Enable(hCheck, FALSE);
               hDlg = CreateDialog(nullptr, MAKEINTRESOURCE(IDD_ConsumerDialog), nullptr, ConDialogProc);
               ShowWindow(hDlg, true);
               return TRUE;
               break;
          }

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



// PRODUCER

INT_PTR CALLBACK ProdDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
     switch (uMsg)
     {
     case WM_INITDIALOG:
          onProdInitDialog(hDlg, wParam, lParam);
          return TRUE;
          break;

     case WM_COMMAND:
     {
          switch (LOWORD(wParam))
          {
          case IDCANCEL:
               //onMainCancel(hDlg);
               return TRUE;
               break;
          case IDOK:

               return TRUE;
               break;

          case IDC_ConnectPipeline:
               onProdConnectPipeline(hDlg, wParam, lParam);
               return TRUE;
               break;

          case IDC_START:
               onProdStart(hDlg, wParam, lParam);
               return TRUE;
               break;

          } // switch

          break;
     }

     case WM_CLOSE:
          EndDialog(hDlg, wParam);
          return TRUE;
          break;
     case WM_DESTROY:
          PostQuitMessage(0);
          return TRUE;
          break;

     } // switch

     return FALSE;
}

void onProdInitDialog(const HWND hDlg, const WPARAM wParam, const LPARAM lParam)
{
     g_pProducer = new CProducer<PROD_CON_DATA>(30);
}

void onProdConnectPipeline(const HWND hDlg, const WPARAM wParam, const LPARAM lParam)
{
     HWND hDialogItem{};

     if (g_pPipeline)
     {
          g_pProducer->ConnectPipeline(g_pPipeline);
          hDialogItem = GetDlgItem(hDlg, IDC_ConnectPipeline);
          Edit_Enable(hDialogItem, FALSE);

          hDialogItem = GetDlgItem(hDlg, IDC_START);
          Edit_Enable(hDialogItem, TRUE);
     }
     else
     {
          MessageBox(hDlg, (LoadStringFromResourceId(IDS_PIPELINE_NOT_AVAIL)).c_str(), (LoadStringFromResourceId(IDS_PIPELINE)).c_str(), (MB_ICONERROR | MB_OK));
     }
}

void onProdStart(const HWND hDlg, const WPARAM wParam, const LPARAM lParam)
{
     HWND hDialogItem{};
     hDialogItem = GetDlgItem(hDlg, IDC_START);
     Edit_Enable(hDialogItem, FALSE);

     std::thread thread(*g_pProducer);

     std::wstringstream ss;
     ss << thread.get_id();
     uint64_t id = std::stoull(ss.str());

     thread.detach(); // Thread will be communicated to with atomic flags. Thread will communicate with SendMessage. Will not be joined.
}


// PIPELINE

INT_PTR CALLBACK PipeDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
     switch (uMsg)
     {
     case WM_INITDIALOG:
          onPipeInitDialog(hDlg, wParam, lParam);
          return TRUE;
          break;

     case WM_COMMAND:
     {
          switch (LOWORD(wParam))
          {
          case IDCANCEL:
               //onMainCancel(hDlg);
               return TRUE;
               break;

          case IDOK:
               return TRUE;
               break;

          } // switch

          break;
     }

     case WM_CLOSE:
          EndDialog(hDlg, wParam);
          return TRUE;
          break;
     case WM_DESTROY:
          PostQuitMessage(0);
          return TRUE;
          break;

     } // switch

     return FALSE;
}

void onPipeInitDialog(const HWND hDlg, const WPARAM wParam, const LPARAM lParam)
{
     g_pPipeline = new CProdConQueue<PROD_CON_DATA>(10);
}




// CONSUMER

INT_PTR CALLBACK ConDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
     switch (uMsg)
     {
     case WM_INITDIALOG:
          onConInitDialog(hDlg, wParam, lParam);
          return TRUE;
          break;

     case WM_COMMAND:
     {
          switch (LOWORD(wParam))
          {
          case IDCANCEL:
               //onMainCancel(hDlg);
               return TRUE;
               break;

          case IDOK:
               return TRUE;
               break;

          case IDC_ConnectPipeline:
               onConConnectPipeline(hDlg, wParam, lParam);
               return TRUE;
               break;

          case IDC_START:
               onConStart(hDlg, wParam, lParam);
               return TRUE;
               break;

          } // switch

          break;
     }

     case WM_CLOSE:
          EndDialog(hDlg, wParam);
          return TRUE;
          break;
     case WM_DESTROY:
          PostQuitMessage(0);
          return TRUE;
          break;

     } // switch

     return FALSE;
}

void onConInitDialog(const HWND hDlg, const WPARAM wParam, const LPARAM lParam)
{
     g_pConsumer = new CConsumer<PROD_CON_DATA>(30);
}

void onConConnectPipeline(const HWND hDlg, const WPARAM wParam, const LPARAM lParam)
{
     HWND hDialogItem{};

     if (g_pPipeline)
     {
          g_pConsumer->ConnectPipeline(g_pPipeline);

          hDialogItem = GetDlgItem(hDlg, IDC_ConnectPipeline);
          Edit_Enable(hDialogItem, FALSE);

          hDialogItem = GetDlgItem(hDlg, IDC_START);
          Edit_Enable(hDialogItem, TRUE);
     }
     else
     {
          MessageBox(hDlg, (LoadStringFromResourceId(IDS_PIPELINE_NOT_AVAIL)).c_str(), (LoadStringFromResourceId(IDS_PIPELINE)).c_str(), (MB_ICONERROR | MB_OK));
     }
}

void onConStart(const HWND hDlg, const WPARAM wParam, const LPARAM lParam)
{
     HWND hDialogItem{};
     hDialogItem = GetDlgItem(hDlg, IDC_START);
     Edit_Enable(hDialogItem, FALSE);

     std::thread thread(*g_pConsumer);

     std::wstringstream ss;
     ss << thread.get_id();
     uint64_t id = std::stoull(ss.str());

     thread.detach(); // Thread will be communicated to with atomic flags. Thread will communicate with SendMessage. Will not be joined.
}
