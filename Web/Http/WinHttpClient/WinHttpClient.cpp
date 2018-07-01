#include <Windows.h>
#include <Windowsx.h>
#include <CommCtrl.h>
#include <tchar.h>
#include <string>
#include "resource.h"

#include "../../../MemoryCrypt/inc/MemoryCrypt.h"
#include "../../../Error_Checks/ERROR_CHECKS.H"

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")

#include "../WinHttpLib/WinHttpLib.h"
#pragma comment(lib, "WinHttpLib.lib")

// GLOBALS

// MAIN DIALOG
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow);
INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onInitMainDialog(const HWND hDlg);
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
     hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_HttpClientMainDialog), nullptr, MainDialogProc);
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
          case IDCLOSE:
               onMainClose(hDlg);
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
     HWND hTemp{};

     SendMessage(hDlg, WM_SETTEXT, TRUE, (LPARAM)(LoadStringFromResourceId(IDS_WIN_HTTP_CLIENT)).c_str());

     SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDCLOSE, (LPARAM)IDCLOSE);
     SendDlgItemMessage(hDlg, IDCLOSE, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_CLOSE)).c_str());

     SendDlgItemMessage(hDlg, IDC_AppNameEditbox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_APP_NAME_SAMPLE)).c_str());

     

     SendDlgItemMessage(hDlg, IDC_ProtoStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_PROTOCOL_TEXT)).c_str());
     hTemp = GetDlgItem(hDlg, IDC_ProtocolCombo);
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_PROTO_HTTP)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_PROTO_HTTPS)).c_str());
     SendMessage(hTemp, CB_SETCURSEL, (WPARAM)1, (LPARAM)0);

     SendDlgItemMessage(hDlg, IDC_ServerNameStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SERVER_NAME_TEXT)).c_str());
     SendDlgItemMessage(hDlg, IDC_ServerNameEditBox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SERVER_NAME_SAMPLE)).c_str());

     SendDlgItemMessage(hDlg, IDC_ServerPortStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SERVER_PORT_TEXT)).c_str());
     hTemp = GetDlgItem(hDlg, IDC_ServerPortCombo);
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SERVER_PORT_HTTP_DEFAULT)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SERVER_PORT_HTTPS_DEFAULT)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_SERVER_PORT_DEFAULT)).c_str());
     SendMessage(hTemp, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);


     SendDlgItemMessage(hDlg, IDC_VerbStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_VERB_TEXT)).c_str());
     hTemp = GetDlgItem(hDlg, IDC_VerbCombo);
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_VERB_GET)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_VERB_POST)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_VERB_PUT)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_VERB_DELETE)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_VERB_HEAD)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_VERB_OPTIONS)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_VERB_PATCH)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_VERB_CONNECT)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_VERB_TRACE)).c_str());
     SendMessage(hTemp, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

     SendDlgItemMessage(hDlg, IDC_AppNameStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_APPLICATION_NAME)).c_str());

     SendDlgItemMessage(hDlg, IDC_ProxyAccessTypeStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_PROXY_ACCESS_TYPE_TEXT)).c_str());
     hTemp = GetDlgItem(hDlg, IDC_AccessTypeCombo);
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_ACCESS_TYPE_NO_PROXY)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_ACCESS_TYPE_NAMED_PROXY)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_ACCESS_TYPE_AUTOMATIC_PROXY)).c_str());
     SendMessage(hTemp, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);
     Edit_Enable(hTemp, TRUE);

     SendDlgItemMessage(hDlg, IDC_ProxyNameStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_PROXY_NAME_TEXT)).c_str());
     hTemp = GetDlgItem(hDlg, IDC_ProxyNameCombo);
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_ACCESS_TYPE_NO_PROXY_NAME)).c_str());
     SendMessage(hTemp, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
     Edit_Enable(hTemp, FALSE);

     SendDlgItemMessage(hDlg, IDC_ProxyBypassStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_PROXY_BYPASS_TEXT)).c_str());
     hTemp = GetDlgItem(hDlg, IDC_ProxyBypassCombo);
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_ACCESS_TYPE_NO_PROXY_BYPASS)).c_str());
     SendMessage(hTemp, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
     Edit_Enable(hTemp, FALSE);

     SendDlgItemMessage(hDlg, IDC_HttpVersionStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_HTTP_VERSION_TEXT)).c_str());
     hTemp = GetDlgItem(hDlg, IDC_HttpVersionComboBox);
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_HTTP_VERSION_DEFAULT)).c_str());
     SendMessage(hTemp, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
     Edit_Enable(hTemp, TRUE);

     SendDlgItemMessage(hDlg, IDC_PathStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_PATH_TEXT)).c_str());
     SendDlgItemMessage(hDlg, IDC_PathEditBox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_PATH_SAMPLE)).c_str());
     hTemp = GetDlgItem(hDlg, IDC_PathEditBox);
     Edit_Enable(hTemp, TRUE);

     SendDlgItemMessage(hDlg, IDC_ReferrerStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_REFERRER_TEXT)).c_str());
     hTemp = GetDlgItem(hDlg, IDC_ReferrerComboBox);
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_REFERRER_DEFAULT)).c_str());
     SendMessage(hTemp, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
     Edit_Enable(hTemp, TRUE);

     SendDlgItemMessage(hDlg, IDC_AcceptTypesStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_ACCEPT_TYPES_TEXT)).c_str());
     hTemp = GetDlgItem(hDlg, IDC_AcceptTypesComboBox);
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_ACCEPT_TYPES_DEFAULT)).c_str());
     SendMessage(hTemp, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
     Edit_Enable(hTemp, FALSE);

     SendDlgItemMessage(hDlg, IDC_FlagsStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_FLAGS_TEXT)).c_str());
     hTemp = GetDlgItem(hDlg, IDC_FlagsListBox);
     SendMessage(hTemp, LB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_FLAGS_SECURE)).c_str());
     SendMessage(hTemp, LB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_FLAGS_BYPASS_PROXY_CACHE)).c_str());
     SendMessage(hTemp, LB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_FLAGS_ESCAPE_DISABLED)).c_str());
     SendMessage(hTemp, LB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_FLAGS_ESCAPE_PERCENT)).c_str());
     SendMessage(hTemp, LB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_FLAGS_ESCAPE_DISABLED_QUERY)).c_str());
     SendMessage(hTemp, LB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_FLAGS_NULL_CODEPAGE)).c_str());
     SendMessage(hTemp, LB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_FLAGS_REFRESH)).c_str());
     SendMessage(hTemp, LB_SETSEL, (WPARAM)TRUE, (LPARAM)0);
     Edit_Enable(hTemp, TRUE);


     SendDlgItemMessage(hDlg, IDC_UsernameStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_USERNAME_TEXT)).c_str());
     SendDlgItemMessage(hDlg, IDC_PasswordStaticText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_PASSWORD_TOKEN_TEXT)).c_str());
     SendDlgItemMessage(hDlg, IDC_AuthorizationGroupBox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_AUTH_TEXT)).c_str());
     hTemp = GetDlgItem(hDlg, IDC_AuthSchemeComboBox);
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_AUTH_NONE)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_AUTH_BASIC)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_AUTH_NTLM)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_AUTH_PASSPORT)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_AUTH_DIGEST)).c_str());
     SendMessage(hTemp, CB_ADDSTRING, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_AUTH_NEGOTIATE)).c_str());
     SendMessage(hTemp, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
     Edit_Enable(hTemp, TRUE);


     SendDlgItemMessage(hDlg, IDC_AdditionalHeadersCheckBox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_ADDITIONAL_HEADERS_TEXT)).c_str());
     SendDlgItemMessage(hDlg, IDC_OptionalRequestDataCheckBox, WM_SETTEXT, (WPARAM)0, (LPARAM)(LoadStringFromResourceId(IDS_OPTIONAL_REQUEST_DATA_TEXT)).c_str());

     

     
     


     /*
     HRESULT hrResult = ERROR_SUCCESS;
     IWinHttpLib* pIWinHttpLib = nullptr;

     hrResult = IWinHttpLib::CreateInstance(&pIWinHttpLib);
     hrResult = pIWinHttpLib->Open();
     hrResult = pIWinHttpLib->Connect();
     hrResult = pIWinHttpLib->OpenRequest();

     IWinHttpLib::EAuthTargets eAuthTargets = IWinHttpLib::EAuthTargets::TargetServer;
     IWinHttpLib::EAuthScheme  eAuthScheme = IWinHttpLib::EAuthScheme::Basic;
     std::wstring sUserName = TEXT("marcusolini");
     std::wstring sPassword = TEXT("");

     hrResult = pIWinHttpLib->SetCredentials(eAuthTargets, eAuthScheme, sUserName, sPassword );
     hrResult = pIWinHttpLib->SendRequest();

     std::wstring sData;

     hrResult = pIWinHttpLib->ReceiveResponseAndReadData(sData);
     hrResult = IWinHttpLib::DeleteInstance(pIWinHttpLib);
     */
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

