#pragma once

#include "WinHttpLibTypes.h"


class IWinHttpLib
{
public:

     static HRESULT CreateInstance(IWinHttpLib** ppiWinHttpLib);
     static HRESULT DeleteInstance(IWinHttpLib* piWinHttpLib);

     enum class EProxyAccessType { NoProxy, NamedProxy, AutomaticProxy };
     enum class EDefaultServerPorts : WORD { DefaultPort = 0, DefaultHttpPort = 80, DefaultHttpsPort = 443 };
     enum class EVerb { Get, Post, Put, Delete, Head, Options, Patch, Connect, Trace };

     using VAcceptTypesArray = std::vector<std::wstring>;

     // OPTIONAL PARAMETER DEFAULTS
     static const IWinHttpLib::EProxyAccessType DEFAULT_PROXY_ACCESS_TYPE = IWinHttpLib::EProxyAccessType::AutomaticProxy;
     static const std::wstring DEFAULT_APP_NAME;
     static const WORD DEFAULT_SERVER_PORT = (WORD)EDefaultServerPorts::DefaultPort;
     static const std::wstring NO_PROXY_NAME;
     static const std::wstring NO_PROXY_BYPASS;
     static const std::wstring DEFAULT_VERSION;
     static const std::wstring NO_REFERER;
     static const DWORD DEFAULT_FLAGS = ERequestFlags::Secure | ERequestFlags::Refresh;


     virtual HRESULT Open( OPTIONAL IN const std::wstring&                      sApplicationName         = IWinHttpLib::DEFAULT_APP_NAME,
                           OPTIONAL IN const IWinHttpLib::EProxyAccessType      eProxyAccessType         = IWinHttpLib::DEFAULT_PROXY_ACCESS_TYPE,
                           OPTIONAL IN const std::wstring&                      sProxyName               = IWinHttpLib::NO_PROXY_NAME,
                           OPTIONAL IN const std::wstring&                      sProxyBypassList         = IWinHttpLib::NO_PROXY_BYPASS
                         ) = 0;

     virtual HRESULT Connect( IN const std::wstring&        sServerName, 
                              OPTIONAL IN const WORD        wServerPort         = IWinHttpLib::DEFAULT_SERVER_PORT
                            ) = 0;

     virtual HRESULT OpenRequest( IN const IWinHttpLib::EVerb                             eVerb,
                                  IN const std::wstring&                                  sObjectPathName,
                                  OPTIONAL IN const std::wstring&                         sVersion                 = IWinHttpLib::DEFAULT_VERSION,
                                  OPTIONAL IN const std::wstring&                         sReferrer                = IWinHttpLib::NO_REFERER,
                                  OPTIONAL IN const IWinHttpLib::VAcceptTypesArray*       pAcceptTypesArray        = nullptr,
                                  OPTIONAL IN const DWORD                                 dwFlags                  = IWinHttpLib::DEFAULT_FLAGS
                                ) = 0;
};


