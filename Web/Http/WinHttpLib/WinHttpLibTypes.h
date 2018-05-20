#pragma once

#include <windows.h>
#include <string>
#include <vector>


enum class ERequestFlags : DWORD {
     Secure = 0x00800000,
     EscapePercent = 0x00000004,
     NullCodepage = 0x00000008,
     BypassProxyCache = 0x00000100,
     Refresh = BypassProxyCache,
     EscapeDisable = 0x00000040,
     EscapeDisableQuery = 0x00000080
};


constexpr DWORD operator| (const ERequestFlags& lhs, const ERequestFlags& rhs)
{
     return (static_cast<DWORD>(lhs) | static_cast<DWORD>(rhs));
}
