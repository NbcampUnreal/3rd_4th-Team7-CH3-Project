// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LSGameState.generated.h"

/**
 * 
 */
UCLASS()
class LSPROJECT_API ALSGameState : public AGameState
{
	GENERATED_BODY()

public:
	ALSGameState();
	FORCEINLINE void SetShopUse(bool bCanUse){bCanUseShop=bCanUse;}

private:
	bool bCanUseShop;
};
