// BasicLogger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "log.h"
#include "CreateLogger.h"

int main()
{
     LOG("This is a test", 200);

     CCreateLogger::CreateConsoleLogger();

     LOG("New test", " ", 777, " ", "stuff");

     CCreateLogger::CreateFileLogger("C:\\test\\stuff.log");

     LOG("New more test ", 999, " morestuff");

     CCreateLogger::DeleteConsoleLogger();
     CCreateLogger::DeleteFileLogger();

    return 0;
}

