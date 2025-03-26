// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiBaseballPlayerController.h"

#include "MultiBaseballGameMode.h"
#include "MultiBaseballPlayerState.h"
#include "Blueprint/UserWidget.h"

void AMultiBaseballPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() && WidgetClass)
	{
		WidgetInstance = CreateWidget<UBaseballPlayerWidget>(this, WidgetClass);
		if (WidgetInstance)
		{
			bShowMouseCursor = true;
			WidgetInstance->AddToViewport();
		}
	}

	GetWorld()->GetGameState<AMultiBaseballGameState>()->OnBaseballReportChanged.AddUObject(this, &AMultiBaseballPlayerController::ReceivedBaseballReport);
	GetWorld()->GetGameState<AMultiBaseballGameState>()->OnNowPlayingNickNameChanged.AddUObject(this, &AMultiBaseballPlayerController::TurnCountChanged);
	if (AMultiBaseballGameMode* MultiBaseballGameMode = GetWorld()->GetAuthGameMode<AMultiBaseballGameMode>())
	{
		MultiBaseballGameMode->OnBaseballTurn.AddUObject(this, &AMultiBaseballPlayerController::ReceivedReportDoServer);
	}
}

void AMultiBaseballPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	if (WidgetInstance)
	{
		bShowMouseCursor = true;
		WidgetInstance->AddToViewport();
		WidgetInstance->Init();
	}
}

void AMultiBaseballPlayerController::ReceivedReportDoServer(bool bIsEnd, const AMultiBaseballPlayerState* WinnerPlayerState)
{
	int32 PlayerId = -1;
	if (WinnerPlayerState)
	{
		PlayerId = WinnerPlayerState->GetPlayerId();
	}
	
	SendReportForClient(bIsEnd, PlayerId);
	if (HasAuthority())
	{
		SendReportForClient_Implementation(bIsEnd, PlayerId);
	}
}


void AMultiBaseballPlayerController::ChangeWinCount() const
{
	if (WidgetInstance)
	{
		WidgetInstance->UpdateWinCount();
	}
}

void AMultiBaseballPlayerController::ChangeTimerCount() const
{
	if (WidgetInstance)
	{
		WidgetInstance->UpdateRemainTime();
	}
}

void AMultiBaseballPlayerController::SendReportForClient_Implementation(bool bIsEnd, const int32 PlayerId)
{
	if (bIsEnd)
	{
		EndGame(PlayerId);
	}
	else
	{
		ReceivedBaseballReport();
	}
	if (AMultiBaseballGameState* MultiBaseballGameState = GetWorld()->GetGameState<AMultiBaseballGameState>())
	{
		MultiBaseballGameState->TimerReset();
	}
}

void AMultiBaseballPlayerController::EndGame(const int32 PlayerId) const
{
	if (WidgetInstance)
	{
		EBaseballResult BaseballResult;
		if (PlayerId < 0)
		{
			BaseballResult = EBaseballResult::Draw;
		}
		else if (PlayerState->GetPlayerId() == PlayerId)
		{
			BaseballResult = EBaseballResult::Victory;
		}
		else
		{
			BaseballResult = EBaseballResult::Lose;
		}
		WidgetInstance->ShowResult(BaseballResult);
	}
}

void AMultiBaseballPlayerController::ReceivedBaseballReport() const
{
	if (WidgetInstance)
	{
		WidgetInstance->UpdateResultDisplay();
	}
}


void AMultiBaseballPlayerController::TurnCountChanged() const
{
	if (WidgetInstance)
	{
		WidgetInstance->UpdatePlayerTurn();
	}
}

void AMultiBaseballPlayerController::SendBaseBallSheet_Implementation(const FString& Text)
{
	if (AMultiBaseballGameMode* GameMode = Cast<AMultiBaseballGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->BaseBallPlay(this, *Text);
	}
}


