// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBallResult.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct FBaseballResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Strike;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Ball;
};

USTRUCT(BlueprintType)
struct FBaseballReport
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PlayerInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBaseballResult Result;

	FBaseballReport(int32 PlayerId = 0,FName PlayerInput = "", FBaseballResult Result = FBaseballResult()): PlayerId(PlayerId), PlayerInput(PlayerInput), Result(Result)
	{}
};
