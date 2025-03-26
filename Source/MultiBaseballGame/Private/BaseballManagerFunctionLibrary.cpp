// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballManagerFunctionLibrary.h"

TArray<int32> UBaseballManagerFunctionLibrary::BaseBallArray;
UBaseballManagerFunctionLibrary::UBaseballManagerFunctionLibrary()
{
	BaseBallArray.Init(-1, 10);
}

/**
 * Index = 숫자, element = 위치 (-1은 없음)
 * 초기화후 1~MaxIndex를 배열에 각각부여 및 배열을 섞어 숫자가 랜덤한 위치를 가짐 (랜덤한 위치 설정)
 **/
void UBaseballManagerFunctionLibrary::SetRandomBaseballNumber(int32 BaseballMaxCount)
{
	if (BaseballMaxCount < 10)
	{
		BaseBallArray.Init(-1, 10);

		for (int32 Index = 1; Index <= BaseballMaxCount; Index++)
		{
			BaseBallArray[Index] = Index;
		}

		for (int32 Index = 1; Index < BaseballMaxCount; Index++)
		{
			BaseBallArray.Swap(Index, FMath::RandRange(1, BaseBallArray.Num() - 1));
		}
	}
}

bool UBaseballManagerFunctionLibrary::bValidInputString(const FString& InputString, const int32 MaxStringLen)
{
	if (InputString.Len() == MaxStringLen + 1 && InputString[0] == '/')
	{
		TSet<int32> CharChecker;
		for (int32 StringIndex = 1; StringIndex < InputString.Len(); StringIndex++)
		{
			if (CharChecker.Contains(InputString[StringIndex]))
			{
				return false;
			}
			else
			{
				CharChecker.Add(InputString[StringIndex]);
			}
		}

		return true;
	}
	return false;
}

TOptional<FBaseballResult> UBaseballManagerFunctionLibrary::GetBaseballResult(const FString& PlayerInput)
{
	TOptional<FBaseballResult> Result = FBaseballResult();
	
	for (int PlayerInputIndex = 1; PlayerInputIndex < PlayerInput.Len(); PlayerInputIndex++)
	{
		int32 PlayerInputIndexNum = PlayerInput[PlayerInputIndex] - '0';
		if (BaseBallArray.IsValidIndex(PlayerInputIndex))
		{
			if (BaseBallArray[PlayerInputIndexNum] == PlayerInputIndex)
			{
				Result.GetValue().Strike++;
			}
			else if (BaseBallArray[PlayerInputIndexNum] > 0)
			{
				Result.GetValue().Ball++;
			}
		}
	}
	return Result;
}
