#pragma once

#include <minwindef.h>

class MsXmlLIb
{
public:
     MsXmlLIb();
     virtual ~MsXmlLIb();

     HRESULT LoadXML(IN const std::wstring& xmlFilePathname);
     HRESULT OutputXML(OUT std::wstring& xmlData);
     HRESULT ValidateXML();


private:
     //HRESULT WalkNode(IN IXMLDOMNode* pNode);


};

