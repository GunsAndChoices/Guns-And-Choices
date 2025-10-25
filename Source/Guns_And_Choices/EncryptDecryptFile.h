// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EncryptDecrypt.h"
#include "EncryptDecryptFile.generated.h"

/**
 * 
 */
UCLASS()
class GUNS_AND_CHOICES_API UEncryptDecryptFile : public UEncryptDecrypt
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Encryption");
	static FString EncryptString(const FString& Input, const FString& Password);

	UFUNCTION(BlueprintCallable, Catefory = "Encryption");
	static FString DecryptString(const FString& EncryptedBase64, const FString& Password);
	
};
