#pragma once

#define WIN32_LEAN_AND_MEAN
#include <SDKDDKVer.h>
#include <windows.h>

#include <string>

class IMsXmlLib
{
public:
     static HRESULT CreateInstance(IMsXmlLib** ppMsXmlLib);
     static HRESULT DestoryInstance(IMsXmlLib* ppMsXmlLib);
     virtual HRESULT LoadXML(const std::wstring& xmlFilePathname) = 0;
     virtual HRESULT OutputXML(std::wstring& xmlData) = 0;
     virtual HRESULT ValidateXML() = 0;
};