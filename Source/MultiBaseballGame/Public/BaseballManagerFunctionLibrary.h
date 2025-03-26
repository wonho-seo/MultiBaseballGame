// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBallResult.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaseballManagerFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MULTIBASEBALLGAME_API UBaseballManagerFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
protected:
	static TArray<int32> BaseBallArray;
	
public:
	UBaseballManagerFunctionLibrary();
	// 랜덤한 숫자값의 MaxIndex의 수만큼 생성
	UFUNCTION(BlueprintCallable, Category = "BaseBallManager")
	static void SetRandomBaseballNumber(int32 MaxIndex);
	UFUNCTION(BlueprintCallable, Category = "BaseBallManager")
	static bool bValidInputString(const FString& InputString, const int32 MaxStringLen);
	static TOptional<FBaseballResult> GetBaseballResult(const FString& PlayerInput);
};
