// Fill out your copyright notice in the Description page of Project Settings.


#include "EncryptDecryptFile.h"
#include "Crypto/Public/Crypto.h"
#include "Misc/Base64.h"

FString UEncryptDecryptFile::EncryptString(const FString& Input, const FString& Password) {
	// Convert Input to Bytes
	TArray<uint8> PlainBytes;
	FTCHARToUTF8 ConvertInput(*Input);
	PlainBytes.Append(reinterpret_cast<const uint8*>(ConvertInput.Get()), ConvertInput.Length());

	// Password to Bytes
	TArray<uint8> KeyBytes;
	FTCHARToUTF8 ConvertKey(*Password);
	KeyBytes.Append(reinterpret_cast<const uint8*>(ConvertKey.Get()), ConvertKey.Length());

	// Key to 32-Bytes (AES-256)
	KeyBytes.SetNumZeroed(16);

	// Optional: IV (Initialervector) - 16 Bytes
	TArray<uint8> IV;
	IV.SetNumZeroed(16);

	// Encryption
	TArray<uint8> EncrycptedBytes;
	if (!FCrypto::Encrypt_AES256(PlainBytes, KeyBytes, IV, EncrycptedBytes)) {
		UE_LOG(LogTemp, Error, TEXT("Encryption Failed!"));
		return TEXT("");
	}

	// Base64-decoding for saving
	return FBase64::Encode(EncrycptedBytes);
}

FString UEncryptDecryptFile::DecryptString(const FString& EncryptedBase64, const FString& Password) {
	TArray<uint8> EncrycptedBytes;
	FBase64::Decode(EncryptedBase64, EncrycptedBytes);

	// Password to Bytes
	TArray<uint8> KeyBytes;
	FTCHARToUTF8 ConvertKey(*Password);
	KeyBytes.Append(reinterpret_cast<const uint8*>(ConvertKey.Get()), ConvertKey.Length());
	KeyBytes.SetNumZeroed(32);

	// IV (needs to be the same)
	TArray<uint8> IV;
	IV.SetNumZeroed(16);

	// Decryption
	TArray<uint8> DecryptedBytes;
	if (!FCrypto::Decrypt_AES256(EncrycptedBytes, KeyBytes, IV, EncrycptedBytes)) {
		UE_LOG(LogTemp, Error, TEXT("Decryption Failed"));
		return TEXT("");
	}

	return FString(UTF8_TO_TCHAR(reinterpret_cast<const char*>(DecryptedBytes.GetData())));
}