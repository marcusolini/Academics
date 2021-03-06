// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

// BasicLogger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "log.h"
#include "CreateLogger.h"

int main()
{
     LOG("Should not Log anywhere", 200);

     std::shared_ptr< CLogger < CConsoleLogPolicy > > consoleLogger = CCreateLogger::CreateConsoleLogger();

     LOG("Should log to console only", " ", 777, " ", "stuff");

     std::shared_ptr< CLogger < CFileLogPolicy > > fileLogger = CCreateLogger::CreateFileLogger("C:\\test\\stuff.log");

     LOG("Should log to console and file ", 999, " morestuff");

     LOG_START_TRACE("START TRACE OUT");
     LOG("INFO - OUT");
     LOG_WARNING("WARNING OUT");
     LOG_ERROR("ERROR OUT");
     LOG_END_TRACE("END TRACE OUT");

     CCreateLogger::DeleteConsoleLogger( consoleLogger );
     CCreateLogger::DeleteFileLogger( fileLogger );

    return 0;
}

