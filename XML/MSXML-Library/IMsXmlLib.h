// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.


#pragma once

#include "IMsXmlLIbTypes.h"

class IMsXmlLib
{
public:
     static HRESULT CreateInstance(IMsXmlLib** ppMsXmlLib);
     static HRESULT DeleteInstance(IMsXmlLib* ppMsXmlLib);
     
     virtual HRESULT LoadXML(const std::wstring& xmlFilePathname) = 0;
     virtual HRESULT GetTextXML(std::wstring& xmlData) = 0;
     virtual HRESULT GetParsedXML(XML_NODE_MAP& xmlNodeMap) = 0;
     virtual HRESULT ValidateXML() = 0;
};


