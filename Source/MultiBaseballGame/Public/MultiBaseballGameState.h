// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBallResult.h"
#include "BaseballTimerManager.h"
#include "GameFramework/GameState.h"
#include "MultiBaseballPlayerState.h"
#include "MultiBaseballGameState.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnBaseballReportChanged)
DECLARE_MULTICAST_DELEGATE(FOnNowPlayingNickNameChanged)
UCLASS()
class MULTIBASEBALLGAME_API AMultiBaseballGameState : public AGameState
{
	GENERATED_BODY()
protected:
	int32 NowPlayingIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseBallChecker")
	float TurnTime;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_NowPlayingNickNameChange)
	FName NowPlayingNickName;
	
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_BaseballReportChange)
	FBaseballReport BaseballReport;

	UPROPERTY()
	UBaseballTimerManager* BaseballTimerManager;
public:

	AMultiBaseballGameState();
	AMultiBaseballPlayerState* GetNowPlayingState() const;
	AMultiBaseballPlayerState* GetNextPlayingState() const;
	
	void PlayerTurnNext();
	
	void TimerReset();
	void SetBaseballReport(const int32& PlayerId, const FName& PlayerInput, const FBaseballResult& Result);

	FBaseballReport GetBaseballReport() const;
	FName GetNowPlayingNickName() const;
	UBaseballTimerManager* GetBaseballTimerManager() const;
	
	UFUNCTION()
	void OnRep_BaseballReportChange() const;
	UFUNCTION()
	void OnRep_NowPlayingNickNameChange() const;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	
	void Init();
	void RestartPlay();

	FOnBaseballReportChanged OnBaseballReportChanged;
	FOnNowPlayingNickNameChanged OnNowPlayingNickNameChanged;
};

