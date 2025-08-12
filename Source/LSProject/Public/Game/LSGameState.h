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
	FORCEINLINE void SetDoorOverlapped(bool bIsOverlapped){bIsCharacterOverlappedWithDoor=bIsOverlapped;}
	FORCEINLINE void SetDay(bool bDay){bIsDay=bDay;}

	FORCEINLINE bool GetDoorOverlapped() const {return bIsCharacterOverlappedWithDoor;}
	FORCEINLINE bool GetDay() const {return bIsDay;}

	bool bGetCanOpenShopUI();

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void UpdateHUD();

private:
	bool bIsCharacterOverlappedWithDoor;
	bool bIsDay;
	//bool bIShowingShopUI;
	bool bCanUseShop;

	FTimerHandle HUDUpdateTimerHandle;
	UPROPERTY() ALSDayNightController* DayNightCtrl = nullptr;

	UPROPERTY() UTextBlock* DayTextBlock = nullptr;
	UPROPERTY() UTextBlock* TimeTextBlock = nullptr;
	UPROPERTY() UTextBlock* ShopPressTextBlock = nullptr;
	
};
