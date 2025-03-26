// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiBaseballGameMode.h"

#include "BaseballManagerFunctionLibrary.h"
#include "MultiBaseballGameState.h"
#include "MultiBaseballPlayerState.h"
#include "BaseBallResult.h"

AMultiBaseballGameMode::AMultiBaseballGameMode()
{
	BaseBallMaxLen = 0;
	GameStateClass = AMultiBaseballGameState::StaticClass();
}

void AMultiBaseballGameMode::StartPlay()
{
	Super::StartPlay();

	UBaseballManagerFunctionLibrary::SetRandomBaseballNumber(BaseBallMaxLen);
	BaseBallGameState = GetGameState<AMultiBaseballGameState>();
}

void AMultiBaseballGameMode::RestartPlay()
{
	UBaseballManagerFunctionLibrary::SetRandomBaseballNumber(BaseBallMaxLen);
	BaseBallGameState->RestartPlay();
}

void AMultiBaseballGameMode::BaseBallPlay(const APlayerController* InputPlayer, const FString& PlayerInputString)
{
	if (bIsPlayerTurn(InputPlayer))
	{
		AMultiBaseballPlayerState* InputPlayerState = InputPlayer->GetPlayerState<AMultiBaseballPlayerState>();
		if (UBaseballManagerFunctionLibrary::bValidInputString(PlayerInputString, BaseBallMaxLen))
		{
			if (FBaseballResult* Result = UBaseballManagerFunctionLibrary::GetBaseballResult(PlayerInputString).GetPtrOrNull())
			{
				BaseBallGameState->SetBaseballReport(InputPlayerState->GetPlayerId(), *PlayerInputString, *Result);
				BaseBallGameState->OnRep_BaseballReportChange();
				if (Result->Strike == BaseBallMaxLen)
				{
					PlayerWin();
				}
				else
				{
					InputPlayerState->UseTurn();
					if (InputPlayerState->GetTurnRemain() > 0 || BaseBallGameState->GetNextPlayingState()->GetTurnRemain() > 0)
					{
						TurnChange();
					}
					else
					{
						PlayerDraw();
					}
				}
			}
		}
		else
		{
			PlayerLose();
		}
	}
}

void AMultiBaseballGameMode::TurnChange()
{
	if (BaseBallGameState)
	{
		BaseBallGameState->PlayerTurnNext();
		OnBaseballTurn.Broadcast(false, nullptr);
	}
}

void AMultiBaseballGameMode::PlayerWin()
{
	AMultiBaseballPlayerState* NowPlayerState = BaseBallGameState->GetNowPlayingState();
	NowPlayerState->AddWinCount();
	NowPlayerState->OnRep_WinCountChange();
	
	OnBaseballTurn.Broadcast(true, NowPlayerState);
	
	RestartPlay();
}
void AMultiBaseballGameMode::PlayerLose()
{
	AMultiBaseballPlayerState* NextPlayerState = BaseBallGameState->GetNextPlayingState();
	NextPlayerState->AddWinCount();
	NextPlayerState->OnRep_WinCountChange();
	
	OnBaseballTurn.Broadcast(true, NextPlayerState);

	RestartPlay();
}

void AMultiBaseballGameMode::PlayerDraw()
{
	OnBaseballTurn.Broadcast(true, nullptr);

	RestartPlay();
}

void AMultiBaseballGameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);
}

bool AMultiBaseballGameMode::bIsPlayerTurn(const APlayerController* InputPlayer) const
{
	if (InputPlayer && InputPlayer == GetGameState<AMultiBaseballGameState>()->GetNowPlayingState()->GetOwner())
	{
		return true;
	}
	return false;
}


inline void AMultiBaseballGameMode::TurnTimeout() const
{
	if (BaseBallGameState)
	{
		BaseBallGameState->PlayerTurnNext();
	}
}

