// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//

#include "stdafx.h"

#include <iostream>
#include <sstream>

#include <objbase.h>
#include <msxml6.h>
#include <Propvarutil.h>

#include <atlbase.h> // CComBSTR

#pragma comment(lib, "msxml6.lib")

#include "ERROR_CHECKS.H"

HRESULT PromptForXML(std::wstring& sXmlPath);
HRESULT LoadXML(IXMLDOMDocument2* pDoc, const std::wstring& xmlFilePathname);
HRESULT OutputXML(IXMLDOMDocument2* pDoc);
HRESULT ValidateXML(IXMLDOMDocument2* pDoc);
HRESULT WalkNode(IXMLDOMNode* pNode);

int wmain()
{
     HRESULT hr = ERROR_SUCCESS;
     bool bComInitalized = false;

     try
     {
          CComPtr<IXMLDOMDocument2> pDoc;

          CComBSTR bstrXML;
          CComVariant varFileName;

          CHECK_SUCCEEDED_LOG_THROW(CoInitialize(nullptr));
          bComInitalized = true;

          CHECK_SUCCEEDED_LOG_THROW(hr = CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument2, (void**)&pDoc));

          pDoc->put_async(VARIANT_FALSE);
          pDoc->put_validateOnParse(VARIANT_TRUE);
          pDoc->put_resolveExternals(VARIANT_TRUE);

          std::wstring xmlFilePathname;

          hr = PromptForXML(xmlFilePathname);

          CHECK_SUCCEEDED_LOG_THROW(LoadXML(pDoc, xmlFilePathname));

          CHECK_SUCCEEDED_LOG_NOTHROW(OutputXML(pDoc)); // Continue

          CHECK_SUCCEEDED_LOG_NOTHROW(ValidateXML(pDoc)); // Continue

          CHECK_SUCCEEDED_LOG_NOTHROW(WalkNode(pDoc)); // Continue

     }
     catch (HRESULT& check_catch_hresult)
     {
          hr = check_catch_hresult;
          std::wcout << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult << std::endl;
     }

     if (bComInitalized) CoUninitialize();

     return hr;
}

HRESULT PromptForXML(std::wstring& sXmlPath)
{
     HRESULT hr = ERROR_SUCCESS;

     sXmlPath.clear();

     std::wcout << TEXT("Enter XML File Pathname: ");
     std::wcin >> sXmlPath;

     return hr;
}

HRESULT LoadXML(IXMLDOMDocument2* pDoc, const std::wstring& xmlFilePathname)
{
     HRESULT hr = ERROR_SUCCESS;

     try
     {
          if (nullptr == pDoc) throw E_INVALIDARG;

          std::wostringstream outputStream;

          CComBSTR bstrXML;
          CComVariant varFileName;
          VARIANT_BOOL bVarStatus = VARIANT_TRUE;

          CHECK_SUCCEEDED_LOG_THROW(InitVariantFromString(xmlFilePathname.c_str(), &varFileName));

          try{
          CHECK_SUCCEEDED_LOG_THROW(pDoc->load(varFileName, &bVarStatus));
          CHECK_BOOL_TRUE_LOG_THROW(VARIANT_TRUE == bVarStatus);
          }
          catch (HRESULT& check_catch_hresult)
          {
               CComPtr<IXMLDOMParseError> pXMLErr;
               long errorCode = 0;
               CComBSTR bstrTemp;

               hr = check_catch_hresult;

               try
               {
                    CHECK_SUCCEEDED_LOG_THROW(pDoc->get_parseError(&pXMLErr));

                    CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_errorCode(&errorCode));
                    outputStream << TEXT("ErrorCode: '") << std::hex << std::showbase << errorCode;

                    CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_reason(&bstrTemp));
                    outputStream << TEXT("  Reason: '") << bstrTemp.m_str;
                    bstrTemp.Empty();

                    //CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_srcText(&bstrTemp));
                    //outputStream << TEXT("SrcText: '") << bstrTemp.m_str << std::endl;
                    //bstrTemp.Empty();

                    std::wcout << outputStream.str() << std::endl;

                    hr = (errorCode ? errorCode : check_catch_hresult);
                    throw hr;
               }
               catch (HRESULT& check_catch_hresult)
               {
                    hr = check_catch_hresult;
                    std::wcout << TEXT("XML load parse failure: ") << std::hex << std::showbase << check_catch_hresult << std::endl;
                    throw hr;
               }
          }
     }
     catch (HRESULT& check_catch_hresult)
     {
          hr = check_catch_hresult;
          std::wcout << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult << std::endl;
     }

     return hr;

}

HRESULT OutputXML(IXMLDOMDocument2* pDoc)
{
     HRESULT hr = ERROR_SUCCESS;

     try
     {
          if (nullptr == pDoc) throw E_INVALIDARG;

          CComBSTR bstrTemp;

          CHECK_SUCCEEDED_LOG_THROW(pDoc->get_xml(&bstrTemp));
 
          std::wcout << std::endl << TEXT("XML:") << bstrTemp.m_str << std::endl << std::endl;
     }
     catch (HRESULT& check_catch_hresult)
     {
          hr = check_catch_hresult;
          std::wcout << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult << std::endl;
     }

     return hr;
}

HRESULT ValidateXML(IXMLDOMDocument2* pDoc)
{
     HRESULT hr = ERROR_SUCCESS;

     try
     {
          if (nullptr == pDoc) throw E_INVALIDARG;

          std::wostringstream outputStream;

          CComPtr<IXMLDOMParseError> pXMLErr;
          long errorCode = 0;
          CComBSTR bstrTemp;

          try
          {
               CHECK_SUCCEEDED_LOG_THROW(pDoc->validate(&pXMLErr));
          }
          catch (HRESULT& check_catch_hresult)
          {
               CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_errorCode(&errorCode));
               outputStream << TEXT("ErrorCode: '") << std::hex << std::showbase << errorCode;

               CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_reason(&bstrTemp));
               outputStream << TEXT("  Reason: '") << bstrTemp.m_str;
               bstrTemp.Empty();

               //CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_srcText(&bstrTemp));
               //outputStream << TEXT("SrcText: '") << bstrTemp.m_str << std::endl;
               //bstrTemp.Empty();

               std::wcout << outputStream.str() << std::endl;

               hr = (errorCode ? errorCode: check_catch_hresult);
               throw hr;
          }
     }
     catch (HRESULT& check_catch_hresult)
     {
          hr = check_catch_hresult;
          std::wcout << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult << std::endl;
     }

     return hr;
}

HRESULT WalkNode(IXMLDOMNode* pNode)
{
     HRESULT hr = ERROR_SUCCESS;

     try
     {
          if (nullptr == pNode) throw E_INVALIDARG;

          CComPtr<IXMLDOMNamedNodeMap>  pDocAttribMap;
          long numDocAttribs = 0;

          CComPtr<IXMLDOMNodeList> pChildNodes;
          long numChildNodes = 0;

          std::wostringstream outputStream;

          CComBSTR bstrTemp;
          CComVariant varNameTemp;

          std::wcout << std::endl << std::endl;

          std::wcout << TEXT("NODE >>>>>>>> ") << std::endl;

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_nodeName(&bstrTemp));
               outputStream << TEXT("NAME: '") << bstrTemp.m_str << TEXT("'  ");
               bstrTemp.Empty();
          }
          catch (HRESULT) { /*Continue*/ }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_text(&bstrTemp));
               outputStream << TEXT("TEXT: '") << bstrTemp.m_str << TEXT("'  ");
               bstrTemp.Empty();
          }
          catch (HRESULT) { /*Continue*/ }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_nodeTypeString(&bstrTemp));
               outputStream << TEXT("TYPE: '") << bstrTemp.m_str << TEXT("'  ");
               bstrTemp.Empty();
          }
          catch (HRESULT) { /*Continue*/ }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_nodeValue(&varNameTemp));
               //outputStream << TEXT("VALUE: '") << varNameTemp << TEXT("'  ");
               varNameTemp.Clear();
          }
          catch (HRESULT) { /*Continue*/ }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_dataType(&varNameTemp));
               //outputStream << TEXT("DATATYPE: '") << varNameTemp << TEXT("'  ");
               varNameTemp.Clear();
          }
          catch (HRESULT) { /*Continue*/ }

          outputStream << std::endl;
          std::wcout << outputStream.str();
          outputStream.str(TEXT(""));

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_attributes(&pDocAttribMap));
          }
          catch (HRESULT) { /*Continue*/ }

          if (pDocAttribMap)
          {
               try {
                    CHECK_SUCCEEDED_LOG_THROW(pDocAttribMap->get_length(&numDocAttribs));
               }
               catch (HRESULT) { numDocAttribs = 0; /*Continue*/ }

               if (numDocAttribs)
               {
                    outputStream << TEXT("ATTRIBUTES - ");

                    for (long a = 0; a < numDocAttribs; a++)
                    {
                         CComPtr<IXMLDOMNode> pDocAttrib;

                         try {
                              CHECK_SUCCEEDED_LOG_THROW(pDocAttribMap->get_item(a, &pDocAttrib));
                              CHECK_SUCCEEDED_LOG_THROW(pDocAttrib->get_nodeName(&bstrTemp));
                              outputStream << TEXT("NAME: '") << bstrTemp.m_str << TEXT("'  ");
                              bstrTemp.Empty();
                         }
                         catch (HRESULT) { /*Continue*/ }

                         try {
                              CHECK_SUCCEEDED_LOG_THROW(pDocAttrib->get_text(&bstrTemp));
                              outputStream << TEXT("TEXT: '") << bstrTemp.m_str << TEXT("'  ");
                              bstrTemp.Empty();
                         }
                         catch (HRESULT) { /*Continue*/ }
                    }

                    std::wcout << outputStream.str();
                    outputStream.str(TEXT(""));
               }
          }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_childNodes(&pChildNodes));
          }
          catch (HRESULT) { /*Continue*/ }

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
                         CHECK_SUCCEEDED_LOG_THROW(WalkNode(pChildNode));
                    }
                    catch (HRESULT) { /*Continue*/ }

               }
          }

     }
     catch (HRESULT& check_catch_hresult)
     {
          hr = check_catch_hresult;
          std::wcout << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult << std::endl;
     }

     return hr;
}