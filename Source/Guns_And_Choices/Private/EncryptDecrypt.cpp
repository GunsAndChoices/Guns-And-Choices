#include "EncryptDecrypt.h"
#include "Misc/Base64.h"
#include <openssl/evp.h>
#include <openssl/aes.h>

FString UEncryptDecrypt::EncryptAES(const FString& PlainText, const FString& Password)
{
	// Passwort in 32-Byte-Key umwandeln (AES-256)
	TArray<uint8> KeyBytes;
	FTCHARToUTF8 KeyConvert(*Password);
	KeyBytes.Append(reinterpret_cast<const uint8*>(KeyConvert.Get()), KeyConvert.Length());
	KeyBytes.SetNumZeroed(32);

	// Eingabetext in Bytes
	TArray<uint8> PlainBytes;
	FTCHARToUTF8 TextConvert(*PlainText);
	PlainBytes.Append(reinterpret_cast<const uint8*>(TextConvert.Get()), TextConvert.Length());

	// Fester oder zufälliger IV (Initialization Vector)
	TArray<uint8> IV;
	IV.SetNumZeroed(16);

	// Ergebnis-Buffer
	TArray<uint8> EncryptedBytes;
	EncryptedBytes.SetNum(PlainBytes.Num() + AES_BLOCK_SIZE);

	EVP_CIPHER_CTX* Ctx = EVP_CIPHER_CTX_new();
	int Len = 0;
	int FinalLen = 0;

	EVP_EncryptInit_ex(Ctx, EVP_aes_256_cbc(), nullptr, KeyBytes.GetData(), IV.GetData());
	EVP_EncryptUpdate(Ctx, EncryptedBytes.GetData(), &Len, PlainBytes.GetData(), PlainBytes.Num());
	EVP_EncryptFinal_ex(Ctx, EncryptedBytes.GetData() + Len, &FinalLen);
	EVP_CIPHER_CTX_free(Ctx);

	EncryptedBytes.SetNum(Len + FinalLen);
	return FBase64::Encode(EncryptedBytes);
}

FString UEncryptDecrypt::DecryptAES(const FString& EncryptedBase64, const FString& Password)
{
	// Passwort vorbereiten
	TArray<uint8> KeyBytes;
	FTCHARToUTF8 KeyConvert(*Password);
	KeyBytes.Append(reinterpret_cast<const uint8*>(KeyConvert.Get()), KeyConvert.Length());
	KeyBytes.SetNumZeroed(32);

	// IV muss identisch zu Encrypt sein
	TArray<uint8> IV;
	IV.SetNumZeroed(16);

	// Base64-Decode
	TArray<uint8> CipherBytes;
	FBase64::Decode(EncryptedBase64, CipherBytes);

	TArray<uint8> DecryptedBytes;
	DecryptedBytes.SetNum(CipherBytes.Num());

	EVP_CIPHER_CTX* Ctx = EVP_CIPHER_CTX_new();
	int Len = 0;
	int FinalLen = 0;

	EVP_DecryptInit_ex(Ctx, EVP_aes_256_cbc(), nullptr, KeyBytes.GetData(), IV.GetData());
	EVP_DecryptUpdate(Ctx, DecryptedBytes.GetData(), &Len, CipherBytes.GetData(), CipherBytes.Num());
	EVP_DecryptFinal_ex(Ctx, DecryptedBytes.GetData() + Len, &FinalLen);
	EVP_CIPHER_CTX_free(Ctx);

	DecryptedBytes.SetNum(Len + FinalLen);
	return FString(UTF8_TO_TCHAR(reinterpret_cast<const char*>(DecryptedBytes.GetData())));
}
