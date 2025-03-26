// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "BaseballPlayerWidget.generated.h"

class AMultiBaseballPlayerController;

UENUM(BlueprintType)
enum class EBaseballResult : uint8
{
	Victory UMETA(DisplayName = "Victory"), 
	Lose UMETA(DisplayName = "Loses"),
	Draw UMETA(DisplayName = "Draw")
};

/**
 * 
 */
UCLASS()
class MULTIBASEBALLGAME_API UBaseballPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RoundRemainTime;
	
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* InputBlock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WinCountBlock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TimeTextBlock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* BaseBallResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* NowTurnPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* InputText;
	
	UFUNCTION(BlueprintCallable)
	void SendBaseballSheet(FString Text);

	UFUNCTION(BlueprintCallable)
	void ShowResult(EBaseballResult BaseballResultEnum);

	UFUNCTION(BlueprintCallable)
	void Init();
	
	UFUNCTION(BlueprintCallable)
	void UpdateRemainTime();
	UFUNCTION(BlueprintCallable)
	void UpdateResultDisplay() const;
	UFUNCTION(BlueprintCallable)
	void UpdateWinCount() const;
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerTurn() const;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AMultiBaseballPlayerController* PlayerController;

	FTimerHandle TimerHandle;
protected:
	UFUNCTION(BlueprintCallable)
	void ShowVictory();
	UFUNCTION(BlueprintCallable)
	void ShowLose();
	UFUNCTION(BlueprintCallable)
	void ShowDraw();
};
