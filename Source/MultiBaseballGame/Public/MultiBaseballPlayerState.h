// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MultiBaseballPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIBASEBALLGAME_API AMultiBaseballPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly)
	FName PlayerNickName;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_TurnRemainChanged)
	int32 TurnRemain;
	
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_WinCountChange)
	int32 WinCount;
public:
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(BlueprintCallable)
	void SetPlayerNickName(const FName& InPlayerName);
	UFUNCTION(BlueprintCallable)
	FName GetPlayerNickName() const;
	UFUNCTION(BlueprintCallable)
	void UseTurn();
	UFUNCTION(BlueprintCallable)
	int32 GetTurnRemain() const;
	UFUNCTION(BlueprintCallable)
	void AddWinCount();
	UFUNCTION(BlueprintCallable)
	int32 GetWinCount() const;

	UFUNCTION(BlueprintCallable)
	void OnRep_WinCountChange();
	UFUNCTION(BlueprintCallable)
	void OnRep_TurnRemainChanged();
	void RestartPlay();
};
