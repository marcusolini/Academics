// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.


#pragma once

#include "IMsXmlLIbTypes.h"

class IMsXmlLib
{
public:
     static HRESULT CreateInstance(OUT IMsXmlLib** ppMsXmlLib);
     static HRESULT DeleteInstance(IN IMsXmlLib* ppMsXmlLib);
     
     virtual HRESULT LoadXML(IN const std::wstring& xmlFilePathname) = 0;
     virtual HRESULT GetTextXML(IN std::wstring& xmlData) = 0;
     virtual HRESULT GetParsedXML(IN const std::wstring& parseNode, OUT XML_NODE_MAP& xmlNodeMap) = 0;

     virtual HRESULT ValidateXML() = 0;
};


