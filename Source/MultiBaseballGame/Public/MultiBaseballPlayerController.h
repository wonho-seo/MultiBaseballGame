// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseballPlayerWidget.h"
#include "GameFramework/PlayerController.h"
#include "MultiBaseballPlayerState.h"
#include "MultiBaseballPlayerController.generated.h"
/**
 * 
 */
UCLASS()
class MULTIBASEBALLGAME_API AMultiBaseballPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void BeginPlayingState() override;
	UFUNCTION(Server, Reliable)
	void SendBaseBallSheet(const FString& Text);
	UFUNCTION(Client, Reliable)
	void SendReportForClient(bool bIsEnd, const int32 PlayerId);
	
	void ReceivedReportDoServer(bool bIsEnd, const AMultiBaseballPlayerState* WinnerPlayerState);
	void EndGame(const int32 PlayerId) const;

	void ChangeWinCount() const;
	void ChangeTimerCount() const;
	void ReceivedBaseballReport() const;

	void TurnCountChanged() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBaseballPlayerWidget> WidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBaseballPlayerWidget* WidgetInstance;
	
};
