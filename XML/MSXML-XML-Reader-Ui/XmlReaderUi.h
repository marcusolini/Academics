// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.


#pragma once


#include <Windows.h>
#include <Windowsx.h>
#include <CommCtrl.h>
#include <tchar.h>
#include "resource.h"
#include <sstream>
#include <thread>


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

IMsXmlLib* g_pIMsXmlLib = nullptr;
std::wstring g_sXmlFilename;
std::wstring g_sXmlParseName;
std::wstring g_sXmlData;
XML_NODE_MAP g_xmlNodeMap;

#define WM_USER_XML_THREAD_COMPLETE (WM_USER + 100)
#define WM_USER_XML_DATA_READY (WM_USER + 101)

// THREAD
void XMLThreadFunc(const HWND hDlg, const LPARAM lParam);


// MAIN XML DIALOG - DECLARATIONS
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow);
INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onMainInitDialog(const HWND hDlg);
void onMainOpenFile(const HWND hDlg);
void onMainLoadXML(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onMainOpenXML(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onMainOk(const HWND hDlg);
void onMainCancel(const HWND hDlg);
void onMainClose(const HWND hDlg);

// OPEN XML DIALOG - DECLARATIONS
INT_PTR CALLBACK OpenXmlDialogProc(const HWND hDlg, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
void onOpenXmlDialogInit(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onOpenXmlDialogClose(const HWND hDlg, const WPARAM wParam);
void onOpenXmlDialogDataReady(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onTraverseParsedXmlData(const HWND hDlg, const WPARAM wParam, const LPARAM lParam, const XML_NODE_MAP& xmlNodeMap, std::wostringstream& xmlStream);

// XML PROGRESS - DECLARATIONS
INT_PTR CALLBACK XmlProgressDialogProc(const HWND hDlg, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
void onXmlProgressDialogInit(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onXmlProgressThreadComplete(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);

// XML PARSE INPUT
INT_PTR CALLBACK EnterParsedXmlDialogProc(const HWND hDlg, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
void onEnterParsedXmlDialogInit(const HWND hDlg, const WPARAM wParam, const LPARAM lParam);
void onEnterParsedXmlDialogOk(const HWND hDlg, const WPARAM wParam);
void onEnterParsedXmlDialogCancel(const HWND hDlg, const WPARAM wParam);