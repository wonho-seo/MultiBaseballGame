// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiBaseballGameState.h"
#include "MultiBaseballPlayerState.h"
#include "MultiBaseballGameMode.generated.h"


class UBaseballManagerFunctionLibrary;
/**
 * 
 */
// 플레이어 턴이 한번 실행 했음을 알리는 델리게이트 (bIsEnd, WinnerPlayerControllerPtr), Draw인경우 Ptr은 nullptr 
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnBaseballTurn, bool, const AMultiBaseballPlayerState*)
UCLASS()
class MULTIBASEBALLGAME_API AMultiBaseballGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMultiBaseballGameMode();

	virtual void StartPlay() override;
	void RestartPlay();

	void BaseBallPlay(const APlayerController* InputPlayer, const FString& PlayerInputString);

	void TurnChange();
	void PlayerWin();
	void PlayerLose();
	void PlayerDraw();
	
	virtual void PostLogin(APlayerController* NewPlayerController) override;
	bool bIsPlayerTurn(const APlayerController* InputPlayer) const;
	void TurnTimeout() const;

	FOnBaseballTurn OnBaseballTurn;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseBallChecker")
	int32 BaseBallMaxLen;
	
	AMultiBaseballGameState* BaseBallGameState;
};
