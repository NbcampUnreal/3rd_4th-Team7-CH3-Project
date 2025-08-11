// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LSGameState.generated.h"

class ALSDayNightController;
class UTextBlock;


UCLASS()
class LSPROJECT_API ALSGameState : public AGameState
{
	GENERATED_BODY()

public:
	ALSGameState();
	FORCEINLINE void SetShopUse(bool bCanUse){bCanUseShop=bCanUse;}

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void UpdateHUD();

private:
	bool bCanUseShop;

	FTimerHandle HUDUpdateTimerHandle;
	UPROPERTY() ALSDayNightController* DayNightCtrl = nullptr;

	UPROPERTY() UTextBlock* DayTextBlock = nullptr;
	UPROPERTY() UTextBlock* TimeTextBlock = nullptr;
	
};
