#ifndef __IHKEY_INCLUDE__
#define __IHKEY_INCLUDE__




#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <Tlhelp32.h>
#include <filesystem>
#include <thread>
#include <mutex>
#include <queue>
#include <memory>
#include <exception>
#include <future>
#include <chrono>
#include <setupapi.h>
#include <synchapi.h>
#include <Winternl.h>
#include <chrono>
#include <restartmanager.h>
#include <ctime> 
#include <stdexcept>
#include <exception>
#include <lmshare.h>
#include <lm.h>
#include <tchar.h>
#include <vector>
#include <ioapiset.h>
#include <shlwapi.h>
#include <psapi.h>
#include <wininet.h>
#include <winuser.h>
#include <comdef.h>



#include <cryptopp/rsa.h>
#include <cryptopp/default.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/queue.h>
#include <cryptopp/base64.h>
#include <cryptopp/hex.h>
#include <cryptopp/crc.h>
#include <cryptopp/filters.h>
#include <cryptopp/channels.h>


#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Mpr.lib")
#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "rstrtmgr.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "powrprof.lib")
#pragma comment(lib, "Slwga.lib")
#pragma comment(lib, "ntdll.lib")




using namespace CryptoPP;
using namespace boost::filesystem;
using std::cout;
using std::string;
using std::cin;
using std::endl;

#endif // !__IHKEY_INCLUDE__


