// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiBaseballGameState.h"

#include "MultiBaseballGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"


void AMultiBaseballGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiBaseballGameState, BaseballReport);
	DOREPLIFETIME(AMultiBaseballGameState, NowPlayingNickName);
}

void AMultiBaseballGameState::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void AMultiBaseballGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (HasAuthority())
	{
		if (AMultiBaseballPlayerState* MultiBaseballPlayerState = Cast<AMultiBaseballPlayerState>(PlayerState))
		{
			MultiBaseballPlayerState->SetPlayerNickName(FName(FString::Printf(TEXT("Guest %d"), PlayerArray.Num())));
		}
	}
}

void AMultiBaseballGameState::Init()
{
	NowPlayingIndex = 0;
	
	if (HasAuthority())
	{
		NowPlayingNickName = Cast<AMultiBaseballPlayerState>(PlayerArray[NowPlayingIndex])->GetPlayerNickName();
		BaseballTimerManager->InitServer();
	}
	if (BaseballTimerManager)
	{
		BaseballTimerManager->Init(TurnTime);
	}
}

void AMultiBaseballGameState::RestartPlay()
{
	for (APlayerState* PlayerState : PlayerArray)
	{
		if (AMultiBaseballPlayerState* MultiBaseballPlayerState = Cast<AMultiBaseballPlayerState>(PlayerState))
		{
			MultiBaseballPlayerState->RestartPlay();
		}
	}
}

AMultiBaseballGameState::AMultiBaseballGameState()
{
	if (HasAuthority())
	{
		BaseballTimerManager = CreateDefaultSubobject<UBaseballTimerManager>(TEXT("BaseballTimerManager"));
	}
}

AMultiBaseballPlayerState* AMultiBaseballGameState::GetNowPlayingState() const
{
	return Cast<AMultiBaseballPlayerState>(PlayerArray[NowPlayingIndex]);
}

AMultiBaseballPlayerState* AMultiBaseballGameState::GetNextPlayingState() const
{
	return Cast<AMultiBaseballPlayerState>(PlayerArray[(NowPlayingIndex + 1) % PlayerArray.Num()]);
}

void AMultiBaseballGameState::PlayerTurnNext()
{
	NowPlayingIndex = (NowPlayingIndex + 1) % PlayerArray.Num();
	NowPlayingNickName = GetNowPlayingState()->GetPlayerNickName();
	if (HasAuthority())
	{
		OnRep_NowPlayingNickNameChange();
	}
	if (BaseballTimerManager)
	{
		BaseballTimerManager->StartTimer();
	}
}

void AMultiBaseballGameState::TimerReset()
{
	if (BaseballTimerManager)
	{
		BaseballTimerManager->StartTimer();
	}
}

void AMultiBaseballGameState::SetBaseballReport(const int32& PlayerId, const FName& PlayerInput,
	const FBaseballResult& Result)
{
	BaseballReport = FBaseballReport(PlayerId, PlayerInput, Result);
}

FBaseballReport AMultiBaseballGameState::GetBaseballReport() const
{
	return BaseballReport;
}

FName AMultiBaseballGameState::GetNowPlayingNickName() const
{
	return NowPlayingNickName;
}

UBaseballTimerManager* AMultiBaseballGameState::GetBaseballTimerManager() const
{
	return BaseballTimerManager;
}

void AMultiBaseballGameState::OnRep_BaseballReportChange() const
{
	OnBaseballReportChanged.Broadcast();
}

void AMultiBaseballGameState::OnRep_NowPlayingNickNameChange() const
{
	OnNowPlayingNickNameChanged.Broadcast();
}





