// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class LSPROJECT_API ALSPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ALSPlayerState();

	FORCEINLINE int32 GetCoin() const{return Coin;}
	FORCEINLINE void SetCoin(int32 NewCoin) {Coin=NewCoin;}
private:
	int32 Coin;
};
