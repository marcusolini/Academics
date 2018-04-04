// Logger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "log.h"

int wmain()
{
     LOG("Starting the application..");
     for (short i = 0; i < 3; i++)
     {
          LOG("The value of 'i' is ", i, ". ", 3 - i - 1, " more iterations left ");
     }
     LOG_WARN("Loop over");
     LOG_ERR("All good things come to an end.. :(");
     return 0;
}