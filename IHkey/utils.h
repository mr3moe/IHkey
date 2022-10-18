#ifndef __IHKEY_UTILS__
#define __IHKEY_UTILS__

#include "include.h"

#if _WIN32 || _WIN64
#if _WIN64
    #define ENV64BIT
#else
    #define ENV32BIT
#endif
#endif



#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))
#define	MyHeapAlloc(x)	HeapAlloc(GetProcessHeap(), 0, x)
#define MyHeapFree(x)		HeapFree(GetProcessHeap(), 0, x)


namespace utils
{
    void error(DWORD err);
    void DebuggerVerbos();
    void AntiVmDebugger(const std::string FuncNm, DWORD ProcId);
    void AntiDBDebugger(const std::string FuncNm);
    wchar_t* convertCharArrayToLPCWSTR(const char* charArray);
  
}



#endif