// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiBaseballPlayerState.h"

#include "MultiBaseballPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void AMultiBaseballPlayerState::BeginPlay()
{
	Super::BeginPlay();

	TurnRemain = 3;
	if (HasAuthority())
	{
		OnRep_TurnRemainChanged();
	}
}

void AMultiBaseballPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiBaseballPlayerState, TurnRemain);
	DOREPLIFETIME(AMultiBaseballPlayerState, WinCount);
}

void AMultiBaseballPlayerState::SetPlayerNickName(const FName& InPlayerName)
{
	PlayerNickName = InPlayerName;
}

FName AMultiBaseballPlayerState::GetPlayerNickName() const
{
	return PlayerNickName;
}

void AMultiBaseballPlayerState::UseTurn()
{
	TurnRemain--;
	if (HasAuthority())
	{
		OnRep_TurnRemainChanged();
	}
}

void AMultiBaseballPlayerState::AddWinCount()
{
	WinCount++;
}

int32 AMultiBaseballPlayerState::GetWinCount() const
{
	return WinCount;
}

void AMultiBaseballPlayerState::OnRep_WinCountChange()
{
	if (AMultiBaseballPlayerController* MultiBaseballPlayerController =
		Cast<AMultiBaseballPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		MultiBaseballPlayerController->ChangeWinCount();
	}
}

void AMultiBaseballPlayerState::OnRep_TurnRemainChanged()
{
	if (AMultiBaseballPlayerController* MultiBaseballPlayerController =
		Cast<AMultiBaseballPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		MultiBaseballPlayerController->TurnCountChanged();
	}
}

void AMultiBaseballPlayerState::RestartPlay()
{
	TurnRemain = 3;
	if (HasAuthority())
	{
		OnRep_TurnRemainChanged();
	}
}

int32 AMultiBaseballPlayerState::GetTurnRemain() const
{
	return TurnRemain;
}
