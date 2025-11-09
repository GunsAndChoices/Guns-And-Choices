// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EncryptDecrypt.generated.h"

/**
 * 
 */
UCLASS()
class GUNS_AND_CHOICES_API UEncryptDecrypt : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Encryption");
	static FString EncryptAES(const FString& PlainText, const FString& Password);

	UFUNCTION(BlueprintCallable, Category = "Encryption");
	static FString DecryptAES(const FString& EncryptedBase64, const FString& Password);
};
