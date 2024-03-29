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

#include "..\..\Error_Checks\ERROR_CHECKS.H"

HRESULT PromptForXML(OUT std::wstring& sXmlPath);
HRESULT LoadXML(IN CComPtr<IXMLDOMDocument2>& pDoc, IN const std::wstring& xmlFilePathname);
HRESULT OutputXML(IN CComPtr<IXMLDOMDocument2>& pDoc);
HRESULT ValidateXML(IN CComPtr<IXMLDOMDocument2>& pDoc);
HRESULT WalkNode(IN IXMLDOMNode* pNode);


int wmain()
{
     HRESULT hr = ERROR_SUCCESS;
     bool bComInitalized = false;

     std::wostringstream outputStream;

     try
     {
          CComPtr<IXMLDOMDocument2> pDoc;

          CComBSTR bstrXML;
          CComVariant varFileName;

          std::wstring xmlFilePathname;

          CHECK_SUCCEEDED_LOG_THROW(CoInitialize(nullptr));
          bComInitalized = true;

          CHECK_SUCCEEDED_LOG_THROW(hr = CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument2, (void**)&pDoc));

          pDoc->put_async(VARIANT_FALSE);
          pDoc->put_validateOnParse(VARIANT_TRUE);
          pDoc->put_resolveExternals(VARIANT_TRUE);

          CHECK_SUCCEEDED_LOG_THROW(PromptForXML(xmlFilePathname));

          CHECK_SUCCEEDED_LOG_THROW(LoadXML(pDoc, xmlFilePathname));

          CHECK_SUCCEEDED_LOG_NOTHROW(OutputXML(pDoc)); // Continue

          CHECK_SUCCEEDED_LOG_NOTHROW(ValidateXML(pDoc)); // Continue
      
          CHECK_SUCCEEDED_LOG_NOTHROW(WalkNode(pDoc)); // Continue

     }
     catch (HRESULT& check_catch_hresult)
     {
          hr = check_catch_hresult;
          LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult);
     }

     if (bComInitalized) CoUninitialize();

     return hr;
}

HRESULT PromptForXML(OUT std::wstring& sXmlPath)
{
     HRESULT hr = ERROR_SUCCESS;

     sXmlPath.clear();

     std::wcout << TEXT("Enter XML File Pathname: ");
     std::wcin >> sXmlPath;

     return hr;
}

HRESULT LoadXML(IN CComPtr<IXMLDOMDocument2>& pDoc, IN const std::wstring& xmlFilePathname)
{
     HRESULT hr = ERROR_SUCCESS;

     std::wostringstream outputStream;

     try
     {
          if (nullptr == pDoc) throw E_INVALIDARG;

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

                    CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_reason(&bstrTemp));

                    LOG_MESSAGE(L"ErrorCode:0x%08X - Reason:'%s'", errorCode, bstrTemp ? bstrTemp.m_str : L"");

                    hr = (errorCode ? errorCode : check_catch_hresult);
                    throw hr;
               }
               catch (HRESULT& check_catch_hresult)
               {
                    hr = check_catch_hresult;
                    LOG_MESSAGE(L"XML load parse failure:0x%08X", check_catch_hresult);
                    throw hr;
               }
          }
     }
     catch (HRESULT& check_catch_hresult)
     {
          hr = check_catch_hresult;
          LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult);
     }

     return hr;

}

HRESULT OutputXML(IN CComPtr<IXMLDOMDocument2>& pDoc)
{
     HRESULT hr = ERROR_SUCCESS;

     std::wostringstream outputStream;

     try
     {
          if (nullptr == pDoc) throw E_INVALIDARG;

          CComBSTR bstrTemp;

          CHECK_SUCCEEDED_LOG_THROW(pDoc->get_xml(&bstrTemp));
 
          LOG_MESSAGE(L"XML:'%s'", bstrTemp ? bstrTemp.m_str : L"");
     }
     catch (HRESULT& check_catch_hresult)
     {
          hr = check_catch_hresult;
          LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult);
     }

     return hr;
}

HRESULT ValidateXML(IN CComPtr<IXMLDOMDocument2>& pDoc)
{
     HRESULT hr = ERROR_SUCCESS;

     std::wostringstream outputStream;

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

               CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_reason(&bstrTemp));

               LOG_MESSAGE(L"ErrorCode:0x%08X - Reason:'%s'", errorCode, bstrTemp ? bstrTemp.m_str : L""); 

               hr = (errorCode ? errorCode: check_catch_hresult);
               throw hr;
          }
     }
     catch (HRESULT& check_catch_hresult)
     {
          hr = check_catch_hresult;
          LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult);
     }

     return hr;
}

HRESULT WalkNode(IN IXMLDOMNode* pNode)
{
     HRESULT hr = ERROR_SUCCESS;

     std::wostringstream outputStream;

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

          LOG_MESSAGE(L"\n\nNODE >>>>>>>> ")

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
          LOG_MESSAGE(L"%s", outputStream.str());
          outputStream.str(TEXT(""));

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
                    outputStream << TEXT("ATTRIBUTES - ");

                    for (long a = 0; a < numDocAttribs; a++)
                    {
                         CComPtr<IXMLDOMNode> pDocAttrib;

                         try {
                              CHECK_SUCCEEDED_LOG_THROW(pDocAttribMap->get_item(a, &pDocAttrib));
     
                              try {
                                   CHECK_SUCCEEDED_LOG_THROW(pDocAttrib->get_nodeName(&bstrTemp));
                                   outputStream << TEXT("NAME: '") << bstrTemp.m_str << TEXT("'  ");
                                   bstrTemp.Empty();
                              }
                              catch (HRESULT) { /*Continue*/ }

                              try{
                                   CHECK_SUCCEEDED_LOG_THROW(pDocAttrib->get_text(&bstrTemp));
                                   outputStream << TEXT("TEXT: '") << bstrTemp.m_str << TEXT("'  ");
                                   bstrTemp.Empty();
                              }
                              catch (HRESULT) { /*Continue*/ }
                         }
                         catch (HRESULT) { /*Continue*/ }
                    }

                    LOG_MESSAGE(L"%s", outputStream.str());
                    outputStream.str(TEXT(""));
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
                         CHECK_SUCCEEDED_LOG_THROW(WalkNode(pChildNode));
                    }
                    catch (HRESULT) { /*Continue*/ }

               }
          }

     }
     catch (HRESULT& check_catch_hresult)
     {
          hr = check_catch_hresult;
          LOG_MESSAGE(L"Fatal Error:0x%08X", check_catch_hresult);
     }

     return hr;
}