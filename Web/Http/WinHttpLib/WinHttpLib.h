#pragma once

#include "WinHttpLibTypes.h"

class IWinHttpLib
{
public:

     static HRESULT CreateInstance(IWinHttpLib** ppiWinHttpLib);
     static HRESULT DeleteInstance(IWinHttpLib* piWinHttpLib);

     enum class EProxyAccessType { Default, NoProxy, NamedProxy, AutomaticProxy };
     enum class EVerb { Get, Post, Put, Delete, Head, Options, Patch, Connect, Trace };

     // OPTIONAL PARAMETER DEFAULTS
     static const IWinHttpLib::EProxyAccessType DEFAULT_PROXY_ACCESS_TYPE = IWinHttpLib::EProxyAccessType::Default;
     static const std::wstring DEFAULT_APP_NAME;
     static const WORD DEFAULT_SERVER_PORT = 0;
     static const std::wstring NO_PROXY_NAME;
     static const std::wstring NO_PROXY_BYPASS;
     static const std::wstring DEFAULT_VERSION;
     static const std::wstring NO_REFERER;


     virtual HRESULT Open( OPTIONAL IN const std::wstring& sApplicationName                    = IWinHttpLib::DEFAULT_APP_NAME,
                           OPTIONAL IN const IWinHttpLib::EProxyAccessType eProxyAccessType    = IWinHttpLib::DEFAULT_PROXY_ACCESS_TYPE,
                           OPTIONAL IN const std::wstring& sProxyName                          = IWinHttpLib::NO_PROXY_NAME,
                           OPTIONAL IN const std::wstring& sProxyBypassList                    = IWinHttpLib::NO_PROXY_BYPASS
                         ) = 0;

     virtual HRESULT Connect( IN const std::wstring& sServerName, 
                              OPTIONAL IN const WORD wServerPort      = IWinHttpLib::DEFAULT_SERVER_PORT
                            ) = 0;


     virtual HRESULT OpenRequest( IN const IWinHttpLib::EVerb eVerb,
                                  IN const std::wstring sObjectPathName,
                                  OPTIONAL IN const std::wstring sVersion            = IWinHttpLib::DEFAULT_VERSION,
                                  OPTIONAL IN const std::wstring sReferrer           = IWinHttpLib::NO_REFERER
                                ) = 0;
};
