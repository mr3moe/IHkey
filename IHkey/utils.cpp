#include "utils.h"
#include "config.h"


namespace utils
{


    void error(DWORD err)
    {
        WCHAR buf[0x300];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0, buf, 0x300, NULL);
        wprintf(L"%x: %s\n", err, buf);
    }


    std::string concatVector(std::vector<std::string> vec, std::string delimiter)
    {
        std::string accumulator = "";
        for (size_t i = 0; i < vec.size() - 1; ++i) {
            accumulator += vec.at(i) + delimiter;
        }
        return accumulator + vec.at(vec.size() - 1);
    }

    void DebuggerVerbos()
    {
#ifdef DEBUG
        error(GetLastError());
#endif // DEBUG
    }

    void AntiVmDebugger(const std::string FuncNm, DWORD ProcId)
    {
#ifdef DEBUG
        std::cout << " AntiVmDebugger * " << " Fired By " << FuncNm << "  * PocId   " << ProcId;
#endif // DEBUG
    }


    void AntiDBDebugger(const std::string FuncNm)
    {
#ifdef DEBUG
        std::cout <<  "AntiDBDebugger * " << " Fired By " << FuncNm;
#endif // DEBUG

       
    }

    wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
    {
        wchar_t* wString = new wchar_t[8096];
        MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 8096);
        return wString;
    }
 
}
