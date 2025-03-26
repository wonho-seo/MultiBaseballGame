// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseballTimerManager.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnTimerFired);
DECLARE_MULTICAST_DELEGATE(FOnTimerChange);
UCLASS()
class MULTIBASEBALLGAME_API UBaseballTimerManager : public UActorComponent
{
	GENERATED_BODY()

protected:
	FTimerHandle TimerHandle;
	FTimerHandle TimerTickHandle;
	FTimerHandle ServerSendDelayTimeHandle;

	float RoundTime;
	float RoundRemainTime;
	float ServerRemainTime;
	float ServerSendDelayTime;
	
	double ServerTime0;
	double ServerTime1;
	double ClientTime0;
	double ClientTime1;

	double MinPingTime;
public:
	
	void StartTimer();
	void EndTimer();
	void UpdateRemainTime();
	float GetRemainingTime() const;
	
	//초기화 함수
	UBaseballTimerManager();
	void Init(float TurnTime);
	void InitServer();
	
	//동기화 함수
	void SyncTime();
	UFUNCTION(NetMulticast, Reliable)
	void SendServerTime(double RemainTime, double ReceiveServerTime);
	UFUNCTION(Server, Reliable)
	void RequestPing();
	UFUNCTION(NetMulticast, Reliable)
	void SyncUpdateTime(double ReceiveServerTime);

	FOnTimerFired OnTimerFired;
};


