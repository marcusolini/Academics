#ifndef TSTRING_H
#define TSTRING_H

#ifdef _WIN32
    #include <tchar.h>
    #pragma warning(disable : 4996) // for _vsnwprintf
#endif

#include <wchar.h>
#include <string>    
#include <fstream>
#include <sstream>
    

#ifdef _UNICODE
    #ifndef _TEXT
        #define _TEXT(x)      L ## x
    #endif
    #ifndef TCHAR
        #define TCHAR       wchar_t
    #endif
    #define TSTRING         std::wstring
    #define TO_TSTRING      std::to_wstring
    #define TCERR           std::wcerr
    #define TCOUT           std::wcout
    #define TOSTREAM        std::wostream
    #define TISTREAM        std::wistream
    #define TIOSTREAM       std::wiostream
    #define TIFSTREAM       std::wifstream
    #define TOFSTREAM       std::wofstream
    #define TFSTEAM         std::wfstream
    #define TSTRINGSTREAM   std::wstringstream
    #define TOSTRINGSTREAM  std::wostringstream
    #define TMEMCPY_S       wmemcpy_s
    #define TVSNPRINTF      _vsnwprintf   
    #define TSNPRINTF_S      _snwprintf
    #define TSPRINTF        swprintf
#else
    #ifndef _TEXT
        #define _TEXT(x)    x
    #endif
    #ifndef TCHAR
        #define TCHAR       char
    #endif
    #define TSTRING         std::string
    #define TO_TSTRING      std::to_string
    #define TCERR           std::cerr
    #define TCOUT           std::cout
    #define TOSTREAM        std::ostream
    #define TISTREAM        std::istream
    #define TIOSTREAM       std::iostream
    #define TIFSTREAM       std::ifstream
    #define TOFSTREAM       std::ofstream
    #define TFSTEAM         std::fstream
    #define TSTRINGSTREAM   std::stringstream
    #define TOSTRINGSTREAM  std::ostringstream
    #define TMEMCPY_S       memcpy_s
    #define TVSNPRINTF      vsnprintf               
    #define TSNPRINTF_S     snprintf
    #define TSPRINTF        sprintf 

#endif



#endif // TSTRING_H