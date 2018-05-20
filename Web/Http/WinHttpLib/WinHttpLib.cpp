#pragma once

#include "WinHttpLib.h"

#include <Winhttp.h>
#pragma comment(lib, "Winhttp.lib")

#include "../../../Error_Checks/ERROR_CHECKS.H"

#include <mutex>


// DEFINITIONS

class CWinHttpLib : IWinHttpLib
{
public:
     CWinHttpLib();
     virtual ~CWinHttpLib();

     HRESULT Open( OPTIONAL IN const std::wstring& sApplicationName                       = IWinHttpLib::DEFAULT_APP_NAME,
                   OPTIONAL IN const IWinHttpLib::EProxyAccessType eProxyAccessType       = IWinHttpLib::DEFAULT_PROXY_ACCESS_TYPE,
                   OPTIONAL IN const std::wstring& sProxyName                             = IWinHttpLib::NO_PROXY_NAME,
                   OPTIONAL IN const std::wstring& sProxyBypassList                       = IWinHttpLib::NO_PROXY_BYPASS
                 );

     HRESULT Connect( IN const std::wstring& sServerName,
                      OPTIONAL IN const WORD wServerPort    = IWinHttpLib::DEFAULT_SERVER_PORT
                    );

     HRESULT OpenRequest( IN const IWinHttpLib::EVerb eVerb,
                          IN const std::wstring sObjectPathName,
                          OPTIONAL IN const std::wstring sVersion = IWinHttpLib::DEFAULT_VERSION,
                          OPTIONAL IN const std::wstring sReferrer = IWinHttpLib::NO_REFERER
                        );

     // TODO: Add a redo call with that uses all saved parameters???

     CWinHttpLib(const CWinHttpLib&) = delete;
     CWinHttpLib& operator=(const CWinHttpLib&) = delete;

private:
     std::recursive_mutex m_mutex;

     const std::wstring m_sDefaultApplicationName = TEXT("WinHttpClientLib/1.0");
     
     // Open
     HINTERNET m_hSession = nullptr;
     std::wstring m_sApplicationName;
     EProxyAccessType m_eProxyAccessType = EProxyAccessType::NoProxy;
     std::wstring m_sProxyName;
     std::wstring m_sProxyBypassList;
     DWORD m_dwFlags = 0;   // WINHTTP_FLAG_ASYNC


     //Connect
     HINTERNET m_hConnect = nullptr;
     std::wstring m_sServerName;
     WORD m_wServerPort = IWinHttpLib::DEFAULT_SERVER_PORT;


     DWORD TranslateProxyAccessType(EProxyAccessType eProxyAccessType);
};


// DECLARATIONS

HRESULT IWinHttpLib::CreateInstance(IWinHttpLib** ppIWinHttpLib)
{
     HRESULT hrResult = ERROR_SUCCESS;
     CWinHttpLib* pCWinHttpLib = nullptr;

     try
     {
          if (nullptr == ppIWinHttpLib)  return E_INVALIDARG;

          CHECK_NEW_ALLOC_LOG_THROW( pCWinHttpLib = new (std::nothrow) CWinHttpLib() );

          *ppIWinHttpLib = reinterpret_cast<IWinHttpLib*>(pCWinHttpLib);
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
     }

     return hrResult;
}

HRESULT IWinHttpLib::DeleteInstance(IWinHttpLib* piWinHttpLib)
{
     HRESULT hrResult = ERROR_SUCCESS;
     CWinHttpLib* pCWinHttpLib = nullptr;

     try
     {
          if (nullptr == piWinHttpLib)  return E_INVALIDARG;
          
          pCWinHttpLib = reinterpret_cast<CWinHttpLib*>(piWinHttpLib);
          
          delete pCWinHttpLib;
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
     }

     return hrResult;
}

CWinHttpLib::CWinHttpLib()
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     HRESULT hrResult = ERROR_SUCCESS;

     try
     {

     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          throw hrResult;  // throw
     }
}

CWinHttpLib::~CWinHttpLib()
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     HRESULT hrResult = ERROR_SUCCESS;

     try
     {
          CHECK_BOOL_TRUE_LAST_ERROR_LOG_NOTHROW(WinHttpCloseHandle(m_hSession));
          CHECK_BOOL_TRUE_LAST_ERROR_LOG_NOTHROW(WinHttpCloseHandle(m_hConnect));
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          //throw hrResult;  // throw
     }
}


HRESULT CWinHttpLib::Open(OPTIONAL IN const std::wstring& sApplicationName, OPTIONAL IN const IWinHttpLib::EProxyAccessType eProxyAccessType, OPTIONAL IN const std::wstring& sProxyName, OPTIONAL IN const std::wstring& sProxyBypassList)
{
     HRESULT hrResult = ERROR_SUCCESS;

     LPCWSTR pwszProxyName = WINHTTP_NO_PROXY_NAME;
     LPCWSTR pwszProxyBypassList = WINHTTP_NO_PROXY_BYPASS;
     DWORD dwProxyAccessType = WINHTTP_ACCESS_TYPE_NO_PROXY;

     try
     {
          m_hSession = nullptr;
          m_sApplicationName = sApplicationName;
          m_eProxyAccessType = eProxyAccessType;
          m_sProxyName = sProxyName;
          m_sProxyBypassList = sProxyBypassList;
          //m_dwFlags = dwFlags;

          dwProxyAccessType = TranslateProxyAccessType(m_eProxyAccessType);

          if (WINHTTP_ACCESS_TYPE_NAMED_PROXY == dwProxyAccessType)
          {
               pwszProxyName = m_sProxyName.c_str();
               pwszProxyBypassList = m_sProxyBypassList.size() ? m_sProxyBypassList.c_str() : WINHTTP_NO_PROXY_BYPASS;
          }
          else
          {
               pwszProxyName = WINHTTP_NO_PROXY_NAME;
               pwszProxyBypassList = WINHTTP_NO_PROXY_BYPASS;
          }

          if (0 == m_sApplicationName.size())
          {
               m_sApplicationName = m_sDefaultApplicationName;
          }

          CHECK_NOT_NULL_LAST_ERROR_LOG_THROW( m_hSession = WinHttpOpen(m_sApplicationName.c_str(), dwProxyAccessType, pwszProxyName, pwszProxyBypassList, m_dwFlags) );
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          m_hSession = nullptr;
     }

     return hrResult;
}


HRESULT CWinHttpLib::Connect(IN const std::wstring& sServerName, OPTIONAL IN const WORD wServerPort)
{
     HRESULT hrResult = ERROR_SUCCESS;

     try
     {
          if (nullptr == m_hSession) throw E_INVALIDARG;
          if (0 == sServerName.size()) throw E_INVALIDARG;

          m_sServerName = sServerName;
          m_wServerPort = wServerPort;

          CHECK_NOT_NULL_LAST_ERROR_LOG_THROW( m_hConnect = WinHttpConnect(m_hSession, m_sServerName.c_str(), m_wServerPort, 0) );
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
     }

     return hrResult;
}

HRESULT CWinHttpLib::OpenRequest(IN const IWinHttpLib::EVerb eVerb, IN const std::wstring sObjectPathName, OPTIONAL IN const std::wstring sVersion, OPTIONAL IN const std::wstring sReferrer)
{
     HRESULT hrResult = ERROR_SUCCESS;

     try
     {
          if (nullptr == m_hConnect) throw E_INVALIDARG;
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
     }

     return hrResult;
}



DWORD CWinHttpLib::TranslateProxyAccessType(EProxyAccessType eProxyAccessType)
{
     DWORD dwProxyAccessType = WINHTTP_ACCESS_TYPE_NO_PROXY;

     switch (eProxyAccessType)
     {
     case EProxyAccessType::NoProxy:
          dwProxyAccessType = WINHTTP_ACCESS_TYPE_NO_PROXY;
          break;
     case EProxyAccessType::NamedProxy:
          dwProxyAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
          break;
     case EProxyAccessType::AutomaticProxy:
          dwProxyAccessType = WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY;
          break;
     case EProxyAccessType::Default:
     default:
          dwProxyAccessType = WINHTTP_ACCESS_TYPE_NO_PROXY;
          break;
     };

     return dwProxyAccessType;
}