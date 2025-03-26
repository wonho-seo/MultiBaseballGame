// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballTimerManager.h"

#include "MultiBaseballGameMode.h"
#include "MultiBaseballPlayerController.h"
#include "Kismet/GameplayStatics.h"

UBaseballTimerManager::UBaseballTimerManager()
{
	MinPingTime = 1.0f;
	ServerSendDelayTime = 5.0f;
}

void UBaseballTimerManager::StartTimer()
{
	MinPingTime = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBaseballTimerManager::EndTimer, RoundTime, true, RoundTime);
}

void UBaseballTimerManager::EndTimer()
{
	if (AMultiBaseballGameMode* GameMode = GetWorld()->GetAuthGameMode<AMultiBaseballGameMode>())
	{
		GameMode->TurnTimeout();
	}
}

void UBaseballTimerManager::UpdateRemainTime()
{
	RoundRemainTime = GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle);
	if (AMultiBaseballPlayerController* PlayerController = Cast<AMultiBaseballPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController->ChangeTimerCount();
	}
}

void UBaseballTimerManager::InitServer()
{
	GetWorld()->GetTimerManager().SetTimer(ServerSendDelayTimeHandle, this, &UBaseballTimerManager::SyncTime, ServerSendDelayTime, true);
}

void UBaseballTimerManager::Init(float TurnTime)
{
	RoundTime = TurnTime;
	
	StartTimer();
	GetWorld()->GetTimerManager().SetTimer(TimerTickHandle, this, &UBaseballTimerManager::UpdateRemainTime, 0.1f, true);
	
	SyncTime();
}

void UBaseballTimerManager::SyncTime()
{
	double StartTime = GetWorld()->GetRealTimeSeconds();
	SendServerTime(GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle), StartTime);
}

float UBaseballTimerManager::GetRemainingTime() const
{
	return RoundRemainTime;
}


void UBaseballTimerManager::SendServerTime_Implementation(double InServerRemainTime, double ReceiveServerTime)
{
	ServerRemainTime = InServerRemainTime;
	ServerTime0 = ReceiveServerTime;
	ClientTime0 = GetWorld()->GetRealTimeSeconds();
	RequestPing();
}

void UBaseballTimerManager::RequestPing_Implementation()
{
	double StartTime = GetWorld()->GetRealTimeSeconds();
	SyncUpdateTime(StartTime);
}

void UBaseballTimerManager::SyncUpdateTime_Implementation(double ReceiveServerTime)
{
	ClientTime1 = GetWorld()->GetRealTimeSeconds();
	ServerTime1 = ReceiveServerTime;

	double PingServerTime = ServerTime1 - ServerTime0;
	double PingClientTime = ClientTime1 - ClientTime0;

	if (MinPingTime > PingServerTime + PingClientTime)
	{
		MinPingTime = PingServerTime + PingClientTime;
		
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &UBaseballTimerManager::EndTimer, ServerRemainTime - MinPingTime, true);
	}
}

