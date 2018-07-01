#pragma once

#include "WinHttpLibTypes.h"


class IWinHttpLib
{
public:

     // INSTANCE FUNCTIONS

     static HRESULT CreateInstance(IWinHttpLib** ppiWinHttpLib);
     static HRESULT DeleteInstance(IWinHttpLib* piWinHttpLib);

     // ENUMS

     enum class EProxyAccessType { NoProxy, NamedProxy, AutomaticProxy };
     enum class EDefaultServerPorts : WORD { DefaultPort = 0, DefaultHttpPort = 80, DefaultHttpsPort = 443 };
     enum class EVerb { Get, Post, Put, Delete, Head, Options, Patch, Connect, Trace };
     enum class EAuthTargets { TargetServer, TargetProxy };
     enum class EAuthScheme { Basic, Ntlm, Passport, Digest, Negotiate };

     using VAcceptTypesArray = std::vector<std::wstring>;


     // OPTIONAL PARAMETER DEFAULTS

     // Open
     static const std::wstring NO_PROXY_NAME;
     static const std::wstring NO_PROXY_BYPASS;

     // Connect
     static const WORD DEFAULT_SERVER_PORT = (WORD)EDefaultServerPorts::DefaultPort;
     
     // OpenRequest
     static const std::wstring DEFAULT_VERSION;
     static const std::wstring NO_REFERER;
     static const VAcceptTypesArray* NO_ACCEPT_TYPES;
     
     // SendRequest
     static const std::wstring NO_ADDITIONAL_HEADERS;
     static const std::wstring NO_REQUEST_DATA;
     

     // METHODS

     // WinHttpOpen
     virtual HRESULT Open( IN const std::wstring&                     sApplicationName         = TEXT("WinHttpClientLib/1.0"),
                           IN const IWinHttpLib::EProxyAccessType     eProxyAccessType         = IWinHttpLib::EProxyAccessType::AutomaticProxy,
                           OPTIONAL IN const std::wstring&            sProxyName               = IWinHttpLib::NO_PROXY_NAME,
                           OPTIONAL IN const std::wstring&            sProxyBypassList         = IWinHttpLib::NO_PROXY_BYPASS
                         ) = 0;

     // WinHttpConnect
     virtual HRESULT Connect( IN const std::wstring&        sServerName         = TEXT("api.github.com"), 
                              OPTIONAL IN const WORD        wServerPort         = IWinHttpLib::DEFAULT_SERVER_PORT
                            ) = 0;

     // WinHttpOpenRequest
     virtual HRESULT OpenRequest( IN const IWinHttpLib::EVerb                             eVerb                    = IWinHttpLib::EVerb::Get,
                                  IN const std::wstring&                                  sObjectPathName          = TEXT("/user/repos"),
                                  OPTIONAL IN const std::wstring&                         sVersion                 = IWinHttpLib::DEFAULT_VERSION,
                                  OPTIONAL IN const std::wstring&                         sReferrer                = IWinHttpLib::NO_REFERER,
                                  OPTIONAL IN const IWinHttpLib::VAcceptTypesArray*       pAcceptTypesArray        = IWinHttpLib::NO_ACCEPT_TYPES,
                                  IN const DWORD                                          dwFlags                  = ERequestFlags::Secure | ERequestFlags::Refresh
                                ) = 0;

     // WinHttpSetCredentials
     OPTIONAL virtual HRESULT SetCredentials( IN const IWinHttpLib::EAuthTargets                   eAuthTargets             = IWinHttpLib::EAuthTargets::TargetServer,
                                              IN const IWinHttpLib::EAuthScheme                    eAuthScheme              = IWinHttpLib::EAuthScheme::Basic,
                                              IN const std::wstring&                               sUserName                = TEXT("username"),
                                              IN const std::wstring&                               sPassword                = TEXT("password")
                                            ) = 0;

     // WinHttpSendRequest
     virtual HRESULT SendRequest( OPTIONAL IN const std::wstring&          sHeaders            = IWinHttpLib::NO_ADDITIONAL_HEADERS,
                                  OPTIONAL IN const std::wstring&          sOptional           = IWinHttpLib::NO_REQUEST_DATA
                                ) = 0;

     // WinHttpReceiveResponse - WinHttpQueryDataAvailable - WinHttpReadData
     virtual HRESULT ReceiveResponseAndReadData( IN std::wstring& sData ) = 0;
          
};