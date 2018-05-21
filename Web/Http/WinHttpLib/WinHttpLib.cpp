#pragma once

#include "WinHttpLib.h"

#include <Winhttp.h>
#pragma comment(lib, "Winhttp.lib")

#include "../../../Error_Checks/ERROR_CHECKS.H"
#include "../../../MemoryCrypt/inc/MemoryCrypt.h"

#include <mutex>
#include <memory>


// GLOBALS

const std::wstring IWinHttpLib::NO_PROXY_NAME = TEXT("");
const std::wstring IWinHttpLib::NO_PROXY_BYPASS = TEXT("");

const std::wstring IWinHttpLib::DEFAULT_VERSION = TEXT("");
const std::wstring IWinHttpLib::NO_REFERER = TEXT("");

const IWinHttpLib::VAcceptTypesArray* IWinHttpLib::NO_ACCEPT_TYPES = nullptr;

const std::wstring IWinHttpLib::NO_ADDITIONAL_HEADERS = TEXT("");
const std::wstring IWinHttpLib::NO_REQUEST_DATA = TEXT("");

// DEFINITIONS

class CWinHttpLib : IWinHttpLib
{
public:
     CWinHttpLib();
     virtual ~CWinHttpLib();

     // WinHttpOpen
     HRESULT Open( IN const std::wstring&                        sApplicationName         = TEXT("WinHttpClientLib/1.0"),
                   IN const IWinHttpLib::EProxyAccessType        eProxyAccessType         = IWinHttpLib::EProxyAccessType::AutomaticProxy,
                   OPTIONAL IN const std::wstring&               sProxyName               = IWinHttpLib::NO_PROXY_NAME,
                   OPTIONAL IN const std::wstring&               sProxyBypassList         = IWinHttpLib::NO_PROXY_BYPASS
                 );

     // WinHttpConnect
     HRESULT Connect( IN const std::wstring&           sServerName         = TEXT("api.github.com"),
                      OPTIONAL IN const WORD           wServerPort         = IWinHttpLib::DEFAULT_SERVER_PORT
                    );

     // WinHttpOpenRequest
     HRESULT OpenRequest( IN const IWinHttpLib::EVerb                           eVerb                    = IWinHttpLib::EVerb::Get,
                          IN const std::wstring&                                sObjectPathName          = TEXT("/user/repos"),
                          OPTIONAL IN const std::wstring&                       sVersion                 = IWinHttpLib::DEFAULT_VERSION,
                          OPTIONAL IN const std::wstring&                       sReferrer                = IWinHttpLib::NO_REFERER,
                          OPTIONAL IN const IWinHttpLib::VAcceptTypesArray*     pAcceptTypesArray        = IWinHttpLib::NO_ACCEPT_TYPES,
                          OPTIONAL IN const DWORD                               dwFlags                  = ERequestFlags::Secure | ERequestFlags::Refresh
                        );

     // WinHttpSetCredentials
     OPTIONAL HRESULT SetCredentials( IN const IWinHttpLib::EAuthTargets                   eAuthTargets           = IWinHttpLib::EAuthTargets::TargetServer,
                                      IN const IWinHttpLib::EAuthScheme                    eAuthScheme            = IWinHttpLib::EAuthScheme::Basic,
                                      IN const std::wstring&                               sUserName              = TEXT("username"),
                                      IN const std::wstring&                               sPassword              = TEXT("password")
                                    );

     // WinHttpSendRequest
     HRESULT SendRequest( OPTIONAL IN const std::wstring&        sHeaders       = IWinHttpLib::NO_ADDITIONAL_HEADERS,
                          OPTIONAL IN const std::wstring&        sOptional      = IWinHttpLib::NO_REQUEST_DATA
                        );

     // WinHttpReceiveResponse - WinHttpQueryDataAvailable - WinHttpReadData
     HRESULT ReceiveResponseAndReadData(IN std::wstring& sData);

     // TODO: Add a redo call with that uses all saved parameters???

     CWinHttpLib(const CWinHttpLib&) = delete;
     CWinHttpLib& operator=(const CWinHttpLib&) = delete;

private:
     std::recursive_mutex m_mutex;

     const std::wstring m_sDefaultApplicationName = TEXT("WinHttpClientLib/1.0");
     
     // WinHttpOpen
     HINTERNET m_hSession = nullptr;
     std::wstring m_sApplicationName;
     EProxyAccessType m_eProxyAccessType = IWinHttpLib::EProxyAccessType::AutomaticProxy;
     std::wstring m_sProxyName;
     std::wstring m_sProxyBypassList;
     DWORD m_dwOpenFlags = 0;   // WINHTTP_FLAG_ASYNC


     // WinHttpConnect
     HINTERNET m_hConnect = nullptr;
     std::wstring m_sServerName;
     WORD m_wServerPort = IWinHttpLib::DEFAULT_SERVER_PORT;

     // WinHttpOpenRequest
     HINTERNET m_hRequest = nullptr;
     IWinHttpLib::EVerb m_eVerb = IWinHttpLib::EVerb::Get;
     std::wstring m_sObjectPathName;
     std::wstring m_sVersion = IWinHttpLib::DEFAULT_VERSION;
     std::wstring m_sReferrer = IWinHttpLib::NO_REFERER;
     IWinHttpLib::VAcceptTypesArray m_vAcceptTypesArray;
     DWORD m_dwRequestFlags = ERequestFlags::Secure | ERequestFlags::Refresh;

     // WinHttpSetCredentials
     IWinHttpLib::EAuthTargets m_eAuthTargets = IWinHttpLib::EAuthTargets::TargetServer;
     IWinHttpLib::EAuthScheme m_eAuthScheme = IWinHttpLib::EAuthScheme::Basic;
     std::wstring m_sUserName;
     std::wstring m_sPasswordEncrypted;

     // WinHttpSendRequest
     std::wstring m_sHeaders = IWinHttpLib::NO_ADDITIONAL_HEADERS;
     std::wstring m_sOptional = IWinHttpLib::NO_REQUEST_DATA;


     DWORD TranslateProxyAccessType(EProxyAccessType eProxyAccessType);
     std::wstring TranslateVerb(IWinHttpLib::EVerb eVerb);
     DWORD TranslateAuthTargets(IWinHttpLib::EAuthTargets eAuthTargets);
     DWORD TranslateAuthScheme(IWinHttpLib::EAuthScheme eAuthScheme);
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
          CHECK_BOOL_TRUE_LAST_ERROR_LOG_NOTHROW(WinHttpCloseHandle(m_hRequest));
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          //throw hrResult;  // throw
     }
}


HRESULT CWinHttpLib::Open( IN const std::wstring&                          sApplicationName,
                           IN const IWinHttpLib::EProxyAccessType          eProxyAccessType,
                           OPTIONAL IN const std::wstring&                 sProxyName, 
                           OPTIONAL IN const std::wstring&                 sProxyBypassList
                         )
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

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

          CHECK_NOT_NULL_LAST_ERROR_LOG_THROW( m_hSession = WinHttpOpen(m_sApplicationName.c_str(), dwProxyAccessType, pwszProxyName, pwszProxyBypassList, m_dwOpenFlags) );
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          m_hSession = nullptr;
     }

     return hrResult;
}


HRESULT CWinHttpLib::Connect( IN const std::wstring&        sServerName, 
                              OPTIONAL IN const WORD        wServerPort
                            )
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

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


HRESULT CWinHttpLib::OpenRequest( IN const IWinHttpLib::EVerb                        eVerb, 
                                  IN const std::wstring&                             sObjectPathName,
                                  OPTIONAL IN const std::wstring&                    sVersion,
                                  OPTIONAL IN const std::wstring&                    sReferrer,
                                  OPTIONAL IN const IWinHttpLib::VAcceptTypesArray*  pAcceptTypesArray,
                                  OPTIONAL IN const DWORD                            dwFlags
                                )
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     HRESULT hrResult = ERROR_SUCCESS;

     std::wstring sVerb;

     LPCWSTR pwszVerb = nullptr;
     LPCWSTR pwszObjectName = nullptr;
     LPCWSTR pwszVersion = nullptr;
     LPCWSTR pwszReferrer = WINHTTP_NO_REFERER;
     LPCWSTR* pwszAcceptTypes = WINHTTP_DEFAULT_ACCEPT_TYPES;

     try
     {
          if (nullptr == m_hConnect) throw E_INVALIDARG;

          m_eVerb = eVerb;
          sVerb = TranslateVerb(m_eVerb);

          m_sObjectPathName = sObjectPathName;
          pwszObjectName = m_sObjectPathName.size() ? m_sObjectPathName.c_str() : nullptr;

          m_sVersion = sVersion;
          pwszVersion = m_sVersion.size() ? m_sVersion.c_str() : nullptr;

          m_sReferrer = sReferrer;
          pwszReferrer = m_sReferrer.size() ? m_sReferrer.c_str() : WINHTTP_NO_REFERER;

          if (nullptr == pAcceptTypesArray)
          {
               pwszAcceptTypes = WINHTTP_DEFAULT_ACCEPT_TYPES;
          }
          else
          {
               m_vAcceptTypesArray = *pAcceptTypesArray;
               size_t nvAcceptTypesArray = m_vAcceptTypesArray.size();

               pwszAcceptTypes = new LPCWSTR[nvAcceptTypesArray];

               for (auto acceptType = m_vAcceptTypesArray.begin(); acceptType != m_vAcceptTypesArray.end(); acceptType++, pwszAcceptTypes++)
               {
                    *pwszAcceptTypes = acceptType->c_str();
               }
          }

          m_dwRequestFlags = dwFlags;


          CHECK_NOT_NULL_LAST_ERROR_LOG_THROW( m_hRequest = WinHttpOpenRequest(m_hConnect, sVerb.c_str(), pwszObjectName, pwszVersion, pwszReferrer, pwszAcceptTypes, m_dwRequestFlags) );
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
     }

     if (nullptr != pwszAcceptTypes)
     {
          delete[] pwszAcceptTypes;
     }

     return hrResult;
}


OPTIONAL HRESULT CWinHttpLib::SetCredentials( IN const IWinHttpLib::EAuthTargets    eAuthTargets,
                                              IN const IWinHttpLib::EAuthScheme     eAuthScheme,
                                              IN const std::wstring&                sUserName,
                                              IN const std::wstring&                sPassword
                                             ) 
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     HRESULT hrResult = ERROR_SUCCESS;

     DWORD dwAuthTargets = WINHTTP_AUTH_TARGET_SERVER;
     DWORD dwAuthScheme = WINHTTP_AUTH_SCHEME_BASIC;
     LPVOID pAuthParams = nullptr;

     try
     {
          if (nullptr == m_hRequest) throw E_INVALIDARG;

          m_eAuthTargets = eAuthTargets;
          m_eAuthScheme = eAuthScheme;

          m_sUserName = sUserName;
          m_sPasswordEncrypted = sPassword;
          CHECK_SUCCEEDED_LOG_THROW(MemoryCrypt::Encrypt(m_sPasswordEncrypted));

          dwAuthTargets = TranslateAuthTargets(m_eAuthTargets);
          dwAuthScheme = TranslateAuthScheme(m_eAuthScheme);

          CHECK_SUCCEEDED_LOG_THROW(MemoryCrypt::Decrypt(m_sPasswordEncrypted));

          CHECK_BOOL_TRUE_LAST_ERROR_LOG_THROW( WinHttpSetCredentials(m_hRequest, dwAuthTargets, dwAuthScheme, m_sUserName.c_str(), m_sPasswordEncrypted.c_str(), pAuthParams) );

          CHECK_SUCCEEDED_LOG_THROW(MemoryCrypt::Encrypt(m_sPasswordEncrypted));
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
     }

     return hrResult;
}


HRESULT CWinHttpLib::SendRequest( OPTIONAL IN const std::wstring&      sHeaders,
                                  OPTIONAL IN const std::wstring&      sOptional
                                )
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     HRESULT hrResult = ERROR_SUCCESS;

     LPCWSTR pwszHeaders = WINHTTP_NO_ADDITIONAL_HEADERS;
     DWORD   dwHeadersLength = 0;

     LPVOID lpOptional = WINHTTP_NO_REQUEST_DATA;
     DWORD   dwOptionalLength = 0;

     DWORD dwTotalLength = 0;  // For subsequent WinHttpWriteData

     DWORD_PTR dwContext = 0;

     try
     {
          if (nullptr == m_hRequest) throw E_INVALIDARG;

          m_sHeaders = sHeaders;
          m_sOptional = sOptional;

          pwszHeaders = m_sHeaders.size() ? m_sHeaders.c_str() : WINHTTP_NO_ADDITIONAL_HEADERS;
          dwHeadersLength = m_sHeaders.size();

          lpOptional = m_sOptional.size() ? (LPVOID)m_sOptional.c_str() : WINHTTP_NO_REQUEST_DATA;
          dwOptionalLength = m_sOptional.size();

          CHECK_BOOL_TRUE_LAST_ERROR_LOG_THROW( WinHttpSendRequest(m_hRequest, pwszHeaders, dwHeadersLength, lpOptional, dwOptionalLength, dwTotalLength, dwContext) );
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
     }

     return hrResult;
}


// WinHttpReceiveResponse
HRESULT CWinHttpLib::ReceiveResponseAndReadData(IN std::wstring&  sData)
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     HRESULT hrResult = ERROR_SUCCESS;
     
     DWORD dwNumberOfBytesAvailable = 0;
     DWORD dwNumberOfBytesRead = 0;

     sData.clear();

     std::string sAsciiData;

     try
     {
          CHECK_BOOL_TRUE_LAST_ERROR_LOG_THROW( WinHttpReceiveResponse(m_hRequest, nullptr) );

          do
          {
               dwNumberOfBytesAvailable = 0;

               CHECK_BOOL_TRUE_LAST_ERROR_LOG_THROW( WinHttpQueryDataAvailable(m_hRequest, &dwNumberOfBytesAvailable) );

               if (0 != dwNumberOfBytesAvailable)
               {
                    std::unique_ptr<char[]> up(new char[dwNumberOfBytesAvailable]);  // Could have used calloc and realloc

                    std::memset(up.get(), 0, dwNumberOfBytesAvailable);

                    CHECK_BOOL_TRUE_LAST_ERROR_LOG_THROW(WinHttpReadData(m_hRequest, (LPVOID)up.get(), dwNumberOfBytesAvailable, &dwNumberOfBytesRead));

                    sAsciiData.append(up.get(), dwNumberOfBytesRead);

                    dwNumberOfBytesRead = 0;
               }

          } while (0 != dwNumberOfBytesAvailable);

          sData.assign(sAsciiData.begin(), sAsciiData.end());
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
     }

     return hrResult;
}



std::wstring CWinHttpLib::TranslateVerb(IWinHttpLib::EVerb eVerb)
{
     std::wstring sVerb;

     switch (eVerb)
     {
     case IWinHttpLib::EVerb::Get:
          sVerb = TEXT("GET");
          break;
     case IWinHttpLib::EVerb::Post:
          sVerb = TEXT("POST");
          break;
     case IWinHttpLib::EVerb::Put:
          sVerb = TEXT("PUT");
          break;
     case IWinHttpLib::EVerb::Delete:
          sVerb = TEXT("DELETE");
          break;
     case IWinHttpLib::EVerb::Head:
          sVerb = TEXT("HEAD");
          break;
     case IWinHttpLib::EVerb::Options:
          sVerb = TEXT("OPTIONS");
          break;
     case IWinHttpLib::EVerb::Patch:
          sVerb = TEXT("PATCH");
          break;
     case IWinHttpLib::EVerb::Connect:
          sVerb = TEXT("CONNECT");
          break;
     case IWinHttpLib::EVerb::Trace:
          sVerb = TEXT("TRACE");
          break;
     default:
          sVerb = TEXT("GET");
          break;
     };

     return sVerb;
}


DWORD CWinHttpLib::TranslateProxyAccessType(IWinHttpLib::EProxyAccessType eProxyAccessType)
{
     DWORD dwProxyAccessType = WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY;

     switch (eProxyAccessType)
     {
     case IWinHttpLib::EProxyAccessType::NoProxy:
          dwProxyAccessType = WINHTTP_ACCESS_TYPE_NO_PROXY;
          break;
     case IWinHttpLib::EProxyAccessType::NamedProxy:
          dwProxyAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
          break;
     case IWinHttpLib::EProxyAccessType::AutomaticProxy:
          dwProxyAccessType = WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY;
          break;
     default:
          dwProxyAccessType = WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY;
          break;
     };

     return dwProxyAccessType;
}

DWORD CWinHttpLib::TranslateAuthTargets(IWinHttpLib::EAuthTargets eAuthTargets)
{
     DWORD dwAuthTargets = WINHTTP_AUTH_TARGET_SERVER;

     switch (eAuthTargets)
     {
     case IWinHttpLib::EAuthTargets::TargetServer:
          dwAuthTargets = WINHTTP_AUTH_TARGET_SERVER;
          break;
     case IWinHttpLib::EAuthTargets::TargetProxy:
          dwAuthTargets = WINHTTP_AUTH_TARGET_PROXY;
          break;
     default:
          dwAuthTargets = WINHTTP_AUTH_TARGET_SERVER;
          break;
     };

     return dwAuthTargets;
}

DWORD CWinHttpLib::TranslateAuthScheme(IWinHttpLib::EAuthScheme eAuthScheme)
{
     DWORD dwAuthScheme = WINHTTP_AUTH_SCHEME_BASIC;

     switch (eAuthScheme)
     {
     case IWinHttpLib::EAuthScheme::Basic:
          dwAuthScheme = WINHTTP_AUTH_SCHEME_BASIC;
          break;
     case IWinHttpLib::EAuthScheme::Ntlm:
          dwAuthScheme = WINHTTP_AUTH_SCHEME_NTLM;
          break;
     case IWinHttpLib::EAuthScheme::Passport:
          dwAuthScheme = WINHTTP_AUTH_SCHEME_PASSPORT;
          break;
     case IWinHttpLib::EAuthScheme::Digest:
          dwAuthScheme = WINHTTP_AUTH_SCHEME_DIGEST;
          break;
     case IWinHttpLib::EAuthScheme::Negotiate:
          dwAuthScheme = WINHTTP_AUTH_SCHEME_NEGOTIATE;
          break;
     default:
          dwAuthScheme = WINHTTP_AUTH_SCHEME_BASIC;
          break;
     };

     return dwAuthScheme;
}