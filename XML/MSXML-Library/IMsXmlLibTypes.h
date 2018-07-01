// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <SDKDDKVer.h>
#include <windows.h>

#include <string>
#include <map>

// DECLARATIONS

class IMsXmlLib;

using XML_STRING = std::wstring;

struct XML_NODE_DATA;
using XML_NODE_PAIR = std::pair<XML_STRING, XML_NODE_DATA>;
using XML_NODE_MAP = std::multimap<XML_STRING, XML_NODE_DATA>;

struct XML_NODE_ATTRIB_DATA;
using XML_NODE_ATTRIB_MAP = std::multimap<XML_STRING, XML_NODE_ATTRIB_DATA>;
using XML_NODE_ATTRIB_PAIR = std::pair<XML_STRING, XML_NODE_ATTRIB_DATA>;

// DEFINITIONS

struct XML_NODE_DATA
{
     XML_STRING name;
     XML_STRING text;
     XML_STRING type;
     XML_NODE_ATTRIB_MAP attribs;
     XML_NODE_MAP children;
};

struct XML_NODE_ATTRIB_DATA
{
     XML_STRING name;
     XML_STRING text;
};