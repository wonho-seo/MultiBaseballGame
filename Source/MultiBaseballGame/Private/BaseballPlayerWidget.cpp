// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballPlayerWidget.h"

#include "MultiBaseballGameMode.h"
#include "MultiBaseballGameState.h"
#include "MultiBaseballPlayerController.h"
#include "Kismet/GameplayStatics.h"


void UBaseballPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	Init();

	PlayerController = Cast<AMultiBaseballPlayerController>(GetOwningPlayer());
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBaseballPlayerWidget::UpdateRemainTime, 0.1f, true);
}

void UBaseballPlayerWidget::SendBaseballSheet(FString Text)
{
	PlayerController->SendBaseBallSheet(Text);
}

void UBaseballPlayerWidget::ShowVictory()
{
	BaseBallResult->SetText(FText::FromString("Victory!"));
}

void UBaseballPlayerWidget::ShowLose()
{
	BaseBallResult->SetText(FText::FromString("Lose!"));
}

void UBaseballPlayerWidget::ShowDraw()
{
	BaseBallResult->SetText(FText::FromString("Draw!"));
}

void UBaseballPlayerWidget::ShowResult(EBaseballResult BaseballResultEnum)
{
	switch (BaseballResultEnum)
	{
		case EBaseballResult::Victory:
			ShowVictory();
			break;
		case EBaseballResult::Lose:
			ShowLose();
			break;
		case EBaseballResult::Draw:
			ShowDraw();
	}
}

void UBaseballPlayerWidget::Init()
{
	UpdateRemainTime();
	UpdatePlayerTurn();
}

void UBaseballPlayerWidget::UpdateRemainTime()
{
	if (AMultiBaseballGameState* MultiBaseballGameState = GetWorld()->GetGameState<AMultiBaseballGameState>())
	{
		if (TimeTextBlock && MultiBaseballGameState->GetBaseballTimerManager())
		{
			TimeTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), MultiBaseballGameState->GetBaseballTimerManager()->GetRemainingTime())));
		}
	}
}

void UBaseballPlayerWidget::UpdateResultDisplay() const
{
	if (AMultiBaseballGameState* MultiBaseballGameState = GetWorld()->GetGameState<AMultiBaseballGameState>())
	{
		const FBaseballResult& BaseballResult = MultiBaseballGameState->GetBaseballReport().Result;
		InputBlock->SetText(FText::FromString(FString::Printf(TEXT("Q: %s\n Strike : %d, Ball : %d"),
			*(MultiBaseballGameState->GetBaseballReport().PlayerInput.ToString()) + 1,
			BaseballResult.Strike,
			BaseballResult.Ball)));
   }
}

void UBaseballPlayerWidget::UpdateWinCount() const
{
	int32 MyWinCount = 0;
	int32 OtherWinCount = 0;
	
	if (AMultiBaseballPlayerState* MultiBaseballPlayerState = GetOwningPlayerState<AMultiBaseballPlayerState>())
	{
		MyWinCount = MultiBaseballPlayerState->GetWinCount();

		for (int32 Index = 0; Index < UGameplayStatics::GetNumPlayerStates(this); Index++)
		{
			if (AMultiBaseballPlayerState* OtherMultiBaseballPlayerState =
			Cast<AMultiBaseballPlayerState>(UGameplayStatics::GetPlayerState(this, Index)))
			{
				if (OtherMultiBaseballPlayerState &&
					OtherMultiBaseballPlayerState->GetPlayerId() != MultiBaseballPlayerState->GetPlayerId())
				{
					OtherWinCount = OtherMultiBaseballPlayerState->GetWinCount();
					break;
				}
			}
		}
	}
	
	if (WinCountBlock)
	{
		WinCountBlock->SetText(FText::FromString(FString::Printf(TEXT("%d : %d"), MyWinCount, OtherWinCount)));
	}
}

void UBaseballPlayerWidget::UpdatePlayerTurn() const
{
	if (NowTurnPlayer)
	{
		int32 RemainTurn = 0;
		if (GetOwningPlayerState<AMultiBaseballPlayerState>())
		{
			RemainTurn = GetOwningPlayerState<AMultiBaseballPlayerState>()->GetTurnRemain();
		}
		NowTurnPlayer->SetText(FText::FromString(FString::Printf(TEXT("진행중인 플레이어: %s\n 남은 턴수: %d"),
				*GetWorld()->GetGameState<AMultiBaseballGameState>()->GetNowPlayingNickName().ToString(),
				RemainTurn)));
	}
}

