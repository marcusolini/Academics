// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.


#include "IMsXmlLib.h"

#include <objbase.h>
#include <msxml6.h>
#include <Propvarutil.h>

#include <atlbase.h> // CComBSTR
#include <sstream>
#include <mutex>
#include <new>

#pragma comment(lib, "msxml6.lib")

#include "..\\..\\Error_Checks\\ERROR_CHECKS.H"


// DECLARATIONS

class CMsXmlLib : IMsXmlLib
{
public:
     CMsXmlLib();
     virtual ~CMsXmlLib();

     HRESULT LoadXML(IN const std::wstring& xmlFilePathname);
     HRESULT GetTextXML(OUT std::wstring& xmlData);
     HRESULT GetParsedXML(IN const std::wstring& parseNode, OUT XML_NODE_MAP& xmlNodeMap);
     HRESULT ValidateXML();

     CMsXmlLib(const CMsXmlLib&) = delete;
     CMsXmlLib& operator=(const CMsXmlLib&) = delete;

private:
     HRESULT InitializeCom();
     bool m_bComInitialized = false;

     bool m_bXmlLoaded = false;

     HRESULT WalkNodeList(IN CComPtr<IXMLDOMNodeList> pNodeList, OUT XML_NODE_MAP& xmlNodeMap);
     HRESULT WalkNode(IN IXMLDOMNode* pNode, OUT XML_NODE_MAP& xmlNodeMap);

     CComPtr<IXMLDOMDocument2> m_pDoc;
     XML_NODE_MAP m_xmlNodeMap;

     std::recursive_mutex m_mutex;
     std::wostringstream m_outputStream;
};


// DEFINITIONS

/*static*/ HRESULT IMsXmlLib::CreateInstance(OUT IMsXmlLib** ppIMsXmlLib)
{
     HRESULT hrResult = ERROR_SUCCESS;

     std::wostringstream outputStream;
     CMsXmlLib* pCMsXmlLib = nullptr;

     try
     {
          if (nullptr == ppIMsXmlLib) throw E_INVALIDARG;

          CHECK_NEW_ALLOC_LOG_THROW(pCMsXmlLib = new (std::nothrow) CMsXmlLib());

          *ppIMsXmlLib = reinterpret_cast<IMsXmlLib*>(pCMsXmlLib);

     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult)
     }

     return hrResult;
}

/*static*/ HRESULT IMsXmlLib::DeleteInstance(IN IMsXmlLib* pIMsXmlLib)
{
     HRESULT hrResult = ERROR_SUCCESS;

     std::wostringstream outputStream;
     CMsXmlLib* pCMsXmlLib = nullptr;

     try
     {
     if (nullptr == pIMsXmlLib) throw E_INVALIDARG;

     pCMsXmlLib = reinterpret_cast<CMsXmlLib*>(pIMsXmlLib);

     delete pCMsXmlLib;
     pCMsXmlLib = nullptr;
     pIMsXmlLib = nullptr;
     }
     catch (HRESULT& check_catch_hresult)
     {
     hrResult = check_catch_hresult;
     LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult)
     }

     return hrResult;
}


CMsXmlLib::CMsXmlLib()
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     HRESULT hrResult = ERROR_SUCCESS;

     try
     {
     CHECK_SUCCEEDED_LOG_THROW(InitializeCom());
     m_bComInitialized = true;
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult)
          throw check_catch_hresult;  // throw
     }
}


CMsXmlLib::~CMsXmlLib()
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     if (true == m_bComInitialized)
     {
          CoUninitialize();
     }
}


HRESULT CMsXmlLib::InitializeCom()
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     HRESULT hrResult = ERROR_SUCCESS;

     if (false == m_bComInitialized)
     {
          try
          {
          CHECK_SUCCEEDED_LOG_THROW(CoInitialize(nullptr));
          m_bComInitialized = true;

          CHECK_SUCCEEDED_LOG_THROW(CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument2, (void**)&m_pDoc));

          m_pDoc->put_async(VARIANT_FALSE);
          m_pDoc->put_validateOnParse(VARIANT_TRUE);
          m_pDoc->put_resolveExternals(VARIANT_TRUE);

          }
          catch (HRESULT& check_catch_hresult)
          {
          hrResult = check_catch_hresult;
          LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult)
          }
     }

     return hrResult;
}



HRESULT CMsXmlLib::LoadXML(IN const std::wstring& xmlFilePathname)
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     HRESULT hrResult = ERROR_SUCCESS;

     try
     {
          if (nullptr == m_pDoc) throw E_INVALIDARG;

          CComBSTR bstrXML;
          CComVariant varFileName;
          VARIANT_BOOL bVarStatus = VARIANT_TRUE;

          CHECK_SUCCEEDED_LOG_THROW(InitVariantFromString(xmlFilePathname.c_str(), &varFileName));

          try {
               CHECK_SUCCEEDED_LOG_THROW(m_pDoc->load(varFileName, &bVarStatus));
               CHECK_BOOL_TRUE_LOG_THROW(VARIANT_TRUE == bVarStatus);
               m_bXmlLoaded = true;
          }
          catch (HRESULT& check_catch_hresult)
          {
               CComPtr<IXMLDOMParseError> pXMLErr;
               long errorCode = 0;
               CComBSTR bstrTemp;

               hrResult = check_catch_hresult;

               try
               {
                    CHECK_SUCCEEDED_LOG_THROW(m_pDoc->get_parseError(&pXMLErr));

                    CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_errorCode(&errorCode));

                    CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_reason(&bstrTemp));

                    LOG_MESSAGE(L"ErrorCode:0x%08X - Reason:'%s'", errorCode, bstrTemp ? bstrTemp.m_str : L"");

                    hrResult = (errorCode ? errorCode : check_catch_hresult);
                    throw hrResult;
               }
               catch (HRESULT& check_catch_hresult)
               {
                    hrResult = check_catch_hresult;
                    LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult);
                    throw hrResult;
               }
          }
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult)
     }

     return hrResult;

}

HRESULT CMsXmlLib::GetTextXML(OUT std::wstring& xmlData)
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     HRESULT hrResult = ERROR_SUCCESS;

     try
     {
          if (nullptr == m_pDoc) throw E_INVALIDARG;

          CComBSTR bstrTemp;

          CHECK_SUCCEEDED_LOG_THROW(m_pDoc->get_xml(&bstrTemp));

          xmlData = bstrTemp.m_str;
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult);
     }

     return hrResult;
}

HRESULT CMsXmlLib::ValidateXML()
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     HRESULT hrResult = ERROR_SUCCESS;

     try
     {
          if (nullptr == m_pDoc) throw E_INVALIDARG;

          CComPtr<IXMLDOMParseError> pXMLErr;
          long errorCode = 0;
          CComBSTR bstrTemp;

          try
          {
               CHECK_SUCCEEDED_LOG_THROW(m_pDoc->validate(&pXMLErr));
          }
          catch (HRESULT& check_catch_hresult)
          {
               CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_errorCode(&errorCode));

               CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_reason(&bstrTemp));

               LOG_MESSAGE(L"ErrorCode:0x%08X - Reason:'%s'", errorCode, bstrTemp ? bstrTemp.m_str : L"");

               hrResult = (errorCode ? errorCode : check_catch_hresult);
               throw hrResult;
          }
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult);
     }

     return hrResult;
}

HRESULT CMsXmlLib::GetParsedXML(IN const std::wstring& parseNode, OUT XML_NODE_MAP& xmlNodeMap)
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     HRESULT hrResult = ERROR_SUCCESS;

     m_xmlNodeMap.clear();

     try
     {
          if (nullptr == m_pDoc) throw E_INVALIDARG;

          if (0 == parseNode.size())
          {
               CHECK_SUCCEEDED_LOG_NOTHROW(WalkNode(m_pDoc, m_xmlNodeMap));
          }
          else
          {
               std::wstring tempSearch;
               CComBSTR bstrTemp;
               CComPtr<IXMLDOMNodeList> pNodes;

               tempSearch = TEXT("//") + parseNode;
               bstrTemp = tempSearch.c_str();
               CHECK_SUCCEEDED_LOG_THROW(m_pDoc->selectNodes(bstrTemp, &pNodes));
               bstrTemp.Empty();

               CHECK_SUCCEEDED_LOG_THROW(WalkNodeList(pNodes, m_xmlNodeMap));
          }

          xmlNodeMap = m_xmlNodeMap;
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult);
     }

     return hrResult;
}

HRESULT CMsXmlLib::WalkNodeList(IN CComPtr<IXMLDOMNodeList> pNodes, OUT XML_NODE_MAP& xmlNodeMap)
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     HRESULT hrResult = ERROR_SUCCESS;

     long numNodes = 0;

     CComBSTR bstrTemp;
     CComVariant varNameTemp;

     try
     {
          try {
               CHECK_SUCCEEDED_LOG_THROW(pNodes->get_length(&numNodes));
          }
          catch (HRESULT) { numNodes = 0; /*Continue*/ }

          for (long n = 0; n < numNodes; n++)
          {
               CComPtr<IXMLDOMNode> pChildNode;

               try {
                    CHECK_SUCCEEDED_LOG_THROW(pNodes->get_item(n, &pChildNode));
                    CHECK_SUCCEEDED_LOG_THROW(WalkNode(pChildNode, xmlNodeMap));
               }
               catch (HRESULT) { /*Continue*/ }
          }
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult);
     }

     return hrResult;
}
HRESULT CMsXmlLib::WalkNode(IN IXMLDOMNode* pNode, OUT XML_NODE_MAP& xmlNodeMap)
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);
     
     HRESULT hrResult = ERROR_SUCCESS;

     XML_NODE_DATA xmlNodeData;

     try
     {
          if (nullptr == pNode) throw E_INVALIDARG;
          //if (nullptr == pXmlNodeMap) throw E_INVALIDARG;

          CComPtr<IXMLDOMNamedNodeMap>  pDocAttribMap;
          long numDocAttribs = 0;

          CComPtr<IXMLDOMNodeList> pChildNodes;
          long numChildNodes = 0;

          CComBSTR bstrTemp;
          CComVariant varNameTemp;

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_nodeName(&bstrTemp));
               xmlNodeData.name = bstrTemp.m_str;
               bstrTemp.Empty();
          }
          catch (HRESULT) { /*Continue*/ }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_text(&bstrTemp));
               xmlNodeData.text = bstrTemp.m_str;
               bstrTemp.Empty();
          }
          catch (HRESULT) { /*Continue*/ }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_nodeTypeString(&bstrTemp));
               xmlNodeData.type = bstrTemp.m_str;
               bstrTemp.Empty();
          }
          catch (HRESULT) { /*Continue*/ }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_nodeValue(&varNameTemp));
               //m_outputStream << TEXT("VALUE: '") << varNameTemp << TEXT("'  ");
               varNameTemp.Clear();
          }
          catch (HRESULT) { /*Continue*/ }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_dataType(&varNameTemp));
               //m_outputStream << TEXT("DATATYPE: '") << varNameTemp << TEXT("'  ");
               varNameTemp.Clear();
          }
          catch (HRESULT) { /*Continue*/ }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_attributes(&pDocAttribMap));
          }
          catch (HRESULT) { pDocAttribMap = nullptr; /*Continue*/ }

          if (pDocAttribMap)
          {
               try {
                    CHECK_SUCCEEDED_LOG_THROW(pDocAttribMap->get_length(&numDocAttribs));
               }
               catch (HRESULT) { numDocAttribs = 0; /*Continue*/ }

               if (numDocAttribs)
               {
                    for (long a = 0; a < numDocAttribs; a++)
                    {
                         CComPtr<IXMLDOMNode> pDocAttrib;

                         XML_NODE_ATTRIB_DATA xmlNodeAttribData;
                          
                         try {
                              CHECK_SUCCEEDED_LOG_THROW(pDocAttribMap->get_item(a, &pDocAttrib));

                              try {
                                   CHECK_SUCCEEDED_LOG_THROW(pDocAttrib->get_nodeName(&bstrTemp));
                                   xmlNodeAttribData.name = bstrTemp.m_str;
                                   bstrTemp.Empty();
                              }
                              catch (HRESULT) { /*Continue*/ }

                              try {
                                   CHECK_SUCCEEDED_LOG_THROW(pDocAttrib->get_text(&bstrTemp));
                                   xmlNodeAttribData.text = bstrTemp.m_str;
                                   bstrTemp.Empty();
                              }
                              catch (HRESULT) { /*Continue*/ }

                              xmlNodeData.attribs.insert(XML_NODE_ATTRIB_PAIR(xmlNodeAttribData.name, xmlNodeAttribData));
                         }
                         catch (HRESULT) { /*Continue*/ }
                    }
               }
          }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_childNodes(&pChildNodes));
          }
          catch (HRESULT) { pChildNodes = nullptr; /*Continue*/ }

          if (pChildNodes)
          {
               try {
                    CHECK_SUCCEEDED_LOG_THROW(pChildNodes->get_length(&numChildNodes));
               }
               catch (HRESULT) { numChildNodes = 0; /*Continue*/ }

               for (long n = 0; n < numChildNodes; n++)
               {
                    CComPtr<IXMLDOMNode> pChildNode;

                    try {
                         CHECK_SUCCEEDED_LOG_THROW(pChildNodes->get_item(n, &pChildNode));
                         CHECK_SUCCEEDED_LOG_THROW(WalkNode(pChildNode, xmlNodeData.children));
                    }
                    catch (HRESULT) { /*Continue*/ }
               }
          }

          xmlNodeMap.insert(XML_NODE_PAIR(xmlNodeData.name, xmlNodeData));

     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult);
     }

     return hrResult;
}