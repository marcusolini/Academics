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

     HRESULT LoadXML(const std::wstring& xmlFilePathname);
     HRESULT GetTextXML(std::wstring& xmlData);
     HRESULT ValidateXML();
     HRESULT GetParsedXML(XML_NODE_MAP& xmlNodeMap);

     CMsXmlLib(const CMsXmlLib&) = delete;
     CMsXmlLib& operator=(const CMsXmlLib&) = delete;

private:
     HRESULT InitializeCom();
     bool m_bComInitialized = false;

     bool m_bXmlLoaded = false;

     HRESULT WalkNode(IXMLDOMNode* pNode, XML_NODE_MAP* pXmlNodeMap);

     CComPtr<IXMLDOMDocument2> m_pDoc;
     XML_NODE_MAP m_xmlNodeMap;

     std::recursive_mutex m_mutex;
     std::wostringstream m_outputStream;
};


// DEFINITIONS

/*static*/ HRESULT IMsXmlLib::CreateInstance(IMsXmlLib** ppIMsXmlLib)
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

          outputStream << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult;
          LOG_MESSAGE(outputStream.str());
          outputStream.str(TEXT(""));
     }

     return hrResult;
}

/*static*/ HRESULT IMsXmlLib::DeleteInstance(IMsXmlLib* pIMsXmlLib)
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

     outputStream << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult;
     LOG_MESSAGE(outputStream.str());
     outputStream.str(TEXT(""));
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

          m_outputStream << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult;
          LOG_MESSAGE(m_outputStream.str());
          m_outputStream.str(TEXT(""));
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

          m_outputStream << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult;
          LOG_MESSAGE(m_outputStream.str());
          m_outputStream.str(TEXT(""));
          }
     }

     return hrResult;
}



HRESULT CMsXmlLib::LoadXML(const std::wstring& xmlFilePathname)
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
                    m_outputStream << TEXT("ErrorCode: '") << std::hex << std::showbase << errorCode;

                    CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_reason(&bstrTemp));
                    m_outputStream << TEXT("  Reason: '") << bstrTemp.m_str;
                    bstrTemp.Empty();

                    //CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_srcText(&bstrTemp));
                    //outputStream << TEXT("SrcText: '") << bstrTemp.m_str << std::endl;
                    //bstrTemp.Empty();

                    LOG_MESSAGE(m_outputStream.str());
                    m_outputStream.str(TEXT(""));

                    hrResult = (errorCode ? errorCode : check_catch_hresult);
                    throw hrResult;
               }
               catch (HRESULT& check_catch_hresult)
               {
                    hrResult = check_catch_hresult;

                    m_outputStream << TEXT("XML load parse failure: ") << std::hex << std::showbase << check_catch_hresult;
                    LOG_MESSAGE(m_outputStream.str());
                    m_outputStream.str(TEXT(""));

                    throw hrResult;
               }
          }
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          m_outputStream << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult;
          LOG_MESSAGE(m_outputStream.str());
          m_outputStream.str(TEXT(""));
     }

     return hrResult;

}

HRESULT CMsXmlLib::GetTextXML(std::wstring& xmlData)
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
          m_outputStream << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult;
          LOG_MESSAGE(m_outputStream.str());
          m_outputStream.str(TEXT(""));
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
               m_outputStream << TEXT("ErrorCode: '") << std::hex << std::showbase << errorCode;

               CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_reason(&bstrTemp));
               m_outputStream << TEXT("  Reason: '") << bstrTemp.m_str;
               bstrTemp.Empty();

               //CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_srcText(&bstrTemp));
               //outputStream << TEXT("SrcText: '") << bstrTemp.m_str << std::endl;
               //bstrTemp.Empty();

               LOG_MESSAGE(m_outputStream.str());
               m_outputStream.str(TEXT(""));

               hrResult = (errorCode ? errorCode : check_catch_hresult);
               throw hrResult;
          }
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          m_outputStream << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult;
          LOG_MESSAGE(m_outputStream.str());
          m_outputStream.str(TEXT(""));
     }

     return hrResult;
}

HRESULT CMsXmlLib::GetParsedXML(XML_NODE_MAP& xmlNodeMap)
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);

     HRESULT hrResult = ERROR_SUCCESS;

     m_xmlNodeMap.clear();

     try
     {
          if (nullptr == m_pDoc) throw E_INVALIDARG;

          CHECK_SUCCEEDED_LOG_NOTHROW(WalkNode(m_pDoc, &m_xmlNodeMap));

          xmlNodeMap = m_xmlNodeMap;
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          m_outputStream << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult;
          LOG_MESSAGE(m_outputStream.str());
          m_outputStream.str(TEXT(""));
     }

     return hrResult;
}

HRESULT CMsXmlLib::WalkNode(IXMLDOMNode* pNode, XML_NODE_MAP* pXmlNodeMap)
{
     std::lock_guard<std::recursive_mutex> lock(m_mutex);
     
     HRESULT hrResult = ERROR_SUCCESS;

     XML_NODE_DATA xmlNodeData;

     try
     {
          if (nullptr == pNode) throw E_INVALIDARG;
          if (nullptr == pXmlNodeMap) throw E_INVALIDARG;

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
                         CHECK_SUCCEEDED_LOG_THROW(WalkNode(pChildNode, &xmlNodeData.children));
                    }
                    catch (HRESULT) { /*Continue*/ }
               }
          }

          pXmlNodeMap->insert(XML_NODE_PAIR(xmlNodeData.name, xmlNodeData));

     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
          m_outputStream << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult;
          LOG_MESSAGE(m_outputStream.str());
          m_outputStream.str(TEXT(""));
     }

     return hrResult;
}